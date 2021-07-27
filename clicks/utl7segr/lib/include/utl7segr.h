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
 * @file utl7segr.h
 * @brief This file contains API for UT-L 7-SEG R Click Driver.
 */

#ifndef UTL7SEGR_H
#define UTL7SEGR_H

#ifdef __cplusplus
extern "C"{
#endif

#include "drv_digital_out.h"
#include "drv_spi_master.h"

/*!
 * @addtogroup utl7segr UT-L 7-SEG R Click Driver
 * @brief API for configuring and manipulating UT-L 7-SEG R Click driver.
 * @{
 */

/**
 * @defgroup utl7segr_set UT-L 7-SEG R Registers Settings
 * @brief Settings for registers of UT-L 7-SEG R Click driver.
 */

/**
 * @addtogroup utl7segr_set
 * @{
 */

/**
 * @brief UT-L 7-SEG R description setting.
 * @details Specified setting for description of UT-L 7-SEG R Click driver.
 */
#define UTL7SEGR_DOT_LEFT              0x02
#define UTL7SEGR_DOT_RIGHT             0x01
#define UTL7SEGR_DOT_LEFT_RIGHT        0x03
#define UTL7SEGR_NO_DOT                0x00

#define UTL7SEGR_DISPLAY_ON            0x01
#define UTL7SEGR_DISPLAY_OFF           0x00

/*! @} */ // utl7segr_set

/**
 * @defgroup utl7segr_map UT-L 7-SEG R MikroBUS Map
 * @brief MikroBUS pin mapping of UT-L 7-SEG R Click driver.
 */

/**
 * @addtogroup utl7segr_map
 * @{
 */

/**
 * @brief MikroBUS pin mapping.
 * @details Mapping pins of UT-L 7-SEG R Click to the selected MikroBUS.
 */
#define UTL7SEGR_MAP_MIKROBUS( cfg, mikrobus ) \
    cfg.miso = MIKROBUS( mikrobus, MIKROBUS_MISO ); \
    cfg.mosi = MIKROBUS( mikrobus, MIKROBUS_MOSI ); \
    cfg.sck  = MIKROBUS( mikrobus, MIKROBUS_SCK ); \
    cfg.cs   = MIKROBUS( mikrobus, MIKROBUS_CS ); \
    cfg.oe   = MIKROBUS( mikrobus, MIKROBUS_PWM ) 

/*! @} */ // utl7segr_map
/*! @} */ // utl7segr

/**
 * @brief UT-L 7-SEG R Click context object.
 * @details Context object definition of UT-L 7-SEG R Click driver.
 */
   

typedef struct
{
    // Output pins

    digital_out_t  oe;                                /**< Display state pin. */

    // Modules

    spi_master_t  spi;                               /**< SPI driver object. */

    pin_name_t  chip_select;                         /**< Chip select pin descriptor (used for SPI driver). */

} utl7segr_t;

/**
 * @brief UT-L 7-SEG R Click configuration object.
 * @details Configuration object definition of UT-L 7-SEG R Click driver.
 */
typedef struct
{
    // Communication gpio pins

    pin_name_t  miso;                                    /**< Master input - slave output pin descriptor for SPI driver. */
    pin_name_t  mosi;                                    /**< Master output - slave input pin descriptor for SPI driver. */
    pin_name_t  sck;                                     /**< Clock pin descriptor for SPI driver. */
    pin_name_t  cs;                                      /**< Chip select pin descriptor for SPI driver. */

    // Additional gpio pins

    pin_name_t  oe;                                      /**< Display state pin. */

    // static variable

    uint32_t                           spi_speed;        /**< SPI serial speed. */
    spi_master_mode_t                  spi_mode;         /**< SPI master mode. */
    spi_master_chip_select_polarity_t  cs_polarity;      /**< Chip select pin polarity. */

} utl7segr_cfg_t;

/**
 * @brief UT-L 7-SEG R Click return value data.
 * @details Predefined enum values for driver return values.
 */
typedef enum
{
   UTL7SEGR_OK = 0,
   UTL7SEGR_ERROR = -1

} utl7segr_return_value_t;

/*!
 * @addtogroup utl7segr UT-L 7-SEG R Click Driver
 * @brief API for configuring and manipulating UT-L 7-SEG R Click driver.
 * @{
 */

/**
 * @brief UT-L 7-SEG R configuration object setup function.
 * @details This function initializes click configuration structure to initial
 * values.
 * @param[out] cfg : Click configuration structure.
 * See #utl7segr_cfg_t object definition for detailed explanation.
 * @return Nothing.
 * @note The all used pins will be set to unconnected state.
 */
void utl7segr_cfg_setup ( utl7segr_cfg_t *cfg );

/**
 * @brief UT-L 7-SEG R initialization function.
 * @details This function initializes all necessary pins and peripherals used
 * for this click board.
 * @param[out] ctx : Click context object.
 * See #utl7segr_t object definition for detailed explanation.
 * @param[in] cfg : Click configuration structure.
 * See #utl7segr_cfg_t object definition for detailed explanation.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t utl7segr_init ( utl7segr_t *ctx, utl7segr_cfg_t *cfg );

/**
 * @brief UT-L 7-SEG R default configuration function.
 * @details This function executes a default configuration of UT-L 7-SEG R
 * click board.
 * @param[in] ctx : Click context object.
 * See #utl7segr_t object definition for detailed explanation.
 * @return Nothing.
 * 
 * @note This function can consist any necessary configuration or setting to put
 * device into operating mode.
 */
void utl7segr_default_cfg ( utl7segr_t *ctx );

/**
 * @brief UT-L 7-SEG R data writing function.
 * @details This function writes a desired number of data bytes starting from
 * the selected register by using SPI serial interface.
 * @param[in] ctx : Click context object.
 * See #utl7segr_t object definition for detailed explanation.
 * @param[in] data_in : Data to be written.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t utl7segr_generic_write ( utl7segr_t *ctx, uint8_t *data_in );

/**
 * @brief UT-L 7-SEG R display state function.
 * @details This function turns display on and off.
 * @param[in] ctx : Click context object.
 * See #utl7segr_t object definition for detailed explanation.
 * @param[in] state : 
 *                   @li @c 0 ( UTL7SEGR_DISPLAY_OFF ) - Display off,
 *                   @li @c 1 ( UTL7SEGR_DISPLAY_ON  ) - Display on.
 * @return Nothing.
 *
 * @note None.
 */
void utl7segr_display_state ( utl7segr_t *ctx, uint8_t state ) ;

/**
 * @brief UT-L 7-SEG R display number function.
 * @details This function is used to show the number on the display.
 * @param[in] ctx : Click context object.
 * See #utl7segr_t object definition for detailed explanation.
 * @param[in] number  :  Number from 0 to 99.
 * @param[in] dot_pos :  Dot position (No use dot, dot left, dot right and dot
 * left and right).                
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #err_t definition for detailed explanation.
 * @note None.
 */
err_t utl7segr_display_number ( utl7segr_t *ctx, uint8_t number, uint8_t dot_pos );

#ifdef __cplusplus
}
#endif
#endif // UTL7SEGR_H

/*! @} */ // utl7segr

// ------------------------------------------------------------------------ END