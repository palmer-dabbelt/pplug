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
#include <iomanip>
#include <sstream>
using namespace libpplug;

static inline psqlite::table::ptr make_table(void);

template<typename T>
static inline std::string int_to_hex(T i)
{
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(sizeof(T)*2) 
         << std::hex << i;
  return stream.str();
}


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
    map["time"] = int_to_hex(unix_nanoseconds);
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

std::shared_ptr<message> message_table::read_newest(const std::string& prop)
{
    auto resp = _db->select(_table, "property='%s' ORDER BY time DESC LIMIT 1",
                            prop.c_str());
    switch (resp->return_value()) {
    case psqlite::error_code::SUCCESS:
        break;
    case psqlite::error_code::FAILED_UNIQUE:
        abort();
        break;
    }

    if (resp->result_count() == 0)
        return NULL;

    return std::make_shared<message>(
        resp->rowi(0)->value("property"),
        resp->rowi(0)->value("value")
        );
}

psqlite::table::ptr make_table(void)
{
    std::vector<psqlite::column::ptr> out;
    out.push_back(std::make_shared<psqlite::column_t<std::string>>("property"));
    out.push_back(std::make_shared<psqlite::column_t<std::string>>("value"));
    out.push_back(std::make_shared<psqlite::column_t<std::string>>("time"));
    return std::make_shared<psqlite::table>("PPLUG__messages", out);
}

