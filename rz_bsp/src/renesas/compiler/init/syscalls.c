/* Support files for GNU libc.  Files in the system namespace go here.
   Files in the C namespace (ie those that do not start with an
   underscore) go in .c.  */

#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>
#include <reent.h>
#include <unistd.h>
#include <sys/wait.h>

#include "r_typedefs.h"
#include "compiler_settings.h"
#include "r_devlink_wrapper.h"

#include "swi.h"
#include "control.h"

#ifndef NULL
#define NULL 0
#endif

/* Forward prototypes.  */
int     _system     _PARAMS ((const char *));
int     _rename     _PARAMS ((const char *, const char *));
int     _isatty        _PARAMS ((int));
clock_t _times        _PARAMS ((struct tms *));
int     _gettimeofday    _PARAMS ((struct timeval *, void *));
void    _raise         _PARAMS ((void));
int     _unlink        _PARAMS ((const char *));
int     _link         _PARAMS ((void));
int     _stat         _PARAMS ((const char *, struct stat *));
int     _fstat         _PARAMS ((int, struct stat *));
caddr_t _sbrk        _PARAMS ((int));
int     _getpid        _PARAMS ((int));
int     _kill        _PARAMS ((int, int));
void    _exit        _PARAMS ((int));
int     _close        _PARAMS ((int));
int     _swiclose    _PARAMS ((int));
int     _open        _PARAMS ((const char *, int, ...));
int     _swiopen    _PARAMS ((const char *, int));
int     _write        _PARAMS ((int, const char *, unsigned int));

int     _swiwrite    _PARAMS ((int, char *, int));
int     _lseek        _PARAMS ((int, int, int));
int     _swilseek    _PARAMS ((int, int, int));
int     _read        _PARAMS ((int, char *, unsigned int));

int     _swiread    _PARAMS ((int, char *, int));
void    initialise_monitor_handles _PARAMS ((void));

static int    wrap        _PARAMS ((int));
static int    error        _PARAMS ((int));
static int    get_errno    _PARAMS ((void));

#ifdef ARM_RDI_MONITOR
static int    do_AngelSWI    _PARAMS ((int, void *));
#endif

/* Register name faking - works in collusion with the linker.  */
register char * stack_ptr __asm ("sp");


/* following is copied from libc/stdio/local.h to check std streams */
extern void   _EXFUN(__sinit,(struct _reent *));
#define CHECK_INIT(ptr) \
  do                        \
    {                        \
      if ((ptr) && !(ptr)->__sdidinit)        \
    __sinit (ptr);                \
    }                        \
  while (0)

/* Adjust our internal handles to stay away from std* handles.  */
#define FILE_HANDLE_OFFSET (0x20)

static int monitor_stdin;
static int monitor_stdout;
static int monitor_stderr;

/* Struct used to keep track of the file position, just so we
   can implement fseek(fh,x,SEEK_CUR).  */

typedef struct
{
  int handle;
  int pos;
}
poslog;

#define MAX_OPEN_FILES (20)


/* Struct used to keep track of the file position, just so we
   can implement fseek(fh,x,SEEK_CUR).  */
struct fdent
{
  int handle;
  int pos;
  int ipending; /* Indicates fdent is being used but 'handle' not yet
                   allocated by the low level open  function */
};

/* User file descriptors (fd) are integer indexes into
   the openfiles[] array. Error checking is done by using
   findslot().

   This openfiles array is manipulated directly by only
   these 5 functions:

    findslot() - Translate entry.
    newslot() - Find empty entry.
    initilise_monitor_handles() - Initialize entries.
    _swiopen() - Initialize entry.
    _close() - Handle stdout == stderr case.

   Every other function must use findslot().  */

static struct fdent openfiles [MAX_OPEN_FILES];

static int            newslot     _PARAMS ((void));


/* following is copied from libc/stdio/local.h to check std streams */
extern void   _EXFUN(__sinit,(struct _reent *));
#define CHECK_INIT(ptr) \
  do                        \
    {                        \
      if ((ptr) && !(ptr)->__sdidinit)        \
    __sinit (ptr);                \
    }                        \
  while (0)

#if MULTI_INSTANCE_SERIAL_PORT
static int monitor_stdin;
static int monitor_stdout;
static int monitor_stderr;
#endif


/* Return a pointer to the structure associated with
   the user file descriptor fd. */
