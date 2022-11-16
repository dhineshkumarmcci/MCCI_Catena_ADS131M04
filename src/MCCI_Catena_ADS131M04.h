/*

Module: MCCI_Catena_ADS131M04.h

Function:
    Top-level include file for MCCI Catena ADS131M04 Four-channel, 24-bit, 64-kSPS, simultaneous-sampling, delta-sigma ADC library.

Copyright and License:
    See accompanying LICENSE file for copyright and license information.

Author:
    Pranau R, MCCI Corporation   October 2022

*/

/// \file

#ifndef _MCCI_CATENA_ADS131M04_H_
#define _MCCI_CATENA_ADS131M04_H_ /* prevent multiple includes */

#pragma once

#include <cstdint>
#include <SPI.h>

/// \brief namespace for this library
namespace McciCatenaAds131m04 {

/// \brief create a version constant uint32_t
static constexpr std::uint32_t
makeVersion(
    std::uint8_t major, std::uint8_t minor, std::uint8_t patch, std::uint8_t local = 0
    )
    {
    return ((std::uint32_t)major << 24u) | ((std::uint32_t)minor << 16u) | ((std::uint32_t)patch << 8u) | (std::uint32_t)local;
    }

/// \brief extract major number from version
static constexpr std::uint8_t
getMajor(std::uint32_t v)
    {
    return std::uint8_t(v >> 24u);
    }

/// \brief extract minor number from version
static constexpr std::uint8_t
getMinor(std::uint32_t v)
    {
    return std::uint8_t(v >> 16u);
    }

/// \brief extract patch number from version
static constexpr std::uint8_t
getPatch(std::uint32_t v)
    {
    return std::uint8_t(v >> 8u);
    }

/// \brief extract local number from version
static constexpr std::uint8_t
getLocal(std::uint32_t v)
    {
    return std::uint8_t(v);
    }

/// \brief version of library, for use by clients in static_asserts
static constexpr std::uint32_t kVersion = makeVersion(1,0,0,1);

/// \brief instance object for ADS131M04
class cADS131M04
    {
private:
    static constexpr bool kfDebug = false;

public:
    cADS131M04(int8_t chipSelectPin, int8_t clockPin, SPIClass* pSpi, int8_t clockChannel = 1);

    // neither copyable nor movable
    cADS131M04(const cADS131M04&) = delete;
    cADS131M04& operator=(const cADS131M04&) = delete;
    cADS131M04(const cADS131M04&&) = delete;
    cADS131M04& operator=(const cADS131M04&&) = delete;

    /// \brief  Read only registers
    enum class READ_ONLY_REG : std::uint8_t
        {
        ID                                  = 0x00,
        STATUS                              = 0x01,
        };

    /// \brief  Registers Global Settings across channels
    enum class GBL_CH_SETTINGS_REG : std::uint8_t
        {
        MODE                                = 0x02,
        CLOCK                               = 0x03,
        GAIN                                = 0x04,
        CONFIGURE                           = 0x06,
        THRSHLD_MSB                         = 0x07,
        THRSHLD_LSB                         = 0x08,
        };

    /// \brief  Registers Channel 0 Specific Settings
    enum class CH0_SETTINGS_REG : std::uint8_t
        {
        CONFIGURE                           = 0x09,
        OCAL_MSB                            = 0x0A,
        OCAL_LSB                            = 0x0B,
        GCAL_MSB                            = 0x0C,
        GCAL_LSB                            = 0x0D,
        };

    /// \brief  Registers Channel 1 Specific Settings
    enum class CH1_SETTINGS_REG : std::uint8_t
        {
        CONFIGURE                           = 0x0E,
        OCAL_MSB                            = 0x0F,
        OCAL_LSB                            = 0x10,
        GCAL_MSB                            = 0x11,
        GCAL_LSB                            = 0x12,
        };

    /// \brief  Registers Channel 1 Specific Settings
    enum class CH2_SETTINGS_REG : std::uint8_t
        {
        CONFIGURE                           = 0x13,
        OCAL_MSB                            = 0x14,
        OCAL_LSB                            = 0x15,
        GCAL_MSB                            = 0x16,
        GCAL_LSB                            = 0x17,
        };

