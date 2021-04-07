/*
 * memory
 *
 *  Created on: 2014/02/12
 *      Author: a5012939
 */

#include <string.h>


/* copy the memory */
void
__aeabi_memcpy( void *dst, const void *src, size_t n )
{
		 (void)memcpy( dst, src, n );
}

/* copy the memory (4byte aligned)*/
void
__aeabi_memcpy4( void *dst, const void *src, size_t n )
{
	(void)memcpy( dst, src, n );
}

void
__aeabi_memmove4( void *dest, const void *src, size_t n )
{
	(void)memmove( dest, src, n );
}

/* clear the memory */
void
__aeabi_memclr4( void *dst, size_t n )
{
	(void)memset( dst, 0, n );
}

/* fill the memory */
void
__aeabi_memset( void *dst, size_t n, int c )
{
	(void)memset( dst, c, n );
}

int
__aeabi_uread4( void *address )
{
	int		retval;

	retval =
		(((char*)address)[0] << 24 ) |
		(((char*)address)[1] << 16 ) |
		(((char*)address)[2] <<  8 ) |
		(((char*)address)[3] <<  0 );
	return retval;
}
/*EOF*/
