/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Do we know about the C type sigset_t? */
#undef HAVE_SIGSET_T

/* Define if you have the <signal.h> header file.  */
#undef HAVE_SIGNAL_H

/* Define if you don't have Win32 DuplicateHandle. (eg. WinCE) */
#undef NEED_DUPLICATEHANDLE

/* Define if you don't have Win32 _beginthreadex. (eg. WinCE) */
#undef NEED_CREATETHREAD

/* Define if you don't have Win32 errno. (eg. WinCE) */
#undef NEED_ERRNO

/* Define if you don't have Win32 calloc. (eg. WinCE)  */
#undef NEED_CALLOC

/* Define if you don't have Win32 ftime. (eg. WinCE)  */
#undef NEED_FTIME

/* Define if you don't have Win32 semaphores. (eg. WinCE)  */
#undef NEED_SEM

/* Define if you need to convert string parameters to unicode. (eg. WinCE)  */
#undef NEED_UNICODE_CONSTS

/* 
 * Target specific groups 
 */
#ifdef WINCE
#define NEED_DUPLICATEHANDLE
#define NEED_CREATETHREAD
#define NEED_ERRNO
#define NEED_CALLOC
#define NEED_FTIME
#define NEED_SEM
#define NEED_UNICODE_CONSTS
#endif