static struct fdent*
matchslot (int fd)
{
  int i;

  CHECK_INIT(_REENT);

  int iMask = R_OS_SysLock(NULL); /* Lock the scheduler while finding a slot */


  for (i = 0; i < MAX_OPEN_FILES; i++)
  {
      /* User file descriptor is located. */
      if (openfiles[i].handle == fd)
      {
          /* terminate the search */
          break;
      }
  }

  R_OS_SysUnlock(NULL, iMask); /* Unlock scheduler */

  /* User file descriptor is out of range. */
  if (i >= MAX_OPEN_FILES)
  {
    return NULL;
  }
  else
  {
    return  &openfiles[i];
  }
}

/* Return the next lowest numbered free file
   structure, or -1 if we can't find one. */

/* 15.12.15 Modified to make thread-safe.
 * Added scheduler locking and checks ipending to
 * see if the fdent has been previously allocated but
 * open not yet completed.
 *
 * Note: FreeRTOS includes  reentrant support for newlib,
 * (configUSE_NEWLIB_REENTRANT) which is not included in this build.
 * Consider using this FreeRTOS functionality in future code changes.
 */

static int
newslot (void)
{
  int i, retval;

  int iMask = R_OS_SysLock(NULL); /* Lock the scheduler while finding a slot */

  for (i = 0; i < MAX_OPEN_FILES; i++)

      /* Search for an used entry which has not been allocated */
  if ( (openfiles[i].handle == -1) && (openfiles[i].ipending == 0) )
      break;

  if (i == MAX_OPEN_FILES)
  {
      retval = -1;
  }
  else /* found a slot */
  {
      /* Set ipending so it cannot be reallocated before the low level
       * open function completes*/
      openfiles[i].ipending = 1;
      retval = i;
  }

  R_OS_SysUnlock(NULL, iMask); /* Unlock scheduler */

  return retval;
}

void
initialise_monitor_handles (void)
{
  int i;

  for (i = 0; i < MAX_OPEN_FILES; i ++)
  {
    openfiles[i].handle = -1;
    openfiles[i].ipending = 0;
  }

  /* Open the standard file descriptors by opening the special
   * teletype device, ":tt", read-only to obtain a descritpor for
   * standard input and write-only to obtain a descriptor for standard
   * output. Finally, open ":tt" in append mode to obtain a descriptor
   * for standard error. Since this is a write mode, most kernels will
   * probably return the same value as for standard output, but the
   * kernel can differentiate the two using the mode flag and return a
   * different descriptor for standard error.
   */

#ifdef ARM_RDI_MONITOR
  int volatile block[3];

  block[0] = (int) ":tt";
  block[2] = 3;     /* length of filename */
  block[1] = 0;     /* mode "r" */
  monitor_stdin = do_AngelSWI (AngelSWI_Reason_Open, (void *) block);

  block[0] = (int) ":tt";
  block[2] = 3;     /* length of filename */
  block[1] = 4;     /* mode "w" */
  monitor_stdout = do_AngelSWI (AngelSWI_Reason_Open, (void *) block);

  block[0] = (int) ":tt";
  block[2] = 3;     /* length of filename */
  block[1] = 8;     /* mode "a" */
  monitor_stderr = do_AngelSWI (AngelSWI_Reason_Open, (void *) block);
#else
  int fh;
  char * name;
  name = "\\\\.\\stdin";
  fh = low_open(name, O_RDWR);
  monitor_stdin = fh;
  name = "\\\\.\\stdout";
  fh = low_open(name, O_WRONLY);
  monitor_stdout = fh;
  name = "\\\\.\\stderr";
  fh = low_open(name, O_WRONLY);

  /* If we failed to open stderr, redirect to stdout. */
  if (monitor_stderr == -1)
    monitor_stderr = monitor_stdout;


  openfiles[0].handle = monitor_stdin;
  openfiles[0].pos = 0;
  openfiles[1].handle = monitor_stdout;
  openfiles[1].pos = 0;
  openfiles[2].handle = monitor_stderr;
  openfiles[2].pos = 0;
#endif
}


static int
get_errno (void)
{
#ifdef ARM_RDI_MONITOR
  return do_AngelSWI (AngelSWI_Reason_Errno, NULL);
#else
  __asm ("swi %a0" :: "i" (SWI_GetErrno));
  return 0;
#endif
}

static int
error (int result)
{
  errno = get_errno ();
  return result;
}

static int
wrap (int result)
{
  if (result == -1)
    return error (-1);
  return result;
}

