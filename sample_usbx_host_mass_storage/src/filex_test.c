
/**************************************************************************/
/**                                                                       */
/** Functions for testing file read, write and verify wrote file.         */
/**   and for measuring R/W processing time.                              */
/**                                                                       */
/**   file_test.h                                                         */
/**                                                                       */
/**************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <fx_api.h>
#include <tx_api.h>

#include "usb_serial.h"
#include "usb_misc.h"
#include "usb_mem.h"
#include "filex_test.h"

#include <stdio.h>	/* sprintf */


/******************************************************************************
Macro definitions
******************************************************************************/

/* If defined, erase file before writing.
 */
#if 1
#define ERASE
#endif

/* If defined, mode for writing.
 */
#if 1
#define WRITE_TEST
#endif

/* If defined, mode for reading.
 */
#if 1
#define READ_TEST
#endif

/* If defined, mode for verify.
 */
#if 1
#define VERIFY_TEST
#endif

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern FX_MEDIA* media;


/******************************************************************************
Private variables and functions
******************************************************************************/

/* Buffer area setting for read/write testing */
/* This must be 32 bytes aligned for cache maintenance. */
/* Buffer is implemented in sd_mem.c */
static unsigned char* rw_memory = (uint8_t *)&test_usb_rw_buff[0];


/* Define FileX global data structures.  */
static FX_MEDIA* usb_disk;
static FX_FILE usb_file;


/* Define local functions */
static UINT test_read_1( int bytes, int* ticks );
static UINT test_write_1( int bytes, int* ticks );
static UINT test_verify_1( int bytes );
static FX_MEDIA* get_media();


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
* Function Name: UINT speed_test()
* Description  : Test function to measure R/W processing time.
* Arguments    : None
* Return Value : success: FX_SUCCESS
*                failure: other FX return value
******************************************************************************/
UINT speed_test()
{
	UINT status;
	int ticks, bytes = FX_MEM_SIZE;

#ifdef WRITE_TEST
	printf("\nwrite %dMB data [test_write_1]\n", MB(bytes));
	status = test_write_1(bytes, &ticks);
	if ( FX_SUCCESS != status) {
		printf("test_write_1 error, status= %s\n",
				get_fx_error_string(status));
		return status;
		/* connect */
	} else {
		printf("test_write_1 tooks %d ticks.\n", ticks);
		printf("%d bytes / second.\n", bytes * 1000 / (TICK_MS * ticks));
	}
#endif

#ifdef READ_TEST
	printf("\njust read %dMB data [test_read_1]\n", MB(bytes));
	status = test_read_1(bytes, &ticks);
	if ( FX_SUCCESS != status) {
		printf("test_read_1 error, status= %s\n",
				get_fx_error_string(status));
		return status;
		/* connect */
	} else {
		printf("test_read_1 tooks %d ticks.\n", ticks);
		printf("%d bytes / second.\n", bytes * 1000 / (TICK_MS * ticks));
	}
#endif

#ifdef VERIFY_TEST
	printf("\ncheck written %dMB data [test_verify_1]\n", MB(bytes));
	status = test_verify_1(bytes);
	if ( FX_SUCCESS != status) {
		printf("test_verify_1 error, status= %s\n",
				get_fx_error_string(status));
		return status;
		/* connect */
	} else {
		printf("test_verify_1 success\n");
	}
#endif
	return FX_SUCCESS;
}

