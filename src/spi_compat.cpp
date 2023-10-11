#include <SPI.h>

#include "spi_compat.h"

void spi_begin(void)
{
    SPI.begin();
}

void spi_begin_transaction(void)
{
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));
}

void spi_end_transaction(void)
{
    SPI.endTransaction();
}

void spi_write_value(uint8_t value)
{
    SPI.transfer(&value, 1);
}

void spi_read_buffer(void * buffer, const size_t size)
{
    SPI.transfer(buffer, size);
}
