/*
 * dll.c
 *
 * Description:
 * This translation unit implements DLL initialisation.
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads-win32 - POSIX Threads Library for Win32
 *      Copyright(C) 1998 John E. Bossom
 *      Copyright(C) 1999,2012 Pthreads-win32 contributors
 *
 *      Homepage1: http://sourceware.org/pthreads-win32/
 *      Homepage2: http://sourceforge.net/projects/pthreads4w/
 *
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *      http://sources.redhat.com/pthreads-win32/contributors.html
 *
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 *
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#if defined(PTW32_STATIC_LIB) && defined(_MSC_VER) && _MSC_VER >= 1400
#  undef PTW32_STATIC_LIB
#  define PTW32_STATIC_TLSLIB
#endif

#include "pthread.h"
#include "implement.h"

#if !defined(PTW32_STATIC_LIB)

#if defined(_MSC_VER)
/*
 * lpvReserved yields an unreferenced formal parameter;
 * ignore it
 */
#pragma warning( disable : 4100 )
#endif

#if defined(__cplusplus)
/*
 * Dear c++: Please don't mangle this name. -thanks
 */
extern "C"
#endif				/* __cplusplus */
  BOOL WINAPI
DllMain (HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
  BOOL result = PTW32_TRUE;

  switch (fdwReason)
    {

    case DLL_PROCESS_ATTACH:
      result = pthread_win32_process_attach_np ();
      break;

    case DLL_THREAD_ATTACH:
      /*
       * A thread is being created
       */
      result = pthread_win32_thread_attach_np ();
      break;

    case DLL_THREAD_DETACH:
      /*
       * A thread is exiting cleanly
       */
      result = pthread_win32_thread_detach_np ();
      break;

    case DLL_PROCESS_DETACH:
      (void) pthread_win32_thread_detach_np ();
      result = pthread_win32_process_detach_np ();
      break;
    }

  return (result);

}				/* DllMain */

#endif /* !PTW32_STATIC_LIB */

#if ! defined(PTW32_BUILD_INLINED)
/*
 * Avoid "translation unit is empty" warnings
 */
typedef int foo;
#endif

/* Visual Studio 8+ can leverage PIMAGE_TLS_CALLBACK CRT segments, which
 * give a static lib its very own DllMain.
 */
#ifdef PTW32_STATIC_TLSLIB

#if ! defined(PTW32_CONFIG_MINGW)

static void WINAPI
TlsMain(PVOID h, DWORD r, PVOID u)
{
  (void)DllMain((HINSTANCE)h, r, u);
}

#ifdef _M_X64
# pragma comment (linker, "/INCLUDE:_tls_used")
# pragma comment (linker, "/INCLUDE:_xl_b")
# pragma const_seg(".CRT$XLB")
EXTERN_C const PIMAGE_TLS_CALLBACK _xl_b = TlsMain;
# pragma const_seg()
#else
# pragma comment (linker, "/INCLUDE:__tls_used")
# pragma comment (linker, "/INCLUDE:__xl_b")
# pragma data_seg(".CRT$XLB")
EXTERN_C PIMAGE_TLS_CALLBACK _xl_b = TlsMain;
# pragma data_seg()
#endif /* _M_X64 */

#else

static void NTAPI
TlsMain (void * h, DWORD r, void *u)
{
  (void)DllMain((HINSTANCE)h, r, u);
}

#if defined(__MINGW64__) || (__MINGW64_VERSION_MAJOR) || (__MINGW32_MAJOR_VERSION >3) || \
((__MINGW32_MAJOR_VERSION==3) && (__MINGW32_MINOR_VERSION>=18))
extern "C"
{
__attribute__ ((section(".CRT$XLB"))) PIMAGE_TLS_CALLBACK __crt_xl_tls_callback__ = TlsMain;
}
#else
extern "C" {

__attribute__((section(".ctors"))) void (* gcc_ctor )() = on_process_enter;
__attribute__((section(".dtors"))) void (* gcc_dtor)() = on_thread_exit;
__attribute__((section(".dtors.zzz"))) void (* gcc_dtor )() = on_process_exit;

ULONG __tls_index__ = 0;
char __tls_end__ __attribute__((section(".tls$zzz"))) = 0;
char __tls_start__ __attribute__((section(".tls"))) = 0;

__attribute__ ((section(".CRT$XLA"))) PIMAGE_TLS_CALLBACK __crt_xl_start__ = 0;
__attribute__ ((section(".CRT$XLZ"))) PIMAGE_TLS_CALLBACK __crt_xl_end__ = 0;
}

__attribute__ ((section(".rdata$T"))) extern "C" const IMAGE_TLS_DIRECTORY32 _tls_used =
{
(DWORD) &__tls_start__,
(DWORD) &__tls_end__,
(DWORD) &__tls_index__,
(DWORD) (&__crt_xl_start__+1),
(DWORD) 0,
(DWORD) 0
};
#endif

#endif

#endif /* PTW32_STATIC_TLSLIB */
