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

#ifndef LIBPPLUG__BUS_HXX
#define LIBPPLUG__BUS_HXX

#include <psqlite/connection.h++>
#include <memory>
#include "message.h++"
#include "message_table.h++"

namespace libpplug {
    /* The bus routes all pplug messages. */
    class bus {
    private:
        psqlite::connection::ptr _db;
        std::shared_ptr<message_table> _message_table;

    public:
        /* Opens a connection to the pplug bus. */
        bus(void);

    public:
        /* Synchronously sends a message over the bus, returning when
         * it has been committed to stable storage somewhere. */
        int send(const std::shared_ptr<message>& m);

        /* Atomicly reads the set of properties listed, returning the
         * latest message from any of them that have messages that
         * exist. */
        std::shared_ptr<message> atomic_read(const std::string& property);
        std::vector<std::shared_ptr<message>>
        atomic_read(const std::vector<std::string>& properties);

        /* Waits for a value in the given property set to be updated to have a
         * newer modification date than the given one, returning the entire set
         * upon change.  */
       std::vector<std::shared_ptr<message>>
       wait_then_read(const std::vector<std::string>& properties,
                      size_t unix_nanoseconds);
    };
}

#endif