/******************************************************************************
* Function Name: void get_fx_error_string(UINT status)
* Description  : return FX status string
* Arguments    : FX status
* Return Value : FX status string
******************************************************************************/
CHAR* get_fx_error_string(UINT status) {

static CHAR str[32];

	switch (status) {
	case 0x00:	return "FX_SUCCESS";	break;
	case 0x01:	return "FX_BOOT_ERROR";	break;
	case 0x02:	return "FX_MEDIA_INVALID";	break;
	case 0x03:	return "FX_FAT_READ_ERROR";	break;
	case 0x04:	return "FX_NOT_FOUND";	break;
	case 0x05:	return "FX_NOT_A_FILE";	break;
	case 0x06:	return "FX_ACCESS_ERROR";	break;
	case 0x07:	return "FX_NOT_OPEN";	break;
	case 0x08:	return "FX_FILE_CORRUPT";	break;
	case 0x09:	return "FX_END_OF_FILE";	break;
	case 0x0A:	return "FX_NO_MORE_SPACE";	break;
	case 0x0B:	return "FX_ALREADY_CREATED";	break;
	case 0x0C:	return "FX_INVALID_NAME";	break;
	case 0x0D:	return "FX_INVALID_PATH";	break;
	case 0x0E:	return "FX_NOT_DIRECTORY";	break;
	case 0x0F:	return "FX_NO_MORE_ENTRIES";	break;
	case 0x10:	return "FX_DIR_NOT_EMPTY";	break;
	case 0x11:	return "FX_MEDIA_NOT_OPEN";	break;
	case 0x12:	return "FX_INVALID_YEAR";	break;
	case 0x13:	return "FX_INVALID_MONTH";	break;
	case 0x14:	return "FX_INVALID_DAY";	break;
	case 0x15:	return "FX_INVALID_HOUR";	break;
	case 0x16:	return "FX_INVALID_MINUTE";	break;
	case 0x17:	return "FX_INVALID_SECOND";	break;
	case 0x18:	return "FX_PTR_ERROR";	break;
	case 0x19:	return "FX_INVALID_ATTR";	break;
	case 0x20:	return "FX_CALLER_ERROR";	break;
	case 0x21:	return "FX_BUFFER_ERROR";	break;
	case 0x22:	return "FX_NOT_IMPLEMENTED";	break;
	case 0x23:	return "FX_WRITE_PROTECT";	break;
	case 0x24:	return "FX_INVALID_OPTION";	break;
	case 0x89:	return "FX_SECTOR_INVALID";	break;
	case 0x90:	return "FX_IO_ERROR";	break;
	case 0x91:	return "FX_NOT_ENOUGH_MEMORY";	break;
	case 0x92:	return "FX_ERROR_FIXED";	break;
	case 0x93:	return "FX_ERROR_NOT_FIXED";	break;
	case 0x94:	return "FX_NOT_AVAILABLE";	break;
	case 0x95:	return "FX_INVALID_CHECKSUM";	break;
	case 0x96:	return "FX_READ_CONTINUE";	break;
	case 0x97:	return "FX_INVALID_STATE";	break;
	default:
		sprintf( str, "unexpected status: 0x%x\n", status );
		return str;
		break;
	}
}

/******************************************************************************
Private variables and functions
******************************************************************************/

