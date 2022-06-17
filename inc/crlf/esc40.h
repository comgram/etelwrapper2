/*
 * esc40.h
 *
 * Copyright (c) 1997-2021 ETEL SA. All Rights Reserved.
 *
 * This software is the confidential and proprietary information of ETEL SA 
 * ("Confidential Information"). You shall not disclose such Confidential 
 * Information and shall use it only in accordance with the terms of the 
 * license agreement you entered into with ETEL.
 *
 * This software is provided "AS IS" without a warranty or representations of any kind. 
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY IMPLIED WARRANTY 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT ARE HEREBY EXCLUDED. ETEL AND ITS 
 * LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING 
 * THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL ETEL OR ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR 
 * DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS 
 * OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN IF ETEL HAS BEEN ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGES. THE ENTIRE RISK ARISING OUT OF USE, PERFORMANCE OR NON-PERFORMANCE OF THE SOFTWARE 
 * REMAINS WITH THE LICENSEE. IF ETEL SHOULD NEVERTHELESS BE FOUND LIABLE, WHETER DIRECTLY OR INDRECTLY, FOR ANY LOSS, 
 * DAMAGE OR INJURY ARISING UNDER THIS AGREEMENT OR OTHERWISE, REGARDLESS OF CAUSE OR ORIGIN, ON ANY BASIS WHATSOEVER, 
 * ITS TOTAL MAXIMUM LIABILITY IS LIMITED TO CHF 100.000 WHICH WILL BE THE COMPLETE AND EXCLUSIVE REMEDY AGAINST ETEL.

 * This software is in particular not designed or intended .for use in on-line control of aircraft, air traffic, aircraft 
 * navigation or aircraft communications; or in the design, construction, Operation or maintenance of any nuclear facility. 
 * Licensee represents and warrants that it will not use or redistribute the Software for such purposes.
 */
/**********************************************************************************************
									THIRD-PARTY LICENSE


									7ZIP LICENSE
									------------
EDI Windows version uses 7ZIP functionality, especially for System Configuration Management.
7z.exe and 7z.dll are piece of binary code, provided with EDI package.
The source code can be found here: http://www.7-zip.org
These piece of program are subject to following licensing:
---------------------------------------------------------------------------
  7-Zip
  ------
  License for use and distribution
  --------------------------------

  7-Zip Copyright (C) 1999-2011 Igor Pavlov.

  Licenses for files are:

    1) 7z.dll: GNU LGPL + unRAR restriction
    2) All other files:  GNU LGPL

  The GNU LGPL + unRAR restriction means that you must follow both 
  GNU LGPL rules and unRAR restriction rules.


  Note: 
    You can use 7-Zip on any computer, including a computer in a commercial 
    organization. You don't need to register or pay for 7-Zip.


  GNU LGPL information
  --------------------

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You can receive a copy of the GNU Lesser General Public License from 
    http://www.gnu.org/


  unRAR restriction
  -----------------

    The decompression engine for RAR archives was developed using source 
    code of unRAR program.
    All copyrights to original unRAR code are owned by Alexander Roshal.

    The license for original unRAR code has the following restriction:

      The unRAR sources cannot be used to re-create the RAR compression algorithm, 
      which is proprietary. Distribution of modified unRAR sources in separate form 
      or as a part of other software is permitted, provided that it is clearly
      stated in the documentation and source comments that the code may
      not be used to develop a RAR (WinRAR) compatible archiver.


  --
  Igor Pavlov
---------------------------------------------------------------------------

								FreeRTOS lwIP LICENSE
								---------------------
EDI FreeRTOS version uses lwIP TCP/IP stack, delivered under BSD license :

Copyright (c) 2001-2004 Swedish Institute of Computer Science.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,  are 
permitted provided that the following conditions are met:
1.	Redistributions of source code must retain the above copyright notice,  
    this list of conditions and the following disclaimer.
2.	Redistributions in binary form must reproduce the above copyright notice,  
    this list of conditions and the following disclaimer in the documentation and/or
	other materials provided with the distribution.
3.	The name of the author may not be used to endorse or promote products derived from 
    this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Author: Adam Dunkels adam@sics.se

**********************************************************************************************************/