/* Returns # chars not! written.  */
int
_swiread (int fh,
      char * ptr,
      int len)
{
#ifdef ARM_RDI_MONITOR
  int block[3];

  block[0] = fh;
  block[1] = (int) ptr;
  block[2] = len;

  return do_AngelSWI (AngelSWI_Reason_Read, block);
#else
  int_t   i = low_read(fh, (uint8_t*)ptr, (uint32_t)len);
  return wrap(i);
#endif
}

/******************************************************************************
* Function Name: _read
* Description  : GNU interface to low-level I/O read
* Arguments    : int file_no
*              : const char *buffer
*              : unsigned int n
* Return Value : none
******************************************************************************/
int _read(int fh , char *buffer , unsigned int n)
{
      int   iResult = low_read(fh, (uint8_t*) buffer, (uint32_t) n);
      return wrap(iResult);
}


/* fd, is a user file descriptor. */
int
_swilseek (int fd,
    int ptr,
    int dir)
{
  int32_t res;

  /* Make a local fdent and don't use findslot()
  as the id refers to devlink handle not openfiles[] handle */
  struct fdent id;
  struct fdent *pfd = &id;

  pfd->handle = fd;
  pfd->ipending = 0;
  pfd->pos = 0;

  /* Valid whence? */
  if ((dir != SEEK_CUR)
      && (dir != SEEK_SET)
      && (dir != SEEK_END))
    {
      errno = EINVAL;
      return -1;
    }

  /* Convert SEEK_CUR to SEEK_SET */
  if (dir == SEEK_CUR)
    {
      ptr = pfd->pos + ptr;
      /* The resulting file offset would be negative. */
      if (ptr < 0)
        {
          errno = EINVAL;
          if ((pfd->pos > 0) && (ptr > 0))
            errno = EOVERFLOW;
          return -1;
        }
      dir = SEEK_SET;
    }

#ifdef ARM_RDI_MONITOR
  int block[2];
  if (dir == SEEK_END)
    {
      block[0] = pfd->handle;
      res = checkerror (do_AngelSWI (AngelSWI_Reason_FLen, block));
      if (res == -1)
        return -1;
      ptr += res;
    }

  /* This code only does absolute seeks.  */
  block[0] = pfd->handle;
  block[1] = ptr;
  res = checkerror (do_AngelSWI (AngelSWI_Reason_Seek, block));
#else
  res = low_lseek(fd, (uint32_t) ptr, dir);
  wrap (res);
  if (dir == SEEK_END)
    {
      ptr += res;
    }
#endif
  /* At this point ptr is the current file position. */
  if (res >= 0)
    {
      pfd->pos = ptr;
      return ptr;
    }
  else
    return -1;
}

int
_lseek (int file,
    int ptr,
    int dir)
{
  return wrap (_swilseek (file, ptr, dir));
}

/* fh, is a valid internal file handle.
   Returns the number of bytes *not* written. */
int
_swiwrite (
       int    fh,
       char * ptr,
       int    len)
{
    int_t i;

#ifdef ARM_RDI_MONITOR
  int block[3];

  block[0] = fh;
  block[1] = (int) ptr;
  block[2] = len;

  return checkerror (do_AngelSWI (AngelSWI_Reason_Write, block));
#else

  i = low_write(fh, (uint8_t*)ptr, (uint32_t)len);

  return wrap (i);
#endif
}

/******************************************************************************
* Function Name: _write
* Description  : GNU interface to low-level I/O write
* Arguments    : int file_no
*              : const char *buffer
*              : unsigned int n
* Return Value : none
******************************************************************************/
int _write(int fh , const char *buffer , unsigned int n)
{
    int_t i;

    i = low_write(fh, (uint8_t*)buffer, (uint32_t)n);

    return i;
}

