/*
 * MikroSDK - MikroE Software Development Kit
 * Copyright© 2020 MikroElektronika d.o.o.
 * 
 * Permission is hereby granted, free of charge, to any person 
 * obtaining a copy of this software and associated documentation 
 * files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, 
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 * OR OTHER DEALINGS IN THE SOFTWARE. 
 */

/*!
 * \file
 *
 */

#include "mcp16331.h"

// ------------------------------------------------------------- PRIVATE MACROS 

#define MCP16331_DUMMY 0

// ----------------------------------------------- PRIVATE FUNCTION DECLARATION 

static void dac_output ( mcp16331_t *ctx, uint16_t value_dac );

// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

void mcp16331_cfg_setup ( mcp16331_cfg_t *cfg )
{
    // Communication gpio pins 

    cfg->sck = HAL_PIN_NC;
    cfg->miso = HAL_PIN_NC;
    cfg->mosi = HAL_PIN_NC;
    cfg->cs = HAL_PIN_NC;

    // Additional gpio pins

    cfg->adc_out = HAL_PIN_NC;
    cfg->en = HAL_PIN_NC;

    cfg->spi_speed = 100000; 
    cfg->spi_mode = SPI_MASTER_MODE_0;
    cfg->cs_polarity = SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_LOW;
}

MCP16331_RETVAL mcp16331_init ( mcp16331_t *ctx, mcp16331_cfg_t *cfg )
{
    spi_master_config_t spi_cfg;

    spi_master_configure_default( &spi_cfg );
    spi_cfg.speed     = cfg->spi_speed;
    spi_cfg.sck       = cfg->sck;
    spi_cfg.miso      = cfg->miso;
    spi_cfg.mosi      = cfg->mosi;
    spi_cfg.default_write_data = MCP16331_DUMMY;

    digital_out_init( &ctx->cs, cfg->cs );
    ctx->chip_select = cfg->cs;

    if (  spi_master_open( &ctx->spi, &spi_cfg ) == SPI_MASTER_ERROR )
    {
        return MCP16331_INIT_ERROR;
    }

    spi_master_set_default_write_data( &ctx->spi, MCP16331_DUMMY );
    spi_master_set_speed( &ctx->spi, cfg->spi_speed );
    spi_master_set_mode( &ctx->spi, cfg->spi_mode );
    spi_master_set_chip_select_polarity( cfg->cs_polarity );

    // Output pins 
    
    digital_out_init( &ctx->en, cfg->en );
    digital_out_high( &ctx->en );


    // Input pins

    digital_in_init( &ctx->adc_out, cfg->adc_out );

    return MCP16331_OK;
}

void mcp16331_generic_transfer 
( 
    mcp16331_t *ctx, 
    uint8_t *wr_buf, 
    uint16_t wr_len, 
    uint8_t *rd_buf, 
    uint16_t rd_len 
)
{
    spi_master_select_device( ctx->chip_select );
    spi_master_write_then_read( &ctx->spi, wr_buf, wr_len, rd_buf, rd_len );
    spi_master_deselect_device( ctx->chip_select );    
}

void mcp16331_set_vout ( mcp16331_t *ctx, uint16_t millivolts_vout )
{
     uint16_t temp = millivolts_vout;
     
     temp -= 2000;                  // 2000 mV is 0, 12000 mV is 10000
     temp /= 245;
     temp *= 100;

     temp = 4095 - temp;
     dac_output( ctx, temp );
}

// ------------------------------------------------------------ PRIVATE FUNCTION 

static void dac_output ( mcp16331_t *ctx, uint16_t value_dac )
{
    uint8_t temp;
    uint8_t tmp[ 2 ];
    
    tmp[ 0 ] = value_dac >> 8;
    tmp[ 0 ] |= 0x30;
    tmp[ 1 ] |= value_dac;

    spi_master_select_device( ctx->chip_select );               
    spi_master_write( &ctx->spi, tmp, 2 );
    spi_master_deselect_device( ctx->chip_select );                 
}

// ------------------------------------------------------------------------- END

