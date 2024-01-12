/****************************************************************************
** Copyright (C) 2020 MikroElektronika d.o.o.
** Contact: https://www.mikroe.com/contact
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
** DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
**  USE OR OTHER DEALINGS IN THE SOFTWARE.
****************************************************************************/

/*!
 * @file neomesh915mhz.c
 * @brief NeoMesh 915MHz Click Driver.
 */

#include "neomesh915mhz.h"

void neomesh915mhz_cfg_setup ( neomesh915mhz_cfg_t *cfg ) 
{
    // Communication gpio pins
    cfg->rx_pin = HAL_PIN_NC;
    cfg->tx_pin = HAL_PIN_NC;

    // Additional gpio pins
    cfg->app = HAL_PIN_NC;
    cfg->rst = HAL_PIN_NC;
    cfg->wes = HAL_PIN_NC;
    cfg->cts = HAL_PIN_NC;
    cfg->wup = HAL_PIN_NC;

    cfg->baud_rate     = 115200;
    cfg->data_bit      = UART_DATA_BITS_DEFAULT;
    cfg->parity_bit    = UART_PARITY_DEFAULT;
    cfg->stop_bit      = UART_STOP_BITS_DEFAULT;
    cfg->uart_blocking = false;
}

err_t neomesh915mhz_init ( neomesh915mhz_t *ctx, neomesh915mhz_cfg_t *cfg ) 
{
    uart_config_t uart_cfg;

    // Default config
    uart_configure_default( &uart_cfg );

    // Ring buffer mapping
    ctx->uart.tx_ring_buffer = ctx->uart_tx_buffer;
    ctx->uart.rx_ring_buffer = ctx->uart_rx_buffer;

    // UART module config
    uart_cfg.rx_pin = cfg->rx_pin;  // UART RX pin.
    uart_cfg.tx_pin = cfg->tx_pin;  // UART TX pin.
    uart_cfg.tx_ring_size = sizeof( ctx->uart_tx_buffer );
    uart_cfg.rx_ring_size = sizeof( ctx->uart_rx_buffer );

    if ( UART_ERROR == uart_open( &ctx->uart, &uart_cfg ) ) 
    {
        return UART_ERROR;
    }
    uart_set_baud( &ctx->uart, cfg->baud_rate );
    uart_set_parity( &ctx->uart, cfg->parity_bit );
    uart_set_stop_bits( &ctx->uart, cfg->stop_bit );
    uart_set_data_bits( &ctx->uart, cfg->data_bit );

    uart_set_blocking( &ctx->uart, cfg->uart_blocking );

    // Output pins
    digital_out_init( &ctx->rst, cfg->rst );
    digital_out_init( &ctx->wes, cfg->wes );
    digital_out_low( &ctx->rst );
    digital_out_low( &ctx->wes );

    // Input pins
    digital_in_init( &ctx->app, cfg->app );
    digital_in_init( &ctx->cts, cfg->cts );
    digital_in_init( &ctx->wup, cfg->wup );
    
    // Dummy read to enable RX interrupt
    uint8_t dummy = 0;
    uart_read( &ctx->uart, &dummy, 1 );
    
    Delay_100ms ( );
    
    digital_out_high( &ctx->rst );
    digital_out_high( &ctx->wes );

    Delay_1sec ( );
    Delay_1sec ( );

    return NEOMESH915MHZ_OK;
}

err_t neomesh915mhz_send_aapi_frame ( neomesh915mhz_t *ctx, neomesh915mhz_aapi_frame_t *frame )
{
    uint32_t timeout_cnt = 0;
    // Wait for CTS indication flag
    while ( neomesh915mhz_get_cts_pin ( ctx ) )
    {
        if ( ++timeout_cnt > ( NEOMESH915MHZ_TIMEOUT_MS * 20 ) )
        {
            return NEOMESH915MHZ_TIMEOUT;
        }
        Delay_50us ( );
    }
    uart_clear( &ctx->uart );
    // Send AAPI cmd byte
    uart_write( &ctx->uart, &frame->cmd, 1 );
    // Send AAPI payload len
    uart_write( &ctx->uart, &frame->len, 1 );
    if ( frame->len )
    {
        // Send AAPI payload
        uart_write( &ctx->uart, frame->payload, frame->len );
    }
    Delay_10ms ( );
    return NEOMESH915MHZ_OK;
}

err_t neomesh915mhz_read_aapi_frame ( neomesh915mhz_t *ctx, neomesh915mhz_aapi_frame_t *frame )
{
    uint32_t timeout_cnt = 0;
    err_t error_flag = NEOMESH915MHZ_OK;
    // Wait for WUP indication flag
    while ( neomesh915mhz_get_wup_pin ( ctx ) )
    {
        if ( ++timeout_cnt > ( NEOMESH915MHZ_TIMEOUT_MS * 20 ) )
        {
            return NEOMESH915MHZ_TIMEOUT;
        }
        Delay_50us ( );
    }
    Delay_10ms ( );
    // Read AAPI cmd byte
    if ( 1 != uart_read( &ctx->uart, &frame->cmd, 1 ) )
    {
        error_flag = NEOMESH915MHZ_ERROR;
    }
    // Read AAPI payload len
    if ( ( NEOMESH915MHZ_OK == error_flag ) && 
         ( 1 != uart_read( &ctx->uart, &frame->len, 1 ) ) )
    {
        error_flag = NEOMESH915MHZ_ERROR;
    }
    if ( ( NEOMESH915MHZ_OK == error_flag ) && frame->len )
    {
        // Read AAPI payload
        if ( frame->len != uart_read( &ctx->uart, &frame->payload, frame->len ) )
        {
            error_flag = NEOMESH915MHZ_ERROR;
        }
    }
    return error_flag;
}

