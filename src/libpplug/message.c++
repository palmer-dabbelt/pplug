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

#include "message.h++"
#include <libputil/chrono/datetime.h++>
using namespace libpplug;

static size_t now(void);

message::message(const std::string& property,
                 const std::string& value)
    : _property(property),
      _value(value),
      _unix_nanoseconds(now())
{
}

message::message(const std::string& property,
                 const std::string& value,
                 const size_t& unix_nanoseconds)
    : _property(property),
      _value(value),
      _unix_nanoseconds(unix_nanoseconds)
{
}

size_t now(void)
{
    auto time = putil::chrono::datetime::now(CLOCK_MONOTONIC);
    return time.unix_nanoseconds();
}
