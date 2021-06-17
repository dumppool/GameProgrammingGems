/* Copyright (C) Greg James, 2001. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Greg James, 2001"
 */
/*

gbm.h - Generalised Bitmap Module

Data is stored as an array of lines.
Lines are stored with bottom line first, moving upwards.
Each line is an array of pixels, leftmost first.
Lines are padded to be a multiple of a dword long.
Palettised pixels are either a 1 bit, 4 bit, or 8 bit indexes.
Alternately a B, G, R triple in that order is stored.
This format exactly matches the format used by OS/2 and Windows bitmaps.

One notable point: a 1 in a 1bpp image denotes colour 1, as found by
looking at palette entry 1. Data is not inversed when passed to and from GBM.

This interface file provides access to routines for reading and writing
bitmaps in a variety of image file formats.

Normally file I/O is done using lseek,read and write. Occasionally GBM
needs to access additional files, and it uses open and close to do this.
Sometimes it needs to create a new file, and GBM uses create and close
for this. The 'create' function is an invokation of open with
O_CREAT|O_TRUNC combined with the mode flags it is passed, and
S_IREAD|S_IWRITE passed as the additional optional parameter.

You can trick GBM into using your own versions of open, create, close,
lseek, read and write routines, by calling gbm_io_setup.

One example use of this is that the file descriptor could then be an index
into a array of pointers to C++ iostreams, thus allowing GBM to read and
write file data to and from memory.

On some platforms, the GBM file I/O library may be provided in DLL form.
Therefore it can have its own C run time library, and on some platforms
file descriptors obtained by an executable do not make sense to the
C run time which is a part of the DLL. Hence GBM will be unable to use
the file descriptor. One solution is to use gbm_io_setup to get the GBM
library to call back into the calling application and use its C run time.
Another solution is to have the application use the GBM libraries C run
time to open the file - this is made possible via the gbm_io_ routines.
This is the easier solution, and is used by the sample GBM programs.
The particular offending platform is Visual C++ on Windows NT, everything
works fine for VisualAge C++ on OS/2.

gbm_read_header shall seek to the start of the stream indentified by the
file descriptor argument and then shall invoke the bitmap header reader
routine identified by the format type variable.

gbm_read_palette may only be legally invoked after an immediately
preceeding gbm_read_header.

gbm_read_data may only be legally invoked after an immediately preceeding
gbm_read_palette. In the case of a 24bpp file (which therefore has no
palette), gbm_read_data is additionally allowed to follow a gbm_read_header.

*/

#ifndef GBM_H
#define	GBM_H

#ifndef BOOLEAN_DEFINED
#define	BOOLEAN_DEFINED
typedef	int BOOLEAN;
#define	TRUE  1
#define	FALSE 0
#endif

#ifndef BASICTYPES_DEFINED
#define	BASICTYPES_DEFINED
typedef unsigned  char  byte;
typedef unsigned short  word;
typedef unsigned  long dword;
#endif

typedef int GBM_ERR;
#define	GBM_ERR_OK		((GBM_ERR) 0)
#define	GBM_ERR_MEM		((GBM_ERR) 1)
#define	GBM_ERR_NOT_SUPP	((GBM_ERR) 2)
#define	GBM_ERR_BAD_OPTION	((GBM_ERR) 3)
#define	GBM_ERR_NOT_FOUND	((GBM_ERR) 4)
#define	GBM_ERR_BAD_MAGIC	((GBM_ERR) 5)
#define	GBM_ERR_BAD_SIZE	((GBM_ERR) 6)
#define	GBM_ERR_READ		((GBM_ERR) 7)
#define	GBM_ERR_WRITE		((GBM_ERR) 8)
#define	GBM_ERR_BAD_ARG		((GBM_ERR) 9)

#define	GBM_FT_R1		0x0001
#define	GBM_FT_R4		0x0002
#define	GBM_FT_R8		0x0004
#define	GBM_FT_R24		0x0008
#define	GBM_FT_W1		0x0010
#define	GBM_FT_W4		0x0020
#define	GBM_FT_W8		0x0040
#define	GBM_FT_W24		0x0080

typedef struct
	{
	char *short_name;		/* Eg: "Targa"                       */
	char *long_name;		/* Eg: "Truevision Targa / Vista"    */
	char *extensions;		/* Eg: "TGA VST"                     */
	int flags;			/* What functionality exists         */
	} GBMFT;

typedef struct { byte r, g, b; } GBMRGB;

#define	PRIV_SIZE 2000

typedef struct
	{
	int w, h, bpp;			/* Bitmap dimensions                 */
	byte priv[PRIV_SIZE];		/* Private internal buffer           */
	} GBM;

#ifndef _GBM_