err_t neomesh915mhz_send_sapi_frame ( neomesh915mhz_t *ctx, neomesh915mhz_sapi_frame_t *frame )
{
    uint32_t timeout_cnt = 0;
    uint8_t data_wr = 0;
    // Wait for CTS indication flag
    while ( neomesh915mhz_get_cts_pin ( ctx ) )
    {
        if ( ++timeout_cnt > ( NEOMESH915MHZ_TIMEOUT_MS * 20 ) )
        {
            return NEOMESH915MHZ_TIMEOUT;
        }
        Delay_50us ( );
    }
    uart_clear( &ctx->uart );
    // Send SAPI header byte
    data_wr = NEOMESH915MHZ_SAPI_HEADER;
    uart_write( &ctx->uart, &data_wr, 1 );
    // Send SAPI len byte
    data_wr = frame->len + 3;
    uart_write( &ctx->uart, &data_wr, 1 );
    // Send SAPI cmd bytes
    data_wr = ( uint8_t ) ( ( frame->cmd >> 8 ) & 0xFF );
    uart_write( &ctx->uart, &data_wr, 1 );
    data_wr = ( uint8_t ) ( frame->cmd & 0xFF );
    uart_write( &ctx->uart, &data_wr, 1 );
    if ( frame->len )
    {
        // Send SAPI payload
        uart_write( &ctx->uart, frame->payload, frame->len );
    }
    // Send SAPI trail byte
    data_wr = NEOMESH915MHZ_SAPI_TRAIL;
    uart_write( &ctx->uart, &data_wr, 1 );
    Delay_10ms ( );
    return NEOMESH915MHZ_OK;
}

err_t neomesh915mhz_read_sapi_frame ( neomesh915mhz_t *ctx, neomesh915mhz_sapi_frame_t *frame )
{
    uint32_t timeout_cnt = 0;
    uint8_t data_rd = 0;
    err_t error_flag = NEOMESH915MHZ_OK;
    // Read and check SAPI header byte
    while ( ( 1 != uart_read( &ctx->uart, &data_rd, 1 ) ) || 
            ( ( NEOMESH915MHZ_SAPI_HEADER != data_rd ) ) )
    {
        if ( ++timeout_cnt > ( NEOMESH915MHZ_TIMEOUT_MS * 20 ) )
        {
            return NEOMESH915MHZ_TIMEOUT;
        }
        Delay_50us ( );
    }
    Delay_10ms ( );
    // Read and check SAPI len byte
    if ( ( 1 != uart_read( &ctx->uart, &data_rd, 1 ) ) || ( data_rd < 3 ) )
    {
        error_flag = NEOMESH915MHZ_ERROR;
    }
    else
    {
        frame->len = data_rd - 3;
    }
    if ( NEOMESH915MHZ_OK == error_flag )
    {
        // Read SAPI cmd byte 1
        if ( 1 != uart_read( &ctx->uart, &data_rd, 1 ) )
        {
            error_flag = NEOMESH915MHZ_ERROR;
        }
        else
        {
            frame->cmd = ( ( uint16_t ) data_rd << 8 );
        }
    }
    if ( NEOMESH915MHZ_OK == error_flag )
    {
        // Read SAPI cmd byte 2
        if ( 1 != uart_read( &ctx->uart, &data_rd, 1 ) )
        {
            error_flag = NEOMESH915MHZ_ERROR;
        }
        else
        {
            frame->cmd |= data_rd;
        }
    }
    if ( ( NEOMESH915MHZ_OK == error_flag ) && frame->len )
    {
        // Read SAPI payload
        if ( frame->len != uart_read( &ctx->uart, &frame->payload, frame->len ) )
        {
            error_flag = NEOMESH915MHZ_ERROR;
        }
    }
    if ( NEOMESH915MHZ_OK == error_flag )
    {
        // Read and check SAPI trail byte
        if ( ( 1 != uart_read( &ctx->uart, &data_rd, 1 ) ) || 
            ( ( NEOMESH915MHZ_SAPI_TRAIL != data_rd ) ) )
        {
            error_flag = NEOMESH915MHZ_ERROR;
        }
    }
    return error_flag;
}

void neomesh915mhz_set_rst_pin ( neomesh915mhz_t *ctx, uint8_t state )
{
    digital_out_write ( &ctx->rst, state );
}

void neomesh915mhz_set_wes_pin ( neomesh915mhz_t *ctx, uint8_t state )
{
    digital_out_write ( &ctx->wes, state );
}

uint8_t neomesh915mhz_get_app_pin ( neomesh915mhz_t *ctx )
{
    return digital_in_read ( &ctx->app );
}

uint8_t neomesh915mhz_get_cts_pin ( neomesh915mhz_t *ctx )
{
    return digital_in_read ( &ctx->cts );
}

uint8_t neomesh915mhz_get_wup_pin ( neomesh915mhz_t *ctx )
{
    return digital_in_read ( &ctx->wup );
}

// ------------------------------------------------------------------------- END
