/*

Module: MCCI_CATENA_ADS131M04.cpp

Function:
    Implementation code for MCCI Catena ADS131M04 Four-channel, 24-bit, 64-kSPS, simultaneous-sampling, delta-sigma ADC library.

Copyright and License:
    See accompanying LICENSE file for copyright and license information.

Author:
    Pranau R, MCCI Corporation   October 2022

*/

#include "MCCI_Catena_ADS131M04.h"
#include <SPI.h>

using namespace McciCatenaAds131m04;

bool cADS131M04::begin(SPIClass* pSpi, int8_t chipSelectPin, int8_t drdy)
    {
    if (pSpi == NULL)
        {
        // invalid parameter
        Serial.println("pSpi is NULL");
        return false;
        }

    this->m_chipSelectPin = chipSelectPin;
    this->m_pSpi = pSpi;
    this->m_drdy = drdy;

    this->m_pSpi->begin();

    pinMode(this->m_chipSelectPin, OUTPUT);
    digitalWrite(this->m_chipSelectPin, HIGH);

    pinMode(this->m_drdy, INPUT);

    this->m_Initialized = true;

    bool result = this->readID();
    return result;
    }

bool cADS131M04::readID()
    {
    uint16_t idData = this->readRegister((std::uint8_t)READ_ONLY_REG::ID);

    if (idData != (std::uint16_t)readID::ID)
        {
        return false;
        }
    else
        {
        return true;
        }
    }

bool cADS131M04::isDataReady()
    {
    if (digitalRead(this->m_drdy) == HIGH)
        {
        return false;
        }

    return true;
    }

void cADS131M04::readChannels(int8_t *pChannel, int8_t nChannel, int32_t *pOutput)
    {
    uint32_t rawData[6];

    // Get data
    this->spiCommFrame(&rawData[0]);

    // Save the decoded data for each of the channels
    for (int8_t i = 0; i < nChannel; i++)
        {
        *pOutput = this->twoComplement(rawData[*pChannel + 1]);
        pOutput++;
        pChannel++;
        }
    }

int32_t cADS131M04::readSingleChannel(uint8_t channelNumber)
    {
    int32_t pOutput[1];
    int8_t pChannel[1] = {channelNumber};

    this->readChannels(&pChannel[0], sizeof(pChannel), &pOutput[0]);

    return pOutput[0];
    }

float cADS131M04::readVoltage(uint8_t channelNumber)
    {
    int32_t code;
    float voltage;

    code = this->readSingleChannel(channelNumber);
    voltage = (code / (float)this->m_bits) * this->m_fsr;

    return voltage;
    }

float cADS131M04::readCO(uint8_t channelNumber)
    {
    float voltage;
    float gasConcentration;

    voltage = this->readVoltage(channelNumber);

    float Vgas = voltage - this->m_vGasZero;
    gasConcentration = this->m_calibrationFactorCO * Vgas;

    return gasConcentration;
    }

float cADS131M04::readNO2(uint8_t channelNumber)
    {
    float voltage;
    float gasConcentration;

    voltage = this->readVoltage(channelNumber);

    float Vgas = voltage - this->m_vGasZero;
    gasConcentration = this->m_calibrationFactorCO * Vgas;

    return gasConcentration;
    }

float cADS131M04::readO3(uint8_t channelNumber)
    {
    float voltage;
    float gasConcentration;

    voltage = this->readVoltage(channelNumber);

    float Vgas = voltage - this->m_vGasZero;
    gasConcentration = this->m_calibrationFactorCO * Vgas;

    return gasConcentration;
    }

float cADS131M04::readSO2(uint8_t channelNumber)
    {
    float voltage;
    float gasConcentration;

    voltage = this->readVoltage(channelNumber);

    float Vgas = voltage - this->m_vGasZero;
    gasConcentration = this->m_calibrationFactorCO * Vgas;

    return gasConcentration;
    }

