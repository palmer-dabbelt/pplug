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
#include <libputil/chrono/datetime.h++>
#include <fcntl.h>
#include <gitdate.h>
#include <unistd.h>
using namespace libpplug;

static const std::string base_path = "/run/pplug/metadata.sqlite";

bus::bus(void)
    : _db(std::make_shared<psqlite::connection>(base_path)),
      _message_table(std::make_shared<message_table>(_db))
{
}

int bus::send(const std::shared_ptr<message>& m)
{
    auto time = putil::chrono::datetime::now(CLOCK_MONOTONIC);

    _message_table->set(m->property(),
                        m->value(),
                        time.unix_nanoseconds());

    return 0;
}