/**
 * EDI-Version : 4.25A
 * This header file contains public declarations for the compiler interface library.\n
 * This library allows access to ETEL seauence compiler \n
 * This library is conformed to POSIX 1003.1c 
 * @file esc40.h
 */


#ifndef _ESC40_H
#define _ESC40_H


/**********************************************************************************************************/
/*** Libraries */
/**********************************************************************************************************/

/*----------------------*/
/* common libraries		*/
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "etb40.h"
#include "dsa40.h"

/**
 * @defgroup ESCAll ESC All functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup ESCErrorsAndWarnings ESC Errors and warnings
 */
/*@{*/
/*@}*/

/**
 * @defgroup ESCCompilation ESC Download and compilation functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup ESCUtils ESC Utility functions
 */
/*@{*/
/*@}*/

/**
 * @example compiler.c
 */

/**********************************************************************************************************/
/*** Literals */
/**********************************************************************************************************/

#ifdef __WIN32__		/* defined by Borland C++ Builder */
	#ifndef WIN32
		#define WIN32
	#endif
#endif

#ifdef __cplusplus
	#ifdef ETEL_OO_API      /* defined by the user when he need the Object Oriented interface */
		#define ESC_OO_API
	#endif
#endif

#ifdef __cplusplus
	extern "C" {
#endif

/*----------------------------------------------------------------------------------*/
/* BYTE ORDER */
/*----------------------------------------------------------------------------------*/

#ifndef __BYTE_ORDER

	/*------------------------------*/
	/* Windows Byte order			*/
	#if defined WIN32
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#define __BYTE_ORDER __LITTLE_ENDIAN			/* define byte order for INTEL processor */
	#endif /*WIN32*/

	/*------------------------------*/
	/* QNX4 Byte order				*/
	#if defined WIN32 || defined QNX4
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#define __BYTE_ORDER __LITTLE_ENDIAN			/* define byte order for INTEL processor */
	#endif /*QNX4*/

	/*------------------------------*/
	/* POSIX SOLARIS Byte order		*/
	#if defined POSIX && defined SOLARIS
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#ifdef _BIG_ENDIAN
			#define __BYTE_ORDER __BIG_ENDIAN			/* define byte order for SPARC processor */
		#else
			#define __BYTE_ORDER __LITTLE_ENDIAN        /* define byte order for SPARC processor */
		#endif
	#endif /*SOLARIS*/

	/*------------------------------*/
	/* POSIX LINUX Byte order		*/
	#if defined POSIX && defined LINUX
		#include <endian.h>
	#endif /*LINUX*/

	/*------------------------------*/
	/* POSIX XENOMAI Byte order		*/
	#if defined POSIX && defined XENOMAI
		#include <endian.h>
	#endif /*XENOMAI*/

	/*------------------------------*/
	/* POSIX QNX6 Byte order		*/
	#if defined POSIX && defined QNX6
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#ifdef __BIGENDIAN__
			#define __BYTE_ORDER __BIG_ENDIAN			/* define byte order for SPARC processor */
		#else
			#define __BYTE_ORDER __LITTLE_ENDIAN        /* define byte order for SPARC processor */
		#endif
	#endif /*QNX6*/

	/*------------------------------*/
	/* VXWORKS Byte order			*/
	#ifdef VXWORKS
		#define __LITTLE_ENDIAN _LITTLE_ENDIAN
		#define __BIG_ENDIAN _BIG_ENDIAN
		#define __BYTE_ORDER _BYTE_ORDER
	#endif /*VXWORKS*/

#endif /*BYTE_ORDER*/

/*------------------------------------------------------------------------------
 * esc_compile_cseq_buffer flags
 *-----------------------------------------------------------------------------*/
#define ESC_FLAG_AVOID_DOWNLOAD_SRCCODE 0x00000001	/**< Avoid download source code of pre-compiled sequence file */
#define ESC_FLAG_AVOID_DOWNLOAD_DBGINFO 0x00000002	/**< Avoid download debug info of pre-compiled sequence file */

#define ESC_FLAG_AVOID_INSERT_SRCCODE	0x00000004	/**< Avoid insert source code in pre-compiled sequence file */

/*------------------------------------------------------------------------------
 * error codes - c
 *-----------------------------------------------------------------------------*/
#define ESC_EWARNING                     -916        /**< the compilation has generated a warning */
#define ESC_ENODBGINFO                   -915        /**< the debug informations are not available */
#define ESC_ENOTAVAILABLE                -914        /**< the required element is not available */
#define ESC_EBADLABEL                    -913        /**< the label numer is out of range */
#define ESC_EBADSEQVERSION               -912        /**< the sequence version is not correct */
#define ESC_EISOCONV                     -911        /**< an iso converter has not been set */
#define ESC_EUPLOAD                      -910        /**< the upload process return an error */
#define ESC_EFORMAT                      -909        /**< bad file format */
#define ESC_EEOF                         -908        /**< unexpected end of file reached */
#define ESC_EIOACCESS                    -907        /**< cannot access to the file */
#define ESC_EBUFFEROVERFLOW              -906        /**< the buffer is too small */
#define ESC_EDOWNLOAD                    -905        /**< the download process return an error */
#define ESC_ECOMPILE                     -904        /**< the compilation process return an error */
#define ESC_EINTERNAL                    -903        /**< some internal error in the etel software */
#define ESC_EBADSTATE                    -902        /**< this operation is not allowed in this state */
#define ESC_ESYSTEM                      -901        /**< some system resource return an error */
#define ESC_EBADPARAM                    -900        /**< one of the parameter is not valid */

	/* ESC_EOVERFLOW becomes ESC_EBUFFEROVERFLOW because C++ EOVERFLOW is already defined in errno.h */
	/* For C interface, we define manually ESC_EOVERFLOW to be compatible with before */
	#define ESC_EOVERFLOW ESC_EBUFFEROVERFLOW
/*------------------------------------------------------------------------------
 * convert special value / macro
 *-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * diagnostic macros
 *-----------------------------------------------------------------------------*/
#define ESC_DIAG(err, esc)                      (esc_diag(__FILE__, __LINE__, err, esc))
#define ESC_SDIAG(str, err, esc)                (esc_sdiag(str, __FILE__, __LINE__, err, esc))
#define ESC_FDIAG(output_file_name, err, esc)   (esc_fdiag(output_file_name, __FILE__, __LINE__, err, esc))


/**********************************************************************************************************/
/*** Types */
/**********************************************************************************************************/

/*------------------------------------------------------------------------------
 * type modifiers
 *-----------------------------------------------------------------------------*/
/*------------------------------*/
/* WINDOWS type modifiers		*/
#ifdef WIN32
	#define _ESC_EXPORT __cdecl                          /* function exported by static library */
	#define ESC_CALLBACK __cdecl                         /* client callback function called by library */
#endif /* WIN32 */

/*------------------------------*/
/* QNX4 type modifiers			*/
#ifdef QNX4
	#define _ESC_EXPORT __cdecl                          /* function exported by library */
	#define ESC_CALLBACK __cdecl                         /* client callback function called by library */
#endif /* QNX4 */

/*------------------------------*/
/* POSIX type modifiers			*/
#ifdef POSIX
	#define _ESC_EXPORT                           /* function exported by library */
	#define ESC_CALLBACK                          /* client callback function called by library */
#endif /* POSIX */

/*------------------------------*/
/* VXWORKS type modifiers		*/
#ifdef VXWORKS
	#define _ESC_EXPORT                           /* function exported by library */
	#define ESC_CALLBACK                          /* client callback function called by library */
#endif /* VXWORKS */

/*------------------------------------------------------------------------------
 * hidden structures for library clients
 *-----------------------------------------------------------------------------*/
#ifndef ESC
	#define ESC void
#endif

/*------------------------------------------------------------------------------
 * extended types
 *-----------------------------------------------------------------------------*/
/*--------------------------*/
/* byte type				*/
#ifndef __BYTE
	#define __BYTE
	typedef unsigned char byte;
#endif

/*--------------------------*/
/* word type				*/
#ifndef __WORD
	#define __WORD
	typedef unsigned short word;
#endif

/*--------------------------*/
/* dword type				*/
#ifndef __DWORD
	#define __DWORD
	typedef unsigned int dword;
#endif

/*--------------------------*/
/* char_p type				*/
#ifndef __CHAR_P
	#define __CHAR_P
	typedef char *char_p;
#endif

/*--------------------------*/
/* ebool type				*/
#ifndef __EBOOL
	#define __EBOOL
	typedef byte ebool;
#endif

/* Define bool if stdbool.h (__bool_true_false_are_defined) is not included in the client application sources */
/* by this way, no error/warning during compilation. Applicable only for C */
#if !defined (__cplusplus) && !defined (__bool_true_false_are_defined) && !defined (__DEFINEBOOL)
	#define __DEFINEBOOL
	typedef ebool bool;
#endif

/*--------------------------*/
/* char_cp type				*/
#ifndef __CHAR_CP
	#define __CHAR_CP
	typedef const char *char_cp;
#endif /*CHAR_CP */

/*--------------------------*/
/* 64 bits integer			*/
#ifndef __INT64
	#define __INT64
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		typedef struct INT64_DWORD_MODE {
			dword low_dword;
			dword high_dword;
		} INT64_DWORD_MODE;
	#else /*__BYTE_ORDER*/
		typedef struct INT64_DWORD_MODE {
			dword high_dword;
			dword low_dword;
		} INT64_DWORD_MODE;
	#endif /*__BYTE_ORDER*/

	#if __BYTE_ORDER == __LITTLE_ENDIAN
		typedef struct INT64_BYTE_MODE {
			byte byte0;
			byte byte1;
			byte byte2;
			byte byte3;
			byte byte4;
			byte byte5;
			byte byte6;
			byte byte7;
		} INT64_BYTE_MODE;
	#else /*__BYTE_ORDER*/
		typedef struct INT64_BYTE_MODE {
			byte byte7;
			byte byte6;
			byte byte5;
			byte byte4;
			byte byte3;
			byte byte2;
			byte byte1;
			byte byte0;
		} INT64_BYTE_MODE;
	#endif /*__BYTE_ORDER*/

	/*------------------------------*/
	/* WINDOWS 64 bits integer		*/
	#if defined WIN32
		#ifdef _MSC_VER
			//Microsoft Compiler <= 6
			#if _MSC_VER <= 1200
				#define eint64 _int64
			#else
				#define eint64 long long
			#endif
		#else
				#define eint64 long long
		#endif
	/*------------------------------*/
	/* POSIX 64 bits integer		*/
	#elif defined POSIX
		#define eint64 long long
	/*--------------------------------------*/
	/* QNX4 and VXWORKS 64 bits integer		*/
	/* On these systems,64 bits integer are not standard */
	#elif (defined QNX4 || defined VXWORKS)
		typedef union eint64 {
			INT64_DWORD_MODE dwords;
			INT64_BYTE_MODE bytes;
			byte raw[8];
		} eint64;
	#endif
#endif


/*------------------------------------------------------------------------------
 * ESC_LINE_INFO type
 *-----------------------------------------------------------------------------*/
typedef struct ESC_LINE_INFO {
	unsigned long src_line_number;      // line number of the source file
	unsigned long code_offset;          // offset address on the machine code (for SHARC this is in word of 48 bit)
} ESC_LINE_INFO;
#define EscLineInfo ESC_LINE_INFO

/*------------------------------------------------------------------------------
 * ESC_OFFLINE_DEVICE type
 *-----------------------------------------------------------------------------*/
typedef struct ESC_OFFLINE_DEVICE {
	int axis;
	int prod;
	dword firmware;
} ESC_OFFLINE_DEVICE;
#define EscOfflineDevice ESC_OFFLINE_DEVICE

#ifndef ESC_DEBUG_DATA
	#define ESC_DEBUG_DATA void
#endif

typedef int ESC_CALLBACK ESC_LINE_READER(ESC *esc, char *buffer, int buffer_size, void *ruser);
typedef int ESC_CALLBACK ESC_LINE_WRITER(DSA_DEVICE *dev, const char *buffer, void *wuser);
typedef int ESC_CALLBACK ESC_PROGRESS(ESC *esc, const char *text, void *puser);

/**********************************************************************************************************/
/*** Prototypes */
/**********************************************************************************************************/

/**
 * @addtogroup ESCAll
 * @{
 */

int		_ESC_EXPORT esc_create(ESC **esc, DSA_DEVICE_BASE *dwn_grp);
int		_ESC_EXPORT esc_create_offline(ESC **esc, int nb_devices, ESC_OFFLINE_DEVICE present_devices[]);
dword   _ESC_EXPORT esc_get_version(void);
dword	_ESC_EXPORT esc_get_edi_version();
time_t  _ESC_EXPORT esc_get_build_time(void);
int		_ESC_EXPORT esc_set_iso_converter(ESC *esc, ETB_ISO_CONVERTER *iso_conv, void *iso_conv_user_param);
int		_ESC_EXPORT esc_destroy(ESC **esc);
int		_ESC_EXPORT esc_set_backup_sequence_filename (ESC *esc, char* filename);
char_cp _ESC_EXPORT esc_translate_compiler_error(ESC *esc, int code);
int		_ESC_EXPORT esc_download_cseq_file (ESC *esc, char* filename, dword flags, ESC_PROGRESS *progress_fct, void *puser);
int		_ESC_EXPORT esc_download_cseq_buffer (ESC *esc, ESC_LINE_READER *reader, void *ruser, dword flags, ESC_PROGRESS *progress_fct, void *puser);
int		_ESC_EXPORT esc_compile_cseq_file (ESC *esc, char* seq_filename, char *out_filename, dword flags, ebool compile_for_master, ESC_PROGRESS *progress_fct, void *puser);
int		_ESC_EXPORT esc_compile_cseq_buffer (ESC *esc, ESC_LINE_READER *reader, void *ruser, char *out_filename, dword flags, ebool compile_for_master, ESC_PROGRESS *progress_fct, void *puser);
int		_ESC_EXPORT esc_get_output_filename (ESC *esc, char* filename, int max_size);
int		_ESC_EXPORT esc_upload_cseq_file (DSA_DEVICE *dev, char* filename, ESC_PROGRESS *progress_fct, void *puser);
int		_ESC_EXPORT esc_upload_cseq_buffer (DSA_DEVICE *dev, ESC_LINE_WRITER *writer, void *wuser, ESC_PROGRESS *progress_fct, void *puser);
int		_ESC_EXPORT esc_extract_source_code(char* eseq_filename, char* src_filename);
ebool	_ESC_EXPORT esc_is_valid(ESC *esc);
int		_ESC_EXPORT esc_compile(int argc, char* argv[], char* error_filename);
int		_ESC_EXPORT esc_diag(char_cp file_name, int line, int err, ESC *esc);
int		_ESC_EXPORT esc_sdiag(char_p str, char_cp file_name, int line, int err, ESC *esc);
int		_ESC_EXPORT esc_fdiag(char_cp output_file_name , char_cp file_name, int line, int err, ESC *esc);
int		_ESC_EXPORT esc_dbg_data_create(char *filename, ESC_DEBUG_DATA **dbg_data); 
int		_ESC_EXPORT esc_dbg_data_get_code_offset_table(ESC_DEBUG_DATA *dbg_data, ESC_LINE_INFO** table, int *size); 
int		_ESC_EXPORT esc_dbg_data_get_src_line_table(ESC_DEBUG_DATA *dbg_data, ESC_LINE_INFO** table, int *size); 
int		_ESC_EXPORT esc_dbg_data_get_code_offset_from_src_line(ESC_DEBUG_DATA *debug_data, unsigned long src_line, unsigned long *code_offset);
int		_ESC_EXPORT esc_dbg_data_get_src_line_from_code_offset(ESC_DEBUG_DATA *debug_data, unsigned long code_offset, unsigned long *src_line);
int		_ESC_EXPORT esc_dbg_data_destroy(ESC_DEBUG_DATA **dbg_data); 
ebool	_ESC_EXPORT esc_dbg_data_is_valid(ESC_DEBUG_DATA *debug_data);
/** @}*/

#ifdef __cplusplus
	} /* extern "C" */
