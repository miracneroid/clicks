/*!
 * @file      lr1110_hal.h
 *
 * @brief     Hardware Abstraction Layer (HAL) interface for LR1110
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LR1110_HAL_H
#define LR1110_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>
#include "lriot.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/**
 * @brief Write this to SPI bus while reading data, or as a dummy/placeholder
 */
#define LR1110_NOP ( 0x00 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Radio data transfer - write
 *
 * @param [in] ctx              Radio implementation parameters
 * @param [in] command          Pointer to the buffer to be transmitted
 * @param [in] command_length   Buffer size to be transmitted
 * @param [in] data_buf         Pointer to the buffer to be transmitted
 * @param [in] data_length      Buffer size to be transmitted
 *
 * @returns Operation status
 */
err_t lr1110_hal_write( lriot_t *ctx, uint8_t* command, uint16_t command_length,
                                      uint8_t* data_buf, uint16_t data_length );

/*!
 * @brief Radio data transfer - read
 *
 * @remark This is a two-step radio read operation. It consists of writing the command, releasing then re-asserting the
 * NSS line, then reading a discarded dummy byte followed by data_length bytes of response data from the transceiver.
 * While reading the dummy bytes and the response data, the implementation of this function must ensure that only zero
 * bytes (NOP) are written to the SPI bus.
 *
 * @param [in] ctx              Radio implementation parameters
 * @param [in] command          Pointer to the buffer to be transmitted
 * @param [in] command_length   Buffer size to be transmitted
 * @param [out] data_buf        Pointer to the buffer to be received
 * @param [in] data_length      Buffer size to be received
 *
 * @returns Operation status
 *
 * @remark Some hardware SPI implementations write arbitary values on the MOSI line while reading. If this is done on
 * the LR1110, non-zero values may be interpreted as commands. This driver does not exploit this functionality, and
 * expects that zeros be sent on the MOSI line when this command is reading the command response data.
 */
err_t lr1110_hal_read( lriot_t *ctx, uint8_t* command, uint16_t command_length,
                                     uint8_t* data_buf, uint16_t data_length );

/*!
 * @brief  Direct read from the SPI bus
 *
 * @remark Unlike @ref lr1110_hal_read, this is a simple direct SPI bus SS/read/nSS operation. While reading the
 * response data, the implementation of this function must ensure that only zero bytes (NOP) are written to the SPI bus.
 *
 * @remark Formerly, that function depended on a lr1110_hal_write_read API function, which required bidirectional SPI
 * communication. Given that all other radio functionality can be implemented with unidirectional SPI, it has been
 * decided to make this HAL API change to simplify implementation requirements.
 *
 * @remark Only required by the @ref lr1110_system_get_status and @ref lr1110_bootloader_get_status commands
 *
 * @param [in]  ctx          Radio implementation parameters
 * @param [out] data_buf     Pointer to the buffer to be received
 * @param [in]  data_length  Buffer size to be received
 *
 * @returns Operation status
 */
err_t lr1110_hal_direct_read( lriot_t *ctx, uint8_t* data_buf, uint16_t data_length );

/*!
 * @brief Reset the radio
 *
 * @param [in] ctx Radio implementation parameters
 *
 * @returns Operation status
 */
err_t lr1110_hal_reset( lriot_t *ctx );

/*!
 * @brief Wake the radio up.
 *
 * @param [in] ctx Radio implementation parameters
 *
 * @returns Operation status
 */
err_t lr1110_hal_wakeup( lriot_t *ctx );

#ifdef __cplusplus
}
#endif

#endif  // LR1110_HAL_H
