/*

Module: ads131m04_simple.ino

Function:
    Simple example for MCCI Catena ADS131M04 library.

Copyright and License:
    See accompanying LICENSE file for copyright and license information.

Author:
    Pranau R, MCCI Corporation   November 2022

*/

#include <MCCI_Catena_ADS131M04.h>

#include <Catena.h>
#include <Arduino.h>

/****************************************************************************\
|
|   Manifest constants & typedefs.
|
\****************************************************************************/

using namespace McciCatenaAds131m04;
using namespace McciCatena;

/****************************************************************************\
|
|   Variables.
|
\****************************************************************************/

// the primary object
Catena gCatena;

SPIClass gSPI2(
    Catena::PIN_SPI2_MOSI,
    Catena::PIN_SPI2_MISO,
    Catena::PIN_SPI2_SCK
    );

cADS131M04 gAds;

/****************************************************************************\
|
|   Code.
|
\****************************************************************************/

/*
Name:   setup()
Function:
        Initilize ADS131M04 for ADC conversion.
Definition:
        void setup(
            void
            );
Description:
        To initiate a serial connection between board and display and to initiate the connectivity of ADS131M04 four channel ADC.
Returns:
        No explicit result.
*/

void setup()
    {
    Serial.println("**** This is an example program to read RAW ADC data using ADS131M04 ****");

    gCatena.begin();
    gSPI2.begin();

    if(! gAds.begin(&gSPI2))
        {
        Serial.println("ADS131M04 begin failed");
        while(1);
        }
    else
        {
        Serial.println("ADS131M04 begin successfully!");

        // Set chip select pins to high
        pinMode(D5, OUTPUT);
        digitalWrite(D5, HIGH);
        }
    }

/*
Name:   loop()
Function:
        To get raw ADC readings and to display it in serial monitor.
Definition:
        void loop (
            void
            );
Returns:
        No explicit result.
*/

void loop()
    {
    uint8_t channel0 = 0;
    uint8_t channel1 = 1;
    uint8_t channel2 = 2;
    uint8_t channel3 = 3;
    //uint8_t id = gAds.readRegister((std::uint8_t)gAds.READ_ONLY_REG::ID);
    //uint8_t id = (std::uint8_t) gAds.READ_ONLY_REG::ID;
    float FSR = 2.4;
    float BITS = 16777215;
    float code;

    /*Serial.print("Writing: ");
    Serial.print("Write Success? [1]Yes [0]No :");
    Serial.println(gAds.writeRegister(0x04,0x6767));
    Serial.print("Reading: 0x00");
    Serial.println(gAds.readRegister(0x04), HEX);*/

    //uint16_t registerData = gAds.readRegister(id);
    //Serial.print("Reading: 0x00");
    //Serial.println(gAds.readID(), HEX);

    Serial.print("CH0(HEX)          : ");
    Serial.println(gAds.readSingleChannel(channel0), HEX);
    code = gAds.readSingleChannel(channel0);
    Serial.print("CODE(DEC)         : ");
    Serial.println(code);

    Serial.print("CH1(HEX)          : ");
    Serial.println(gAds.readSingleChannel(channel1), HEX);
    code = gAds.readSingleChannel(channel1);
    Serial.print("CODE(DEC)         : ");
    Serial.println(code);

    Serial.print("CH2(HEX)          : ");
    Serial.println(gAds.readSingleChannel(channel2), HEX);
    code = gAds.readSingleChannel(channel2);
    Serial.print("CODE(DEC)         : ");
    Serial.println(code);

    Serial.print("CH3(HEX)          : ");
    Serial.println(gAds.readSingleChannel(channel3), HEX);
    code = gAds.readSingleChannel(channel3);
    Serial.print("CODE(DEC)         : ");
    Serial.println(code);

    Serial.println("");
    Serial.println("####################################");
    Serial.println("");

    delay(1000);
    }