#endif


/**********************************************************************************************************/
/*- C++ WRAPPER CLASSES */
/**********************************************************************************************************/

#ifdef ESC_OO_API
	#define ERRCHK(a, esc) do { int _err = (a); if (_err) throw EscException(esc, _err); } while(0)
#endif

/*------------------------------------------------------------------------------
 * c++ callbacks
 *-----------------------------------------------------------------------------*/
#ifdef ESC_OO_API
	class EscCompiler;
		typedef int ESC_CALLBACK EscLineReader(EscCompiler esc, char *buffer, int bufferSize, void *ruser);
		typedef int ESC_CALLBACK EscLineWriter(const char *buffer, void *wuser);
		typedef int ESC_CALLBACK EscProgress(EscCompiler esc, const char *text, void *puser);
#endif /* ESC_OO_API */

/*------------------------------------------------------------------------------
 * Esc exception - c++
 *-----------------------------------------------------------------------------*/
#ifdef ESC_OO_API
	class EscException {
		friend class EscCompiler;
		friend class Esc;
		friend class EscDebugData;

		public:
			/*
			 * public error codes
			 */
		    enum {EBADLABEL = -913 };                       /* the label numer is out of range */
    enum {EBADPARAM = -900 };                       /* one of the parameter is not valid */
    enum {EBADSEQVERSION = -912 };                  /* the sequence version is not correct */
    enum {EBADSTATE = -902 };                       /* this operation is not allowed in this state */
    enum {EBUFFEROVERFLOW = -906 };                 /* the buffer is too small */
    enum {ECOMPILE = -904 };                        /* the compilation process return an error */
    enum {EDOWNLOAD = -905 };                       /* the download process return an error */
    enum {EEOF = -908 };                            /* unexpected end of file reached */
    enum {EFORMAT = -909 };                         /* bad file format */
    enum {EINTERNAL = -903 };                       /* some internal error in the etel software */
    enum {EIOACCESS = -907 };                       /* cannot access to the file */
    enum {EISOCONV = -911 };                        /* an iso converter has not been set */
    enum {ENODBGINFO = -915 };                      /* the debug informations are not available */
    enum {ENOTAVAILABLE = -914 };                   /* the required element is not available */
    enum {ESYSTEM = -901 };                         /* some system resource return an error */
    enum {EUPLOAD = -910 };                         /* the upload process return an error */
    enum {EWARNING = -916 };                        /* the compilation has generated a warning */


		/*
		 * exception code
		 */
		private:
			int code;
			ESC *esc;

		/*
		 * constructor
		 */
		protected:
			EscException(ESC *_esc, int e) { esc = _esc; code = e; };

		/*
		 * get error description
		 */
		public:
			int getCode() {
				return code;
			}
			const char *getCompilerErrorText() {
				return esc_translate_compiler_error(esc, code);
			}
	};
