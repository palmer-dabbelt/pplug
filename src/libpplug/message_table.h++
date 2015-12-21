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

#ifndef LIBPPLUG__MESSAGE_TABLE_HXX
#define LIBPPLUG__MESSAGE_TABLE_HXX

#include <psqlite/table.h++>
#include <psqlite/connection.h++>
#include "message.h++"

namespace libpplug {
    class message_table {
    private:
        psqlite::table::ptr _table;
        psqlite::connection::ptr _db;

    public:
        message_table(const psqlite::connection::ptr& db);

    public:
        /* Sets the "current" value of a property to a particular
         * value, with the actual current time passed in as an
         * argument (that could be in the past). */
        void set(const std::string& property,
                 const std::string& value,
                 uint64_t unix_nanoseconds);

        /*  */
        std::shared_ptr<message> read_newest(const std::string& prop);
    };
}

#endif