    /// \brief  Registers Channel 1 Specific Settings
    enum class CH3_SETTINGS_REG : std::uint8_t
        {
        CONFIGURE                           = 0x18,
        OCAL_MSB                            = 0x19,
        OCAL_LSB                            = 0x1A,
        GCAL_MSB                            = 0x1B,
        GCAL_LSB                            = 0x1C,
        };

    /// \brief  Registers MAP CRC
    enum class CRC_REG : std::uint8_t
        {
        MAP_CRC                             = 0x3E,
        };

    /// \brief  Registers reserved
    enum class RES_REG : std::uint8_t
        {
        RESERVED                            = 0x3E,
        };

    ///
    /// \brief Power up the ADS131M04 and start operation.
    ///
    /// \return
    ///     \c true for success, \c false for failure.
    ///
    bool begin();

    ///
    /// \brief reads all channels raw data.
    ///
    /// \param [out] pChannel points to the buffer to receive the data
    /// \param [in] nChannel is the number of bytes to read.
    /// \param [out] pOutput is a buffer to store the raw data.
    ///
    void readChannels(int8_t *pChannel, int8_t nChannel, int32_t *pOutput);

    ///
    /// \brief reads raw data from a single channel.
    ///
    /// \param [in] channelNumber is the channel's number to be read.
    ///
    /// \return
    ///     \c rawData from the specific channel
    ///
    int32_t readSingleChannel(int8_t channelNumber);


    ///
    /// \brief sets the gain for four channels of ADC
    ///
    /// \param [in] channelGain0 is channel 0 gain.
    /// \param [in] channelGain1 is channel 1 gain.
    /// \param [in] channelGain2 is channel 2 gain.
    /// \param [in] channelGain3 is channel 3 gain.
    ///
    /// \return
    ///     \c true for success, \c false for failure.
    ///
    bool setGain(uint8_t channelGain0 = 0, uint8_t channelGain1 = 0, uint8_t channelGain2 = 0, uint8_t channelGain3 = 0);


    ///
    /// \brief enable and configure global-chop settings
    ///
    /// \param [in] enable 'true' for enable, 'false' for disable.
    /// \param [in] chopDelay is a delay between two global-chop conversion.
    ///
    /// \return
    ///     \c true for success, \c false for failure.
    ///
    bool globalChop(bool enable = false, uint8_t chopDelay = 7);

protected:
    ///
    /// \brief read a byte from a given register.
    ///
    /// \param [in] registerAddr indicates the register to be read.
    ///
    /// \return
    ///     \c value of register that have been read.
    ///
    uint16_t readRegister(uint8_t registerAddr);

    ///
    /// \brief Write a byte to a given register.
    ///
    /// \param [in] registerAddr selects the register to write
    /// \param [in] data is the value to be written.
    ///
    /// \return
    ///     \c true for success, \c false for failure. The
    ///     last error is set in case of error.
    ///
    bool writeRegister(uint8_t registerAddr, uint16_t data);

private:
    int8_t m_chipSelectPin, m_clkoutPin, clockCh;
    SPIClass* m_pSpi;
    bool m_Initialized;

    ///
    /// \brief forms a SPI communication frame.
    ///
    /// \param [in] pOutput is used to save communication data.
    /// \param [in] command is command to be transfe.
    ///
    void spiCommFrame(uint32_t *pOutput, uint16_t command = 0x0000);

    ///
    /// \brief transfer a 24-bit word
    ///
    /// \param [in] inputData is data to be transfered.
    ///
    uint32_t spiTransferWord(uint16_t inputData = 0x0000);

    ///
    /// \brief performs two's complement of the data
    ///
    /// \param [in] data is data to be two's complimented.
    ///
    int32_t twoCompDeco(uint32_t data);
    };

} // end namespace McciCatenaAds131m04

#endif /* _MCCI_CATENA_ADS131M04_H_ */