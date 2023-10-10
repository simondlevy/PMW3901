#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "pmw3901.hpp"

class PMW3901_Arduino : public PMW3901 {

    public:

        PMW3901_Arduino(uint8_t cspin, SPIClass * spi = &SPI)
            : PMW3901(cspin)
        {
            _spi = spi;
        }

    protected:

        virtual void spi_begin(void) override
        {
            _spi->begin();
        }

        virtual void spi_begin_transaction(void) override
        {
            _spi->beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));
        }

        virtual void spi_end_transaction(void) override
        {
            _spi->endTransaction();
        }

        virtual void spi_transfer(void * data, size_t size)
        {
            _spi->transfer(data, size);
        }

    private:

        SPIClass * _spi ;

};
