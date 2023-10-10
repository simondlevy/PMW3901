#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "pmw3901.hpp"

class PMW3901_Arduino : public PMW3901 {

    public:

        PMW3901_Arduino(uint8_t cspin, SPIClass * spi = &SPI)
        {
            _cspin = cspin;
            _spi = spi;
        }

        bool begin(void)
        {
            // Setup SPI port
            spi_begin();

            pinMode(_cspin, OUTPUT);

            _spi->beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));

            // Make sure the SPI bus is reset
            digitalWrite(_cspin, HIGH);
            delay(1);
            digitalWrite(_cspin, LOW);
            delay(1);
            digitalWrite(_cspin, HIGH);
            delay(1);

            _spi->endTransaction();

            // Power on reset
            registerWrite(0x3A, 0x5A);
            delay(5);

            // Test the SPI communication, checking chipId and inverse chipId
            uint8_t chipId = registerRead(0x00);
            uint8_t dIpihc = registerRead(0x5F);

            if (chipId != 0x49 && dIpihc != 0xB8) return false;

            // Reading the motion registers one time
            registerRead(0x02);
            registerRead(0x03);
            registerRead(0x04);
            registerRead(0x05);
            registerRead(0x06);
            delay(1);

            initRegisters();

            return true;
        }

        void readMotion(motionBurst_t * motion)
        {
            uint8_t address = 0x16;

            _spi->beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));

            digitalWrite(_cspin,LOW);
            delayMicroseconds(50);

            _spi->transfer(&address, 1);

            delayMicroseconds(50);

            _spi->transfer(motion, sizeof(motionBurst_t));

            delayMicroseconds(50);
            digitalWrite(_cspin, HIGH);

            _spi->endTransaction();

            delayMicroseconds(50);
        }

    protected:

        virtual void spi_begin(void) override
        {
            _spi->begin();
        }

    private:

        uint8_t _cspin;

        SPIClass * _spi ;

        virtual void registerWrite(uint8_t reg, uint8_t value) override
        {
            reg |= 0x80u;

            _spi->beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));

            digitalWrite(_cspin, LOW);

            delayMicroseconds(50);
            _spi->transfer(reg);
            _spi->transfer(value);
            delayMicroseconds(50);

            digitalWrite(_cspin, HIGH);

            _spi->endTransaction();

            delayMicroseconds(200);
        }

        uint8_t registerRead(uint8_t reg) {
            reg &= ~0x80u;

            _spi->beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));

            digitalWrite(_cspin, LOW);

            delayMicroseconds(50);
            _spi->transfer(reg);
            delayMicroseconds(50);
            uint8_t value = _spi->transfer(0);
            delayMicroseconds(200);

            digitalWrite(_cspin, HIGH);

            delayMicroseconds(200);

            _spi->endTransaction();

            return value;
        }
};
