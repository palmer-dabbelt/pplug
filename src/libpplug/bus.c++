/*
 * Copyright (C) 2015 Palmer Dabbelt
 *   <palmer@dabbelt.com>
 *
 * This file is part of pplug
 * 
 * pplug is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * pplug is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with pplug.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bus.h++"
#include "message_table.h++"
#include <fcntl.h>
#include <gitdate.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <stdlib.h>
using namespace libpplug;

static const std::string base_path = PPLUG_BASE_DIR "/metadata.sqlite";

bus::bus(void)
    : _db(std::make_shared<psqlite::connection>(base_path)),
      _message_table(std::make_shared<message_table>(_db))
{
}

int bus::send(const std::shared_ptr<message>& m)
{
    auto transaction = _db->exclusive_transaction();

    _message_table->set(m->property(),
                        m->value(),
                        m->unix_nanoseconds());

    _message_table->clear_older_than(m->property(),
                                     m->unix_nanoseconds() - (1ULL << 32));

    return 0;
}

std::vector<std::shared_ptr<message>>
bus::atomic_read(const std::vector<std::string>& properties)
{
    auto transaction = _db->exclusive_transaction();

    std::vector<std::shared_ptr<message>> out;
    for (const auto& property: properties) {
        auto m = _message_table->read_newest(property);
        if (m != NULL)
            out.push_back(m);
    }

    return out;
}

std::vector<std::shared_ptr<message>>
bus::wait_then_read(const std::vector<std::string>& properties,
                    size_t unix_nanoseconds)
{
    auto inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        perror("unable to open inotify descriptor");
        abort();
    }

    if (inotify_add_watch(inotify_fd, base_path.c_str(), IN_MODIFY) < 0) {
        perror("Unable to add inotify watch");
        abort();
    }

    while (true) {
        auto messages = atomic_read(properties);
        for (const auto& message: messages) {
            if (message->unix_nanoseconds() > unix_nanoseconds) {
                close(inotify_fd);
                return messages;
            }
        }

        struct inotify_event event;
        if (read(inotify_fd, &event, sizeof(event)) < 0) {
            perror("inotify read");
            abort();
        }
    }
}