/******************************************************************************
* Function Name: UINT test_write_1_1( int bytes, int* ticks )
* Description  : Test function to measure processing time of writing function.
* Arguments    : bytes: data bytes to write
*                ticks: return time of writing
* Return Value : success: FX_SUCCESS
*                failure: other FX return value
******************************************************************************/
UINT test_write_1( int bytes, int* ticks )
{
	UINT        my_status = FX_SUCCESS;
	UINT        status;
	ULONG       actual;
	INT			i;
	int         tick_s, tick_e;

	#define CHAR_TEN 10
	char ten[CHAR_TEN+1];
	int val = 0;
	int remain;


#if 1
	usb_disk = get_media();
#else
	/* no need to open media */
	/* Open the SD disk.  */
	status = fx_media_open(&sd_disk, "SD DISK", fx_sd_driver, 0, media_memory,
			sizeof(media_memory));

	/* Check the media open status.  */
	if (status != FX_SUCCESS) {

		/* Error, break the loop!  */
		return status;
	}
#endif

#ifdef ERASE
	/* erase file */
	printf( "\nerase TEST_1.TXT.\n");
	status = fx_file_delete(usb_disk, "TEST_1.TXT");

	/* Check the file erase status.  */
	if (status != FX_SUCCESS) {
		printf( "fx_file_delete error, status= %s\n", get_fx_error_string(status) );
	}
#endif

	/* Create a file called TEST_1.TXT in the root directory.  */
	status = fx_file_create(usb_disk, "TEST_1.TXT");

	/* Check the create status.  */
	if (status != FX_SUCCESS) {

		/* Check for an already created status. This is expected on the
		 second pass of this loop!  */
		if (status != FX_ALREADY_CREATED) {
			my_status = status;
			goto CLS;
		}
	}

	/* Open the test file.  */
	status = fx_file_open(usb_disk, &usb_file, "TEST_1.TXT", FX_OPEN_FOR_WRITE);

	/* Check the file open status.  */
	if (status != FX_SUCCESS) {
		my_status = status;
		goto CLS;
	}

	/* Seek to the beginning of the test file.  */
	status = fx_file_seek(&usb_file, 0);

	/* Check the file seek status.  */
	if (status != FX_SUCCESS) {
		my_status = status;
		goto CLS;
	}

	/* Write a string to the test file.  */
	memset(rw_memory, 0, bytes);
	remain = bytes;
	for( i=0; i<bytes; i+=CHAR_TEN ) {
		sprintf( ten, "%.*d", CHAR_TEN, val++ );
		memcpy( rw_memory+i, ten, (CHAR_TEN<remain)?CHAR_TEN:remain );
		remain -= CHAR_TEN;
	}

//    tx_time_set( 0 );
    tick_s = tx_time_get();
	status = fx_file_write(&usb_file, rw_memory, bytes);
	if (status != FX_SUCCESS) {
		printf("fx_file_write failed, status= 0x%x.\n", status );
		my_status = status;
		goto CLS;
	}
    tick_e = tx_time_get();

	/* Flush a string to the test file.  */
    status = fx_media_flush(usb_disk);

    /* Check the file flush status.  */
	if (status != FX_SUCCESS) {
		my_status = status;
		goto CLS;
	}


	/* Close the test file.  */
	status = fx_file_close(&usb_file);

	/* Check the file close status.  */
	if (status != FX_SUCCESS) {
		my_status = status;
		goto CLS;
	}

CLS:	/* should close media once open */
	/* Close the media.  */
#if 0	/* no need to close media */
	status = fx_media_close(usb_disk);

	/* Check the media close status.  */
	if( FX_SUCCESS != my_status ) {
		status = my_status;
	}
#endif

	*ticks = tick_e - tick_s;

	return status;
}

/******************************************************************************
* Function Name: UINT test_read_1( int bytes, int* ticks )
* Description  : Test function to measure processing time of reading function.
* Arguments    : bytes: data bytes to read
*                ticks: return time of reading
* Return Value : success: FX_SUCCESS
*                failure: other FX return value
******************************************************************************/
UINT test_read_1( int bytes, int* ticks )
{
	UINT        my_status = FX_SUCCESS;
	UINT        status;
	ULONG       actual;
	int         tick_s, tick_e;

#if 1
	usb_disk = get_media();
#else
	/* no need to open media */
	/* Open the SD disk.  */
	status = fx_media_open(&sd_disk, "SD DISK", fx_sd_driver, 0, media_memory,
			sizeof(media_memory));

	/* Check the media open status.  */
	if (status != FX_SUCCESS) {

		/* Error, break the loop!  */
		return status;
	}
#endif

	/* Open the test file.  */
	status = fx_file_open(usb_disk, &usb_file, "TEST_1.TXT", FX_OPEN_FOR_READ);

	/* Check the file open status.  */
	if (status != FX_SUCCESS) {
		my_status = status;
		goto CLS;
	}

	/* Seek to the beginning of the test file.  */
	status = fx_file_seek(&usb_file, 0);

	/* Check the file seek status.  */
	if (status != FX_SUCCESS) {
		my_status = status;
		goto CLS;
	}

	/* Read the first 28 bytes of the test file.  */
//	tx_time_set( 0 );
	tick_s = tx_time_get();
    int i;
	status = fx_file_read(&usb_file, rw_memory, bytes, &actual);
	tick_e = tx_time_get();
	if (status != FX_SUCCESS) {
		printf("fx_file_read failed, status= 0x%x.\n", status );
		my_status = status;
		goto CLS;
	} else {
		printf("%d bytes read successfully.\n", actual);
	}

	/* Check the file read status.  */
	if ((status != FX_SUCCESS) || (actual != bytes)) {
		my_status = FX_IO_ERROR;
			goto CLS;
	}

	/* Close the test file.  */
	status = fx_file_close(&usb_file);

	/* Check the file close status.  */
	if (status != FX_SUCCESS) {
		my_status = status;
		goto CLS;
	}

CLS:	/* should close media once open */
	/* Close the media.  */
#if 0
	status = fx_media_close(&sd_disk);

	/* Check the media close status.  */
	if( FX_SUCCESS != my_status ) {
		status = my_status;
	}
#endif

	*ticks = tick_e - tick_s;

	return status;
}

