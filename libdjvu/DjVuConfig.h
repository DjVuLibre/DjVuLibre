//C-  -*- C++ -*-
//C- -------------------------------------------------------------------
//C- DjVuLibre-3.5
//C- Copyright (c) 2002  Leon Bottou and Yann Le Cun.
//C- Copyright (c) 2001  AT&T
//C-
//C- This software is subject to, and may be distributed under, the
//C- GNU General Public License, Version 2. The license should have
//C- accompanied the software or you may obtain a copy of the license
//C- from the Free Software Foundation at http://www.fsf.org .
//C-
//C- This program is distributed in the hope that it will be useful,
//C- but WITHOUT ANY WARRANTY; without even the implied warranty of
//C- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//C- GNU General Public License for more details.
//C- 
//C- DjVuLibre-3.5 is derived from the DjVu(r) Reference Library
//C- distributed by Lizardtech Software.  On July 19th 2002, Lizardtech 
//C- Software authorized us to replace the original DjVu(r) Reference 
//C- Library notice by the following text (see doc/lizard2002.djvu):
//C-
//C-  ------------------------------------------------------------------
//C- | DjVu (r) Reference Library (v. 3.5)
//C- | Copyright (c) 1999-2001 LizardTech, Inc. All Rights Reserved.
//C- | The DjVu Reference Library is protected by U.S. Pat. No.
//C- | 6,058,214 and patents pending.
//C- |
//C- | This software is subject to, and may be distributed under, the
//C- | GNU General Public License, Version 2. The license should have
//C- | accompanied the software or you may obtain a copy of the license
//C- | from the Free Software Foundation at http://www.fsf.org .
//C- |
//C- | The computer code originally released by LizardTech under this
//C- | license and unmodified by other parties is deemed "the LIZARDTECH
//C- | ORIGINAL CODE."  Subject to any third party intellectual property
//C- | claims, LizardTech grants recipient a worldwide, royalty-free, 
//C- | non-exclusive license to make, use, sell, or otherwise dispose of 
//C- | the LIZARDTECH ORIGINAL CODE or of programs derived from the 
//C- | LIZARDTECH ORIGINAL CODE in compliance with the terms of the GNU 
//C- | General Public License.   This grant only confers the right to 
//C- | infringe patent claims underlying the LIZARDTECH ORIGINAL CODE to 
//C- | the extent such infringement is reasonably necessary to enable 
//C- | recipient to make, have made, practice, sell, or otherwise dispose 
//C- | of the LIZARDTECH ORIGINAL CODE (or portions thereof) and not to 
//C- | any greater extent that may be necessary to utilize further 
//C- | modifications or combinations.
//C- |
//C- | The LIZARDTECH ORIGINAL CODE is provided "AS IS" WITHOUT WARRANTY
//C- | OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
//C- | TO ANY WARRANTY OF NON-INFRINGEMENT, OR ANY IMPLIED WARRANTY OF
//C- | MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
//C- +------------------------------------------------------------------
// 
// $Id$
// $Name$

#ifndef _DJVUCONFIG_H
#define _DJVUCONFIG_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __CYGWIN32__
#ifdef WIN32
#undef WIN32
#endif
#ifndef UNIX
#define UNIX 1
#endif
#endif

#ifndef AUTOCONF
// Options

#define DJVU_STATIC_LIBRARY 1
#if defined(WIN32) && !defined(__CYGWIN32__)
#define HAVE__WFOPEN 1
#endif
#if defined(UNIX) || defined(__CYGWIN32__)
#define HAS_MEMMAP 1
#endif


// Includes

#define HAVE_STDIO_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_FCNTL_H 1
#define HAVE_CTYPE_H 1
#define HAVE_STDARG_H 1
#define HAVE_MATH_H 1
#define HAVE_TIME_H 1
#define HAVE_PROCESS_H 1
#define HAVE_SETJMP_H 1
#define HAVE_DIRENT_H 1
#define HAVE_JCONFIG_H 1
#define HAVE_JERROR_H 1
#define HAVE_JPEGLIB_H 1


#ifdef HAS_ICONV
#define HAVE_ICONV_H 1
#endif

#ifdef HAS_DLOPEN
#define HAVE_DLFCN_H 1
#endif

#if HAS_WCHAR
# if !defined(AUTOCONF)
#   define HAVE_WCHAR_H 1
# endif
# if HAS_WCTYPE
#   define HAVE_WCTYPE_H 1
# endif
#endif

#if defined(UNIX) || defined(__CYGWIN32__)
#define HAVE_SYS_STAT_H 1
#define HAVE_UNISTD_H 1
#define HAVE_SYS_MMAN_H 1
#define HAVE_PWD_H 1
#define HAVE_GRP_H 1
#define HAVE_SYS_TIME_H 1
#define TIME_WITH_SYS_TIME 1
#if defined(XENIX)
# define USE_DIRECT 1
# define HAVE_SYS_NDIR_H 1
#elif defined(OLDBSD)
# define USE_DIRECT 1
# define HAVE_SYS_DIR_H 1
#endif
#endif /* UNIX */

#if defined(UNIX) || defined(__CYGWIN32__) || (!defined(macintosh) && !defined(UNDER_CE))   
#define HAVE_SYS_TYPES_H 1
#endif

#ifdef macintosh
#define HAVE_UNISTD_H 1
#define HAVE_UNIX_H 1
#endif

#ifndef UNDER_CE
#define HAVE_ASSERT_H 1
#define HAVE_ERRNO_H 1
#define HAVE_LOCALE_H 1
#endif

#if defined(WIN32) || defined(__CYGWIN32__)
#define HAVE_IO_H 1
#endif

#ifdef WIN32
#define HAVE_TCHAR_H 1
#define HAVE_ATLBASE_H 1
#define HAVE_WINDOWS_H 1
#ifndef UNDER_CE
#define HAVE_DIRECT_H 1
#endif // !UNDER_CE
#endif // WIN32

#endif

#endif /* _DJVUCONFIG_H_ */