#endif /* ESC_OO_API */

/*------------------------------------------------------------------------------
 * Base class - c++
 *-----------------------------------------------------------------------------*/
#ifdef ESC_OO_API
	class EscCompiler {
		/*
		 * internal esc pointer
		 */
		protected:
			ESC *esc;

		/*
		 * compiler flags
		 */
		public:


		public:
			/*
			 * constructors / destructor
			 */
			EscCompiler(DsaDeviceBase dwnGrp) {
				esc = NULL;
				ERRCHK(esc_create(&esc, *(DSA_DEVICE_BASE**)&dwnGrp), esc);
			}

			EscCompiler(int nbDevs, EscOfflineDevice devs[]) {
				esc = NULL;
				ERRCHK(esc_create_offline(&esc, nbDevs, devs), esc);
			}
			ebool isValid() {
				return esc_is_valid(esc);
			}

			void downloadCseqFile(char* fileName, dword flags, EscProgress *progressFct, void *puser) {
				ERRCHK(esc_download_cseq_file(esc, fileName, flags, (ESC_PROGRESS*)progressFct, puser), esc);
			}

			void setBackupSequenceFilename(char* fileName) {
				ERRCHK(esc_set_backup_sequence_filename (esc, fileName), esc);
			}

			void downloadCseqBuffer(EscLineReader *lineReader, void *ruser, dword flags, EscProgress *progressFct, void *puser) {
				ERRCHK(esc_download_cseq_buffer(esc, (ESC_LINE_READER*)lineReader, ruser, flags, (ESC_PROGRESS*)progressFct, puser), esc);
			}
			
			void setIsoConverter(EtbIsoConverter conv, void *user) {
				ERRCHK(esc_set_iso_converter(esc, (ETB_ISO_CONVERTER *)conv, user), esc);
			}
			
			void compileCseqFile(char* cseqFileName, char* eseqFileName, dword flags, ebool compile_for_master, EscProgress *progressFct, void *puser) {
				ERRCHK(esc_compile_cseq_file(esc, cseqFileName, eseqFileName, flags, compile_for_master, (ESC_PROGRESS*)progressFct, puser), esc);
			}

			void compileCseqBuffer(EscLineReader *lineReader, void *ruser, char *eseqFilename, dword flags, ebool compile_for_master, EscProgress *progressFct, void *puser) {
				ERRCHK(esc_compile_cseq_buffer(esc, (ESC_LINE_READER*)lineReader, ruser, eseqFilename, flags, compile_for_master, (ESC_PROGRESS*)progressFct, puser), esc);
			}

			void getOutputFilename(char *filename, int size) {
				ERRCHK(esc_get_output_filename (esc, filename, size), esc);
			}

		/*
		 * destructor function
		 */
		public:
			void destroy() {
				ERRCHK(esc_destroy(&esc), esc);
			}
	};