/******************************************************************************
* Function Name: UINT test_verify_1( int bytes )
* Description  : Test function to verify wrote data.
* Arguments    : bytes: data bytes to read
* Return Value : success: FX_SUCCESS
*                failure: FX_ERROR
******************************************************************************/
UINT test_verify_1( int bytes )
{
	UINT        status;
	UINT        my_status = FX_SUCCESS;
	ULONG       actual;

	#define R_BYTES 10
	unsigned char r_buf[R_BYTES];
	int remain = bytes;
	int r_bytes = R_BYTES;

	#define CHAR_TEN 10
	char ten[CHAR_TEN+1];
	int val = 0;

	printf( "Verify start...\n");

#if 1
	usb_disk = get_media();
#else
	/* no need to open media */
	/* Open the SD disk.  */
	status = fx_media_open(&sd_disk, "SD DISK", fx_sd_driver, 0, media_memory,
			sizeof(media_memory));

	/* Check the media open status.  */
	if (status != FX_SUCCESS) {

		printf( "fx_media_open error\n");
		/* Error, break the loop!  */
		return status;
	}
#endif

	/* Open the test file.  */
	status = fx_file_open(usb_disk, &usb_file, "TEST_1.TXT", FX_OPEN_FOR_READ);

	/* Check the file open status.  */
	if (status != FX_SUCCESS) {
		printf( "fx_file_open error\n");
		my_status = status;
		goto CLS;
	}

	/* Seek to the beginning of the test file.  */
	status = fx_file_seek(&usb_file, 0);

	/* Check the file seek status.  */
	if (status != FX_SUCCESS) {
		printf( "fx_file_seek error\n");
		my_status = status;
		goto CLS;
	}

	int i = 0;
	while (remain) {
		status = fx_file_read(&usb_file, r_buf, r_bytes, &actual);
		if (r_bytes != actual) {
			printf("%dth read, r_bytes= %d, actual= %d\n", i, r_bytes,
					&actual);
			my_status = FX_IO_ERROR;	/* connect */
			goto CLS;
		}

		sprintf(ten, "%.*d", CHAR_TEN, i);
		if (strncmp(ten, r_buf, r_bytes)) {
			printf("%dth read, unexpected read data\n", i);
			usb_dump("r_buf", r_buf, r_bytes);
			my_status = FX_IO_ERROR;	/* connect */
			goto CLS;
		}

		remain -= r_bytes;
		if ( R_BYTES > remain) {
			r_bytes = remain;
		}
		i++;

	}

	printf( "read all data\n" );

	/* Close the test file.  */
	status = fx_file_close(&usb_file);

	/* Check the file close status.  */
	if (status != FX_SUCCESS) {
		printf( "fx_file_close error, status= %02x\n", status );
		my_status = status;
		goto CLS;
	}

CLS:	/* should close media once open */
	/* Close the media.  */
#if 0	/* no need to close media */
	status = fx_media_close(&sd_disk);

	/* Check the media close status.  */
	if (status != FX_SUCCESS) {

		printf( "fx_file_close error, status= %02x\n", status );
		/* Error closing the media, break the loop.  */
		return status;
	}

	if( FX_SUCCESS != my_status ) {
		status = my_status;
	}
#endif

	return status;
}

/******************************************************************************
* Function Name: FX_MEDIA* get_media()
* Description  : Return FX_MEDIA pointer.
* Arguments    : None
* Return Value : FX_MEDIA pointer
******************************************************************************/
FX_MEDIA* get_media()
{
	return media;
}
