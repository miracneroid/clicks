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

#include "hallcurrent6.h"

// ------------------------------------------------------------- PRIVATE MACROS 

#define HALLCURRENT6_DEVICE_SLAVE_ADDRESS  0x4D

// -------------------------------------------------------------- PRIVATE TYPES

static uint16_t averagin_data ( hallcurrent6_t *ctx );

// ------------------------------------------------ PUBLIC FUNCTION DEFINITIONS

void hallcurrent6_cfg_setup ( hallcurrent6_cfg_t *cfg )
{
    // Communication gpio pins 

    cfg->scl = HAL_PIN_NC;
    cfg->sda = HAL_PIN_NC;

    cfg->i2c_speed = I2C_MASTER_SPEED_STANDARD; 
    cfg->i2c_address = 0x4D;
}

HALLCURRENT6_RETVAL hallcurrent6_init ( hallcurrent6_t *ctx, hallcurrent6_cfg_t *cfg )
{
     i2c_master_config_t i2c_cfg;

    i2c_master_configure_default( &i2c_cfg );
    i2c_cfg.speed    = cfg->i2c_speed;
    i2c_cfg.scl      = cfg->scl;
    i2c_cfg.sda      = cfg->sda;

    ctx->slave_address = cfg->i2c_address;

    if (  i2c_master_open( &ctx->i2c, &i2c_cfg ) == I2C_MASTER_ERROR )
    {
        return HALLCURRENT6_INIT_ERROR;
    }

    i2c_master_set_slave_address( &ctx->i2c, ctx->slave_address );
    i2c_master_set_speed( &ctx->i2c, cfg->i2c_speed );

    return HALLCURRENT6_OK;
}

void hallcurrent6_generic_write ( hallcurrent6_t *ctx, uint8_t reg, uint8_t *data_buf, uint8_t len )
{
    uint8_t tx_buf[ 256 ];
    uint8_t cnt;
    
    tx_buf[ 0 ] = reg;
    
    for ( cnt = 1; cnt <= len; cnt++ )
    {
        tx_buf[ cnt ] = data_buf[ cnt - 1 ]; 
    }
    
    i2c_master_write( &ctx->i2c, tx_buf, len + 1 );    
}

void hallcurrent6_generic_read ( hallcurrent6_t *ctx, uint8_t reg, uint8_t *data_buf, uint8_t len )
{
    i2c_master_write_then_read( &ctx->i2c, &reg, 1, data_buf, len );
}

uint16_t hallcurrent6_read_data ( hallcurrent6_t *ctx )
{
    uint8_t read_reg[ 2 ];
    uint16_t r_data;

   
    hallcurrent6_generic_read ( ctx, 0x00, read_reg, 2 ); 

    r_data = read_reg[ 0 ];
    r_data = r_data << 8;
    r_data = r_data | read_reg[ 1 ];

    return r_data;
}

float hallcurrent6_get_current ( hallcurrent6_t *ctx )
{
    float current;
    float voltage;
    uint16_t sensor_value;
    uint8_t polarity = 1;

    sensor_value = averagin_data( ctx );

    if ( sensor_value < 2065 )
    {
        sensor_value = 2065 - sensor_value;
        polarity = 1;
    }
    else
    {
        sensor_value -= 2065;
        polarity = 0;
    }

    voltage = ( sensor_value  /  3700.0 )  *  4590.0;
    current  = ( voltage / 400.0 )  *  1000.0;  

    if ( polarity == 1 )
    {
        current = - current;
    }

    return current;
}

// ----------------------------------------------- PRIVATE FUNCTION DEFINITIONS

static uint16_t averagin_data ( hallcurrent6_t *ctx )
{
    uint32_t sum_data = 0;
    uint16_t new_data = 0;
    uint8_t  cnt = 0;
    
    for ( cnt = 0; cnt < 10; cnt++ )
    {
        
        new_data = hallcurrent6_read_data( ctx );
        sum_data = sum_data + new_data;
        
        Delay_10ms( );
        
        Delay_10ms( );
    }
    new_data = sum_data / 10;
    
    return new_data;
}

// ------------------------------------------------------------------------- END

