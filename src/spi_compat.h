#pragma once

#include <stdint.h>
#include <stddef.h>

void spi_begin(void);

void spi_begin_transaction(void);

void spi_end_transaction(void);

void spi_write_value(uint8_t value);

void spi_read_buffer(void * buffer, const size_t size);
