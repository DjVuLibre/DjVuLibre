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

#ifdef __GNUG__
#pragma implementation
#endif
#include "DjVuConfig.h"

#include "GException.h"
#include "GThreads.h"
#include "GOS.h"
#include "GURL.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif
#ifdef HAVE_MATH_H
#include <math.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifndef __CYGWIN32__
#ifdef HAVE_ATLBASE_H
#include <atlbase.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
#ifdef HAVE_DIRECT_H
#include <direct.h>
#endif
#endif

#ifdef HAVE_UNIX_H
#include <unix.h>
#endif
#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif
#ifdef HAVE_PWD_H
# include <pwd.h>
#endif
#ifdef HAVE_STDIO_H
# include <stdio.h>
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

// -- TRUE FALSE
#undef TRUE
#undef FALSE
#define TRUE 1
#define FALSE 0

// -- MAXPATHLEN
#ifndef MAXPATHLEN
#ifdef _MAX_PATH
#define MAXPATHLEN _MAX_PATH
#else
#define MAXPATHLEN 1024
#endif
#else
#if ( MAXPATHLEN < 1024 )
#undef MAXPATHLEN
#define MAXPATHLEN 1024
#endif
#endif


#ifdef HAVE_NAMESPACES
namespace DJVU {
# ifdef NOT_DEFINED // Just to fool emacs c++ mode
}
#endif
#endif

// static const char filespecslashes[] = "file://";
static const char slash='/';
static const char percent='%';
static const char localhostspec1[] = "//localhost/";
static const char backslash='\\';
static const char colon=':';

static const char localhost[] = "localhost";
static const char localhostspec2[] = "///";
static const char filespec[] = "file:";
static const char dot='.';
static const char nillchar=0;
#if defined(UNIX) || defined(__CYGWIN32__)
  static const char tilde='~';
  static const char root[] = "/";
#elif defined(WIN32)
  static const char root[] = "\\";
#elif defined(macintosh)
  static char const * const root = &nillchar; 
#else
#error "Define something here for your operating system"
#endif


// -----------------------------------------
// Functions for dealing with filenames
// -----------------------------------------

static inline int
finddirsep(const GUTF8String &fname)
{
#if defined(UNIX) || defined(__CYGWIN32__)
  return fname.rsearch('/',0);
#elif defined(WIN32)
  return fname.rcontains("\\/",0);
#elif defined(macintosh)
  return fname.rcontains(":/",0);
#else
#error "Define something here for your operating system"
#endif  
}


// basename(filename[, suffix])
// -- returns the last component of filename and removes suffix
//    when present. works like /bin/basename.
GUTF8String 
GOS::basename(const GUTF8String &gfname, const char *suffix)
{
  if(!gfname.length())
    return gfname;

  const char *fname=gfname;
#if defined(WIN32) && !defined(__CYGWIN32__)
  // Special cases
  if (fname[1] == colon)
  {
    if(!fname[2])
    {
      return gfname;
    }
    if (!fname[3] && (fname[2]== slash || fname[2]== backslash))
    {
      char string_buffer[4];
      string_buffer[0] = fname[0];
      string_buffer[1] = colon;
      string_buffer[2] = backslash; 
      string_buffer[3] = 0; 
      return string_buffer;
    }
  }
#endif


  // Allocate buffer
  GUTF8String retval(gfname,finddirsep(gfname)+1,(unsigned int)(-1));
  fname=retval;

  // Process suffix
  if (suffix)
  {
    if (suffix[0]== dot )
      suffix ++;
    if (suffix[0])
    {
      const GUTF8String gsuffix(suffix);
      const int sl = gsuffix.length();
      const char *s = fname + strlen(fname);
      if (s > fname + sl)
      {
        s = s - (sl + 1);
        if(*s == dot && (GUTF8String(s+1).downcase() == gsuffix.downcase()))
        {
          retval.setat((int)((size_t)s-(size_t)fname),0);
        }
      }
    }
  }
  return retval;
}



// errmsg --
// -- A small helper function returning a 
//    stdio error message in a static buffer.

static GNativeString 
errmsg()
{
  GNativeString buffer;
#ifdef REIMPLEMENT_STRERROR
  const char *errname = "Unknown libc error";
  if (errno>0 && errno<sys_nerr)
    errname = sys_errlist[errno];
#else
#ifndef UNDER_CE
  const char *errname = strerror(errno);
  buffer.format("%s (errno = %d)", errname, errno);
#else
  const char *errname = "Unknown error from GOS.cpp under Windows CE" ;
  buffer.format("%s (errno = %d)", errname, -1);
#endif
#endif
  return buffer;
}



// -----------------------------------------
// Functions for measuring time
// -----------------------------------------

