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

       void readMotion(motionBurst_t * motion)
        {
            uint8_t address = 0x16;

            spi_begin_transaction();

            digitalWrite(_cspin,LOW);
            delayMicroseconds(50);

            spi_transfer(&address, 1);

            delayMicroseconds(50);

            spi_transfer(motion, sizeof(motionBurst_t));

            delayMicroseconds(50);
            digitalWrite(_cspin, HIGH);

            spi_end_transaction();

            delayMicroseconds(50);
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
