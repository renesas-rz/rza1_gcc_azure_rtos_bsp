/****** ************************************************************************
 *
 * $Rev: 6174 $
 * $Date:: 2018-01-15 14:12:23 +0000#$
 *
 * Description : errno for POSIX IEEE Std 1003.1-2001
 *
 * (C) Copyright RENESAS ELECTRONICS Ltd 2012 All Rights Reserved
 *****************************************************************************/

/* Detect if SHC include errno.h has been included first, this is not supported */
#ifdef _ERRNO
    #error Renesas/Sh/9_x_x/include/errno.h has been included before SDK errno.h. Please specify SDK include path first.
#endif

#ifndef SDK_ERRNO
#define SDK_ERRNO

/* Include std headers to override _errno with SDK version */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************
 System Includes
***********************************************************************************/
/* SDK errno extension, SDK returns 0 as success, and -1 when errno set */
#define IOIF_ESUCCESS    (0)
#define IOIF_EERROR      (-1)

#define IOIF_EBADF       (-17) /* Bad file descriptor */
#define IOIF_EINVAL      (-28) /* Invalid argument */
#define IOIF_ENOMEM      (-33) /* No memory available */
#define IOIF_ENOLCK      (-34) /* No lock available */
#define IOIF_ENOENT      (-42) /* No such file or directory */
#define IOIF_ETIMEDOUT   (-50) /* Timed out */
#define IOIF_EACCES      (-64) /* Access denied */
#define IOIF_EAGAIN      (-65) /* Resource unavailable. Try again */
#define IOIF_EBUSY       (-67) /* Device or resource busy */
#define IOIF_ECANCELED   (-68) /* Operation was cancelled */
#define IOIF_EDOM        (-69) /* Maths argument out of domain */
#define IOIF_EEXIST      (-70) /* File, directory or mount exists */
#define IOIF_EFBIG       (-71) /* File too big */
#define IOIF_EINPROGRESS (-72) /* Operation in progress */
#define IOIF_EIO         (-74) /* Input or Output error (possibly recoverable)*/
#define IOIF_EISDIR      (-75) /* Is a directory */
#define IOIF_EMFILE      (-76) /* Too many open files */
#define IOIF_ENAMETOOLONG (-77) /* File name too long */
#define IOIF_ENFILE      (-78) /* Too many files are open in the system */
#define IOIF_ENOBUFS     (-79) /* No buffers available */
#define IOIF_ENODEV      (-80) /* Not a valid device */
#define IOIF_ENOSPC      (-84) /* No space available on device */
#define IOIF_ENOTDIR     (-85) /* Not a directory */
#define IOIF_ENOTEMPTY   (-86) /* Directory not empty */
#define IOIF_ENOTSUP     (-87) /* Not supported */
#define IOIF_ENOTTY      (-88) /* No TTY available */
#define IOIF_ENXIO       (-89) /* Not valid IO address or device (unrecoverable)*/
#define IOIF_EPERM       (-90) /* Operation not permitted */
#define IOIF_EROFS       (-91) /* Read only file system */
#define IOIF_ESPIPE      (-92) /* Invalid seek on pipe */
#define IOIF_EXDEV       (-93) /* Device cross linked */
#define IOIF_ENOTSOCK    (-94) /* Not a socket */
#define IOIF_EMSGSIZE    (-95) /* Message too large */
#define IOIF_EINTR       (-96) /* Function interrupted */
#define IOIF_EFAULT      (-97) /* Section fault, bad address */
#define IOIF_EPIPE       (-98) /* Broken pipe */
#define IOIF_EALREADY    (-99) /* Connection already started */
#define IOIF_EDESTADDRREQ (-100) /* Destination address requred */
#define IOIF_EPROTOTYPE  (-101) /* Wrong protocol or codec type */
#define IOIF_ENOPROTOOPT (-102) /* Protocol or codec not available */
#define IOIF_EPROTONOSUPPORT (-103) /* Protocol or codec not supported */
#define IOIF_EOPNOTSUPP  (-104) /* Operation not supported */
#define IOIF_EAFNOSUPPORT (-105) /* Address family not supported */
#define IOIF_EADDRINUSE  (-106) /* Address already in use */
#define IOIF_EADDRNOTAVAIL (-107) /* Address not available */
#define IOIF_ECONNRESET  (-108) /* Connection reset */
#define IOIF_EISCONN     (-109) /* Connected */
#define IOIF_ENOTCONN    (-110) /* Not connected */
#define IOIF_ECONNREFUSED (-112) /* Connection refused */
#define IOIF_EPROCLIM    (-113) /* Too many processes */
#define IOIF_EUSERS      (-114) /* Too many users */
#define IOIF_EDEADLK     (-115) /* Resource deadlock would occur */
#define IOIF_EPROTO      (-116) /* Protocol or codec error */
#define IOIF_EOVERFLOW   (-117) /* Value too large to store in data type */
#define IOIF_ESRCH       (-118) /* No such task */
#define IOIF_EILSEQ      (-119) /* Illegal byte sequence format */
#define IOIF_EBADMSG     (-120) /* Bad message */
#define IOIF_ENOSYS      (-121) /* Function not supported */
#define IOIF_ECRC        (-122) /* CRC failure */
#define IOIF_EEOF        (-123) /* Position is End Of File */
#define IOIF_ESEEKPASTEND (-124) /* Seek past EOF */
#define IOIF_EMEDIUMTYPE (-127) /* Wrong medium type */
#define IOIF_ENOMEDIUM   (-128) /* No medium present */
#define IOIF_ESEEKRANGE  (-129) /* seek setting error */  /* Source Merge 11-1 */

/***********************************************************************************
 Defines
***********************************************************************************/
/* errno is a macro so it is re-entrant (per thread/task value) - this
 * definition works with SH C libs.  The errno returns an l-value so
 * it can be used on the left of an assignment as it is legal for an
 * application to assign to errno.
 *
 * Note: Application code should not call ioif_get_errno directly. All
 * access to errno should be made using the POSIX "errno" macro.
*/
extern int *ioif_get_errno(void);

/* Remove SHC definitions so SDK can be used instead */
#ifdef _errno
    #undef _errno
#endif

#define _errno (*ioif_get_errno())

#define  ioif_errno _errno

#define iserrno(e)((unsigned int)(e) >= 0xffffff80UL)

#ifdef __cplusplus
}
#endif

#endif