#endif /* ESC_OO_API */

	/*------------------------------------------------------------------------------
 * Esc base class - c++
 *-----------------------------------------------------------------------------*/
#ifdef ESC_OO_API
	class Esc {
		/*
		 * some public constants
		 */
		/*
		 * versions access
		 */
		public:
			static dword getVersion() {
				return esc_get_version();
			}
			static dword getEdiVersion() {
				return esc_get_edi_version();
			}
			static time_t getBuildTime() {
				return esc_get_build_time();
			}
			static void uploadCseqFile(DSA_DEVICE *dev, char* fileName, EscProgress *progressFct, void *puser) {
				ERRCHK(esc_upload_cseq_file (dev, fileName, (ESC_PROGRESS*)progressFct, puser), NULL);
			}
			static void uploadCseqBuffer(DSA_DEVICE *dev, EscLineWriter *lineWriter, void *wuser, EscProgress *progressFct, void *puser) {
				ERRCHK(esc_upload_cseq_buffer (dev, (ESC_LINE_WRITER*)lineWriter, wuser, (ESC_PROGRESS*)progressFct, puser), NULL);
			}
			static void compile(int argc, char* argv[], char* errorFilename) {
				ERRCHK(esc_compile(argc, argv, errorFilename), NULL);
			}
			static void extractSourceCode(char* eseq_filename, char* src_filename) {
				ERRCHK(esc_extract_source_code(eseq_filename, src_filename), NULL);
			}

	};