int
_swiopen (const char * path, int flags)
{
  int aflags = 0, fh;
#ifdef ARM_RDI_MONITOR
  int block[3];
#endif

  int fd = newslot ();

  if (fd == -1)
    {
      errno = EMFILE;
      return -1;
    }

  /* It is an error to open a file that already exists. */
  if ((flags & O_CREAT)
      && (flags & O_EXCL))
    {
      struct stat st;
      int res;
      res = _stat (path, &st);
      if (res != -1)
        {
      errno = EEXIST;
      return -1;
        }
    }

  /* The flags are Unix-style, so we need to convert them. */
#ifdef O_BINARY
  if (flags & O_BINARY)
    aflags |= 1;
#endif

  /* In O_RDONLY we expect aflags == 0. */
  if (flags & O_RDWR) {
    aflags |= O_RDWR;
  }
  if (flags & O_WRONLY) {
    aflags |= O_WRONLY;
  }
  if (flags & O_CREAT){
    aflags |= O_CREAT;
  }
  if (flags & O_TRUNC){
    aflags |= O_TRUNC;
  }
  if (flags & O_WRONLY){
    aflags |= O_WRONLY;
  }
  if (flags & O_APPEND){
    aflags |= O_APPEND;
  }

#ifdef ARM_RDI_MONITOR
  block[0] = (int) path;
  block[2] = strlen (path);
  block[1] = aflags;

  fh = do_AngelSWI (AngelSWI_Reason_Open, block);

#else
  fh = low_open((char *)path, flags);
#endif

  /* Return a user file descriptor or an error. */
  if (fh >= 0)
    {
      openfiles[fd].handle = fh;
      openfiles[fd].pos = 0;
      openfiles[fd].ipending = 0;    /* file handle now allocated, clear ipending */
      return openfiles[fd].handle;
    }
  else
  {
    openfiles[fd].ipending = 0;    /* No file handle, clear ipending so it can be reused */
    return error (fh);
  }
}


int
_open (const char * path,
       int          flags,
       ...)
{
  return wrap (_swiopen (path, flags));
}

/* fh, is a valid internal file handle. */
int
_swiclose (int fh)
{
#ifdef ARM_RDI_MONITOR
  return wrap (do_AngelSWI (AngelSWI_Reason_Close, &fh));
#else
  int   iResult = low_close(fh);
  return iResult;
#endif
}


int
_close (int file)
{
    struct fdent *pfd;
    int res;

    /* Attempt to close the handle. */
    res = _swiclose (file);

    pfd = matchslot (file);
    if (pfd == NULL)
    {
        errno = EBADF;
    }
    else
    {
        /* Reclaim handle? */
        pfd->handle = -1;
    }

    return wrap (res);
}

int
_kill (int pid, int sig)
{
  (void)pid; (void)sig;
#ifdef ARM_RDI_MONITOR
  /* Note: The pid argument is thrown away.  */
  switch (sig) {
      case SIGABRT:
          return do_AngelSWI (AngelSWI_Reason_ReportException,
                  (void *) ADP_Stopped_RunTimeError);
      default:
          return do_AngelSWI (AngelSWI_Reason_ReportException,
                  (void *) ADP_Stopped_ApplicationExit);
  }
#else
  __asm ("swi %a0" :: "i" (SWI_Exit));
  return 0;
#endif
}

void
_exit (int status)
{
  /* There is only one SWI for both _exit and _kill. For _exit, call
     the SWI with the second argument set to -1, an invalid value for
     signum, so that the SWI handler can distinguish the two calls.
     Note: The RDI implementation of _kill throws away both its
     arguments.  */
  _kill(status, -1);
  while(1)
  {
      /* exit occurred */
  };
}

int
_getpid (int n)
{
  (void)(n);
  return 1;
}

#define NL_HEAP_START          (&_nl_heap_start)
#define NL_HEAP_END            (&_nl_heap_end)

extern char _nl_heap_start;
extern char _nl_heap_end;

static char *pnl_heap_pointer =  NL_HEAP_START;



/* _sbrk - this is used as a basic memory allocator over and above to the OS level malloc.
 * This should only be used by calls from the Newlib. A small section has been defined in
 * the linker to provide dedicated memory for this heap - NL_HEAP.
 */
caddr_t
_sbrk (int incr)
{
      char * prev_heap_end;
      int iMask = R_OS_SysLock(NULL);

      prev_heap_end = pnl_heap_pointer;

      if (pnl_heap_pointer + incr > (char*)NL_HEAP_END)
        {
          /* Some of the libstdc++-v3 tests rely upon detecting
         out of memory errors, so do not abort here.  */
    #if HEAP_CLASH_DETECTION_ENABLED
          extern void abort (void);

          _write (1, "_sbrk: Heap and stack collision\n", 32);

          abort ();
    #else
          errno = ENOMEM;
          R_OS_SysUnlock(NULL, iMask);
          return (caddr_t) -1;
    #endif
        }

      pnl_heap_pointer += incr;

      R_OS_SysUnlock(NULL, iMask);
      return (caddr_t) prev_heap_end;
}