#if defined(OS2)
#define	GBMEXPORT
#define	GBMENTRY _Optlink
#elif defined(WIN32)
#define	GBMEXPORT 
//#define	GBMENTRY 
//#define	GBMEXPORT __declspec(dllexport)
#define	GBMENTRY 
#elif defined(DOS32)
#define	GBMEXPORT
#define	GBMENTRY __export _cdecl
#else
#define	GBMEXPORT
#define	GBMENTRY
#endif
         
#if defined(__cplusplus)
extern "C"
{
#endif

GBMEXPORT GBM_ERR GBMENTRY gbm_init(void);
GBMEXPORT GBM_ERR GBMENTRY gbm_deinit(void);

GBMEXPORT GBM_ERR GBMENTRY gbm_io_setup(
	int  (*open  )(const char *fn, int mode),
	int  (*create)(const char *fn, int mode),
	void (*close )(int fd),
	long (*lseek )(int fd, long pos, int whence),
	int  (*read  )(int fd, void *buf, int len),
	int  (*write )(int fd, const void *buf, int len)
	);

GBMEXPORT int  GBMENTRY gbm_io_open  (const char *fn, int mode);
GBMEXPORT int  GBMENTRY gbm_io_create(const char *fn, int mode);
GBMEXPORT void GBMENTRY gbm_io_close (int fd);
GBMEXPORT long GBMENTRY gbm_io_lseek (int fd, long pos, int whence);
GBMEXPORT int  GBMENTRY gbm_io_read  (int fd, void *buf, int len);
GBMEXPORT int  GBMENTRY gbm_io_write (int fd, const void *buf, int len);

GBMEXPORT GBM_ERR GBMENTRY gbm_query_n_filetypes(int *n_ft);
GBMEXPORT GBM_ERR GBMENTRY gbm_query_filetype(int ft, GBMFT *gbmft);
GBMEXPORT GBM_ERR GBMENTRY gbm_guess_filetype(const char *fn, int *ft);

GBMEXPORT GBM_ERR GBMENTRY gbm_read_header(const char *fn, int fd, int ft, GBM *gbm, const char *opt);
GBMEXPORT GBM_ERR GBMENTRY gbm_read_palette(int fd, int ft, GBM *gbm, GBMRGB *gbmrgb);
GBMEXPORT GBM_ERR GBMENTRY gbm_read_data(int fd, int ft, GBM *gbm, byte *data);
GBMEXPORT GBM_ERR GBMENTRY gbm_write(const char *fn, int fd, int ft, const GBM *gbm, const GBMRGB *gbmrgb, const byte *data, const char *opt);

GBMEXPORT const char * GBMENTRY gbm_err(GBM_ERR rc);

GBMEXPORT int     GBMENTRY gbm_version(void);

extern BOOLEAN gbm_ref_vert(const GBM *gbm, byte *data);
extern BOOLEAN gbm_ref_horz(const GBM *gbm, byte *data);
extern void gbm_transpose(const GBM *gbm, const byte *data, byte *data_t);



#ifdef OS2
/*...s_System entrypoints:0:*/
/* For GBM.DLL to be callable from IBM Smalltalk under OS/2, the entrypoints
   must be of _System calling convention. These veneers help out here.
   I can't just change the usual entrypoints because people depend on them.
   For portability, avoid these entrypoints, use the gbm_ ones. */

GBM_ERR _System Gbm_init(void);
GBM_ERR _System Gbm_deinit(void);
int     _System Gbm_io_open(const char *fn, int mode);
int     _System Gbm_io_create(const char *fn, int mode);
void    _System Gbm_io_close(int fd);
long    _System Gbm_io_lseek(int fd, long pos, int whence);
int     _System Gbm_io_read (int fd, void *buf, int len);
int     _System Gbm_io_write(int fd, const void *buf, int len);
GBM_ERR _System Gbm_query_n_filetypes(int *n_ft);
GBM_ERR	_System Gbm_guess_filetype(const char *fn, int *ft);
GBM_ERR _System Gbm_query_filetype(int ft, GBMFT *gbmft);
GBM_ERR _System Gbm_read_header(const char *fn, int fd, int ft, GBM *gbm, const char *opt);
GBM_ERR _System Gbm_read_palette(int fd, int ft, GBM *gbm, GBMRGB *gbmrgb);
GBM_ERR _System Gbm_read_data(int fd, int ft, GBM *gbm, byte *data);
GBM_ERR _System Gbm_write(const char *fn, int fd, int ft, const GBM *gbm, const GBMRGB *gbmrgb, const byte *data, const char *opt);
const char * _System Gbm_err(GBM_ERR rc);
int     _System Gbm_version(void);
/*...e*/
#endif

#if defined(__cplusplus)
};
#endif

#endif


/*...sincludes:0:*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>
#include <malloc.h>
#if defined(AIX) || defined(LINUX)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef O_BINARY
#define O_BINARY  0
#endif

#endif
