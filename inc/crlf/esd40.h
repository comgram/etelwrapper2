/*
 * esd40.h
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
EDI Windows version uses 7ZIP functionality, especially for:
- System Configuration Management
- Downloading firmware
- Uploading UltimET advanced zip log files
- Opening communication in simulation mode
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
 * This header file contains public declarations for the etel sequence downloader EDI library.\n
 * This library is conformed to POSIX 1003.1c
 * @file esd40.h
 */


#ifndef _ESD40_H
#define _ESD40_H


/**********************************************************************************************************/
/*** Libraries */
/**********************************************************************************************************/

/*----------------------*/
/* common libraries		*/
#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#ifndef F_ITRON
	#include <time.h>
#endif	

#include "lib40.h"
#include "etb40.h"

/**
 * @defgroup ESDAll ESD All functions
*/
/*@{*/
/*@}*/

/**
 * @defgroup ESDErrorsAndWarnings ESD Error and warnings
*/
/*@{*/
/*@}*/


/**********************************************************************************************************/
/*** Literals */
/**********************************************************************************************************/


#ifdef __cplusplus
	extern "C" {
#endif

/*------------------------------*/
/* WINDOWS type modifiers		*/
#if defined WIN32 || defined _WIN64
	#define _ESD_EXPORT __cdecl                          /* function exported by static library */
#endif /* WIN32 || _WIN64*/

/*------------------------------*/
/* POSIX type modifiers			*/
#if defined POSIX || defined VXWORKS_6x
	#define _ESD_EXPORT                           		/* function exported by library */
#endif /* POSIX  || VXWORKS_6x*/

/*------------------------------*/
/* F_ITRON type modifiers		*/
#ifdef F_ITRON
	#define _ESD_EXPORT  __cdecl                      	/* function exported by library */
#endif /* F_ITRON */

/*------------------------------*/
/* FREERTOS type modifiers		*/
#ifdef FREERTOS
#define _ESD_EXPORT
#endif /* FREERTOS */

/*------------------------------*/
/* VXWORKS_5_4 type modifiers		*/
#ifdef VXWORKS_5_4
	#define _ESD_EXPORT                       	        /* function exported by library */
#endif /* VXWORKS_5_4 */

/*------------------------------------------------------------------------------
 * error codes - c
 *-----------------------------------------------------------------------------*/
#define ESD_ENOSOURCE                    -2515       /**< source code not available */
#define ESD_ENOTAVAILABLE                -2514       /**< the required element is not available */
#define ESD_EBADLABEL                    -2513       /**< the label numer is out of range */
#define ESD_EFORMAT                      -2509       /**< bad file format */
#define ESD_EEOF                         -2508       /**< unexpected end of file reached */
#define ESD_EIOACCESS                    -2507       /**< cannot access to the file */
#define ESD_EOVERFLOW                    -2506       /**< the buffer is too small */
#define ESD_EINTERNAL                    -2503       /**< some internal error in the etel software */
#define ESD_ENOTIMPLEMENTED              -2502       /**< Function not implemented */
#define ESD_ESYSTEM                      -2501       /**< some system resource return an error */
#define ESD_EBADPARAM                    -2500       /**< one of the parameter is not valid */



/*------------------------------------------------------------------------------
 * DWONLOAD FLAGS
 *-----------------------------------------------------------------------------*/
#define ESD_FLAG_AVOID_DOWNLOAD_SRCCODE 0x00000001
#define ESD_FLAG_AVOID_DOWNLOAD_DBGINFO 0x00000002

/*------------------------------------------------------------------------------
 * hidden structures for library clients
 *-----------------------------------------------------------------------------*/
#ifndef ESD_XML_STREAM
	#define ESD_XML_STREAM void
#endif

#define ESD_MAX_SEQ_FLASH_SIZE			1048576			/* Size of the available flash for sequence and source in the drive */

/**********************************************************************************************************/
/*** Types */
/**********************************************************************************************************/
typedef struct ESD_DEVICE {
	int axis;
	int prod;
	dword firmware;
} ESD_DEVICE;

/*********************************************************************************************************/
/*** ESD exported functions */
/*********************************************************************************************************/
/**
 * @addtogroup ESDAll
 * @{
 */
dword _ESD_EXPORT esd_get_version(void);
dword _ESD_EXPORT esd_get_edi_version(void);
time_t _ESD_EXPORT esd_get_build_time(void);
char_cp _ESD_EXPORT esd_translate_error(int code);

int	_ESD_EXPORT esd_xml_create_input_stream(ESD_XML_STREAM **s, char *filename);
int	_ESD_EXPORT esd_xml_destroy_stream(ESD_XML_STREAM *s);
int _ESD_EXPORT esd_xml_read_tag(ESD_XML_STREAM *s, char *buff, int len);
int _ESD_EXPORT esd_xml_read_text(ESD_XML_STREAM *s, char *buff, int len);
int _ESD_EXPORT esd_xml_reset(ESD_XML_STREAM *s);
int _ESD_EXPORT esd_get_source_code(char *stream_data, int stream_data_size, char **source_code_begin, long *source_code_size);
ebool _ESD_EXPORT esd_check_stream_data_crc(char *stream_data, int stream_data_size);
int _ESD_EXPORT esd_build_eseq_file(eint64 dwn_mask, ESD_DEVICE devs[], char *src_file_name, char *xml_file_name, char *eseq_file_name);
int _ESD_EXPORT esd_build_stream_data(char *src_file_name, char *xml_file_name, ebool download_debug_info, char *stream_data, long stream_data_max, long *stream_data_size);
int _ESD_EXPORT esd_build_stream_data_ex(char *src_file_name, char *xml_file_name, dword flags, char *stream_data, long stream_data_max, long *stream_data_size);
int _ESD_EXPORT esd_build_stream_data2(char *xml_file_name, ebool download_debug_info, char *stream_data, long stream_data_max, long *stream_data_size);
int _ESD_EXPORT esd_build_stream_data2_ex(char *xml_file_name, dword flags, char *stream_data, long stream_data_max, long *stream_data_size);
int _ESD_EXPORT esd_extract_source_code(char *eseq_file_name, char *src_file_name);
int	_ESD_EXPORT esd_extract_configuration(char* eseq_filename, eint64 *dwn_mask, ESD_DEVICE devs[]);
/** @} */

#ifdef __cplusplus
	} /* extern "C" */
#endif

#endif /* _ESD40_H */
