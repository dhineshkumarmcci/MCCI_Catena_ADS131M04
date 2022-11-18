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

/*cADS131M04::cADS131M04(int8_t chipSelectPin, int8_t clockOutPin, SPIClass* pSpi, int8_t clockChannel)
    {
    this->m_chipSelectPin = chipSelectPin;
    this->m_clockOutPin = clockOutPin;
    this->m_pSpi = pSpi;
    this->m_clockChannel = clockChannel;
    this->m_Initialized = false;
    }*/

bool cADS131M04::begin(SPIClass* pSpi, int8_t chipSelectPin, int8_t clockOutPin, int8_t clockChannel)
    {
	if (pSpi == NULL)
		{
		// invalid parameter
		Serial.println("pSpi is NULL");
		return false;
		}

    this->m_chipSelectPin = chipSelectPin;
    this->m_clockOutPin = clockOutPin;
    this->m_pSpi = pSpi;
    this->m_clockChannel = clockChannel;

    pinMode(this->m_chipSelectPin, OUTPUT);
    digitalWrite(this->m_chipSelectPin, HIGH);

    this->m_pSpi->begin();

    this->m_Initialized = true;

    return true;
    }

void cADS131M04::readChannels(int8_t *pChannel, int8_t nChannel, int32_t *pOutput)
    {
    uint32_t rawData[6];

    // Get data
    spiCommFrame(&rawData[0]);

    // Save the decoded data for each of the channels
    for (int8_t i = 0; i < nChannel; i++)
        {
        *pOutput = twoComplement(rawData[*pChannel + 1]);
        pOutput++;
        pChannel++;
        }
    }

int32_t cADS131M04::readSingleChannel(int8_t channelNumber)
    {
    int32_t pOutput[1];
    int8_t pChannel[1] = {channelNumber};

    readChannels(&pChannel[0], 1, &pOutput[0]);

    return pOutput[0];
    }

bool cADS131M04::setGain(uint8_t channelGain0, uint8_t channelGain1, uint8_t channelGain2, uint8_t channelGain3)
    {
    uint16_t gainCommand;
    bool result;

    gainCommand = channelGain3 << 4;
    gainCommand += channelGain2;
    gainCommand <<= 8;
    gainCommand += (channelGain1 <<4 );
    gainCommand += channelGain0;

    result = writeRegister((std::uint8_t)GBL_CH_SETTINGS_REG::GAIN, gainCommand);
    return result;
    }

bool cADS131M04::globalChop(bool enable, uint8_t chopDelay)
    {
    bool result;
    uint8_t delayData = chopDelay - 1;

    // Get current settings for current detect mode from the CFG register
    uint16_t currentSetting = (readRegister((std::uint8_t)GBL_CH_SETTINGS_REG::CONFIGURE) << 8) >>8;

    uint16_t newData = (delayData << 12) + (enable << 8) + currentSetting;

    result = writeRegister((std::uint8_t)GBL_CH_SETTINGS_REG::CONFIGURE, newData);
    return result;
    }

uint16_t cADS131M04::readRegister(uint8_t registerAddr)
    {
    //uint8_t commandPrefix = CommandPrefix::READ;
    // uint16_t commandWord = (int(CommandPrefix::READ) << 12) + (registerAddr << 7);
    uint16_t commandWord = (int)(Command::Read) | (registerAddr << 7) | 0;

    uint32_t pBuffer[6];

    // Use first frame to send command
    spiCommFrame(&pBuffer[0], commandWord);

    // Read response
    spiCommFrame(&pBuffer[0]);

    return pBuffer[0] >> 16;
    }

bool cADS131M04::writeRegister(uint8_t registerAddr, uint16_t data)
    {
    //uint8_t commandPrefix = CommandPrefix::WRITE;
    // uint16_t commandWord = (int(CommandPrefix::WRITE) << 12) + (registerAddr << 7);

    uint16_t commandWord = (int)(Command::Write) | (registerAddr << 7) | 0;

    digitalWrite(this->m_chipSelectPin, LOW);
    this->m_pSpi->beginTransaction(SPISettings((std::uint32_t)SpeedSettings::SerialClock, MSBFIRST, (std::uint8_t)SPI_MODE1));

    spiTransferWord(commandWord);

    spiTransferWord(data);

    // Send 4 empty words
    for (uint8_t i = 0; i < 4; i++)
        {
        spiTransferWord();
        }

    this->m_pSpi->endTransaction();
    digitalWrite(this->m_chipSelectPin, HIGH);

    // Get response
    uint32_t pBuffer[6];
    spiCommFrame(&pBuffer[0]);

    //commandPrefix = CommandResponse::WRITE;
    if (((int)(Command::Write) | (registerAddr << 7) | 0) == pBuffer[0])
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
    *pOutput = spiTransferWord(command);

    // For the next 4 words, just read the data
    for (uint8_t i = 1; i < 5; i++)
        {
        pOutput++;
        *pOutput = spiTransferWord() >> 8;
        }

    // Save CRC bits
    pOutput++;
    *pOutput = spiTransferWord();

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