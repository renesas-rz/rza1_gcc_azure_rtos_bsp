
/**************************************************************************/
/**                                                                       */
/** Functions for testing file read, write and verify wrote file.         */
/**   and for measuring R/W processing time.                              */
/**                                                                       */
/**   file_test.h                                                         */
/**                                                                       */
/**************************************************************************/

#ifndef _FILE_TEST_H_
#define _FILE_TEST_H_

#include <tx_api.h>

UINT speed_test();
CHAR* get_fx_error_string(UINT status);

#endif	/* _FILE_TEST_H_ */