#endif /* ESC_OO_API */

#ifdef ESC_OO_API
	class EscDebugData {
		/*
		 * internal esc pointer
		 */
		protected:
			ESC_DEBUG_DATA *esc_debug_data;

		public:
			/*
			 * constructors / destructor
			 */
			EscDebugData(char *fileName) {
				esc_debug_data = NULL;
				ERRCHK(esc_dbg_data_create(fileName, &esc_debug_data), NULL);
			}

			void getCodeOffsetTable(EscLineInfo** table, int *size) {
				ERRCHK(esc_dbg_data_get_code_offset_table(esc_debug_data, (ESC_LINE_INFO**)table, size), NULL); 
			}

			void getSrcLineTable(EscLineInfo** table, int *size) { 
				ERRCHK(esc_dbg_data_get_src_line_table(esc_debug_data, (ESC_LINE_INFO**)table, size), NULL); 
			}
			unsigned long getCodeOffsetFromSrcLine(unsigned long srcLine) {
				unsigned long val;
				ERRCHK(esc_dbg_data_get_code_offset_from_src_line(esc_debug_data, srcLine, &val), NULL);
				return val;
			}

			unsigned long getSrcLineFromCodeOffset(unsigned long codeOffset) {
				unsigned long val;
				ERRCHK(esc_dbg_data_get_src_line_from_code_offset(esc_debug_data, codeOffset, &val), NULL);
				return val;
			}

	public:
			/*
			 * destructor function
			 */
	#ifdef WIN32
		#if _MSC_VER >= 1900		
			~EscDebugData(void) noexcept(false) {
		#else /* _MSC_VER >= 1900*/
			~EscDebugData(void) {
		#endif /* _MSC_VER >= 1900*/
	#else /* WIN32 */
			~EscDebugData(void) {
	#endif /* WIN32 */			
				ERRCHK(esc_dbg_data_destroy(&esc_debug_data), NULL);
			}
			ebool isValid() {
				return esc_dbg_data_is_valid(esc_debug_data);
			}
	};
#endif /* ESC_OO_API */

#undef ERRCHK

#endif /* _ESC40_H */