int
_fstat (int file, struct stat * st)
{
  (void)file;
  memset (st, 0, sizeof (* st));
  st->st_mode = S_IFCHR;
  st->st_blksize = 1024;
  return 0;
}

int _stat (const char *fname, struct stat *st)
{

  while(1);

#if 0

  int file;
  /* The best we can do is try to open the file readonly.  If it exists,
     then we can guess a few things about it.  */
  if ((file = _open (fname, O_RDONLY)) < 0)
    return -1;

  memset (st, 0, sizeof (* st));
  st->st_mode = S_IFREG | S_IREAD;
  st->st_blksize = 1024;
  _swiclose (file); /* Not interested in the error.  */
#else

  UNUSED_PARAM(fname);
  UNUSED_PARAM(st);

#endif
  return 0;
}

int
_link (void)
{
  return -1;
}

int
_unlink (const char *path)
{
#ifdef ARM_RDI_MONITOR
  int block[2];
  block[0] = path;
  block[1] = strlen(path);
  return wrap (do_AngelSWI (AngelSWI_Reason_Remove, block)) ? -1 : 0;
#else
  UNUSED_PARAM(path);
  return -1;
#endif
}

void
_raise (void)
{
  return;
}

int
_gettimeofday (struct timeval * tp, void * tzvp)
{
  struct timezone *tzp = tzvp;
  if (tp)
    {
    /* Ask the host for the seconds since the Unix epoch.  */
#ifdef ARM_RDI_MONITOR
      tp->tv_sec = do_AngelSWI (AngelSWI_Reason_Time,NULL);
#else
      {
        int value;
        __asm ("swi %a1; mov %0, r0" : "=r" (value): "i" (SWI_Time) : "r0");
        tp->tv_sec = value;
      }
#endif
      tp->tv_usec = 0;
    }

  /* Return fixed data for the timezone.  */
  if (tzp)
    {
      tzp->tz_minuteswest = 0;
      tzp->tz_dsttime = 0;
    }

  return 0;
}

/* Return a clock that ticks at 100Hz.  */
clock_t
_times (struct tms * tp)
{
  clock_t timeval;

#ifdef ARM_RDI_MONITOR
  timeval = do_AngelSWI (AngelSWI_Reason_Clock,NULL);
#else
  __asm ("swi %a1; mov %0, r0" : "=r" (timeval): "i" (SWI_Clock) : "r0");
#endif

  if (tp)
    {
      tp->tms_utime  = timeval;    /* user time */
      tp->tms_stime  = 0;    /* system time */
      tp->tms_cutime = 0;    /* user time, children */
      tp->tms_cstime = 0;    /* system time, children */
    }

  return timeval;
};


int
_isatty (int fd)
{
#ifdef ARM_RDI_MONITOR
  int fh = remap_handle (fd);
  return wrap (do_AngelSWI (AngelSWI_Reason_IsTTY, &fh));
#else
  return (fd <= 2) ? 1 : 0;  /* one of stdin, stdout, stderr */
#endif
}

int
_system (const char *s)
{
#ifdef ARM_RDI_MONITOR
  int block[2];
  int e;

  /* Hmmm.  The ARM debug interface specification doesn't say whether
     SYS_SYSTEM does the right thing with a null argument, or assign any
     meaning to its return value.  Try to do something reasonable....  */
  if (!s)
    return 1;  /* maybe there is a shell available? we can hope. :-P */
  block[0] = s;
  block[1] = strlen (s);
  e = wrap (do_AngelSWI (AngelSWI_Reason_System, block));
  if ((e >= 0) && (e < 256))
    {
      /* We have to convert e, an exit status to the encoded status of
         the command.  To avoid hard coding the exit status, we simply
     loop until we find the right position.  */
      int exit_code;

      for (exit_code = e; e && WEXITSTATUS (e) != exit_code; e <<= 1)
    continue;
    }
  return e;
#else
  if (s == NULL)
    return 0;
  errno = ENOSYS;
  return -1;
#endif
}

int
_rename (const char * oldpath, const char * newpath)
{
#ifdef ARM_RDI_MONITOR
  int block[4];
  block[0] = oldpath;
  block[1] = strlen(oldpath);
  block[2] = newpath;
  block[3] = strlen(newpath);
  return wrap (do_AngelSWI (AngelSWI_Reason_Rename, block)) ? -1 : 0;
#else
  UNUSED_PARAM(oldpath);
  UNUSED_PARAM(newpath);

  errno = ENOSYS;
  return -1;
#endif
}