bool cADS131M04::setGain(uint8_t channelGain0, uint8_t channelGain1, uint8_t channelGain2, uint8_t channelGain3)
    {
    uint16_t gainCommand;
    bool result;

    gainCommand = channelGain3 << 12;
    gainCommand = gainCommand | channelGain2;
    gainCommand = gainCommand << 8;
    gainCommand = gainCommand | channelGain1;
    gainCommand = gainCommand << 4;
    gainCommand = gainCommand | channelGain0;

    result = this->writeRegister((std::uint8_t)GBL_CH_SETTINGS_REG::GAIN, gainCommand);
    return result;
    }

bool cADS131M04::globalChop(bool enable, uint8_t chopDelay)
    {
    bool result;
    uint8_t delayData = chopDelay - 1;

    // Get current settings for current detect mode from the CFG register
    uint16_t currentSetting = (this->readRegister((std::uint8_t)GBL_CH_SETTINGS_REG::CONFIGURE) << 8) >>8;

    uint16_t newData = (delayData << 12) + (enable << 8) + currentSetting;

    result = this->writeRegister((std::uint8_t)GBL_CH_SETTINGS_REG::CONFIGURE, newData);
    return result;
    }

uint16_t cADS131M04::readRegister(uint8_t registerAddr)
    {
    uint16_t commandWord = (int)(Command::Read) | (registerAddr << 7) | 0;

    uint32_t pBuffer[6];

    // Use first frame to send command
    this->spiCommFrame(&pBuffer[0], commandWord);

    // Read response
    this->spiCommFrame(&pBuffer[0]);

    return pBuffer[0] >> 16;
    }

bool cADS131M04::writeRegister(uint8_t registerAddr, uint16_t data)
    {
    uint16_t commandWord = (int)(Command::Write) | (registerAddr << 7) | 0;

    digitalWrite(this->m_chipSelectPin, LOW);
    this->m_pSpi->beginTransaction(SPISettings((std::uint32_t)SpeedSettings::SerialClock, MSBFIRST, (std::uint8_t)SPI_MODE1));

    this->spiTransferWord(commandWord);

    this->spiTransferWord(data);

    // Send 4 empty words
    for (uint8_t i = 0; i < 4; i++)
        {
        this->spiTransferWord();
        }

    this->m_pSpi->endTransaction();
    digitalWrite(this->m_chipSelectPin, HIGH);

    // Get response
    uint32_t pBuffer[6];
    this->spiCommFrame(&pBuffer[0]);

    if (((int)(CommandResponse::Write) | (registerAddr << 7) | 0) == pBuffer[0])
        {
        return true;
        }
    else
        {
        return false;
        }
    }

void cADS131M04::spiCommFrame(uint32_t * pOutput, uint16_t command)
    {
    digitalWrite(this->m_chipSelectPin, LOW);

    this->m_pSpi->beginTransaction(SPISettings((std::uint32_t)SpeedSettings::SerialClock, MSBFIRST, (std::uint8_t)SPI_MODE1));

    // Send the command in the first word
    *pOutput = this->spiTransferWord(command);

    // For the next 4 words, just read the data
    for (uint8_t i = 1; i < 5; i++)
        {
        pOutput++;
        *pOutput = this->spiTransferWord() >> 8;
        }

    // Save CRC bits
    pOutput++;
    *pOutput = this->spiTransferWord();

    this->m_pSpi->endTransaction();

    digitalWrite(this->m_chipSelectPin, HIGH);
    }

uint32_t cADS131M04::spiTransferWord(uint16_t inputData)
    {
    uint32_t data = this->m_pSpi->transfer(inputData >> 8);
    data <<= 8;
    data |= this->m_pSpi->transfer((inputData<<8) >> 8);
    data <<= 8;
    data |= this->m_pSpi->transfer(0x00);

    return data << 8;
    }

int32_t cADS131M04::twoComplement(uint32_t data)
    {
    data <<= 8;
    int32_t intData = (int)data;

    return intData / pow(2,8);
    }
/**** end of MCCI_Catena_ADS131M04.cpp ****/