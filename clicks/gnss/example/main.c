/*!
 * @file main.c
 * @brief GNSS Click Example.
 *
 * # Description
 * This example demonstrates the use of GNSS click by reading and displaying
 * the GPS coordinates.
 *
 * The demo application is composed of two sections :
 *
 * ## Application Init
 * Initializes the driver and logger.
 *
 * ## Application Task
 * Reads the received data, parses the GPGGA info from it, and once it receives the position fix
 * it will start displaying the coordinates on the USB UART.
 *
 * ## Additional Function
 * - static void gnss_clear_app_buf ( void )
 * - static err_t gnss_process ( gnss_t *ctx )
 * - static void gnss_parser_application ( char *rsp )
 * 
 * @author Stefan Filipovic
 *
 */

#include "board.h"
#include "log.h"
#include "gnss.h"
#include "string.h"

#define PROCESS_BUFFER_SIZE 200

static gnss_t gnss;
static log_t logger;

static char app_buf[ PROCESS_BUFFER_SIZE ] = { 0 };
static int32_t app_buf_len = 0;

/**
 * @brief GNSS clearing application buffer.
 * @details This function clears memory of application buffer and reset its length.
 * @return None.
 * @note None.
 */
static void gnss_clear_app_buf ( void );

/**
 * @brief GNSS data reading function.
 * @details This function reads data from device and concatenates data to application buffer.
 * @param[in] ctx : Click context object.
 * See #gnss_t object definition for detailed explanation.
 * @return @li @c  0 - Read some data.
 *         @li @c -1 - Nothing is read.
 * See #err_t definition for detailed explanation.
 * @note None.
 */
static err_t gnss_process ( gnss_t *ctx );

/**
 * @brief GNSS parser application function.
 * @details This function parses GNSS data and logs it on the USB UART. It clears app and ring buffers
 * after successfully parsing data.
 * @param[in] ctx : Click context object.
 * See #gnss_t object definition for detailed explanation.
 * @param[in] rsp Response buffer.
 * @return None.
 * @note None.
 */
static void gnss_parser_application ( gnss_t *ctx, char *rsp );

void application_init ( void ) 
{
    log_cfg_t log_cfg;  /**< Logger config object. */
    gnss_cfg_t gnss_cfg;  /**< Click config object. */

    /** 
     * Logger initialization.
     * Default baud rate: 115200
     * Default log level: LOG_LEVEL_DEBUG
     * @note If USB_UART_RX and USB_UART_TX 
     * are defined as HAL_PIN_NC, you will 
     * need to define them manually for log to work. 
     * See @b LOG_MAP_USB_UART macro definition for detailed explanation.
     */
    LOG_MAP_USB_UART( log_cfg );
    log_init( &logger, &log_cfg );
    log_info( &logger, " Application Init " );

    // Click initialization.
    gnss_cfg_setup( &gnss_cfg );
    GNSS_MAP_MIKROBUS( gnss_cfg, MIKROBUS_1 );
    if ( UART_ERROR == gnss_init( &gnss, &gnss_cfg ) ) 
    {
        log_error( &logger, " Communication init." );
        for ( ; ; );
    }
    log_info( &logger, " Application Task " );
}

void application_task ( void ) 
{
    if ( GNSS_OK == gnss_process( &gnss ) )
    {
        if ( PROCESS_BUFFER_SIZE == app_buf_len )
        {
            gnss_parser_application( &gnss, app_buf );
        }
    }
}

void main ( void ) 
{
    application_init( );

    for ( ; ; ) 
    {
        application_task( );
    }
}

static void gnss_clear_app_buf ( void ) 
{
    memset( app_buf, 0, app_buf_len );
    app_buf_len = 0;
}

static err_t gnss_process ( gnss_t *ctx ) 
{
    char rx_buf[ PROCESS_BUFFER_SIZE ] = { 0 };
    int32_t rx_size = 0;
    rx_size = gnss_generic_read( ctx, rx_buf, PROCESS_BUFFER_SIZE );
    if ( rx_size > 0 ) 
    {
        int32_t buf_cnt = app_buf_len;
        if ( ( ( app_buf_len + rx_size ) > PROCESS_BUFFER_SIZE ) && ( app_buf_len > 0 ) ) 
        {
            buf_cnt = PROCESS_BUFFER_SIZE - ( ( app_buf_len + rx_size ) - PROCESS_BUFFER_SIZE );
            memmove ( app_buf, &app_buf[ PROCESS_BUFFER_SIZE - buf_cnt ], buf_cnt );
        }
        for ( int32_t rx_cnt = 0; rx_cnt < rx_size; rx_cnt++ ) 
        {
            if ( rx_buf[ rx_cnt ] ) 
            {
                app_buf[ buf_cnt++ ] = rx_buf[ rx_cnt ];
                if ( app_buf_len < PROCESS_BUFFER_SIZE )
                {
                    app_buf_len++;
                }
            }
        }
        return GNSS_OK;
    }
    return GNSS_ERROR;
}

static void gnss_parser_application ( gnss_t *ctx, char *rsp )
{
    char element_buf[ 100 ] = { 0 };
    if ( GNSS_OK == gnss_parse_gpgga( rsp, GNSS_GPGGA_LATITUDE, element_buf ) )
    {
        static uint8_t wait_for_fix_cnt = 0;
        if ( strlen( element_buf ) > 0 )
        {
            log_printf( &logger, "\r\n Latitude: %.2s degrees, %s minutes \r\n", element_buf, &element_buf[ 2 ] );
            gnss_parse_gpgga( rsp, GNSS_GPGGA_LONGITUDE, element_buf );
            log_printf( &logger, " Longitude: %.3s degrees, %s minutes \r\n", element_buf, &element_buf[ 3 ] );
            memset( element_buf, 0, sizeof( element_buf ) );
            gnss_parse_gpgga( rsp, GNSS_GPGGA_ALTITUDE, element_buf );
            log_printf( &logger, " Altitude: %s m \r\n", element_buf );
            wait_for_fix_cnt = 0;
        }
        else
        {
            if ( wait_for_fix_cnt % 5 == 0 )
            {
                log_printf( &logger, " Waiting for the position fix...\r\n\n" );
                wait_for_fix_cnt = 0;
            }
            wait_for_fix_cnt++;
        }
        gnss_clear_ring_buffers( ctx );
        gnss_clear_app_buf( );
    }
}

// ------------------------------------------------------------------------ END
