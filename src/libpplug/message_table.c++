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

#include "message_table.h++"
using namespace libpplug;

static inline psqlite::table::ptr make_table(void);

message_table::message_table(const psqlite::connection::ptr& db)
    : _table(make_table()),
      _db(db)
{
    _db->create(_table);
}

void message_table::set(const std::string& property,
                        const std::string& value,
                        uint64_t unix_nanoseconds)
{
    auto map = std::map<std::string, std::string>();
    map["property"] = property;
    map["value"] = value;
    map["time"] = std::to_string(unix_nanoseconds);
    auto row = std::make_shared<psqlite::row>(map);

    auto resp = _db->insert(_table, row);
    switch (resp->return_value()) {
    case psqlite::error_code::SUCCESS:
        return;
    case psqlite::error_code::FAILED_UNIQUE:
        abort();
        return;
    }
}

psqlite::table::ptr make_table(void)
{
    std::vector<psqlite::column::ptr> out;
    out.push_back(std::make_shared<psqlite::column_t<std::string>>("property"));
    out.push_back(std::make_shared<psqlite::column_t<std::string>>("value"));
    out.push_back(std::make_shared<psqlite::column_t<std::string>>("time"));
    return std::make_shared<psqlite::table>("PPLUG__messages", out);
}

