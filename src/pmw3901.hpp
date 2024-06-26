#pragma once

/*
 * Arduino library for PMW3901 optical flow sensor
 *
 * Copyright 2017 Bitcraze AB
 * Copyright 2024 Simon D. Levy
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


#include <Arduino.h>
#include <SPI.h>

class PMW3901 {

    public:

        /**
         *  Starts the sensor on a specified pin
         */
        bool begin(SPIClass * spi=&SPI, const uint8_t csPin=SS)
        {
            _spi = spi;
            _csPin = csPin;

            bool retval = false;

            // Initialize CS Pin
            pinMode(csPin, OUTPUT);
            digitalWrite(csPin, HIGH);

            delayMicroseconds(40000);

            digitalWrite(csPin, HIGH);

            delayMicroseconds(20000);

            digitalWrite(csPin, LOW);

            delayMicroseconds(20000);

            digitalWrite(csPin, HIGH);

            delayMicroseconds(20000);

            uint8_t chipId    = registerRead(csPin, 0x00);
            uint8_t invChipId = registerRead(csPin, 0x5f);

            if (chipId == 0x49 && invChipId == 0xB6)
            {
                // Power on reset
                registerWrite(csPin, 0x3a, 0x5a);
                delayMicroseconds(5000);

                // Reading the motion registers one time
                registerRead(csPin, 0x02);
                registerRead(csPin, 0x03);
                registerRead(csPin, 0x04);
                registerRead(csPin, 0x05);
                registerRead(csPin, 0x06);
                delayMicroseconds(1000);

                initRegisters(csPin);

                retval = true;
            }

            return retval;
        }

        /**
         *  Reads the current deltaX, deltaY values from the sensor.
         *  @return true if motion was detected, false otherwise
         */
        bool readMotion(int16_t & deltaX, int16_t &  deltaY) 
        {
            uint8_t address = 0x16;

            beginTransaction();
            digitalWrite(_csPin,LOW);
            delayMicroseconds(50);

            writeByte(address);

            delayMicroseconds(50);

            _spi->transfer((uint8_t*)&_currentMotion, sizeof(motionBurst_t));

            delayMicroseconds(50);
            digitalWrite(_csPin, HIGH);

            _spi->endTransaction();

            delayMicroseconds(50);

            uint16_t realShutter = (_currentMotion.shutter >> 8) & 0x0FF;
            realShutter |= (_currentMotion.shutter & 0x0ff) << 8;
            _currentMotion.shutter = realShutter;

            deltaX = _currentMotion.deltaX;
            deltaY = _currentMotion.deltaY;

            return _currentMotion.motion == 0XB0;
        }

    private:

        typedef struct motionBurst_s {
            union {
                uint8_t motion;
                struct {
                    uint8_t frameFrom0    : 1;
                    uint8_t runMode       : 2;
                    uint8_t reserved1     : 1;
                    uint8_t rawFrom0      : 1;
                    uint8_t reserved2     : 2;
                    uint8_t motionOccurred: 1;
                };
            };

            uint8_t observation;
            int16_t deltaX;
            int16_t deltaY;

            uint8_t squal;

            uint8_t rawDataSum;
            uint8_t maxRawData;
            uint8_t minRawData;

            uint16_t shutter;
        } __attribute__((packed)) motionBurst_t;

        motionBurst_t _currentMotion;

        SPIClass * _spi;

        uint8_t _csPin;

        void registerWrite(const uint8_t csPin, uint8_t rgstr, uint8_t value)
        {
            // Set MSB to 1 for write
            rgstr |= 0x80u;

            beginTransaction();
            digitalWrite(csPin, LOW);

            delayMicroseconds(50);

            writeByte(rgstr);

            delayMicroseconds(50);

            writeByte(value);

            delayMicroseconds(50);

            digitalWrite(csPin, HIGH);

            _spi->endTransaction();

            delayMicroseconds(200);
        }

        uint8_t registerRead(const uint8_t csPin, const uint8_t rgstr)
        {
            // Set MSB to 0 for read
            uint8_t reg = rgstr & ~0x80u;

            beginTransaction();

            digitalWrite(csPin, LOW);

            delayMicroseconds(50);

            writeByte(reg);

            delayMicroseconds(500);

            auto data = readByte();

            delayMicroseconds(50);

            digitalWrite(csPin, HIGH);

            _spi->endTransaction();

            delayMicroseconds(200);

            return data;
        }

        void initRegisters(const uint8_t csPin)
        {
            registerWrite(csPin, 0x7F, 0x00);
            registerWrite(csPin, 0x61, 0xAD);
            registerWrite(csPin, 0x7F, 0x03);
            registerWrite(csPin, 0x40, 0x00);
            registerWrite(csPin, 0x7F, 0x05);
            registerWrite(csPin, 0x41, 0xB3);
            registerWrite(csPin, 0x43, 0xF1);
            registerWrite(csPin, 0x45, 0x14);
            registerWrite(csPin, 0x5B, 0x32);
            registerWrite(csPin, 0x5F, 0x34);
            registerWrite(csPin, 0x7B, 0x08);
            registerWrite(csPin, 0x7F, 0x06);
            registerWrite(csPin, 0x44, 0x1B);
            registerWrite(csPin, 0x40, 0xBF);
            registerWrite(csPin, 0x4E, 0x3F);
            registerWrite(csPin, 0x7F, 0x08);
            registerWrite(csPin, 0x65, 0x20);
            registerWrite(csPin, 0x6A, 0x18);
            registerWrite(csPin, 0x7F, 0x09);
            registerWrite(csPin, 0x4F, 0xAF);
            registerWrite(csPin, 0x5F, 0x40);
            registerWrite(csPin, 0x48, 0x80);
            registerWrite(csPin, 0x49, 0x80);
            registerWrite(csPin, 0x57, 0x77);
            registerWrite(csPin, 0x60, 0x78);
            registerWrite(csPin, 0x61, 0x78);
            registerWrite(csPin, 0x62, 0x08);
            registerWrite(csPin, 0x63, 0x50);
            registerWrite(csPin, 0x7F, 0x0A);
            registerWrite(csPin, 0x45, 0x60);
            registerWrite(csPin, 0x7F, 0x00);
            registerWrite(csPin, 0x4D, 0x11);
            registerWrite(csPin, 0x55, 0x80);
            registerWrite(csPin, 0x74, 0x1F);
            registerWrite(csPin, 0x75, 0x1F);
            registerWrite(csPin, 0x4A, 0x78);
            registerWrite(csPin, 0x4B, 0x78);
            registerWrite(csPin, 0x44, 0x08);
            registerWrite(csPin, 0x45, 0x50);
            registerWrite(csPin, 0x64, 0xFF);
            registerWrite(csPin, 0x65, 0x1F);
            registerWrite(csPin, 0x7F, 0x14);
            registerWrite(csPin, 0x65, 0x67);
            registerWrite(csPin, 0x66, 0x08);
            registerWrite(csPin, 0x63, 0x70);
            registerWrite(csPin, 0x7F, 0x15);
            registerWrite(csPin, 0x48, 0x48);
            registerWrite(csPin, 0x7F, 0x07);
            registerWrite(csPin, 0x41, 0x0D);
            registerWrite(csPin, 0x43, 0x14);
            registerWrite(csPin, 0x4B, 0x0E);
            registerWrite(csPin, 0x45, 0x0F);
            registerWrite(csPin, 0x44, 0x42);
            registerWrite(csPin, 0x4C, 0x80);
            registerWrite(csPin, 0x7F, 0x10);
            registerWrite(csPin, 0x5B, 0x02);
            registerWrite(csPin, 0x7F, 0x07);
            registerWrite(csPin, 0x40, 0x41);
            registerWrite(csPin, 0x70, 0x00);

            delayMicroseconds(10000);

            registerWrite(csPin, 0x32, 0x44);
            registerWrite(csPin, 0x7F, 0x07);
            registerWrite(csPin, 0x40, 0x40);
            registerWrite(csPin, 0x7F, 0x06);
            registerWrite(csPin, 0x62, 0xF0);
            registerWrite(csPin, 0x63, 0x00);
            registerWrite(csPin, 0x7F, 0x0D);
            registerWrite(csPin, 0x48, 0xC0);
            registerWrite(csPin, 0x6F, 0xD5);
            registerWrite(csPin, 0x7F, 0x00);
            registerWrite(csPin, 0x5B, 0xA0);
            registerWrite(csPin, 0x4E, 0xA8);
            registerWrite(csPin, 0x5A, 0x50);
            registerWrite(csPin, 0x40, 0x80);

            registerWrite(csPin, 0x7F, 0x00);
            registerWrite(csPin, 0x5A, 0x10);
            registerWrite(csPin, 0x54, 0x00);
        }

        void beginTransaction(void)
        {
            _spi->beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
        }

        void writeByte(const uint8_t byte)
        {
            uint8_t b = byte;

            _spi->transfer(&b, 1);
        }

        uint8_t readByte(void)
        {
            uint8_t byte = 0;

            _spi->transfer(&byte, 1);

            return byte;
        }
};