// ticks() --
// -- returns the number of milliseconds elapsed since 
//    a system dependent date.
unsigned long 
GOS::ticks()
{
#ifndef AUTOCONF
#if defined(UNIX) || defined(__CYGWIN32__)
#define HAVE_GETTIMEOFDAY 1
#endif
#endif
#ifdef HAVE_GETTIMEOFDAY
  struct timeval tv;
  if (gettimeofday(&tv, NULL) < 0)
    G_THROW(errmsg());
  return (unsigned long)( ((tv.tv_sec & 0xfffff)*1000) 
                          + (tv.tv_usec/1000) );
#elif defined(WIN32)
  DWORD clk = GetTickCount();
  return (unsigned long)clk;
#else
#ifdef macintosh
  return (unsigned long)((double)TickCount()*16.66);
#else
#error "Define something here for your operating system"
#endif
#endif  
}

// sleep(int milliseconds) --
// -- sleeps during the specified time (in milliseconds)
void 
GOS::sleep(int milliseconds)
{
#ifndef AUTOCONF
#if defined(UNIX) || defined(__CYGWIN32__)
#define HAVE_SELECT 1
#endif
#endif
#ifdef HAVE_SELECT
  struct timeval tv;
  tv.tv_sec = milliseconds / 1000;
  tv.tv_usec = (milliseconds - (tv.tv_sec * 1000)) * 1000;
#if defined(THREADMODEL) && (THREADMODEL==COTHREADS)
  GThread::select(0, NULL, NULL, NULL, &tv);
#else
  select(0, NULL, NULL, NULL, &tv);
#endif
#elif defined(WIN32)
  Sleep(milliseconds);
#else
#ifdef macintosh
    unsigned long tick = ticks(), now;
    while (1) {
        now = ticks();
        if ((tick+milliseconds) < now)
            break;
        GThread::yield();
    }
#endif
#endif
}

  

#if 0
/*MBCS*/
GString
GOS::encode_mbcs_reserved(const char * filename)
      // Called from ByteStream to encode new OS-safe filenames
{
   const char *hex = "0123456789ABCDEF";
   
   GString res;

   for(const char * ptr=filename;*ptr;ptr++)
   {
#if defined(WIN32) && !defined(__CYGWIN32__)
 		if (IsDBCSLeadByte((BYTE)*ptr)) { //MBCS DBCS
			// escape sequence
			res+=hex[(*ptr >> 4) & 0xf];
			res+=hex[(*ptr) & 0xf];
			ptr++;
			if (*ptr){
				// escape sequence
				res+=hex[(*ptr >> 4) & 0xf];
				res+=hex[(*ptr) & 0xf];
			}
			continue;
		}
#endif
     if ((*ptr>='a' && *ptr<='z')
        || (*ptr>='A' && *ptr<='Z')
        || (*ptr>='0' && *ptr<='9')
        || (strchr("$-_.+!*'(),:;", *ptr))) // Added : because of windows!
      {
        res+=*ptr;
      }else
      {
      // escape sequence
        //res+=percent;
        res+=hex[(*ptr >> 4) & 0xf];
        res+=hex[(*ptr) & 0xf];
      }
   }
   
   return res;
}
/*MBCS*/
#endif

// -----------------------------------------
// Testing
// -----------------------------------------

#if defined(AUTOCONF) && !defined(HAVE_STRERROR)
#define NEED_STRERROR
#elif defined(sun) && ! defined(svr4)
#define NEED_STRERROR
#endif
#ifdef NEED_STRERROR
// strerror() is not defined under SunOS.
char *
strerror(int errno)
{
  extern int sys_nerr;
  extern char *sys_errlist[];
  if (errno>0 && errno<sys_nerr) 
    return sys_errlist[errno];
  return "unknown stdio error";
}
#endif

// cwd([dirname])
// -- changes directory to dirname (when specified).
//    returns the full path name of the current directory. 
GUTF8String 
GOS::cwd(const GUTF8String &dirname)
{
#if defined(WIN32) && !defined(__CYGWIN32__)
#define CHDIR(x) _chdir(x)
#else
#define CHDIR(x) chdir(x)
#endif

#ifdef UNDER_CE
  return GUTF8String(dot) ;
#else
  if (dirname.length() && CHDIR(dirname.getUTF82Native())==-1)//MBCS cvt
    G_THROW(errmsg());
  char *string_buffer;
  GPBuffer<char> gstring_buffer(string_buffer,MAXPATHLEN+1);
  char *result = getcwd(string_buffer,MAXPATHLEN);
  if (!result)
    G_THROW(errmsg());
#if defined(WIN32) && !defined(__CYGWIN32__)
  char drv[2];
  drv[0]= dot ; drv[1]=0;
  GetFullPathName(drv, MAXPATHLEN, string_buffer, &result);
#elif !defined(UNIX) && !defined(macintosh)
#error "Define something here for your operating system"
#endif 
  return GNativeString(string_buffer).getNative2UTF8();//MBCS cvt
#endif
}

GUTF8String
GOS::getenv(const GUTF8String &name)
{
  GUTF8String retval;
#ifndef UNDER_CE
  if(name.length())
  {
    const char *env=::getenv(name.getUTF82Native());
    if(env)
    {
      retval=GNativeString(env);
    }
  }
#endif
  return retval;
}



#ifdef HAVE_NAMESPACES
}
# ifndef NOT_USING_DJVU_NAMESPACE
using namespace DJVU;
# endif
#endif

