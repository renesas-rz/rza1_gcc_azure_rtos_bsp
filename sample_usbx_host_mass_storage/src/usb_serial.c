
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdarg.h>	/* va_start, va_end */
#include <stdio.h>
#include <string.h>	/* strlen */

#include "dev_drv.h"
#if 0	/* disappeared */	/* grape */
#include "devdrv_scif_uart.h"
#endif
#include "usb_serial.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_MAX 64	/* receive buffer size */
#define W_MAX 256	/* send buffer size */

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern int scif_read (void* pStream, uint8_t *pbyBuffer, uint32_t uiCount);
extern int scif_write (void* pStream, uint8_t *pbyBuffer, uint32_t uiCount);

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static char str_read[R_MAX];
static char str_write[W_MAX];

/******************************************************************************
* Function Name: int usb_printf( const char *fmt, ...)
* Description  : send data to serial port
* Arguments    : const char* fmt
* Return Value : number of sent data bytes
******************************************************************************/
int usb_printf( const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsprintf(str_write, fmt, args);
	va_end(args);

#if 0	/* grape */
	printf( "%s", str_write );
#else
	scif_write( 0, str_write, strlen(str_write) );
#endif

	return strlen( str_write );
}

/******************************************************************************
* Function Name: int usb_scanf( const char *fmt, ...)
* Description  : receive data from serial port
* Arguments    : const char* fmt
* Return Value : number of received data
******************************************************************************/
int usb_scanf( const char *fmt, ... )
{
	int ret;
	int count = 1;	/* restrict only for SDHI_command.c */
					/* just confirmed %d, %s, %x */

	usb_gets( str_read, R_MAX );

	va_list args;
	va_start( args, fmt );
    if( count != vsscanf( str_read, fmt, args ) ){
        usb_printf("vsscanf error");
    }
    va_end( args );

	return count;
}

/******************************************************************************
* Function Name: char *usb_gets( char* buffer, int max )
* Description  : receive data from serial port
* Arguments    : char* buffer, int max
* Return Value : buffer pointer of received data
******************************************************************************/
char *usb_gets( char* buffer, int max )
{
	char* now = buffer;
	char* end = buffer + max;
	int32_t status;
	char c;

	memset( buffer, 0, max );

    while (now < end) {
        status = scif_read( 0, &c, 1 );
        if( -1==status ) {	/* error */
        	break;
        }
        /* discard LF sent by terminal software */
        if( (buffer==now) && ('\n'==c) ) {
        	continue;
        }
        if( '\r'==c ) {	/* carriage return */
    		scif_write( 0, "\n", 1 );	/* new line */
        	break;
        }

        if( '\b'==c ) {	/* back space */
        	if(buffer!=now) {	/* not first byte */
        		scif_write( 0, "\b", 1 );	/* echo back */
        		scif_write( 0, " ", 1 );	/* space */
        		scif_write( 0, "\b", 1 );	/* back space */
        		now--;	/* decrement buffer pointer */
        		*now = 0;	/* clear input */
        	}
       	}
        else {
            scif_write( 0, &c, 1 );	/* echo back */
        	*now = c;	/* save */
        	now++;	/* increment buffer pointer */
        }
    }

	return buffer;
}

void usb_dump( char* label, unsigned char* buf, int bytes )
{
	char one[4];
#define LINE_BYTES_MAX 16
#define LINE_BUF_MAX 128
	char line[LINE_BUF_MAX];
	int bytes_in_line;
	int done = 0;
	int remain;
	int i;

	usb_printf( "[%s]\n", label );

	while( bytes > done ) {

		memset( line, 0, LINE_BUF_MAX );
		remain = bytes-done;
		bytes_in_line = (LINE_BYTES_MAX>remain) ? remain : LINE_BYTES_MAX;
		for( i=0; i<bytes_in_line; i++) {
			sprintf( one, "%02x ", *(buf+done+i) );
			strcat( line, one );
		}
		usb_printf( "%s\n", line );
		done += bytes_in_line;

	}
	usb_printf( "\n" );

}

/******************************************************************************
* Function Name: void usb_sample()
* Description  : test function for usb_printf, usb_scanf
* Arguments    : none
* Return Value : none
******************************************************************************/
#if 1	/* test sample for usb_printf, usb_scanf */
void usb_sample() {

	char gets_str[32];
	int test_int;
	int test_hex;
	char test_str[64];

	while (1) {

gets:
		usb_printf("\nsd_gets test\n\ninput string.\n");
		usb_gets(gets_str, 32);
		usb_printf("\nstring= %s\n", gets_str);
//		goto gets;

scanf_int:
		usb_printf("\ninput integer value.\n");
		usb_scanf("%d", &test_int);
		usb_printf("\ninteger value is %d\n", test_int);
//		goto scanf_int;

scanf_hex:
		usb_printf("\ninput hex value.\n");
		usb_scanf("%x", &test_hex);
		usb_printf("\nhex value is %d\n", test_hex);
//		goto scanf_hex;

scanf_str:
		usb_printf("\ninput string.\n");
		usb_scanf("%s", test_str);
		usb_printf("\nstring is %s\n", test_str);
//		goto scanf_str;
	}
}
#endif
