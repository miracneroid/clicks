/*!
 * @file      lr1110_regmem.h
 *
 * @brief     Register/memory driver definition for LR1110
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

#ifndef LR1110_REGMEM_H
#define LR1110_REGMEM_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>
#include "common/lr1110_types.h"
#include "common/lr1110_hal.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Write words into register memory space of LR1110.
 *
 * A word is 32-bit long. The writing operations write contiguously in register memory, starting at the address
 * provided.
 *
 * @param [in] ctx Chip implementation context
 * @param [in] address The register memory address to start writing operation
 * @param [in] buffer The buffer of words to write into memory. Its size must be enough to contain length words.
 * @param [in] length Number of words to write into memory
 *
 * @returns Operation status
 *
 * @see lr1110_regmem_read_regmem32
 */
err_t lr1110_regmem_write_regmem32( lriot_t *ctx, uint32_t address, uint32_t* buffer, uint8_t length );

/*!
 * @brief Read words into register memory space of LR1110.
 *
 * A word is 32-bit long. The reading operations read contiguously from register memory, starting at the address
 * provided.
 *
 * @param [in] ctx Chip implementation context
 * @param [in] address The register memory address to start reading operation
 * @param [out] buffer Pointer to a words array to be filled with content from memory. Its size must be enough to
 * contain at least length words.
 * @param [in] length Number of words to read from memory
 *
 * @returns Operation status
 *
 * @see lr1110_regmem_write_regmem32
 */
err_t lr1110_regmem_read_regmem32( lriot_t *ctx, uint32_t address, uint32_t* buffer, uint8_t length );

/*!
 * @brief Write bytes into register memory space of LR1110.
 *
 * A byte is 8-bit long. The writing operations write contiguously in register memory, starting at the address provided.
 *
 * @param [in] ctx Chip implementation context
 * @param [in] address The register memory address to start writing operation
 * @param [in] buffer The buffer of bytes to write into memory. Its size must be enough to contain length bytes
 * @param [in] length Number of bytes to write into memory
 *
 * @returns Operation status
 *
 * @see lr1110_regmem_read_mem8
 */
err_t lr1110_regmem_write_mem8( lriot_t *ctx, uint32_t address, uint8_t* buffer, uint8_t length );

/*!
 * @brief Read bytes into register memory space of LR1110.
 *
 * A byte is 8-bit long. The reading operations read contiguously from register memory, starting at the address
 * provided.
 *
 * @param [in] ctx Chip implementation context
 * @param [in] address The register memory address to start reading operation
 * @param [in] buffer Pointer to a byte array to be filled with content from memory. Its size must be enough to contain
 * at least length bytes
 * @param [in] length Number of bytes to read from memory
 *
 * @returns Operation status
 *
 * @see lr1110_regmem_write_mem8
 */
err_t lr1110_regmem_read_mem8( lriot_t *ctx, uint32_t address, uint8_t* buffer, uint8_t length );

/*!
 * @brief Write bytes into radio TX buffer memory space of LR1110.
 *
 * @param [in] ctx Chip implementation context
 * @param [in] data_ The buffer of bytes to write into radio buffer. Its size must be enough to contain length bytes
 * @param [in] length Number of bytes to write into radio buffer
 *
 * @returns Operation status
 *
 * @see lr1110_regmem_read_buffer8
 */
err_t lr1110_regmem_write_buffer8( lriot_t *ctx, uint8_t* buffer, uint8_t length );

/*!
 * @brief Read bytes from radio RX buffer memory space of LR1110.
 *
 * @param [in] ctx Chip implementation context
 * @param [out] buffer Pointer to a byte array to be filled with content from radio buffer. Its size must be enough to
 * contain at least length bytes
 * @param [in] offset Memory offset to start reading
 * @param [in] length Number of bytes to read from radio buffer
 *
 * @returns Operation status
 *
 * @see lr1110_regmem_write_buffer8
 */
err_t lr1110_regmem_read_buffer8( lriot_t *ctx, uint8_t* buffer, uint8_t offset, uint8_t length );

/*!
 * @brief Clear radio RX buffer
 *
 * Set to 0x00 all content of the radio RX buffer
 *
 * @param [in] ctx Chip implementation context
 *
 * @returns Operation status
 */
err_t lr1110_regmem_clear_rxbuffer( lriot_t *ctx );

/*!
 * @brief Read-modify-write data at given register/memory address
 *
 * @param [in] ctx Chip implementation context
 * @param [in] address The register memory address to be modified
 * @param [in] mask The mask to be applied on read data
 * @param [in] data_buf The data to be written
 *
 * @returns Operation status
 */
err_t lr1110_regmem_write_regmem32_mask( lriot_t *ctx, uint32_t address, uint32_t mask, uint32_t data_buf );

#ifdef __cplusplus
}
#endif

#endif  // LR1110_REGMEM_H

/* --- EOF ------------------------------------------------------------------ */
