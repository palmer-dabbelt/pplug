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

#ifndef LIBPPLUG__MESSAGE_HXX
#define LIBPPLUG__MESSAGE_HXX

#include <memory>
#include <vector>

namespace libpplug {
    /* */
    class message {
    private:
        const std::string _property;
        const std::string _value;
        const size_t _unix_nanoseconds;

    public:
        /* Creates a new message using the current system time, in
         * case you want to send one. */
        message(const std::string& property,
                const std::string& value);

        message(const std::string& property,
                const std::string& value,
                const size_t& unix_nanoseconds);

    public:
        const std::string& property(void) const { return _property; }
        const std::string& value(void) const { return _value; }
        size_t unix_nanoseconds() const { return _unix_nanoseconds; }
    };
}

#endif
