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

#include "version.h"
#include <tclap/CmdLine.h>
#include <libpplug/message.h++>
#include <libpplug/bus.h++>

int main(int argc, const char **argv)
{
    try {
        TCLAP::CmdLine cmd(
            "pplug-change: changes a pplug property to have a new value",
            ' ',
            PCONFIGURE_VERSION
            );

        TCLAP::ValueArg<std::string> property(
            "p",
            "property",
            "Property name",
            true,
            "",
            "name"
            );
        cmd.add(property);

        TCLAP::ValueArg<std::string> data(
            "d",
            "data",
            "Data to be stored",
            true,
            "",
            "data"
            );
        cmd.add(data);

        cmd.parse(argc, argv);

        auto message = std::make_shared<libpplug::message>(
            property.getValue(),
            data.getValue()
            );

        auto bus = std::make_shared<libpplug::bus>();

        bus->send(message);
    } catch (...) {
        fprintf(stderr, "excepting parsing command-line arguments\n");
        abort();
    }

    return 0;
}
