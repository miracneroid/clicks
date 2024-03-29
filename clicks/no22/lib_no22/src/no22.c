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

#include "no22.h"

// ------------------------------------------------------------- PRIVATE MACROS 

#define NO22_DUMMY 0

// ---------------------------------------------- PRIVATE FUNCTION DECLARATIONS 

void no22_generic_transfer 
( 
    no22_t *ctx, 
    uint8_t *wr_buf, 
    uint16_t wr_len, 
    uint8_t *rd_buf, 
    uint16_t rd_len 
);

// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

void no22_cfg_setup ( no22_cfg_t *cfg )
{
    // Communication gpio pins 

    cfg->sck = HAL_PIN_NC;
    cfg->miso = HAL_PIN_NC;
    cfg->mosi = HAL_PIN_NC;
    cfg->cs = HAL_PIN_NC;

    // Additional gpio pins

    cfg->pht = HAL_PIN_NC;

    cfg->spi_speed = 100000; 
    cfg->spi_mode = SPI_MASTER_MODE_0;
    cfg->cs_polarity = SPI_MASTER_CHIP_SELECT_POLARITY_ACTIVE_LOW;
}

NO22_RETVAL no22_init ( no22_t *ctx, no22_cfg_t *cfg )
{
    spi_master_config_t spi_cfg;

    spi_master_configure_default( &spi_cfg );
    spi_cfg.speed     = cfg->spi_speed;
    spi_cfg.sck       = cfg->sck;
    spi_cfg.miso      = cfg->miso;
    spi_cfg.mosi      = cfg->mosi;
    spi_cfg.default_write_data = NO22_DUMMY;

    digital_out_init( &ctx->cs, cfg->cs );
    ctx->chip_select = cfg->cs;

    if (  spi_master_open( &ctx->spi, &spi_cfg ) == SPI_MASTER_ERROR )
    {
        return NO22_INIT_ERROR;
    }

    spi_master_set_default_write_data( &ctx->spi, NO22_DUMMY );
    spi_master_set_speed( &ctx->spi, cfg->spi_speed );
    spi_master_set_mode( &ctx->spi, cfg->spi_mode );
    spi_master_set_chip_select_polarity( cfg->cs_polarity );

    // Output pins 

    digital_out_init( &ctx->pht, cfg->pht );

    return NO22_OK;

}

uint16_t no22_read_data ( no22_t *ctx )
{
    uint8_t rx_data[ 2 ];
    uint8_t temp_data = 0;

    spi_master_select_device( ctx->chip_select );
    spi_master_read( &ctx->spi, &rx_data[ 0 ], 1 );
    spi_master_read( &ctx->spi, &rx_data[ 1 ], 1 );
    spi_master_deselect_device( ctx->chip_select ); 

    temp_data = rx_data[ 0 ];
    temp_data <<= 8;
    temp_data |= rx_data[ 1 ];

    temp_data &= 0x1FFF;
    temp_data >>= 1;

    return temp_data;
}

float no22_get_ppb ( no22_t *ctx )
{
    uint16_t adc_data;
    float temp;

    adc_data = no22_read_data( ctx );

    temp = adc_data / 4095.0;
    temp *= 1000;

    return temp;
}

uint8_t no22_set_pht_state ( no22_t *ctx, uint8_t state )
{
    if ( ( state == NO22_PIN_STATE_HIGH ) || ( state == NO22_PIN_STATE_LOW ) )
    {
        digital_out_write( &ctx->pht, state );
        
        return NO22_DEVICE_OK;
    }
    else
    {
        return NO22_DEVICE_ERROR;
    }
}

// ----------------------------------------------- PRIVATE FUNCTION DEFINITIONS

void no22_generic_transfer 
( 
    no22_t *ctx, 
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

// ------------------------------------------------------------------------- END

