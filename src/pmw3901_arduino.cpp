#include <SPI.h>

/*
 * Copyright 2023 Simon D. Levy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pmw3901.hpp"

#include <SPI.h>

void PMW3901::spi_begin_transaction(void)
{
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));

}

void PMW3901::spi_write_byte(const uint8_t byte)
{
    uint8_t b = byte;

    SPI.transfer(&b, 1);
}

uint8_t PMW3901::spi_read_byte(void)
{
    uint8_t byte = 0;
    
    SPI.transfer(&byte, 1);

    return byte;
}

void PMW3901::spi_read_buffer(size_t size, uint8_t * buffer)
{
    SPI.transfer(buffer, size);
}

void PMW3901::spi_end_transaction(void)
{
    SPI.endTransaction();
}
