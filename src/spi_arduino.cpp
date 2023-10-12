/*
 * Copyright 2017 Bitcraze AB
 * Copyright 2023 Simon D. Levy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <SPI.h>

#include "spi_compat.h"

void spi_begin(void * bus)
{
    SPIClass * spi = (SPIClass *)bus;

    spi->begin();
}

void spi_begin_transaction(void * bus)
{
    SPIClass * spi = (SPIClass *)bus;

    spi->beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));
}

void spi_end_transaction(void * bus)
{
    SPIClass * spi = (SPIClass *)bus;

    spi->endTransaction();
}

void spi_write_value(void * bus, uint8_t value)
{
    SPIClass * spi = (SPIClass *)bus;

    spi->transfer(&value, 1);
}

void spi_read_buffer(void * bus, void * buffer, const size_t size)
{
    SPIClass * spi = (SPIClass *)bus;

    spi->transfer(buffer, size);
}
