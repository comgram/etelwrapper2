/*
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
 * this header file contains public declaration for low-level library. it also
 * contains macro-definition for real-time objects/operations used to achieve
 * multi-platform source code.
 * @library lib40
 */

#ifndef _LIB40_H
#define _LIB40_H

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------*/
/* BYTE ORDER */
/*----------------------------------------------------------------------------------*/
#ifndef __BYTE_ORDER

	/*------------------------------*/
	/* Windows Byte order			*/
	#if defined WIN32 || defined _WIN64
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#define __BYTE_ORDER __LITTLE_ENDIAN			/* define byte order for INTEL processor */
	#endif /* WIN32 || _WIN64*/

	/*------------------------------*/
	/* POSIX LINUX Byte order		*/
	#if defined POSIX && defined LINUX
		#include <endian.h>
	#endif /*LINUX*/

	/*------------------------------*/
	/* POSIX QNX6 Byte order		*/
	#if defined POSIX && defined QNX6
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#ifdef __BIGENDIAN__
			#define __BYTE_ORDER __BIG_ENDIAN			/* define byte order for SPARC processor */
		#else
			#define __BYTE_ORDER __LITTLE_ENDIAN        /* define byte order for SPARC processor */
		#endif /*__BIGENDIAN__*/
	#endif /*QNX6*/

	/*------------------------------*/
	/* VXWORKS_6x Byte order		*/
	#if defined VXWORKS_6x
		#include <vxWorks.h>							//to define the endianness, needed to define the architecture
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#ifdef _BYTE_ORDER
			#if (_BYTE_ORDER == _BIG_ENDIAN)			//VxWorks 6.9 defines _BIG_ENDIAN or _LITTLE_ENDIAN
				#define __BYTE_ORDER __BIG_ENDIAN
			#else
				#define __BYTE_ORDER __LITTLE_ENDIAN
			#endif
		#else
			#error _BYTE_ORDER NOT DEFINED
		#endif
	#endif /*VXWORKS_6x*/

   	/*------------------------------*/
	/* VXWORKS_5_4 Byte order		*/
	#if defined VXWORKS_5_4
		#define __LITTLE_ENDIAN _LITTLE_ENDIAN
		#define __BIG_ENDIAN _BIG_ENDIAN
		#define __BYTE_ORDER _BYTE_ORDER
	#endif /*VXWORKS_5_4*/

	/*------------------------------*/
	/* F_ITRON Byte order			*/
	#if defined F_ITRON
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#define __BYTE_ORDER __LITTLE_ENDIAN			
	#endif /*F_ITRON*/

	/*------------------------------*/
	/* FREERTOS Byte order			*/
	#if defined FREERTOS
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#define __BYTE_ORDER __LITTLE_ENDIAN
	#endif /*F_ITRON*/

#endif /*BYTE_ORDER*/


/**********************************************************************************************************/
/*** Libraries */
/**********************************************************************************************************/
/*-----------------------*/
/* Windows RTX libraries */
#if defined WIN32 || defined _WIN64
	#ifdef UNDER_RTSS
		#include <windows.h>
		#include <RtApi.h>

	/*-----------------------*/
	/* Windows W32 libraries */
	#else
		#include <windows.h>
		#include "jni.h"
	#endif /*UNDER_RTSS */
#endif /* WIN32 || _WIN64*/

/*-----------------------*/
/* Common libraries*/
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/*----------------------*/
/* Windows libraries*/
#if defined WIN32 || defined _WIN64
	#include <time.h>
	#ifdef _MSC_VER
		#if _MSC_VER >= 1600
			#include <stdint.h>
		#else
			typedef unsigned int uint32_t;
		#endif /* _MSC_VER >= 1600*/

		#if _MSC_VER >= 1900
			#include <VersionHelpers.h>
		#endif /* _MSC_VER >= 1900*/
	#else
		typedef unsigned int uint32_t;
	#endif /*_MSC_VER*/
#endif /* WIN32 || _WIN64*/

/*----------------------*/
/* POSIX LINUX libraries*/
#if defined POSIX && defined LINUX
	#include <time.h>
	#include <stdint.h>
	#include <pthread.h>
	#include <sys/select.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <sys/resource.h>
	#include <semaphore.h>
	#include <unistd.h>
	#include <time.h>
	#include <errno.h>
	#include <math.h>
#endif /*LINUX*/

/*----------------------*/
/* POSIX-QNX6 libraries*/
#if defined POSIX && defined QNX6
	#include <time.h>
	#include <stdint.h>
	#include <pthread.h>
	#include <sys/select.h>
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <semaphore.h>
	#include <netdb.h>
	#include <unistd.h>
#endif /* QNX6 */

/*----------------------*/
/* VXWORKS_6x libraries*/
#ifdef VXWORKS_6x
	#include <selectLib.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <sys/resource.h>
	#include <semaphore.h>
	#include <unistd.h>
	#include <time.h>
	#include <errno.h>
	#include <math.h>
	#include <ioLib.h>
	#include <taskLib.h>
	#include <semLib.h>
	#include <vxCpuLib.h>
	#include <cpuset.h>
#endif /*VXWORKS_6x */

/*----------------------*/
/* VXWORKS_5_4 libraries*/
#ifdef VXWORKS_5_4
    #include <time.h>
	#include <taskLib.h>
	#include <sockLib.h>
	#include <dirent.h>
	#include <taskVarLib.h>
	#include <ioLib.h>
	#include <selectLib.h>
	#include <hostLib.h>
	#include <inetLib.h>
	#include <resolvLib.h>
	#include <logLib.h>
    typedef unsigned long long uint64_t;
    typedef signed long long int64_t;  
#endif /*VXWORKS_5_4 */

/*----------------------*/
/* F_ITRON libraries*/
#ifdef F_ITRON
	#include <ctype.h>
	#include <OS_IF/os_api.h>
	#include <File_api.h>
	#include <FjComm.h>
	#include <time.h>
#endif /*F_ITRON */

/*----------------------*/
/* FREERTOS libraries*/
#ifdef FREERTOS
	#include <FreeRTOS.h>
	#include <event_groups.h>
	#include <semphr.h>
	#include <ctype.h>
	#include <ff.h>
	#include <lwip/sockets.h>
#endif /*FREERTOS */

/**********************************************************************************************************/
/*** Literals */
/**********************************************************************************************************/
/*----------------------------*/
/* WINDOWS special definitions*/
#if (defined WIN32 || defined _WIN64) && !defined UNDER_RTSS
	#define FWPOOL_FIND                   /* The OS is able to find itself the directory of firmware pool */
#endif

/*------------------------*/
/* RTX special definitions*/
#if defined UNDER_RTSS
	#define NO_DIRECTORY_MANAGEMENT				/* The OS does not provide directory handling */
#endif

/*--------------------------------*/
/* VXWORKS_5_4 special definitions*/
#if defined VXWORKS_5_4
	#define NO_DIRECTORY_MANAGEMENT				/* The OS does not provide directory handling */
#endif

/*--------------------------------*/
/* FREERTOS special definitions*/
#if defined FREERTOS
	#define NO_FILE_SYSTEM					/* The OS does not provide a file system neither a file system though NFS */
	#define NO_DIRECTORY_MANAGEMENT				/* The OS does not provide directory handling */
#endif

#define PRO_PAR                          1           /* pro parameter used with this platform */

/*------------------------------------------------------------------------------
 * infinite (no) timeout special value
 *-----------------------------------------------------------------------------*/
#ifndef INFINITE
	#define INFINITE                        (-1)
#endif

/*------------------------------------------------------------------------------
 * true/false boolean values
 *-----------------------------------------------------------------------------*/
#ifndef FALSE
	#define FALSE                            (unsigned char)0
#endif
#ifndef TRUE
	#define TRUE                             (unsigned char)1
#endif


/*------------------------------------------------------------------------------
 * return value of wait functions
 *-----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
/* POSIX, F_ITRON, VXWORKS_6x, VXWORKS_5_4, FREERTOS return value of wait functions		*/
#if defined POSIX || defined F_ITRON || defined VXWORKS_6x || defined VXWORKS_5_4 || defined FREERTOS
	#define WAIT_OBJECT_0   0
	#define WAIT_TIMEOUT    -1
	#define WAIT_FAILED     -2
#endif /*defined POSIX || defined F_ITRON || defined VXWORKS_6x || defined VXWORKS_5_4 || defined FREERTOS*/


/*------------------------------------------*/
/* F_ITRON synchronization objectes			*/
#ifdef F_ITRON
	#define SYNC_OBJ_TLS 100
#endif /*F_ITRON*/
/*------------------------------------------------------------------------------
 * thread priority levels
 *-----------------------------------------------------------------------------*/
/*------------------------------------------*/
/* POSIX-LINUX thread priority levels		*/
#if defined POSIX && defined LINUX
	#define THREAD_PRIORITY_IDLE                20
	#define THREAD_PRIORITY_LOWEST              10
	#define THREAD_PRIORITY_BELOW_NORMAL        5
	#define THREAD_PRIORITY_NORMAL              0
	#define THREAD_PRIORITY_ABOVE_NORMAL        -5
	#define THREAD_PRIORITY_HIGHEST             -10
	#define THREAD_PRIORITY_TIME_CRITICAL       -20
#endif /* POSIX &&LINUX */

/*------------------------------------------*/
/* POSIX-QNX6 thread priority levels		*/
#if defined POSIX && defined QNX6
	#define THREAD_PRIORITY_IDLE                1
	#define THREAD_PRIORITY_LOWEST              10
	#define THREAD_PRIORITY_BELOW_NORMAL        12
	#define THREAD_PRIORITY_NORMAL              15
	#define THREAD_PRIORITY_ABOVE_NORMAL        18
	#define THREAD_PRIORITY_HIGHEST             20
	#define THREAD_PRIORITY_TIME_CRITICAL       30
#endif /* POSIX &&QNX6 */

/*------------------------------------------*/
/* VXWORKS thread priority levels			*/
/* Highest priority is 0 */
/* Should not be higher than 50 (priority < 50), because tNetTask runs at priority 50 */
#ifdef VXWORKS_6x
	#define THREAD_PRIORITY_IDLE                170
	#define THREAD_PRIORITY_LOWEST              160
	#define THREAD_PRIORITY_BELOW_NORMAL        155
	#define THREAD_PRIORITY_NORMAL              150
	#define THREAD_PRIORITY_ABOVE_NORMAL        145
	#define THREAD_PRIORITY_HIGHEST             140
	#define THREAD_PRIORITY_TIME_CRITICAL       130
	//#define DEFAULT_CORE_AFFINITY 3
#endif /* VXWORKS_6x */

/*------------------------------------------*/
/* VXWORKS_5_4 thread priority levels	    */
/* High priority is 0 */
/* Should not be higher than 50 (priority < 50), because tNetTask runs at priority 50 */
#ifdef VXWORKS_5_4
	#define THREAD_PRIORITY_IDLE                255
	#define THREAD_PRIORITY_LOWEST              200
	#define THREAD_PRIORITY_BELOW_NORMAL        150
	#define THREAD_PRIORITY_NORMAL              100
	#define THREAD_PRIORITY_ABOVE_NORMAL        90
	#define THREAD_PRIORITY_HIGHEST             80
	#define THREAD_PRIORITY_TIME_CRITICAL       75
#endif /* VXWORKS_5_4 */

/*------------------------------------------*/
/* F_ITRON thread priority levels			*/
#ifdef F_ITRON
	#define THREAD_PRIORITY_IDLE                OS_PRI_IDLE			//0x7F
	#define THREAD_PRIORITY_LOWEST              OS_PRI_APL_BTM		//0x60
	#define THREAD_PRIORITY_BELOW_NORMAL        OS_PRI_APL_TOP+7	//0x58	
	#define THREAD_PRIORITY_NORMAL              OS_PRI_APL_TOP+5	//0x56
	#define THREAD_PRIORITY_ABOVE_NORMAL        OS_PRI_APL_TOP+4	//0x55
	#define THREAD_PRIORITY_HIGHEST             OS_PRI_APL_TOP+2	//0x53
	#define THREAD_PRIORITY_TIME_CRITICAL       OS_PRI_APL_TOP 		//0x51
#endif /* F_ITRON */

/*------------------------------------------*/
/* FREERTOS thread priority levels		*/
#if defined FREERTOS
	#define THREAD_PRIORITY_CYCLIC_RTV			(configMAX_PRIORITIES - 2)				//14
	#define THREAD_PRIORITY_CYCLIC       		(configMAX_PRIORITIES - 3)				//13
	#define THREAD_PRIORITY_TIME_CRITICAL       (configMAX_PRIORITIES - 6)				//10
	#define THREAD_PRIORITY_IDLE                1										//1
	#define THREAD_PRIORITY_NORMAL              (THREAD_PRIORITY_TIME_CRITICAL / 2)		//5
	#define THREAD_PRIORITY_ABOVE_NORMAL        (THREAD_PRIORITY_NORMAL + 1)			//6
	#define THREAD_PRIORITY_BELOW_NORMAL        (THREAD_PRIORITY_NORMAL - 1)			//4
	#define THREAD_PRIORITY_HIGHEST             (THREAD_PRIORITY_NORMAL + 2)			//7
	#define THREAD_PRIORITY_LOWEST              (THREAD_PRIORITY_NORMAL - 2)			//3
#endif /* FREERTOS */

/*------------------------------------------------------------------------------
 * EDI thread priority levels remapping
 *-----------------------------------------------------------------------------*/
/*------------------------------------------*/
/* RTX priority levels		*/
#if defined UNDER_RTSS
	#define EDI_THREAD_PRIORITY_IDLE            0
	#define EDI_THREAD_PRIORITY_LOWEST          20
	#define EDI_THREAD_PRIORITY_BELOW_NORMAL    40
	#define EDI_THREAD_PRIORITY_NORMAL          64
	#define EDI_THREAD_PRIORITY_ABOVE_NORMAL    88
	#define EDI_THREAD_PRIORITY_HIGHEST         108
	#define EDI_THREAD_PRIORITY_TIME_CRITICAL   127
#else
	#define EDI_THREAD_PRIORITY_IDLE            THREAD_PRIORITY_IDLE
	#define EDI_THREAD_PRIORITY_LOWEST          THREAD_PRIORITY_LOWEST
	#define EDI_THREAD_PRIORITY_BELOW_NORMAL    THREAD_PRIORITY_BELOW_NORMAL
	#define EDI_THREAD_PRIORITY_NORMAL          THREAD_PRIORITY_NORMAL
	#define EDI_THREAD_PRIORITY_ABOVE_NORMAL    THREAD_PRIORITY_ABOVE_NORMAL
	#define EDI_THREAD_PRIORITY_HIGHEST         THREAD_PRIORITY_HIGHEST
	#define EDI_THREAD_PRIORITY_TIME_CRITICAL   THREAD_PRIORITY_TIME_CRITICAL
#endif // UNDER_RTSS

/*------------------------------------------------------------------------------
 * debug constants - kind of event
 *-----------------------------------------------------------------------------*/
#define DBG_KIND_INFORMATION                0x01
#define DBG_KIND_WARNING                    0x02
#define DBG_KIND_ERROR                      0x03
#define DBG_KIND_FATAL_ERROR                0x04
#define DBG_KIND_STREAM_IN                  0x05
#define DBG_KIND_STREAM_OUT                 0x06
#define DBG_KIND_FCT_BEGIN                  0x07
#define DBG_KIND_FCT_END                    0x08
#define DBG_KIND_MEM_ALLOC                  0x09


/*------------------------------------------------------------------------------
 * debug constants - source of event
 *-----------------------------------------------------------------------------*/
#define DBG_SOURCE_LIB                      0x01
#define DBG_SOURCE_DMD                      0x02
#define DBG_SOURCE_ETB                      0x04
#define DBG_SOURCE_TRA                      0x05
#define DBG_SOURCE_DSA                      0x06
#define DBG_SOURCE_ETN                      0x10


/*------------------------------------------------------------------------------
 * debug constants - kind of stream
 *-----------------------------------------------------------------------------*/
#define DBG_STREAM_SHM                      0x01
#define DBG_STREAM_DUMMY                    0x02
#define DBG_STREAM_TCP                      0x03
#define DBG_STREAM_PCI                      0x04
#define DBG_STREAM_USB                      0x07

/*------------------------------------------------------------------------------
 * debug constants - protocol
 *-----------------------------------------------------------------------------*/
#define DBG_PROTOCOL_EBL2                   0x01
#define DBG_PROTOCOL_ETCOM                  0x02

/*------------------------------------------------------------------------------
 * socket constants
 *-----------------------------------------------------------------------------*/
/*---------------------------------------------------------------*/
/* socket constants for POSIX, F_ITRON, VXWORKS_6x, FREERTOS, VXWORKS_5_4 */
#if defined POSIX || defined F_ITRON || defined VXWORKS_6x || defined VXWORKS_5_4 || defined FREERTOS
	#define INVALID_SOCKET                      -1
#endif /*defined POSIX || defined F_ITRON || defined VXWORKS_6x || defined VXWORKS_5_4 || defined FREERTOS*/

/*------------------------------------------------------------------------------
 * FileName maximal size
 *-----------------------------------------------------------------------------*/
#if defined POSIX || defined VXWORKS_6x || defined VXWORKS_5_4
	#define _MAX_PATH 260
#endif /*defined POSIX || defined VXWORKS_6x || defined VXWORKS_5_4*/

#if defined F_ITRON
	#define _MAX_PATH 512
#endif /*F_ITRON*/

#if defined FREERTOS
	#define _MAX_PATH _MAX_LFN
#endif /*FREERTOS*/


/**********************************************************************************************************/
/*** Types */
/**********************************************************************************************************/

/*------------------------------------------------------------------------------
 * common types used in all libraries
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
	typedef uint32_t dword;
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
#if !defined __INT64 || !defined __INT64_DATATYPE_
	#if !defined __INT64
		#define __INT64
	#endif
	#if !defined __INT64_DATATYPE
		#define __INT64_DATATYPE
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
		#if defined WIN32 || defined _WIN64
			#define eint64 long long
			static eint64 ZERO64 = 0;
			#define INIT64(i)							do { \
															(i) = 0; \
														} while (0)
			#define SET64FROM64(dest,src)				do { \
															(dest) = (src); \
														} while (0)
			#define SET64FROM32(dest,low)				do { \
															(dest) = (low); \
														} while (0)
			#define SET64FROMLOWHIGH(dest, low, high)	do { \
															(dest) = (((eint64)(high)) << 32) | (low); \
														} while (0)
			#define CREATEANDSET64FROM32(low)			(low)
			#define CREATEANDSET64FROMLOWHIGH(low,high) ((((eint64)(high)) << 32) | (low))
			#define SETBIT64(i,bit)						 do { \
															(i) |= (eint64)1 << (bit); \
														} while (0)
			#define SETBITS64(i,mask)					do { \
															(i) |= (mask); \
														} while (0)
			#define CREATEANDSETBIT64(bit)				((eint64)1 << (bit))
			#define RESETBIT64(i,bit)					do { \
															(i) &= ~((eint64)1 << (bit)); \
														} while (0)
			#define RESETBITS64(i,mask)					do { \
															(i) &= ~(mask); \
														} while (0)
			#define ISBITSET64(i,bit)					(((i) & ((eint64)1 << (bit))) != 0)
			#define ISANYBITSET64(i1,i2)				(((i1) & (i2)) != 0)
			#define ISZERO64(i)							((i) == 0)
			#define INVERT64(i)							do { \
															(i) = ~(i); \
														} while (0)
			#define ANDBITS64(i,j)						do { \
															(i) &= (j); \
														} while (0)
			#define LOWDWORD64(i)						(((INT64_DWORD_MODE*)&(i))->low_dword)
			#define HIGHDWORD64(i)						(((INT64_DWORD_MODE*)&(i))->high_dword)
			#define EQUAL64(i1, i2)							((i1) == (i2))
			#define STRTOI64(s,e,b)						( \
															_strtoi64(s,e,b) \
														)
			#define STRTOUI64(s,e,b)					( \
															_strtoui64(s,e,b) \
														)
			#define I32_TO_I64(i32, i64)				do { \
															(i64) = (eint64)(i32); \
														} while (0)
			#define F32_TO_I64(f32, i64)				do { \
															(i64) = (eint64)(f32); \
														} while (0)
			#define F64_TO_I64(f64, i64)				do { \
															(i64) = (eint64)(f64); \
														} while (0)
			#define I64_TO_I32(i64, i32)				do { \
															(i32) = (int)(i64); \
														} while (0)
			#define I64_TO_F32(i64, f32)				do { \
															(f32) = (float)(i64); \
														} while (0)
			#define I64_TO_F64(i64, f64)				do { \
															(f64) = (float)(i64); \
														} while (0)

		/*-----------------------------------------------*/
		/* POSIX, F_ITRON, VXWORKS_6x, VXWORKS_5_4, FREERTOS 64 bits integer	 */
		#elif defined POSIX || defined F_ITRON || defined VXWORKS_6x || defined FREERTOS || defined VXWORKS_5_4
			#define eint64 long long
			#define ZERO64 0LL
			#define INIT64(i)							do { \
															(i) = 0; \
														} while (0)
			#define SET64FROM64(dest,src)				do { \
															(dest) = (src); \
														} while (0)
			#define SET64FROM32(dest,low)				do { \
															(dest) = (low); \
														} while (0)
			#define SET64FROMLOWHIGH(dest, low, high)	do { \
															(dest) = (eint64)(((eint64)(high) << 32) | (low)); \
														} while (0)
			#define CREATEANDSET64FROM32(low)			(low)
			#define CREATEANDSET64FROMLOWHIGH(low,high) ((((eint64)(high)) << 32) | (low))
			#define SETBIT64(i,bit)						 do { \
															(i) |= (eint64)1 << (bit); \
														} while (0)
			#define SETBITS64(i,mask)					do { \
															(i) |= (mask); \
														} while (0)
			#define CREATEANDSETBIT64(bit)				((eint64)1 << (bit))
			#define RESETBIT64(i,bit)					do { \
															(i) &= ~((eint64)1 << (bit)); \
														} while (0)
			#define RESETBITS64(i,mask)					do { \
															(i) &= ~(mask); \
														} while (0)
			#define ISBITSET64(i,bit)					(((i) & ((eint64)1 << (bit))) != 0)
			#define ISANYBITSET64(i1,i2)				(((i1) & (i2)) != 0)
			#define ISZERO64(i)							((i) == 0)
			#define INVERT64(i)							do { \
															(i) = ~(i); \
														} while (0)
			#define ANDBITS64(i,j)						do { \
															(i) &= (j); \
														} while (0)
			#define LOWDWORD64(i)						(((INT64_DWORD_MODE*)&(i))->low_dword)
			#define HIGHDWORD64(i)						(((INT64_DWORD_MODE*)&(i))->high_dword)
			#define EQUAL64(i1, i2)							((i1) == (i2))
			#define STRTOI64(s,e,b)						( \
															strtoll(s,e,b) \
														)
			#define STRTOUI64(s,e,b)					( \
															strtoull(s,e,b) \
														)
			#define I32_TO_I64(i32, i64)				do { \
															(i64) = (eint64)(i32); \
														} while (0)
			#define F32_TO_I64(f32, i64)				do { \
															(i64) = (eint64)(f32); \
														} while (0)
			#define F64_TO_I64(f64, i64)				do { \
															(i64) = (eint64)(f64); \
														} while (0)
			#define I64_TO_I32(i64, i32)				do { \
															(i32) = (int)(i64); \
														} while (0)
			#define I64_TO_F32(i64, f32)				do { \
															(f32) = (float)(i64); \
														} while (0)
			#define I64_TO_F64(i64, f64)				do { \
															(f64) = (float)(i64); \
														} while (0)
		#endif //defined POSIX || defined F_ITRON || defined VXWORKS_6x || defined FREERTOS || defined VXWORKS_5_4*/
	#endif //!defined __INT64_DATATYPE
#endif //!defined __INT64 || !defined __INT64_DATATYPE_

/*------------------------------------------------------------------------------
 * WINDOWS types used in JNI code
 *-----------------------------------------------------------------------------*/
#if defined WIN32 || defined _WIN64
	#ifndef UNDER_RTSS
		#ifndef __JNIVM_P
			#define __JNIVM_P
			typedef JavaVM *jnivm_p;
		#endif /*__JNIVM_P */
		#ifndef __JNIENV_P
			#define __JNIENV_P
			typedef JNIEnv *jnienv_p;
		#endif /*__JNIENV_P*/
	#endif /* UNDER_RTSS */
#endif /* WIN32 || _WIN64*/

/*------------------------------------------------------------------------------
 * debug buffer entry
 *-----------------------------------------------------------------------------*/
typedef enum {
	DATA_TYPE = 1,
	ETCOM_TYPE = 2,
} DBG_ENTRY_BUFFER_TYPE;

#define DBG_BUFFER_LINE_SIZE 1024

typedef struct _dbg_entry {
    char process_name[32];
    int event_id;
    int pair_event_id;
    int process_id;
    int process_priority;
    int thread_id;
    int thread_priority;
    double timestamp;
    int event_kind;
    int event_source;
    int event_stream;
    int event_ecode;
	int event_protocol;
	int event_port;
    DBG_ENTRY_BUFFER_TYPE buffer_type;
    int buffer_size;
    byte buffer[DBG_BUFFER_LINE_SIZE];
    int pair_buffer_size;
    byte pair_buffer[DBG_BUFFER_LINE_SIZE];
	eint64 mask;
	dword optional;
    char fct_name[32];
    char event_msg[64];
} DBG_ENTRY;


/*------------------------------------------------------------------------------
 * firmware manifest definition structure
 *-----------------------------------------------------------------------------*/
typedef struct _fw_manifest {
    char name[64];
    char version[64];
    char reg_blocks[256];
    char seq_blocks[256];
    char title[64];
	char product_number[64];
} FW_MANIFEST;


/*------------------------------------------------------------------------------
 * directory entry definition structure
 *-----------------------------------------------------------------------------*/
typedef struct _directory_entry {
    char name[_MAX_PATH];
    ebool directory;
} DIRECTORY_ENTRY;


/*------------------------------------------------------------------------------
 * types for net module
 *-----------------------------------------------------------------------------*/

/*--------------------------------------------*/
/* POSIX, VXWORKS_6x, VXWORKS_5_4            */
#if defined POSIX || defined VXWORKS_6x || defined VXWORKS_5_4
	typedef int SOCKET;
	typedef struct hostent HOSTENT;
	typedef struct sockaddr_in SOCKADDR_IN;
    #if !defined VXWORKS_6x && !defined VXWORKS_5_4
	    typedef struct sockaddr SOCKADDR; 
    #endif // !defined VXWORKS_6x && !defined VXWORKS_5_4
#endif /* POSIX || VXWORKS_6x || VXWORKS_5_4*/
	
/*------------------------------*/
/* F_ITRON types for net module */
#ifdef F_ITRON
	typedef int SOCKET;
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr SOCKADDR;
#endif /*F_ITRON*/


/*------------------------------*/
/* FREERTOS types for net module */
#ifdef FREERTOS
	typedef int SOCKET;
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr SOCKADDR;
#endif /*FREERTOS*/

/*------------------------------------------------------------------------------
 * type modifiers
 *-----------------------------------------------------------------------------*/
/*------------------------------*/
/* WINDOWS type modifiers		*/
#if defined WIN32 || defined _WIN64
	#ifndef _LIB_EXPORT
		#ifndef  LIB_STATIC
			#define _LIB_EXPORT __cdecl                     /* function exported by DLL library */
		#else
			#define _LIB_EXPORT __cdecl                     /* function exported by static library */
		#endif
	#endif /* _LIB_EXPORT */
	#define LIB_CALLBACK __cdecl                            /* client callback function called by library */
#endif /* WIN32 || _WIN64*/

/*---------------------------------------*/
/* POSIX || VXWORKS_6x type modifiers	 */
#if defined POSIX || defined VXWORKS_6x || defined VXWORKS_5_4
	#define _LIB_EXPORT
	#define LIB_EXPORT
#endif /*defined POSIX || defined VXWORKS_6x || defined VXWORKS_5_4*/

/*------------------------------*/
/* F_ITRON type modifiers		*/
#ifdef F_ITRON
	#define _LIB_EXPORT __cdecl
	#define LIB_EXPORT __cdecl
#endif /*F_ITRON*/

/*---------------------------------------*/
/* FREERTOS type modifiers	 */
#if defined FREERTOS
	#define _LIB_EXPORT
	#define LIB_EXPORT
#endif /*FREERTOS*/

/*------------------------------------------------------------------------------
 * hidden structures for library clients
 *-----------------------------------------------------------------------------*/
#ifndef PRO
	#define PRO void
#endif
#ifndef SHM
	#define SHM void
#endif
#ifndef MD5_CONTEXT
	#define MD5_CONTEXT void
#endif


/**********************************************************************************************************/
/*- MACROS */
/**********************************************************************************************************/
/*------------------------------------------------------------------------------
 * Define verify macro - like assert but continue
 * to evaluate and check the argument with the release build
 *-----------------------------------------------------------------------------*/
#if !defined NDEBUG && !defined _NDEBUG
	#define verify(v) assert(v)
#else
	#define verify(v) do { if(!(v)) abort(); } while(0)
#endif

/*------------------------------------------------------------------------------
 * special macro to specify register static or global variables
 * this macro expand to nothing in standard platform
 *-----------------------------------------------------------------------------*/
#define REGISTER

/*------------------------------------------------------------------------------
 * clear the specified structure - utility function
 *-----------------------------------------------------------------------------*/
#define CLEAR(s) (memset(&(s), '\0', sizeof(s)))

/*------------------------------------------------------------------------------
 * waiting macro - wait the specified
 * number of milliseconds
 *-----------------------------------------------------------------------------*/
/*----------------------------------*/
/* WINDOWS-RTX waiting macro		*/
#if defined WIN32 || defined _WIN64
	#ifdef UNDER_RTSS
		#define SLEEP(time)                     (Sleep(time))
		#define USLEEP(time)                    (uSleep(time))
	/*----------------------------------*/
	/* WINDOWS-W32 waiting macro		*/
	#else
		#define SLEEP(time)                     (Sleep(time))
	#endif /* UNDER_RTSS */
#endif /* WIN32 || _WIN64*/

/*----------------------------------------------*/
/* POSIX, F_ITRON, VXWORKS_6x, FREERTOS, VXWORKS_5_4 waiting macro	*/
#if defined POSIX || defined F_ITRON || defined VXWORKS_6x || defined FREERTOS || defined VXWORKS_5_4
	void	_LIB_EXPORT special_sleep(long etime);
	#define SLEEP(etime)                     special_sleep(etime)
#endif /*defined POSIX || defined F_ITRON || defined VXWORKS_6x || defined FREERTOS || defined VXWORKS_5_4*/

/*------------------------------------------------------------------------------
 * thread macros - use thread in a multi-platform way
 *-----------------------------------------------------------------------------*/
/*--------------------------------------*/
/* WINDOWS-RTX thread macros			*/
#if defined WIN32 || defined _WIN64
	#ifdef UNDER_RTSS
		#define THREAD HANDLE
		#define THREAD_INVALID                              ((HANDLE)(-1))
		#define THREAD_INIT_PRIO(thr, fct, arg,prio)        do { \
																DWORD tid; \
																(thr) = CreateThread(NULL, 20480, (LPTHREAD_START_ROUTINE)(fct), (arg), 0, &tid); \
																RtSetThreadPriority((thr), (prio)); \
															} while(0)
		#define THREAD_INIT_AND_NAME_PRIO(thr,name,fct,arg,prio) do { \
																		DWORD tid; \
																		(thr) = CreateThread(NULL, 20480, (LPTHREAD_START_ROUTINE)(fct), (arg), 0, &tid); \
																		RtSetThreadPriority((thr), (prio)); \
																	} while(0)
		#define THREAD_SET_CORE(thr, core)			        (NULL)
		#define THREAD_WAIT(thr, timeout)                   (lib_wait_for_thread(thr, timeout))
		#define THREAD_GET_CURRENT()                        (GetCurrentThread())
		#define THREAD_GET_CURRENT_ID()                     (GetCurrentThreadId())
		#define THREAD_GET_NAME(thr)                        (NULL)
		#define PROCESS_GET_CURRENT()                       (NULL)
		#define PROCESS_GET_CURRENT_ID()                    (GetCurrentProcessId())
		#define THREAD_END()								(NULL)

		/******************************** */
		/* Obsolete: Use THREAD_INIT_PRIO */
		#define THREAD_INIT(thr, fct, arg)                  do { \
																DWORD tid; \
																(thr) = CreateThread(NULL, 20480, (LPTHREAD_START_ROUTINE)(fct), (arg), 0, &tid); \
															} while(0)
		#define THREAD_INIT_AND_NAME(thr, name, fct, arg)   do { \
																DWORD tid; \
																(thr) = CreateThread(NULL, 20480, (LPTHREAD_START_ROUTINE)(fct), (arg), 0, &tid); \
															} while(0)
		#define THREAD_SET_PRIORITY(thr, pri)               do { \
																RtSetThreadPriority((thr), (pri)); \
															} while(0)
		#define THREAD_GET_PRIORITY(thr)                    (RtGetThreadPriority((thr)))
		#define THREAD_SET_CURRENT_PRIORITY(pri)            do { \
																RtSetThreadPriority(GetCurrentThread(), (pri)); \
															} while(0)
		#define THREAD_GET_CURRENT_PRIORITY()               (RtGetThreadPriority(GetCurrentThread()))
		
		/******************************** */

	/*--------------------------------------*/
	/* WINDOWS-W32 thread macros			*/
	#else
		#define THREAD HANDLE
		#define THREAD_INVALID                              ((HANDLE)(-1))
		#define THREAD_INIT_PRIO(thr, fct, arg, prio)       do { \
																(thr) = rtx_beginthread((fct), (arg)); \
																SetThreadPriority((thr), (prio)); \
															} while(0)
		#define THREAD_INIT_AND_NAME_PRIO(thr,name,fct,arg,prio)   do { \
																	(thr) = rtx_begin_named_thread((name), (fct), (arg)); \
																	SetThreadPriority((thr), (prio)); \
																} while(0)
		#define THREAD_SET_CORE(thr, core)                  (NULL)
		#define THREAD_WAIT(thr, timeout)                   (WaitForSingleObject((thr), (timeout)))
		#define THREAD_GET_CURRENT()                        (GetCurrentThread())
		#define THREAD_GET_CURRENT_ID()                     (GetCurrentThreadId())
		#define THREAD_GET_NAME(thr)                        (rtx_get_thread_name(thr))
		#define PROCESS_GET_CURRENT()                       (GetCurrentProcess())
		#define PROCESS_GET_CURRENT_ID()                    (GetCurrentProcessId())
		#define THREAD_END()								(NULL)

		/******************************** */
		/* Obsolete: Use THREAD_INIT_PRIO */
		#define THREAD_INIT(thr, fct, arg)                  do { \
																(thr) = rtx_beginthread((fct), (arg)); \
															} while(0)
		#define THREAD_INIT_AND_NAME(thr, name, fct, arg)   do { \
																(thr) = rtx_begin_named_thread((name), (fct), (arg)); \
															} while(0)
		#define THREAD_SET_PRIORITY(thr, pri)               do { \
																SetThreadPriority((thr), (pri)); \
															} while(0)
		#define THREAD_GET_PRIORITY(thr)                    (GetThreadPriority((thr)))
		#define THREAD_SET_CURRENT_PRIORITY(pri)            do { \
																SetThreadPriority(GetCurrentThread(), (pri)); \
															} while(0)
		#define THREAD_GET_CURRENT_PRIORITY()               (GetThreadPriority(GetCurrentThread()))
		/******************************** */

	#endif /* UNDER_RTSS */
#endif /* WIN32 || _WIN64*/

/*------------------------------*/
/* POSIX thread macros			*/
#if defined POSIX
	#define THREAD                                      pthread_t
	#define THREAD_INVALID                              ((pthread_t)(-1))
	#ifdef VXWORKS_6x	
		#define THREAD_INIT_PRIO(thr,fct,arg,prio)          do {\
																	pthread_attr_t attr;\
																	pthread_attr_init(&attr);\
																	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);\
																	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);\
																	pthread_create (&thr, &attr, (void*)&fct, arg); \
																	thread_set_prio(thr, prio); \
															}while (0)
		#define THREAD_INIT_AND_NAME_PRIO(thr,name,fct,arg,prio) do {\
																pthread_attr_t attr;\
																pthread_attr_init(&attr);\
																pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);\
																pthread_attr_setschedpolicy(&attr, SCHED_FIFO);\
																pthread_attr_setname(&attr, name);\
																pthread_create (&thr, &attr, (void*)&fct, arg); \
																thread_set_prio(thr, prio); \
															}while (0)
	#else															
		#define THREAD_INIT_PRIO(thr,fct,arg,prio)          do {\
																pthread_create (&thr, NULL, (void*)&fct, arg); \
																thread_set_prio(thr, prio); \
															}while (0)
		#define THREAD_INIT_AND_NAME_PRIO(thr,name,fct,arg,prio) do {\
																pthread_create (&thr, NULL, (void*)&fct, arg); \
																thread_set_prio(thr, prio); \
															}while (0)
	#endif /*VXWORKS_6x	*/															
	#define THREAD_SET_CORE(thr, core)                  (NULL)
	#define THREAD_INIT_DETACH(thr,fct,arg)             do { \
															pthread_attr_t attr; \
															pthread_attr_init(&attr); \
															pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); \
															pthread_create (&thr, &attr, (void*)&fct, arg); \
															pthread_attr_destroy(&attr); \
														}while (0)
	//pas de timeout
	#define THREAD_SET_POLICY(thr,policy,pri)           (thread_set_policy(thr, policy, pri))
	#define THREAD_WAIT(thr,timeout)                    (pthread_join (thr, NULL))
	#define THREAD_GET_CURRENT()                        (pthread_self())
	#define THREAD_GET_CURRENT_ID()                     (pthread_self())
	#define THREAD_GET_NAME(thr)                        NULL
	#define PROCESS_GET_CURRENT()                       (getpid())
	#define PROCESS_GET_CURRENT_ID()                    (getpid())
	#define THREAD_END()								(NULL)
	
	/******************************** */
	/* Obsolete: Use THREAD_INIT_PRIO */
	#define THREAD_INIT(thr,fct,arg)                    do { \
															pthread_create (&thr, NULL, (void*)&fct, arg); \
														}while (0)
	#define THREAD_INIT_AND_NAME(thr,name,fct,arg)      do {\
															pthread_create (&thr, NULL, (void*)&fct, arg); \
														}while (0)
	#define THREAD_SET_CURRENT_PRIORITY(pri)            (thread_set_prio(pthread_self(), pri))
	#define THREAD_GET_CURRENT_PRIORITY()               (thread_get_prio(pthread_self()))
	#define THREAD_SET_PRIORITY(thr,pri)                (thread_set_prio(thr, pri))
	#define THREAD_GET_PRIORITY(thr)                    (thread_get_prio(thr))
	/******************************** */
#endif /*POSIX*/

/*--------------------------------------*/
/* VXWORKS_6x thread macros	    */
#if defined VXWORKS_6x
	#define THREAD                                      int
	#define THREAD_INVALID                              (ERROR)
	#define THREAD_AFFINITY_SET
	#define THREAD_INIT_PRIO(thr, fct, arg, prio)        do {\
	   	   	   	   	   	   	   	   	   	   	   	   	   		thr = taskCreate(NULL, prio, VX_FP_TASK | VX_NO_STACK_PROTECT, 0x40000, (FUNCPTR)fct, (int)arg, 0, 0, 0, 0, 0, 0, 0, 0, 0);\
	   	   	   	   	   	   	   	   	   	   	   	   	   		ioTaskStdSet(thr, 0, ioTaskStdGet(taskIdSelf(), 0)); \
															ioTaskStdSet(thr, 1, ioTaskStdGet(taskIdSelf(), 1)); \
	   	   	   	   	   	   	   	   	   	   	   	   	   		ioTaskStdSet(thr, 2, ioTaskStdGet(taskIdSelf(), 2)); \
	   	   	   	   	   	   	   	   	   	   	   	   	   		taskResume(thr);\
														} while(0)
	#define THREAD_INIT_AND_NAME_PRIO(thr,name,fct,arg,prio) do {\
															thr = taskCreate(name, prio, VX_FP_TASK | VX_NO_STACK_PROTECT, 0x40000, (FUNCPTR)fct, (int)arg, 0, 0, 0, 0, 0, 0, 0, 0, 0);\
	   	   	   	   	   	   	   	   	   	   	   	   	   		ioTaskStdSet(thr, 0, ioTaskStdGet(taskIdSelf(), 0)); \
															ioTaskStdSet(thr, 1, ioTaskStdGet(taskIdSelf(), 1)); \
	   	   	   	   	   	   	   	   	   	   	   	   	   		ioTaskStdSet(thr, 2, ioTaskStdGet(taskIdSelf(), 2)); \
	   	   	   	   	   	   	   	   	   	   	   	   	   		taskResume(thr);\
														} while(0)
	#define THREAD_SET_CORE(thr, core)                  do {\
															unsigned int nb_of_cores = vxCpuConfiguredGet();\
									   	   	   	   	   	   	if (nb_of_cores >= core + 1) { \
								  	   	   	   	   	   	   	   cpuset_t affinity;\
								  	   	   	   	   	   	   	   CPUSET_ZERO (affinity);\
								  	   	   	   	   	   	   	   CPUSET_SET  (affinity, core);\
								  	   	   	   	   	   	   	   if (taskCpuAffinitySet (thr, affinity) == ERROR) {\
								  	   	   	   	   	   	   		   edilog("This thread affinity cannot be changed on core : %d\n", core);\
															   }\
															}\
														} while(0)
	#define THREAD_INIT_DETACH(thr,fct,arg)             do { \
															THREAD_INIT_AND_NAME_PRIO(thr, NULL, fct, arg, EDI_THREAD_PRIORITY_NORMAL);\
														}while (0)
	#define THREAD_WAIT(thr,timeout)                    (task_wait(thr, timeout))
	#define THREAD_GET_CURRENT()                        (taskIdSelf())
	#define THREAD_GET_CURRENT_ID()                     (taskIdSelf())
	#define THREAD_GET_NAME(thr)                        (taskName(thr))
	#define PROCESS_GET_CURRENT()                       (taskIdSelf())
	#define PROCESS_GET_CURRENT_ID()                    (taskIdSelf())
	#define THREAD_END()								
	#define THREAD_SET_POLICY(thr,policy,pri)           (NULL)	//Not possible to set policy with native VxWorks scheduler
		
		
	/******************************** */
	/* Obsolete: Use THREAD_INIT_PRIO */
		
	#define THREAD_INIT(thr,fct,arg)                    do { \
															thr = taskSpawn(NULL, EDI_THREAD_PRIORITY_NORMAL, VX_FP_TASK | VX_NO_STACK_PROTECT, 0x40000, (FUNCPTR)fct, (int)arg, 0, 0, 0, 0, 0, 0, 0, 0, 0); \
														}while (0)
	#define THREAD_INIT_AND_NAME(thr,name,fct,arg)      do {\
															thr = taskSpawn(name, EDI_THREAD_PRIORITY_NORMAL, VX_FP_TASK | VX_NO_STACK_PROTECT, 0x40000, (FUNCPTR)fct, (int)arg, 0, 0, 0, 0, 0, 0, 0, 0, 0); \
														}while (0)
		
	#define THREAD_SET_PRIORITY(thr,pri)                (task_set_prio(thr, pri))
	#define THREAD_GET_PRIORITY(thr)                    (task_get_prio(thr))
	#define THREAD_SET_CURRENT_PRIORITY(pri)            (task_set_prio(taskIdSelf(), pri))
	#define THREAD_GET_CURRENT_PRIORITY()               (task_get_prio(taskIdSelf()))
	/******************************** */
#endif /*VXWORKS_6x*/

/*--------------------------------------*/
/* VXWORKS_5_4 thread macros			*/
#ifdef VXWORKS_5_4
	#define THREAD                                      int
	#define THREAD_INVALID                              (ERROR)
	#define THREAD_INIT_PRIO(thr, fct, arg, prio)        do {\
	   	   	   	   	   	   	   	   	   	   	   	   	   		thr = taskSpawn(NULL, prio, VX_FP_TASK, 128000, (FUNCPTR)fct, (int)arg, 0, 0, 0, 0, 0, 0, 0, 0, 0);\
														} while(0)
	#define THREAD_INIT_AND_NAME_PRIO(thr,name,fct,arg,prio) do {\
															thr = taskSpawn(name, prio, VX_FP_TASK, 128000, (FUNCPTR)fct, (int)arg, 0, 0, 0, 0, 0, 0, 0, 0, 0);\
														} while(0)
	#define THREAD_INIT_DETACH(thr,fct,arg)             do { \
															THREAD_INIT_AND_NAME_PRIO(thr, NULL, fct, arg, EDI_THREAD_PRIORITY_NORMAL);\
														}while (0)
	#define THREAD_WAIT(thr,timeout)                    do { \
															SLEEP(1000); \
														} while (taskIdVerify(thr) == OK)
	#define THREAD_GET_CURRENT()                        (taskIdSelf())
	#define THREAD_GET_CURRENT_ID()                     (taskIdSelf())
	#define THREAD_GET_NAME(thr)                        NULL
	#define PROCESS_GET_CURRENT()                       (taskIdSelf())
	#define PROCESS_GET_CURRENT_ID()                    (taskIdSelf())
	#define THREAD_END()								
	#define THREAD_SET_POLICY(thr,policy,pri)           (NULL)	//Not possible to set policy with native VxWorks scheduler

	/******************************** */
	/* Obsolete: Use THREAD_INIT_PRIO */
	#define THREAD_INIT(thr,fct,arg)                    do { \
															thr = taskSpawn(NULL, THREAD_PRIORITY_NORMAL, VX_FP_TASK, 128000, (FUNCPTR)fct, (int)arg, 0, 0, 0, 0, 0, 0, 0, 0, 0); \
														}while (0)
	#define THREAD_INIT_AND_NAME(thr,name,fct,arg)      do {\
															thr = taskSpawn(name, THREAD_PRIORITY_NORMAL, VX_FP_TASK, 128000, (FUNCPTR)fct, (int)arg, 0, 0, 0, 0, 0, 0, 0, 0, 0); \
														}while (0)
	#define THREAD_SET_PRIORITY(thr,pri)                (task_set_prio(thr, pri))
	#define THREAD_GET_PRIORITY(thr)                    (task_get_prio(thr))
	#define THREAD_SET_CURRENT_PRIORITY(pri)            (task_set_prio(taskIdSelf(), pri))
	#define THREAD_GET_CURRENT_PRIORITY()               (task_get_prio(taskIdSeld()))
#endif /*VXWORKS_5_4*/

/*--------------------------------------*/
/* F_ITRON thread macros				*/
#ifdef F_ITRON
	#define THREAD                                      FJ_ID
	#define THREAD_INVALID                              ((FJ_ID)-1)
	#define THREAD_INIT_PRIO(thr,fct,arg,prio)          do { \
															thr = lib_fitron_task_create(fct, arg, prio, NULL); \
														}while (0)
	#define THREAD_INIT_AND_NAME_PRIO(thr,name,fct,arg,prio) do {\
															thr = lib_fitron_task_create(fct, arg, prio, name); \
														}while (0)
	#define THREAD_SET_CORE(thr, core)                  NULL
	#define THREAD_WAIT(thr,timeout)					(lib_fitron_task_wait(thr, timeout))
	#define THREAD_END()								do {\
															lib_fitron_task_destroy(); \
														}while (0)
	#define THREAD_GET_CURRENT()                        (lib_fitron_get_task_id())
	#define THREAD_GET_CURRENT_ID()                     (lib_fitron_get_task_id())
	#define THREAD_GET_NAME(thr)                        NULL
	#define PROCESS_GET_CURRENT()                       (lib_fitron_get_task_id())
	#define PROCESS_GET_CURRENT_ID()                    (lib_fitron_get_task_id())

	/******************************** */
	/* Obsolete: Use THREAD_INIT_PRIO */
/*
	#define THREAD_INIT(thr,fct,arg)
	#define THREAD_INIT_AND_NAME(thr,name,fct,arg)      
	#define THREAD_SET_PRIORITY(thr,pri)				do {\
															(lib_fitron_task_set_prio(thr, pri));\
														}while (0)
														
	#define THREAD_GET_PRIORITY(thr)					(lib_fitron_task_get_prio(thr))
	#define THREAD_SET_CURRENT_PRIORITY(pri) 
	#define THREAD_GET_CURRENT_PRIORITY() 
*/	
	/******************************** */
#endif /*F_ITRON*/

/*--------------------------------------*/
/* FREERTOS thread macros	    */
#if defined FREERTOS
	eint64	_LIB_EXPORT tim_counter(void);
	#define THREAD                                      		TaskHandle_t
	#define THREAD_INVALID                              		(NULL)
	#define THREAD_INIT_PRIO(thr, fct, arg, prio)        		do {\
																	if (xTaskCreate((void (*)(void*))fct, NULL, 16384, arg, prio, &thr) != pdPASS) \
																		thr = NULL;\
																} while(0)
	#define THREAD_INIT_AND_NAME_PRIO(thr,name,fct,arg,prio) 	do {\
																	if (xTaskCreate((void (*)(void*))fct, name, 16384, arg, prio, &thr) != pdPASS) \
																		thr = NULL;\
																} while(0)
	#define THREAD_WAIT(thr,timeout)                    		do {\
																	eint64 t1 = tim_counter();\
																	do {\
																		if (eTaskGetState(thr) == eDeleted)\
																			break;\
																		SLEEP(500);\
																	}\
																	while ((timeout == INFINITE) || ((tim_counter() - t1) < timeout));\
																} while(0)
	#define THREAD_GET_CURRENT()                        		(xTaskGetCurrentTaskHandle())
	#define THREAD_GET_CURRENT_ID()                     		(xTaskGetCurrentTaskHandle())
	#define THREAD_GET_NAME(thr)                        		(pcTaskGetName(NULL))
	#define PROCESS_GET_CURRENT()                       		(xTaskGetCurrentTaskHandle())
	#define PROCESS_GET_CURRENT_ID()                    		(xTaskGetCurrentTaskHandle())
	#define THREAD_END()										do {\
																	vTaskDelete(NULL); \
																} while (0);
	#define THREAD_SET_POLICY(thr,policy,pri)           		;


	/******************************** */
	/* Obsolete: Use THREAD_INIT_PRIO */

	#define THREAD_INIT(thr,fct,arg)                   			 do { \
																	if (xTaskCreate((void (*)(void*))fct, NULL, 16384, arg, EDI_THREAD_PRIORITY_NORMAL, &thr) != pdPASS) \
																		thr = NULL;\
																} while (0)
	#define THREAD_INIT_AND_NAME(thr,name,fct,arg)      		do {\
																	if (xTaskCreate((void (*)(void*))fct, name, 16384, arg, EDI_THREAD_PRIORITY_NORMAL, &thr) != pdPASS) \
																		thr = NULL;\
																} while (0)

	#define THREAD_SET_PRIORITY(thr,pri)                		(vTaskPrioritySet(thr, pri))
	#define THREAD_GET_PRIORITY(thr)                    		(uxTaskPriorityGet(thr))
	#define THREAD_SET_CURRENT_PRIORITY(pri)            		(vTaskPrioritySet(xTaskGetCurrentTaskHandle(), pri))
	#define THREAD_GET_CURRENT_PRIORITY()               		(uxTaskPriorityGet(xTaskGetCurrentTaskHandle()))
	/******************************** */
#endif /*FREERTOS*/

/*------------------------------------------------------------------------------
 * Synchronization lock object reservation
 * Must be called before first EDI function call of each F_ITRON process
 *-----------------------------------------------------------------------------*/
#ifdef F_ITRON
	#define EDI_INIT(firstLockIndex,nbLock,firstEventIndex,nbEvent,firstSemaIndex,nbSema) \
			(lib_fitron_edi_init(firstLockIndex,nbLock,firstEventIndex,nbEvent,firstSemaIndex,nbSema))
	#define EDI_EXIT() \
			(lib_fitron_edi_exit())
#endif /*F_ITRON*/	
/*------------------------------------------------------------------------------
 * critical sections - used to protect task against others or DPC
 * when shared variables are used betweed tasks.
 *-----------------------------------------------------------------------------*/
/*--------------------------------------*/
/* WINDOWS-RTX critical section			*/
#if defined WIN32 || defined _WIN64
	#ifdef UNDER_RTSS
		#define DEFINE_CRITICAL         CRITICAL_SECTION monitor;
		#define CRITICAL                struct {DEFINE_CRITICAL}
		#define CRITICAL_INIT(ob)       do { \
											InitializeCriticalSection(&(ob).monitor); \
										} while(0)
		#define CRITICAL_DESTROY(ob)    do { \
											DeleteCriticalSection(&(ob).monitor); \
										} while(0)
		#define CRITICAL_ENTER(ob)      do { \
											EnterCriticalSection(&(ob).monitor); \
										} while(0)
		#define CRITICAL_LEAVE(ob)      do { \
											LeaveCriticalSection(&(ob).monitor); \
										} while(0)


	/*--------------------------------------*/
	/* WINDOWS-W32 critical section			*/
	#else
		#define DEFINE_CRITICAL         CRITICAL_SECTION monitor;
		#define CRITICAL                struct {DEFINE_CRITICAL}
		#define CRITICAL_INIT(ob)       do { \
											InitializeCriticalSection(&(ob).monitor); \
										} while(0)
		#define CRITICAL_DESTROY(ob)    do { \
											DeleteCriticalSection(&(ob).monitor); \
										} while(0)
		#define CRITICAL_ENTER(ob)      do { \
											EnterCriticalSection(&(ob).monitor); \
										} while(0)
		#define CRITICAL_LEAVE(ob)      do { \
											LeaveCriticalSection(&(ob).monitor); \
										} while(0)
	#endif /* UNDER_RTSS */
#endif /* WIN32 critical section*/

/*--------------------------------------*/
/* POSIX Critical section				*/
#if defined POSIX
	/*--------------------------------------*/
	/* POSIX-STANDARD MUTEX Critical section : Followed by context switch*/
	#if defined MUTEX_STD
		#define DEFINE_CRITICAL         pthread_mutex_t mutex; int m_counter; THREAD m_pid;
		#define CRITICAL                struct {DEFINE_CRITICAL}
		#define CRITICAL_INIT(ob)       do { \
											pthread_mutex_init(&((ob).mutex), NULL); \
											(ob).m_counter=0;\
											(ob).m_pid=-1;\
										} while(0)
		#define CRITICAL_DESTROY(ob)    do { \
											pthread_mutex_destroy(&(ob).mutex); \
										} while(0)
		#define CRITICAL_ENTER(ob)      do { \
											if ((ob).m_pid==pthread_self()) { \
												(ob).m_counter++; \
											} \
											else { \
												pthread_mutex_lock(&(ob).mutex); \
												(ob).m_pid=pthread_self(); \
												(ob).m_counter++; \
											} \
										} while(0)
		#define CRITICAL_LEAVE(ob)      do { \
											if ((ob).m_pid==pthread_self()) { \
												if (--((ob).m_counter)==0) { \
													(ob).m_pid=-1; \
													pthread_mutex_unlock(&(ob).mutex); \
												} \
											} \
										} while(0)

	/*--------------------------------------*/
	/* POSIX-FAST MUTEX Critical section : Not followed by context switch */
	#elif defined MUTEX_FAST
		#define DEFINE_CRITICAL         pthread_mutex_t mutex; pthread_mutexattr_t attr;
		#define CRITICAL                struct {DEFINE_CRITICAL}
		#if defined QNX6 || defined LINUX
		/* Only available for QNX6 */
			#define CRITICAL_INIT(ob)       do { \
												pthread_mutexattr_init(&((ob).attr)); \
												pthread_mutexattr_settype(&((ob).attr), PTHREAD_MUTEX_RECURSIVE); \
												pthread_mutex_init(&((ob).mutex), &((ob).attr)); \
											} while(0)
		#else
			#define CRITICAL_INIT(ob)       do { \
												(ob).attr.__mutexkind = PTHREAD_MUTEX_RECURSIVE_NP; \
												pthread_mutex_init(&((ob).mutex), &((ob).attr)); \
											} while(0)
		#endif /*QNX6*/
		#define CRITICAL_DESTROY(ob)    do { \
											pthread_mutex_destroy(&(ob).mutex); \
										} while(0)
		#define CRITICAL_ENTER(ob)      do { \
											pthread_mutex_lock(&((ob).mutex)); \
										} while(0)
		#define CRITICAL_LEAVE(ob)      do { \
											pthread_mutex_unlock(&((ob).mutex)); \
										} while(0)
	/*--------------------------------------*/
	/* POSIX-DEFAULT MUTEX Critical section : Semaphore */
	#else
		#define DEFINE_CRITICAL         sem_t m_sem; int m_counter; THREAD m_pid;
		#define CRITICAL                struct {DEFINE_CRITICAL}
		#define CRITICAL_INIT(ob)       do { \
											sem_init (&(ob).m_sem, FALSE, 1); \
											(ob).m_counter=0; \
											(ob).m_pid=-1; \
										} while(0)
		#define CRITICAL_DESTROY(ob)    do { \
											sem_destroy(&(ob).m_sem); \
										} while(0)
		#define CRITICAL_ENTER(ob)      do { \
											if ((ob).m_pid==pthread_self()) { \
												(ob).m_counter++; \
											} \
											else { \
												sem_wait(&(ob).m_sem); \
												(ob).m_pid=pthread_self(); \
												(ob).m_counter++; \
											} \
										} while(0)
		#define CRITICAL_LEAVE(ob)      do { \
											if ((ob).m_pid==pthread_self()) { \
												if (--((ob).m_counter)==0) { \
													(ob).m_pid=-1; \
													sem_post(&(ob).m_sem); \
												} \
											} \
										} while(0)
	#endif /* MUTEX_STD */
#endif /*POSIX critical section */

/*--------------------------------------*/
/* VXWORKS Critical section  	*/
#if defined VXWORKS_6x
	#define DEFINE_CRITICAL         SEM_ID sem;
	#define CRITICAL                struct {DEFINE_CRITICAL}
	#define CRITICAL_INIT(ob)       do { \
										(ob).sem = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE); \
									} while(0)
	#define CRITICAL_DESTROY(ob)    do { \
										semDelete((ob).sem); \
									} while(0)
	#define CRITICAL_ENTER(ob)      do { \
										critical_enter((ob).sem); \
									} while(0)
	#define CRITICAL_LEAVE(ob)      do { \
										semGive((ob).sem); \
									} while(0)
#endif /*VXWORKS_6x*/

/*--------------------------------------*/
/* VXWORKS_5_4 Critical section */
#ifdef VXWORKS_5_4
	#define DEFINE_CRITICAL         SEM_ID sem;
	#define CRITICAL                struct {DEFINE_CRITICAL}
	#define CRITICAL_INIT(ob)       do { \
										(ob).sem = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE ); \
									} while(0)
	#define CRITICAL_DESTROY(ob)    do { \
										semDelete((ob).sem); \
									} while(0)
	#define CRITICAL_ENTER(ob)      do { \
										critical_enter((ob).sem); \
									} while(0)
	#define CRITICAL_LEAVE(ob)      do { \
										semGive((ob).sem); \
									} while(0)
#endif /*VXWORKS_5_4*/

/*--------------------------------------*/
/* F_ITRON Critical section  	        */
#ifdef F_ITRON
	#define DEFINE_CRITICAL 		FJ_ID cs; int m_counter; FJ_ID m_pid; 
	#define CRITICAL 				struct {DEFINE_CRITICAL}
	#define CRITICAL_INIT(ob)		do { \
										lib_fitron_critical_create(0, &(ob).cs, &(ob).m_counter, &(ob).m_pid); \
									} while(0)
	#define CRITICAL_DESTROY(ob) 	do { \
										lib_fitron_critical_destroy(&(ob).cs); \
									} while(0)
	#define CRITICAL_ENTER(ob)		do { \
										lib_fitron_critical_enter(&(ob).cs, &(ob).m_counter, &(ob).m_pid); \
									} while(0)
	#define CRITICAL_LEAVE(ob) 		do { \
										lib_fitron_critical_leave(&(ob).cs, &(ob).m_counter, &(ob).m_pid); \
									} while(0)
#endif /*F_ITRON */

/*--------------------------------------*/
/* FREERTOS Critical section			*/

#if defined FREERTOS
	#define DEFINE_CRITICAL         SemaphoreHandle_t sem;
	#define CRITICAL                struct {DEFINE_CRITICAL}
	#define CRITICAL_INIT(ob)       do { \
										(ob).sem = xSemaphoreCreateRecursiveMutex(); \
									} while(0)
	#define CRITICAL_DESTROY(ob)    do { \
										vSemaphoreDelete((ob).sem); \
									} while(0)
	#define CRITICAL_ENTER(ob)      do { \
										xSemaphoreTakeRecursive((ob).sem, portMAX_DELAY); \
									} while(0)
	#define CRITICAL_LEAVE(ob)      do { \
										xSemaphoreGiveRecursive((ob).sem); \
									} while(0)
#endif /*FREERTOS*/
/*------------------------------------------------------------------------------
 * events macros - create manual event with specified initial state
 *-----------------------------------------------------------------------------*/
/*--------------------------------------*/
/* WINDOWS - RTX events macros			*/
#if defined WIN32 || defined _WIN64
	#ifdef UNDER_RTSS
		#define EVENT                   HANDLE
		#define EVENT_INVALID           NULL
		#define EVENT_INIT(ev, init)    do { \
											(ev) = RtCreateEvent(NULL, TRUE, (init), NULL); \
										} while(0)
		#define EVENT_DESTROY(ev)       do { \
											RtCloseHandle(ev); \
										} while(0)
		#define EVENT_SET(ev)           do { \
											RtSetEvent((ev)); \
										} while(0)
		#define EVENT_RESET(ev)         do { \
											RtResetEvent((ev)); \
										} while(0)
		#define EVENT_WAIT(ev, timeout) (RtWaitForSingleObject((ev), (timeout)))
		#define IS_VALID_EVENT(ev)      (ev != EVENT_INVALID)

	/*--------------------------------------*/
	/* WINDOWS - W32 events macros			*/
	#else
		#define EVENT                   HANDLE
		#define EVENT_INVALID           NULL
		#define EVENT_INIT(ev, init)    do { \
											(ev) = CreateEvent(NULL, TRUE, (init), NULL); \
										} while(0)
		#define EVENT_DESTROY(ev)       do { \
											CloseHandle(ev); \
										} while(0)
		#define EVENT_SET(ev)           do { \
											SetEvent((ev)); \
										} while(0)
		#define EVENT_RESET(ev)         do { \
											ResetEvent((ev)); \
										} while(0)
		#define EVENT_WAIT(ev, timeout) (WaitForSingleObject((ev), (timeout)))
		#define IS_VALID_EVENT(ev)      (ev != EVENT_INVALID)
	#endif /* UNDER_RTSS */
#endif /* WIN32 || _WIN64*/

/*--------------------------------------*/
/* POSIX events macros					*/
#ifdef POSIX
	#define DEFINE_EVENT            pthread_mutex_t mutex; pthread_cond_t cond; int state; int error; int valid;
	#define EVENT_INVALID           NULL
	#define EVENT                   struct {DEFINE_EVENT}
	#define EVENT_INIT(ev,init)     do { \
										pthread_cond_init(&(ev).cond, NULL); \
										pthread_mutex_init(&(ev).mutex, NULL); \
										ev.state = init; \
										ev.error = 0; \
										ev.valid = 1; \
									} while(0)
	#define EVENT_DESTROY(ev)       do { \
										pthread_mutex_lock(&(ev).mutex); \
										ev.error = WAIT_FAILED; \
										pthread_mutex_unlock(&(ev).mutex); \
										pthread_cond_broadcast(&(ev).cond); \
										pthread_cond_destroy(&(ev).cond); \
										pthread_mutex_destroy(&(ev).mutex); \
										ev.valid = 0; \
									} while (0)
	#define EVENT_SET(ev)           do { \
										pthread_mutex_lock(&(ev).mutex); \
										ev.state = TRUE; \
										pthread_mutex_unlock(&(ev).mutex); \
										pthread_cond_broadcast(&(ev).cond); \
									} while (0)
	#define EVENT_RESET(ev)         do { \
										pthread_mutex_lock(&(ev).mutex); \
										ev.state = FALSE; \
										pthread_mutex_unlock(&(ev).mutex); \
									} while (0)
	#define EVENT_WAIT(ev,timeout)  (event_wait(&(ev.mutex), &(ev.cond), &(ev.state), &(ev.error), timeout))
	#define IS_VALID_EVENT(ev)      (ev.valid == 1)
#endif /*POSIX*/

/*--------------------------------------*/
/* VXWORKS events macros			*/
#if defined VXWORKS_6x
	#define EVENT_INVALID           NULL
	#define EVENT                   SEM_ID
	#define EVENT_INIT(ev,init)     do { \
										if (init) { \
											ev = semBCreate(SEM_Q_PRIORITY, SEM_FULL); \
										} \
										else { \
											ev = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY); \
										} \
									} while(0)
	#define EVENT_DESTROY(ev)       do{ \
										semDelete(ev); \
									} while (0)
	#define EVENT_SET(ev)           do { \
										semGive(ev); \
									} while (0)
	#define EVENT_RESET(ev)         do { \
										taskLock(); \
										semGive(ev); \
										semTake(ev, WAIT_FOREVER); \
										taskUnlock(); \
									} while (0)
	#define EVENT_WAIT(ev,timeout)  (event_wait(ev, timeout))
	#define IS_VALID_EVENT(ev)      (ev != EVENT_INVALID)
#endif /*VXWORKS_6x*/

/*--------------------------------------*/
/* VXWORKS_5_4 events macros			*/
#ifdef VXWORKS_5_4
	#define EVENT_INVALID           NULL
	#define EVENT                   SEM_ID
	#define EVENT_INIT(ev,init)     do { \
										if (init) { \
											ev = semBCreate(SEM_Q_PRIORITY, SEM_FULL); \
										} \
										else { \
											ev = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY); \
										} \
									} while(0)
	#define EVENT_DESTROY(ev)       do{ \
										semDelete(ev); \
									} while (0)
	#define EVENT_SET(ev)           do { \
										semGive(ev); \
									} while (0)
	#define EVENT_RESET(ev)         do { \
										taskLock(); \
										semGive(ev); \
										semTake(ev, WAIT_FOREVER); \
										taskUnlock(); \
									} while (0)
	#define EVENT_WAIT(ev,timeout)  (event_wait(ev, timeout))
	#define IS_VALID_EVENT(ev)      (ev != EVENT_INVALID)
#endif /*VXWORKS_5_4*/

/*--------------------------------------*/
/* F_ITRON events macros				*/
#ifdef F_ITRON
	#define EVENT_INVALID           ((FJ_ID)-1)
	#define EVENT           	    FJ_ID
	#define EVENT_INIT(ev,init)     do { \
										ev = lib_fitron_event_create(0, init); \
									} while(0)
	#define EVENT_DESTROY(ev)       do{ \
										lib_fitron_event_destroy(ev); \
									} while (0)
	#define EVENT_SET(ev)           do { \
										lib_fitron_event_set(ev); \
									} while (0)
	#define EVENT_RESET(ev)         do { \
										lib_fitron_event_reset(ev); \
									} while (0)
	#define EVENT_WAIT(ev,timeout)  (lib_fitron_event_wait(ev, timeout))
	#define IS_VALID_EVENT(ev)      (ev != EVENT_INVALID)
#endif /* F_ITRON */

/*--------------------------------------*/
/* FREERTOS events macros				*/
#if defined FREERTOS
	#define EVENT_INVALID           NULL
	#define EVENT                   SemaphoreHandle_t
	int _LIB_EXPORT lib_freeRTOS_event_wait(SemaphoreHandle_t ev, int timeout);
	#define EVENT_INIT(ev,init)     do { \
										ev = xSemaphoreCreateBinary(); \
										if (init) { \
											xSemaphoreGive(ev); \
										} \
									} while(0)
	#define EVENT_SET(ev)           do { \
										xSemaphoreGive(ev); \
									} while(0)

	#define EVENT_DESTROY(ev)       do { \
										vSemaphoreDelete(ev); \
									} while (0)
	#define EVENT_RESET(ev)         do { \
										xSemaphoreGive(ev); \
										xSemaphoreTake(ev, portMAX_DELAY); \
									} while (0)
	#define EVENT_WAIT(ev,timeout)  (lib_freeRTOS_event_wait(ev, timeout))

	#define IS_VALID_EVENT(ev)      (ev != EVENT_INVALID)
#endif /*FREERTOS*/

/*------------------------------------------------------------------------------
 * auto events macros - create automatic event with
 * specified initial state
 *-----------------------------------------------------------------------------*/
/*--------------------------------------*/
/* WINDOWS-RTX auto events macros		*/
#if defined WIN32 || defined _WIN64
	#ifdef UNDER_RTSS
		#define AUTOEVENT HANDLE
		#define AUTOEVENT_INVALID NULL
		#define AUTOEVENT_INIT(ev, init)    do { \
												(ev) = RtCreateEvent(NULL, FALSE, (init), NULL); \
											} while(0)
		#define AUTOEVENT_DESTROY(ev)       do { \
												RtCloseHandle(ev); \
											} while(0)
		#define AUTOEVENT_SET(ev)           do { \
												RtSetEvent((ev)); \
											} while(0)
		#define AUTOEVENT_RESET(ev)         do { \
												RtResetEvent((ev)); \
											} while(0)
		#define AUTOEVENT_WAIT(ev, timeout) (RtWaitForSingleObject((ev), (timeout)))
		#define IS_VALID_AUTOEVENT(ev)      (ev != AUTOEVENT_INVALID)

	/*--------------------------------------*/
	/* WINDOWS-W32 auto events macros		*/
	#else
		#define AUTOEVENT HANDLE
		#define AUTOEVENT_INVALID NULL
		#define AUTOEVENT_INIT(ev, init)    do { \
												(ev) = CreateEvent(NULL, FALSE, (init), NULL); \
											} while(0)
		#define AUTOEVENT_DESTROY(ev)       do { \
												CloseHandle(ev); \
											} while(0)
		#define AUTOEVENT_SET(ev)           do { \
												SetEvent((ev)); \
											} while(0)
		#define AUTOEVENT_RESET(ev)         do { \
												ResetEvent((ev)); \
											} while(0)
		#define AUTOEVENT_WAIT(ev, timeout) (WaitForSingleObject((ev), (timeout)))
		#define IS_VALID_AUTOEVENT(ev)      (ev != AUTOEVENT_INVALID)
	#endif /* UNDER_RTSS */
#endif /* WIN32 || _WIN64*/

/*--------------------------------------*/
/* POSIX auto events macros				*/
#ifdef POSIX
    #define DEFINE_AUTOEVENT            pthread_mutex_t mutex; pthread_cond_t cond; int state; int error; int valid;
	#define AUTOEVENT_INVALID           NULL
	#define AUTOEVENT                   struct {DEFINE_AUTOEVENT}
	#define AUTOEVENT_INIT(ev,init)     do { \
											pthread_cond_init(&(ev).cond, NULL); \
											pthread_mutex_init(&(ev).mutex, NULL); \
											ev.state = init; \
											ev.error = 0;\
											ev.valid = 1;\
										} while(0)
	#define AUTOEVENT_DESTROY(ev)       do{ \
											pthread_mutex_lock(&(ev).mutex); \
											ev.error = WAIT_FAILED; \
											pthread_mutex_unlock(&(ev).mutex); \
											pthread_cond_broadcast(&(ev).cond); \
											pthread_cond_destroy(&(ev).cond); \
											pthread_mutex_destroy(&(ev).mutex);\
											ev.valid = 0; \
										} while (0)
	#define AUTOEVENT_SET(ev)           do { \
											pthread_mutex_lock(&(ev).mutex);\
											ev.state = TRUE;\
											pthread_mutex_unlock(&(ev).mutex); \
											pthread_cond_broadcast(&(ev).cond); \
										}while (0)
	#define AUTOEVENT_RESET(ev)         do { \
											pthread_mutex_lock(&(ev).mutex); \
											ev.state = FALSE; \
											pthread_mutex_unlock(&(ev).mutex); \
										} while (0)
	#define AUTOEVENT_WAIT(ev,timeout)  (autoevent_wait(&(ev.mutex), &(ev.cond), &(ev.state), &(ev.error), timeout))
	#define IS_VALID_AUTOEVENT(ev)      (ev.valid == 1)
#endif /* POSIX */

/*--------------------------------------*/
/* VXWORKS auto events macros	*/
#if defined VXWORKS_6x
	#define AUTOEVENT_INVALID           NULL
	#define AUTOEVENT                   SEM_ID
	#define AUTOEVENT_INIT(ev,init)     do { \
											if (init) { \
												ev = semBCreate(SEM_Q_PRIORITY, SEM_FULL); \
											} \
											else { \
												ev = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY); \
											} \
										} while(0)
	#define AUTOEVENT_DESTROY(ev)       do{ \
											semDelete(ev); \
										} while (0)
	#define AUTOEVENT_SET(ev)           do { \
											semGive(ev); \
										} while (0)
	#define AUTOEVENT_RESET(ev)         do { \
											taskLock(); \
											semGive(ev); \
											semTake(ev, WAIT_FOREVER); \
											taskUnlock(); \
										} while (0)
	#define AUTOEVENT_WAIT(ev,timeout)  (autoevent_wait(ev, timeout))
	#define IS_VALID_AUTOEVENT(ev)      (ev != AUTOEVENT_INVALID)
#endif /*VXWORKS_6x*/

/*--------------------------------------*/
/* VXWORKS_5_4 auto events macros	    */
#ifdef VXWORKS_5_4
	#define AUTOEVENT_INVALID           NULL
	#define AUTOEVENT                   SEM_ID
	#define AUTOEVENT_INIT(ev,init)     do { \
											if (init) { \
												ev = semBCreate(SEM_Q_PRIORITY, SEM_FULL); \
											} \
											else { \
												ev = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY); \
											} \
										} while(0)
	#define AUTOEVENT_DESTROY(ev)       do{ \
											semDelete(ev); \
										} while (0)
	#define AUTOEVENT_SET(ev)           do { \
											semGive(ev); \
										} while (0)
	#define AUTOEVENT_RESET(ev)         do { \
											taskLock(); \
											semGive(ev); \
											semTake(ev, WAIT_FOREVER); \
											taskUnlock(); \
										} while (0)
	#define AUTOEVENT_WAIT(ev,timeout)  (autoevent_wait(ev, timeout))
	#define IS_VALID_AUTOEVENT(ev)      (ev != AUTOEVENT_INVALID)
#endif /*VXWORKS_5_4 */

/*--------------------------------------*/
/* F_ITRON autoevent macros				*/
#ifdef F_ITRON
	#define AUTOEVENT_INVALID      	    	((FJ_ID)-1)
	#define AUTOEVENT           	        FJ_ID
	#define AUTOEVENT_INIT(ev,init)     	do { \
												ev = lib_fitron_autoevent_create(0, init); \
											} while(0)
	#define AUTOEVENT_DESTROY(ev)       	do{ \
												lib_fitron_autoevent_destroy(ev); \
											} while (0)
	#define AUTOEVENT_SET(ev)           	do { \
												lib_fitron_autoevent_set(ev); \
											} while (0)
	#define AUTOEVENT_RESET(ev)         	do { \
												lib_fitron_autoevent_reset(ev); \
											} while (0)
	#define AUTOEVENT_WAIT(ev,timeout)  	(lib_fitron_autoevent_wait(ev, timeout))
	#define IS_VALID_AUTOEVENT(ev)      	(ev != AUTOEVENT_INVALID)
#endif /* F_ITRON */


/*--------------------------------------*/
/* FREERTOS autoevent macros			*/
#if defined FREERTOS
	#define AUTOEVENT_INVALID           NULL
	#define AUTOEVENT                   SemaphoreHandle_t
	int _LIB_EXPORT lib_freeRTOS_autoevent_wait(SemaphoreHandle_t ev, int timeout);
	#define AUTOEVENT_INIT(ev,init)     do { \
											ev = xSemaphoreCreateBinary(); \
											if (init) { \
												xSemaphoreGive(ev); \
											} \
										} while(0)
	#define AUTOEVENT_SET(ev)           do { \
											xSemaphoreGive(ev); \
										} while(0)

	#define AUTOEVENT_DESTROY(ev)       do { \
											vSemaphoreDelete(ev); \
										} while (0)
	#define AUTOEVENT_RESET(ev)         do { \
											xSemaphoreGive(ev); \
											xSemaphoreTake(ev, portMAX_DELAY); \
										} while (0)
	#define AUTOEVENT_WAIT(ev,timeout)  (lib_freeRTOS_autoevent_wait(ev, timeout))

	#define IS_VALID_AUTOEVENT(ev)      (ev != AUTOEVENT_INVALID)
#endif /*FREERTOS*/

/*------------------------------------------------------------------------------
 * mutexes macros - create manual event with
 * specified initial state
 *-----------------------------------------------------------------------------*/

/*--------------------------------------*/
/* WINDOWS-RTX mutexes macros			*/
#if defined WIN32 || defined _WIN64
	#ifdef UNDER_RTSS
		#define MUTEX                   HANDLE
		#define MUTEX_INVALID           NULL
		#define MUTEX_INIT(ev, init)    do { \
											(ev) = RtCreateMutex(NULL, (init), NULL); \
										} while(0)
		#define MUTEX_DESTROY(ev)       do { \
											RtCloseHandle((ev)); \
										} while(0)
		#define MUTEX_RELEASE(ev)       do { \
											RtReleaseMutex((ev)); \
										} while(0)
		#define MUTEX_WAIT(ev, timeout) (RtWaitForSingleObject((ev), (timeout)))
		#define IS_VALID_MUTEX(ev)      (ev != MUTEX_INVALID)

	/*--------------------------------------*/
	/* WINDOWS-W32 mutexes macros			*/
	#else
		#define MUTEX                   HANDLE
		#define MUTEX_INVALID           NULL
		#define MUTEX_INIT(ev, init)    do { \
											(ev) = CreateMutex(NULL, (init), NULL); \
										} while(0)
		#define MUTEX_DESTROY(ev)       do { \
											CloseHandle((ev)); \
										} while(0)
		#define MUTEX_RELEASE(ev)       do { \
											ReleaseMutex((ev)); \
										} while(0)
		#define MUTEX_WAIT(ev, timeout) (WaitForSingleObject((ev), (timeout)))
		#define IS_VALID_MUTEX(ev)      (ev != MUTEX_INVALID)
	#endif /* UNDER_RTSS */
#endif /* WIN32 || _WIN64*/

/*--------------------------------------*/
/* POSIX mutexes macros					*/
#ifdef POSIX
	#define DEFINE_MUTEX            pthread_mutex_t mutex; pthread_cond_t cond;  THREAD tid; int counter; int error; int valid;
	#define MUTEX_INVALID           NULL
	#define MUTEX                   struct {DEFINE_MUTEX}
	#define MUTEX_INIT(mut,init)    do { \
										pthread_cond_init(&(mut).cond, NULL); \
										pthread_mutex_init(&(mut).mutex, NULL); \
										mut.error = 0; \
										if (init) { \
											mut.tid = pthread_self(); \
											mut.counter = 0; \
										} \
										else { \
											mut.tid = -1; \
											mut.counter = 1; \
										} \
										mut.valid = 1; \
									} while(0)
	#define MUTEX_DESTROY(mut)      do { \
										pthread_mutex_lock(&(mut).mutex); \
										mut.error = WAIT_FAILED; \
										pthread_mutex_unlock(&(mut).mutex); \
										pthread_cond_broadcast(&(mut).cond); \
										pthread_cond_destroy(&(mut).cond); \
										pthread_mutex_destroy(&(mut).mutex); \
										mut.valid = 0; \
									} while (0)
	#define MUTEX_RELEASE(mut)      do { \
										if ((mut).tid==pthread_self()) { \
											pthread_mutex_lock(&(mut).mutex); \
											if (++((mut).counter)==1) { \
												(mut).tid=-1; \
												pthread_mutex_unlock(&(mut).mutex); \
												pthread_cond_signal(&(mut).cond); \
											} \
											else \
												pthread_mutex_unlock(&(mut).mutex); \
										} \
									} while(0)
	#define MUTEX_WAIT(mut,timeout) mutex_wait(&(mut.mutex), &(mut.cond), &(mut.tid), &(mut.counter), &(mut.error), timeout)
	#define IS_VALID_MUTEX(mut)     (mut.valid == 1)
#endif /*POSIX*/

/*--------------------------------------*/
/* VXWORKS mutexes macros		*/
#if defined VXWORKS_6x
	#define MUTEX                   SEM_ID
	#define MUTEX_INVALID           NULL
	#define MUTEX_INIT(mut,init)    do { \
										mut = semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE | SEM_INTERRUPTIBLE); \
										if (init) \
											semTake(mut, WAIT_FOREVER); \
									} while(0)
	#define MUTEX_DESTROY(mut)      do { \
										semDelete(mut); \
									} while(0)
	#define MUTEX_RELEASE(mut)      do { \
										semGive(mut); \
									} while(0)
	#define MUTEX_WAIT(mut,timeout) (mutex_wait(mut, timeout))
	#define IS_VALID_MUTEX(mut)     (mut != MUTEX_INVALID)
#endif /*VXWORKS_6x*/

/*--------------------------------------*/
/* VXWORKS_5_4 mutexes macros			*/
#ifdef VXWORKS_5_4
	#define MUTEX                   SEM_ID
	#define MUTEX_INVALID           NULL
	#define MUTEX_INIT(mut,init)    do { \
										mut = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE ); \
										if (init) \
											semTake(mut, WAIT_FOREVER); \
									} while(0)
	#define MUTEX_DESTROY(mut)      do { \
										semDelete(mut); \
									} while(0)
	#define MUTEX_RELEASE(mut)      do { \
										semGive(mut); \
									} while(0)
	#define MUTEX_WAIT(mut,timeout) (mutex_wait(mut, timeout))
	#define IS_VALID_MUTEX(mut)     (mut != MUTEX_INVALID)
#endif /*VXWORKS_5_4*/

/*----------------------------------*/
/* F_ITRON mutex macros				*/
#ifdef F_ITRON
	#define DEFINE_MUTEX 			FJ_ID mut;int counter; FJ_ID pid; 
	#define MUTEX_INVALID       	NULL
	#define MUTEX 					struct {DEFINE_MUTEX}
	#define MUTEX_INIT(ob, init)	do { \
										lib_fitron_mutex_create(0, &(ob).mut, &(ob).counter, &(ob).pid, init); \
									} while(0)
	#define MUTEX_DESTROY(ob) 		do { \
										lib_fitron_mutex_destroy(&(ob).mut); \
									} while(0)
	#define MUTEX_RELEASE(ob)		do { \
										lib_fitron_mutex_release(&(ob).mut, &(ob).counter, &(ob).pid); \
									} while(0)
	#define MUTEX_WAIT(ob, timeout)	(lib_fitron_mutex_wait(&(ob).mut, &(ob).counter, &(ob).pid, timeout))
#endif /*F_ITRON */

/*----------------------------------*/
/* FREERTOS mutex macros			*/
#if defined FREERTOS
	#define MUTEX         			SemaphoreHandle_t
	#define MUTEX_INVALID  			NULL
	int _LIB_EXPORT lib_freeRTOS_mutex_wait(SemaphoreHandle_t mutex, int timeout);
	#define MUTEX_INIT(mut, init)	do { \
										mut = xSemaphoreCreateRecursiveMutex(); \
										if (init) \
											xSemaphoreTakeRecursive(mut, portMAX_DELAY); \
									} while(0)
	#define MUTEX_DESTROY(mut)    	do { \
										vSemaphoreDelete(mut); \
									} while(0)
	#define MUTEX_RELEASE(mut)       do { \
										xSemaphoreGiveRecursive(mut); \
									} while(0)
	#define MUTEX_WAIT(mut,timeout) (lib_freeRTOS_mutex_wait(mut, timeout))
	#define IS_VALID_MUTEX(mut)     (mut != MUTEX_INVALID)
#endif /*FREERTOS*/

/*------------------------------------------------------------------------------
 * counting semaphore macros - create semaphore
 * with specified initial and maximum value
 *-----------------------------------------------------------------------------*/
/*------------------------------------------*/
/* WINDOWS-RTX counting semaphore macros	*/
#if defined WIN32 || defined _WIN64
	#ifdef UNDER_RTSS
		#define SEMACOUNT                       HANDLE
		#define SEMACOUNT_INVALID               NULL
		#define SEMACOUNT_INIT(sem, init, max)  do { \
													(sem) = RtCreateSemaphore(NULL, (init), (max), NULL); \
												} while(0)
		#define SEMACOUNT_DESTROY(sem)          do { \
													RtCloseHandle(sem); \
												} while(0)
		#define SEMACOUNT_RELEASE(sem)          do { \
													RtReleaseSemaphore((sem), 1, NULL); \
												} while(0)
		#define SEMACOUNT_WAIT(sem, timeout)    (RtWaitForSingleObject((sem), (timeout)))
		#define IS_VALID_SEMACOUNT(ev)          (ev != SEMACOUNT_INVALID)
		#define SET_SEMACOUNT(target, source)   do { \
													memcpy(&target, &source, sizeof(target));\
												} while (0)
	/*------------------------------------------*/
	/* WINDOWS-W32 counting semaphore macros	*/
	#else
		#define SEMACOUNT                       HANDLE
		#define SEMACOUNT_INVALID               NULL
		#define SEMACOUNT_INIT(sem, init, max)  do { \
													(sem) = CreateSemaphore(NULL, (init), (max), NULL); \
												} while(0)
		#define SEMACOUNT_DESTROY(sem)          do { \
													CloseHandle(sem); \
												} while(0)
		#define SEMACOUNT_RELEASE(sem)          do { \
													ReleaseSemaphore((sem), 1, NULL); \
												} while(0)
		#define SEMACOUNT_WAIT(sem, timeout)    (WaitForSingleObject((sem), (timeout)))
		#define IS_VALID_SEMACOUNT(ev)          (ev != SEMACOUNT_INVALID)
		#define SET_SEMACOUNT(target, source)   do { \
													memcpy(&target, &source, sizeof(target));\
												} while (0)
	#endif /*UNDER_RTSS*/
#endif /* WIN32 || _WIN64*/


/*------------------------------------------*/
/* POSIX counting semaphore macros			*/
#ifdef POSIX
    #define DEFINE_SEMACOUNT                        pthread_mutex_t mutex; pthread_cond_t cond; int counter; \
													int error; int max_count; int valid;
	#define SEMACOUNT_INVALID                       NULL
	#define SEMACOUNT                               struct {DEFINE_SEMACOUNT}
	#define SEMACOUNT_INIT(sema,init,max_count_val) do { \
														pthread_cond_init(&(sema).cond, NULL); \
														pthread_mutex_init(&(sema).mutex, NULL); \
														sema.counter = init; \
														sema.max_count = max_count_val; \
														sema.error = 0;\
														sema.valid = 1;\
													} while(0)
	#define SEMACOUNT_DESTROY(sema)                 do { \
														pthread_mutex_lock(&(sema).mutex); \
														sema.error = WAIT_FAILED; \
														pthread_mutex_unlock(&(sema).mutex); \
														pthread_cond_broadcast(&(sema).cond); \
														pthread_cond_destroy(&(sema).cond); \
														pthread_mutex_destroy(&(sema).mutex); \
														sema.valid = 0; \
													} while (0)
	#define SEMACOUNT_RELEASE(sema)                 do { \
														pthread_mutex_lock(&(sema).mutex); \
														if (++((sema).counter) > sema.max_count) \
															sema.counter = sema.max_count; \
														pthread_mutex_unlock(&(sema).mutex); \
														pthread_cond_signal(&(sema).cond); \
													} while(0)
	#define SEMACOUNT_WAIT(sema,timeout)            local_sema_wait(&(sema.mutex), &(sema.cond), &(sema.counter), &(sema.error), timeout)
	#define IS_VALID_SEMACOUNT(sema)                (sema.valid == 1)
	#define SET_SEMACOUNT(target, source)           (memcpy(&target, &source, sizeof(target)))
#endif /*POSIX*/

/*-----------------------------------------------*/
/* VXWORKS_6x counting semaphore macros  */
#if defined VXWORKS_6x
	#define DEFINE_SEMACOUNT                        SEM_ID sema; int valid; int i;
	#define SEMACOUNT                               struct {DEFINE_SEMACOUNT}
	#define SEMACOUNT_INVALID                       NULL
	#define SEMACOUNT_INIT(sem,init,max_count_val)  do { \
														(sem).sema = semCCreate(SEM_Q_PRIORITY, max_count_val); \
														for ((sem).i = max_count_val; (sem).i > init; (sem).i--) \
															semTake((sem).sema, WAIT_FOREVER); \
														(sem).valid = 1; \
													} while(0)
	#define SEMACOUNT_DESTROY(sem)                  do { \
														semDelete(sem.sema); \
													} while (0)
	#define SEMACOUNT_RELEASE(sem)                  do { \
														semGive(sem.sema); \
													} while(0)
	#define SEMACOUNT_WAIT(sem,timeout)             (local_sema_wait(sem.sema, timeout))
	#define IS_VALID_SEMACOUNT(sem)                 (sem.valid == 1)
	#define SET_SEMACOUNT(target, source)           (memcpy(&target, &source, sizeof(target)))
#endif /*VXWORKS_6x*/

/*------------------------------------------*/
/* VXWORKS_5_4 counting semaphore macros	*/
#ifdef VXWORKS_5_4
	#define DEFINE_SEMACOUNT                        SEM_ID sema; int valid; int i;
	#define SEMACOUNT                               struct {DEFINE_SEMACOUNT}
	#define SEMACOUNT_INVALID                       NULL
	#define SEMACOUNT_INIT(sem,init,max_count_val)  do { \
														(sem).sema = semCCreate(SEM_Q_PRIORITY, max_count_val); \
														for ((sem).i = max_count_val; (sem).i > init; (sem).i--) \
															semTake((sem).sema, WAIT_FOREVER); \
														(sem).valid = 1; \
													} while(0)
	#define SEMACOUNT_DESTROY(sem)                  do { \
														semDelete((sem).sema); \
													} while (0)
	#define SEMACOUNT_RELEASE(sem)                  do { \
														semGive((sem).sema); \
													} while(0)
	#define SEMACOUNT_WAIT(sem,timeout)             (local_sema_wait((sem).sema, timeout))
	#define IS_VALID_SEMACOUNT(sem)                 ((sem).valid == 1)
	#define SET_SEMACOUNT(target, source)           (memcpy(&target, &source, sizeof(target)))
#endif /*VXWORKS_5_4*/

/*--------------------------------------*/
/* F_ITRON semacount macros				*/
#ifdef F_ITRON
	#define SEMACOUNT_INVALID       		((FJ_ID)-1)
	#define SEMACOUNT           	    	FJ_ID
	#define SEMACOUNT_INIT(sema,init,max)  	do { \
												sema = lib_fitron_semaphore_create(0, init, max); \
											} while(0)
	#define SEMACOUNT_DESTROY(sema)			do{ \
												lib_fitron_semaphore_destroy(sema); \
											} while (0)
	#define SEMACOUNT_RELEASE(sema)		   	do { \
												lib_fitron_semaphore_release(sema); \
											} while (0)
	#define SEMACOUNT_WAIT(sema,timeout)  	(lib_fitron_semaphore_wait(sema, timeout))
	#define IS_VALID_SEMACOUNT(sema) 	   	(sema != SEMACOUNT_INVALID)
	#define SET_SEMACOUNT(target, source)	do { \
												lib_fitron_semaphore_set(&target, &source); \
											} while (0)
#endif /* F_ITRON */

/*--------------------------------------*/
/* FREERTOS semacount macros			*/
#if defined FREERTOS
	#define SEMACOUNT                               SemaphoreHandle_t
	#define SEMACOUNT_INVALID                       NULL
	int _LIB_EXPORT lib_freeRTOS_semacount_wait(SemaphoreHandle_t mutex, int timeout);
	#define SEMACOUNT_INIT(sem,init,max_count_val)  do { \
														sem = xSemaphoreCreateCounting(max_count_val, init); \
													} while(0)
	#define SEMACOUNT_DESTROY(sem)                  do { \
														vSemaphoreDelete(sem); \
													} while (0)
	#define SEMACOUNT_RELEASE(sem)                  do { \
														xSemaphoreGive(sem); \
													} while(0)
	#define SEMACOUNT_WAIT(sem,timeout)             (lib_freeRTOS_semacount_wait(sem, timeout))
	#define IS_VALID_SEMACOUNT(sem)                 (sem != NULL)
	#define SET_SEMACOUNT(target, source)           (memcpy(&target, &source, sizeof(target)))

#endif /*FREERTOS*/

/*------------------------------------------------------------------------------
 * thread local storage macros
 *-----------------------------------------------------------------------------*/
/*------------------------------------------*/
/* WINDOWS-RTX thread local storage macros	*/
#if defined WIN32 || defined _WIN64
	#ifdef UNDER_RTSS
		#define TLS_ALLOC(idx) ((idx = TlsAlloc()) == 0xFFFFFFFF)
		#define TLS_FREE(idx) (!TlsFree(idx))
		#define TLS_SET_VALUE(idx, val) (!TlsSetValue(idx, val))
		#define TLS_GET_VALUE(idx) (TlsGetValue(idx))

	/*------------------------------------------*/
	/* WINDOWS-W32 thread local storage macros	*/
	#else
		#define TLS_ALLOC(idx) ((idx = TlsAlloc()) == 0xFFFFFFFF)
		#define TLS_FREE(idx) (!TlsFree(idx))
		#define TLS_SET_VALUE(idx, val) (!TlsSetValue(idx, val))
		#define TLS_GET_VALUE(idx) (TlsGetValue(idx))
	#endif /* UNDER_RTSS */
#endif /* WIN32 || _WIN64*/

/*------------------------------------------*/
/* POSIX thread local storage macros		*/
#ifdef POSIX
	#define TLS_ALLOC(idx)          (pthread_key_create((pthread_key_t*)&idx, NULL))
	#define TLS_FREE(idx)           (pthread_key_delete((pthread_key_t)idx))
	#define TLS_SET_VALUE(idx, val) (pthread_setspecific((pthread_key_t)idx, (void*)val))
	#define TLS_GET_VALUE(idx)      (pthread_getspecific((pthread_key_t)idx))
#endif /*POSIX*/

/*--------------------------------------------------*/
/* VXWORKS_6x thread local storage macros	*/
#if defined VXWORKS_6x
	#define TLS_ALLOC(idx) 			(lib_tls_alloc(&idx))
	#define TLS_FREE(idx) 			(lib_tls_free(idx))
	#define TLS_SET_VALUE(idx, val) (lib_tls_set_value(idx,val))
	#define TLS_GET_VALUE(idx) 		(lib_tls_get_value(idx))
#endif /*VXWORKS_6x*/

/*------------------------------------------*/
/* VXWORKS_5_4 thread local storage macros	*/
#ifdef VXWORKS_5_4
	#define TLS_ALLOC(idx)          (lib_tls_alloc(&idx))
	#define TLS_FREE(idx)           (lib_tls_free(idx))
	#define TLS_SET_VALUE(idx, val) (lib_tls_set_value(idx, val))
	#define TLS_GET_VALUE(idx)      (lib_tls_get_value(idx))
#endif /* VXWORKS_5_4 */

/*------------------------------------------*/
/* F_ITRON thread local storage macros		*/
#if defined F_ITRON
	#define TLS_ALLOC(idx) 			(lib_tls_alloc(&idx))
	#define TLS_FREE(idx) 			(lib_tls_free(idx))
	#define TLS_SET_VALUE(idx, val) (lib_tls_set_value(idx,val))
	#define TLS_GET_VALUE(idx) 		(lib_tls_get_value(idx))
#endif /*F_ITRON*/

/*------------------------------------------*/
/* FREERTOS thread local storage macros		*/
#if defined FREERTOS
	#define TLS_ALLOC(idx) 			(lib_tls_alloc(&idx))
	#define TLS_FREE(idx) 			(lib_tls_free(idx))
	#define TLS_SET_VALUE(idx, val) (lib_tls_set_value(idx,val))
	#define TLS_GET_VALUE(idx) 		(lib_tls_get_value(idx))
#endif /*FREERTOS*/
/*------------------------------------------------------------------------------
 * Yield function implementation
 *-----------------------------------------------------------------------------*/
/*------------------------------------------*/
/* WINDOWS-RTX Yield function				*/
#if defined WIN32 || defined _WIN64
	#ifdef UNDER_RTSS
		#define YIELD() do {uSleep(1);} while(0)

	/*------------------------------------------*/
	/* WINDOWS-W32 Yield function				*/
	#else
		#define YIELD() do {Sleep(1);} while(0)
	#endif /* UNDER_RTSS */
#endif /* WIN32 || _WIN64*/

/*------------------------------------------*/
/* POSIX-LINUX Yield function				*/
#if defined POSIX && defined LINUX
	#define YIELD()     (usleep(1))
#endif /*LINUX*/

/*------------------------------------------*/
/* POSIX-QNX6 Yield function */
#if defined POSIX && defined QNX6
	#define YIELD()     (sched_yield())
#endif  /*POSIX-QNX6*/

/*------------------------------------------*/
/* VXWORKS_6x Yield function */
#ifdef VXWORKS_6x
	#define YIELD()     lib_vxworks_6x_yield()
#endif /* VXWORKS_6x */

/*------------------------------------------*/
/* VXWORKS_5_4 Yield function */
#if defined VXWORKS_5_4
	#define YIELD()     (sched_yield())
#endif  /*VXWORKS_5_4*/

/*------------------------------------------*/
/* F_ITRON Yield function */
#ifdef F_ITRON
	#define YIELD() lib_fitron_yield()
#endif /* F_ITRON */

/*------------------------------------------*/
/* FREERTOS Yield function */
#ifdef FREERTOS
	void _LIB_EXPORT lib_freeRTOS_yield();
	#define YIELD() lib_freeRTOS_yield()
#endif /* FREERTOS */

/*------------------------------------------------------------------------------
 * file access functions
 *-----------------------------------------------------------------------------*/
#ifdef F_ITRON
	#define EFILE								FJ_FILE
	#define EFOPEN(name, access)                (lib_fitron_file_open(name, access))
	#define EFCLOSE(pfile) 		                (lib_fitron_file_close(pfile))
	#define EFPRINTF(pfile,fmt,...)             (lib_fitron_file_printf(pfile,fmt, ## __VA_ARGS__))
	#define EVFPRINTF(pfile,fmt,args)           (lib_fitron_file_vprintf(pfile,fmt, args))
	//	#define EFSCANF(pfile,fmt,...)              (lib_fitron_file_scanf(pfile,fmt, ## __VA_ARGS__)) // Not implemented on F_ITRON
	#define EFWRITE(buf,size,count,pfile)       (lib_fitron_file_write(buf,size,count,pfile))
	#define EFREAD(buf,size,count,pfile)        (lib_fitron_file_read(buf,size,count,pfile))
	#define EFSEEK(pfile,offset,origin)  	    (lib_fitron_file_seek(pfile,offset,origin))
	#define EFTELL(pfile) 				 	    (lib_fitron_file_tell(pfile))
	#define EFGETS(buf,nb,pfile)		  	    (lib_fitron_fgets(buf,nb,pfile))
	#define EFEOF(pfile)				  	    (lib_fitron_feof(pfile))
	#define EFERROR(pfile)				  	    (lib_fitron_ferror(pfile))
#elif defined FREERTOS
	#define EFILE								FIL
	#define EFOPEN(name, access)                (lib_file_open(name, access))
	#define EFCLOSE(pfile) 		                (lib_file_close(pfile))
	#define EFPRINTF(pfile,fmt,...)             (lib_file_printf(pfile,fmt, ## __VA_ARGS__))
	#define EVFPRINTF(pfile,fmt,args)           (lib_file_vprintf(pfile,fmt, args))
	#define EFSCANF(pfile,fmt,...)              (lib_file_scanf(pfile,fmt, ## __VA_ARGS__))
	#define EFWRITE(buf,size,count,pfile)       (lib_file_write(buf,size,count,pfile))
	#define EFREAD(buf,size,count,pfile)        (lib_file_read(buf,size,count,pfile))
	#define EFSEEK(pfile,offset,origin)  	    (lib_file_seek(pfile,offset,origin))
	#define EFTELL(pfile) 				 	    (lib_file_tell(pfile))
	#define EFGETS(buf,nb,pfile)		  	    (lib_file_gets(buf,nb,pfile))
	#define EFEOF(pfile)				  	    (lib_file_eof(pfile))
	#define EFERROR(pfile)				  	    (lib_file_error(pfile))
#elif defined VXWORKS_5_4
	#define EFILE								FILE
	#define EFOPEN                              lib_file_open
	#define EFCLOSE      		                lib_file_close
	#define EFPRINTF                            lib_file_printf
	#define EVFPRINTF                           lib_file_vprintf
//	#define EFSCANF(pfile,fmt,...)              (lib_file_scanf(pfile,fmt, ## __VA_ARGS__)) // Not implemented on VXWORKS_5_4
	#define EFWRITE                             lib_file_write
	#define EFREAD                              lib_file_read
	#define EFSEEK                        	    lib_file_seek
	#define EFTELL       				 	    lib_file_tell
	#define EFGETS              		  	    lib_file_gets
	#define EFEOF       				  	    lib_file_eof
	#define EFERROR     				  	    lib_file_error
    #define EFGETPOS                            lib_file_getpos
    #define EFSETPOS                            lib_file_setpos
    #define EFFLUSH     				  	    lib_file_flush
#else	
	#define EFILE								FILE
	#define EFOPEN(name, access)                (lib_file_open(name, access))
	#define EFCLOSE(pfile) 		                (lib_file_close(pfile))
	#define EFPRINTF(pfile,fmt,...)             (lib_file_printf(pfile,fmt, ## __VA_ARGS__))
	#define EVFPRINTF(pfile,fmt,args)           (lib_file_vprintf(pfile,fmt, args))
	#ifndef VXWORKS_6x
	#define EFSCANF(pfile,fmt,...)              (lib_file_scanf(pfile,fmt, ## __VA_ARGS__)) // Not implemented for RTX2012
	#endif	
	#define EFWRITE(buf,size,count,pfile)       (lib_file_write(buf,size,count,pfile))
	#define EFREAD(buf,size,count,pfile)        (lib_file_read(buf,size,count,pfile))
	#define EFSEEK(pfile,offset,origin)  	    (lib_file_seek(pfile,offset,origin))
	#define EFTELL(pfile) 				 	    (lib_file_tell(pfile))
	#define EFGETS(buf,nb,pfile)		  	    (lib_file_gets(buf,nb,pfile))
	#define EFEOF(pfile)				  	    (lib_file_eof(pfile))
	#define EFERROR(pfile)				  	    (lib_file_error(pfile))
    #define EFGETPOS(pfile, pos)                (lib_file_getpos(pfile, pos))
    #define EFSETPOS(pfile, pos)                (lib_file_setpos(pfile, pos))
    #define EFFLUSH(pfile)				  	    (lib_file_flush(pfile))
#endif

/*------------------------------------------------------------------------------
 * fifo macro - put/get/extract a message in/form a first in first out queue.
 * a valid fifo queue is a structure who defines 'first'
 * and 'last' pointer to a message.
 * a valid message is a structure which define a 'next' pointer
 *-----------------------------------------------------------------------------*/
#define FIFO_EXTRACT(queue, msg, lmsg)                                      \
    do {                                                                    \
        if(lmsg) {                                                          \
            if(((msg) = (lmsg)->next))                                      \
                if(!((lmsg)->next = (msg)->next)) (queue).last = lmsg;      \
        } else {                                                            \
            if(((msg) = (queue).first))                                     \
                if(!((queue).first = (msg)->next))(queue).last = NULL;      \
        }                                             \
    } while(0)
#define FIFO_GET(queue, msg)                    \
    do {                                        \
        if(((msg) = (queue).first)) {             \
            if(!((queue).first = (msg)->next))  \
                (queue).last = NULL;            \
            (msg)->next = NULL;                 \
        }                                       \
    } while(0)
#define FIFO_PUT(queue, msg)                          \
    do {                                              \
        (msg)->next = NULL;                           \
        if(!(queue).first) {                          \
            (queue).first = (msg);                    \
            (queue).last = (msg);                     \
        } else {                                      \
            (queue).last->next = (msg);               \
            queue.last = (msg);                       \
        }                                             \
    } while(0)
#define FIFO_INS(queue, msg)                          \
    do {                                              \
        if(!(queue).first) {                          \
            (msg)->next = NULL;                       \
            (queue).first = (msg);                    \
            (queue).last = (msg);                     \
        } else {                                      \
            (msg)->next = (queue).first;              \
            (queue).first = (msg);                    \
        }                                             \
    } while(0)


/*------------------------------------------------------------------------------
 * lifo macro - put/get/extract a message in/form a last in first out queue.
 * a valid lifo queue is a structure who defines 'first' pointer to a message.
 * a valid message is a structure which define a 'next' pointer
 *-----------------------------------------------------------------------------*/
#define LIFO_GET(queue, msg)                          \
    do {                                              \
        if(((msg) = (queue).first)) {                   \
            (queue).first = (msg)->next;              \
            (msg)->next = NULL;                       \
        }                                             \
    } while(0)
#define LIFO_PUT(queue, msg)                          \
    do {                                              \
        (msg)->next = (queue).first;                  \
        (queue).first = (msg);                        \
    } while(0)


/*------------------------------------------------------------------------------
 * diverses MACROS
 *-----------------------------------------------------------------------------*/
#ifndef MIN
	#define MIN(A, B)   ((A) < (B) ? (A) : (B))
	#define MAX(A, B)   ((A) > (B) ? (A) : (B))
#endif	
#define ONEBIT(A)   ((A) && !((A) & ((A)-1)))


/**********************************************************************************************************/
/* functions */
/**********************************************************************************************************/

/*------------------------------------------------------------------------------
 * Math functions
 *-----------------------------------------------------------------------------*/
/*------------------------------------------*/
/* POSIX except VXWORKS_6x Math functions	*/
#if defined POSIX
	#define _isnan(d)   (isnan(d))
	#define _finite(d)  (finite(d))
#endif /*POSIX*/
	
/*------------------------------------------*/
/* VXWORKS_6x Math functions				*/
#ifdef VXWORKS_6x
	#define _isnan(d) ((((dword*)&d)[1] & 0x7FF00000) == 0x7FF00000 && (((dword*)&d)[0] != 0 || (((dword*)&d)[1] & 0x000FFFFF) != 0))
	#define _finite(d) (!(((dword*)&d)[0] == 0 && (((dword*)&d)[1] & 0x7FFFFFFF) == 0x7FF00000) && !_isnan(d))
#endif /*VXWORKS_6x*/

/*------------------------------------------*/
/* VXWORKS_5_4 Math functions					*/
#ifdef VXWORKS_5_4
	#define _isnan(d) ((((dword*)&d)[1] & 0x7FF00000) == 0x7FF00000 && (((dword*)&d)[0] != 0 || (((dword*)&d)[1] & 0x000FFFFF) != 0))
	#define _finite(d) (!(((dword*)&d)[0] == 0 && (((dword*)&d)[1] & 0x7FFFFFFF) == 0x7FF00000) && !_isnan(d))
#endif /*VXWORKS_5_4*/

/*------------------------------------------*/
/* F_ITRON Math functions					*/
#ifdef F_ITRON
	#define _isnan(d)   (isnan(d))
	#define _finite(d)  (finite(d))
#endif /*F_ITRON*/

/*------------------------------------------*/
/* FREERTOS Math functions					*/
#ifdef FREERTOS
	#define _isnan(d)   (isnan(d))
	#define _finite(d)  (isfinite(d))
#endif /*FREERTOS*/

/*------------------------------------------------------------------------------
 * libver.c
 *-----------------------------------------------------------------------------*/
time_t	_LIB_EXPORT lib_get_build_time(void);
dword   _LIB_EXPORT lib_get_version(void);
dword   _LIB_EXPORT lib_get_edi_version(void);

/*------------------------------------------------------------------------------
 * libtim.c
 * Function for time access 
 *-----------------------------------------------------------------------------*/
eint64	_LIB_EXPORT tim_counter(void);
double	_LIB_EXPORT tim_dbl_counter(void);

/*------------------------------------------------------------------------------
 * libdbg.c
 * Function for dbgview 
 * DBGVIEW Not implemented on RTX yet
 * DBGVIEW Not implemented on F_ITRON yet
 * DBGVIEW Not implemented on FREERTOS yet
 * DBGVIEW Not implemented on VXWORKS_5_4 yet
 * DBGVIEW Not implemented on VXWORKS_6x yet
 *-----------------------------------------------------------------------------*/
#if !defined UNDER_RTSS && !defined F_ITRON && !defined FREERTOS && !defined VXWORKS_6x && !defined VXWORKS_5_4
    void    _LIB_EXPORT dbg_init(void);
    void    _LIB_EXPORT dbg_reset(void);
    void    _LIB_EXPORT dbg_set_kind_mask(dword mask);
    dword   _LIB_EXPORT dbg_get_kind_mask(void);
    void    _LIB_EXPORT dbg_set_source_mask(dword mask);
    dword   _LIB_EXPORT dbg_get_source_mask(void);
    void    _LIB_EXPORT dbg_set_stream_mask(dword mask);
    dword   _LIB_EXPORT dbg_get_stream_mask(void);
    int     _LIB_EXPORT dbg_get_entry_size(void);
    int     _LIB_EXPORT dbg_get_entry_number(void);
    int     _LIB_EXPORT dbg_get_entry_count(void);
    void    _LIB_EXPORT dbg_fetch_data(DBG_ENTRY *buffer);
    int     _LIB_EXPORT dbg_fetch_last_data(DBG_ENTRY *buffer, int *entry_count);
    void    _LIB_EXPORT dbg_put_im(int source, const char *fct, const char *msg, ...);
    void    _LIB_EXPORT dbg_put_wm(int source, const char *fct, const char *msg, ...);
    void    _LIB_EXPORT dbg_put_em(int source, const char *fct, const char *msg, int ecode, ...);
    void    _LIB_EXPORT dbg_put_fm(int source, const char *fct, const char *msg, int ecode, ...);
    void    _LIB_EXPORT dbg_put_is(int source, const char *fct, const char *msg, int stream, int protocol, int port, int etcom_size, const byte *etcom_buffer, eint64 mask, dword optional, int pair_event_id, int pair_etcom_size, const byte *pair_etcom_buffer);
    void    _LIB_EXPORT dbg_put_os(int source, const char *fct, const char *msg, int stream, int protocol, int port, int etcom_size, const byte *etcom_buffer, eint64 mask, dword optional, int *event_id);
    void    _LIB_EXPORT dbg_put_buffer_is(int source, const char *fct, const char *msg, int stream, int protocol, int port, int buffer_size, const char* buffer, ...);
    void    _LIB_EXPORT dbg_put_buffer_os(int source, const char *fct, const char *msg, int stream, int protocol, int port, int buffer_size, const char* buffer, ...);
    void    _LIB_EXPORT dbg_put_bf(int source, const char *fct, const char *msg, ...);
    void    _LIB_EXPORT dbg_put_ef(int source, const char *fct, const char *msg, int ecode, ...);
	void	_LIB_EXPORT dbg_set_log_enable(int mode);
	int		_LIB_EXPORT dbg_get_log_enable();
	int		_LIB_EXPORT dbg_get_log_enable_for_stream(int stream);
    void	_LIB_EXPORT dbg_set_device_data(int device_idx, int prod_nb, dword version);
    void	_LIB_EXPORT dbg_get_device_data(int device_idx, int *prod_nb, dword *version);

#endif /*!defined UNDER_RTSS && !defined F_ITRON && !defined FREERTOS && !defined VXWORKS_6x && !defined VXWORKS_5_4*/

#if !defined UNDER_RTSS && !defined F_ITRON && !defined FREERTOS && !defined VXWORKS_6x && !defined VXWORKS_5_4
	#define DBG_PUT_IM(source, fct, msg, ...) dbg_put_im(source, fct,msg, ## __VA_ARGS__)
	#define DBG_PUT_WM(source, fct, msg, ...) dbg_put_wm(source, fct,msg, ## __VA_ARGS__)
	#define DBG_PUT_EM(source, fct, msg, ecode, ...) dbg_put_em(source, fct, msg, ecode, ## __VA_ARGS__)
	#define DBG_PUT_FM(source, fct, msg, ecode, ...) dbg_put_fm(source, fct, msg, ecode, ## __VA_ARGS__)
	#define DBG_PUT_IS(source, fct, msg, stream, protocol, port, etcom_size, etcom_buffer, mask, optional, pair_etcom_id, pair_etcom_size, pair_etcom_buffer) dbg_put_is(source, fct, msg, stream, protocol, port, etcom_size, etcom_buffer, mask, optional, pair_etcom_id, pair_etcom_size, pair_etcom_buffer)
	#define DBG_PUT_OS(source, fct, msg, stream, protocol, port, etcom_size, etcom_buffer, mask, optional, event_id) dbg_put_os(source, fct, msg, stream, protocol, port, etcom_size, etcom_buffer, mask, optional, event_id)
	#define DBG_PUT_BUFFER_IS(source, fct, msg, stream, protocol, port, size, buffer, ...) dbg_put_buffer_is(source, fct, msg, stream, protocol, port, size, buffer, ## __VA_ARGS__)
	#define DBG_PUT_BUFFER_OS(source, fct, msg, stream, protocol, port, size, buffer, ...) dbg_put_buffer_os(source, fct, msg, stream, protocol, port, size, buffer, ## __VA_ARGS__)
	#define DBG_PUT_BF(source, fct, msg, ...) dbg_put_bf(source,fct,msg, ## __VA_ARGS__)
	#define DBG_PUT_EF(source, fct, msg, ecode, ...) dbg_put_ef(source, fct, msg, ecode, ## __VA_ARGS__)
	#define DBG_IS_LOG_ENABLED() dbg_get_log_enable()
	#define DBG_IS_LOG_ENABLED_FOR_STREAM(stream) dbg_get_log_enable_for_stream(stream)
	#define DBG_SET_DEVICE(dev_idx, prod_nb, version) dbg_set_device_data(dev_idx, prod_nb, version)
	#define DBG_GET_DEVICE(dev_idx, prod_nb, version) dbg_get_device_data(dev_idx, prod_nb, version)

#else
	void _LIB_EXPORT nothing(int source, ...);
	#define DBG_PUT_IM nothing
	#define DBG_PUT_WM nothing
	#define DBG_PUT_EM nothing
	#define DBG_PUT_FM nothing
	#define DBG_PUT_IS nothing
	#define DBG_PUT_OS nothing
	#define DBG_PUT_BUFFER_IS nothing
	#define DBG_PUT_BUFFER_OS nothing
	#define DBG_PUT_BF nothing
	#define DBG_PUT_EF nothing
	#define DBG_IS_LOG_ENABLED() FALSE
	#define DBG_IS_LOG_ENABLED_FOR_STREAM(stream) FALSE
	#define DBG_SET_DEVICE nothing
	#define DBG_GET_DEVICE nothing
#endif //!defined UNDER_RTSS && !defined F_ITRON && !defined FREERTOS && !defined VXWORKS_6x && !defined VXWORKS_5_4

/*------------------------------------------------------------------------------
 * libmem.c
 * Base functions to handle memory allocation.
 *-----------------------------------------------------------------------------*/
/*------------------------------------------*/
/* WINDOWS-RTX memory allocation functions	*/
#if defined WIN32 || defined _WIN64
	size_t _LIB_EXPORT                  lib_get_mem_usage();
	#define MALLOC(size)                malloc(size)
	#define CALLOC(num, size)           calloc(num, size)
	#define REALLOC(memblock, size)     realloc(memblock, size)
	#define FREE(membloc)               free(membloc)
#endif /* WIN32 || _WIN64*/


/*---------------------------------------------------*/
/* POSIX || VXWORKS_6x memory allocation functions	 */
#if defined POSIX || defined VXWORKS_6x
	#define MALLOC(size)                malloc(size)
	#define CALLOC(num, size)           calloc(num, size)
	#define REALLOC(memblock, size)     realloc(memblock, size)
	#define FREE(membloc)               free(membloc)
#endif /* defined POSIX || defined VXWORKS_6x*/

/*------------------------------------------*/
/* VXWORKS_5_4 memory allocation functions		*/
#ifdef VXWORKS_5_4
	#undef MALLOC
	#define MALLOC(size)                malloc(size)
	#define CALLOC(num, size)           calloc(num, size)
	#define REALLOC(memblock, size)     realloc(memblock, size)
	#undef FREE
	#define FREE(membloc)               free(membloc)
#endif /* VXWORKS_5_4*/

/*------------------------------------------*/
/* F_ITRON memory allocation functions		*/
#ifdef F_ITRON
    int     _LIB_EXPORT lib_mem_get_edi_heap_info(FJ_ID *head_ID, void** heap_addr, int *heap_size);
//	DBGVIEW not implemented in ITRON because shared memory not available	
//  int     _LIB_EXPORT lib_mem_get_dbgview_shm_info(void** shm_addr, int *shm_size);
    int     _LIB_EXPORT lib_mem_get_etnd_shm_info(void** shm_addr, int *shm_size);
	void* 	_LIB_EXPORT lib_mem_malloc(size_t size);
	void* 	_LIB_EXPORT lib_mem_calloc(size_t num, size_t size);
	void* 	_LIB_EXPORT lib_mem_realloc(void *memblock, size_t size);
	void   	_LIB_EXPORT lib_mem_free(void *memblock);
	#define MALLOC(size)                lib_mem_malloc(size)
	#define CALLOC(num, size)           lib_mem_calloc(num, size)
	#define REALLOC(memblock, size)     lib_mem_realloc(memblock, size)
	#define FREE(membloc)               lib_mem_free(membloc)
#endif /* F_ITRON */

/*---------------------------------------------------*/
/* FREERTOS memory allocation functions	 */
#if defined FREERTOS
	void* _LIB_EXPORT lib_freeRTOS_calloc(int num, int size);
	#define MALLOC(size)                (pvPortMalloc(size))
	#define CALLOC(num, size)           (lib_freeRTOS_calloc(num, size))
	#define REALLOC(memblock, size)     (pvPortRealloc(memblock, size))
	#define FREE(membloc)               vPortFree(membloc)
#endif /* FREERTOS*/

/*------------------------------------------------------------------------------
 * libpro.c
 * All OS except F_ITRON, VXWORKS_5_4, FREERTOS system properties access function
 *-----------------------------------------------------------------------------*/
#if defined WIN32 || defined _WIN64 || defined POSIX || defined VXWORKS_6x
    int		_LIB_EXPORT pro_create(PRO **rpro);
    int     _LIB_EXPORT pro_destroy(PRO **rpro);
    int     _LIB_EXPORT pro_open_f(PRO *pro, char_cp fn);
    int     _LIB_EXPORT pro_open_s(PRO *pro, char_cp host, short port);
    int     _LIB_EXPORT pro_send_sh(PRO *pro, SOCKET sock);
    char_cp _LIB_EXPORT pro_get_next(PRO *pro, char_cp name);
    char_cp _LIB_EXPORT pro_get_string(PRO *pro, char_cp name, char_cp def);
    int     _LIB_EXPORT pro_get_int(PRO *pro, char_cp name, int def);
    long    _LIB_EXPORT pro_get_long(PRO *pro, char_cp name, long def);
    double  _LIB_EXPORT pro_get_double(PRO *pro, char_cp name, double def);
    int     _LIB_EXPORT pro_add_property(PRO *pro, char_cp name);
    int     _LIB_EXPORT pro_add_string(PRO *pro, char_cp name, char_cp str);
    int     _LIB_EXPORT pro_add_int(PRO *pro, char_cp name, int val);
    int     _LIB_EXPORT pro_add_long(PRO *pro, char_cp name, long val);
    int     _LIB_EXPORT pro_add_double(PRO *pro, char_cp name, double val);
    int     _LIB_EXPORT pro_erase(PRO *pro);
    int     _LIB_EXPORT pro_commit(PRO *pro);
#endif /* defined WIN32 || defined _WIN64 || defined POSIX || defined VXWORKS_6x*/


/*------------------------------------------------------------------------------
 * libtls.c
 *-----------------------------------------------------------------------------*/
/*------------------------------*/
/* F_ITRON tls functions		*/
#if defined F_ITRON
	int 	_LIB_EXPORT lib_tls_alloc();
	int 	_LIB_EXPORT lib_tls_free(dword idx);
	int 	_LIB_EXPORT lib_tls_set_value(dword idx, void* val);
	void* 	_LIB_EXPORT lib_tls_get_value(dword idx);
#endif /* F_ITRON */

/*--------------------------------------*/
/* VXWORKS_6x tls functions		*/
#if defined VXWORKS_6x
	int 	_LIB_EXPORT lib_tls_alloc();
	int 	_LIB_EXPORT lib_tls_free(dword idx);
	int 	_LIB_EXPORT lib_tls_set_value(dword idx, void* val);
	void* 	_LIB_EXPORT lib_tls_get_value(dword idx);
#endif /*VXWORKS_6x*/

/*--------------------------------------*/
/* FREERTOS tls functions		*/
#if defined FREERTOS
	int 	_LIB_EXPORT lib_tls_alloc();
	int 	_LIB_EXPORT lib_tls_free(dword idx);
	int 	_LIB_EXPORT lib_tls_set_value(dword idx, void* val);
	void* 	_LIB_EXPORT lib_tls_get_value(dword idx);
#endif /*FREERTOS*/

/*--------------------------------------*/
/* VXWORKS_5_4                          */
#if defined VXWORKS_5_4
	int 	_LIB_EXPORT lib_tls_alloc();
	int 	_LIB_EXPORT lib_tls_free(dword idx);
	int 	_LIB_EXPORT lib_tls_set_value(dword idx, void* val);
	void* 	_LIB_EXPORT lib_tls_get_value(dword idx);
#endif /*VXWORKS_5_4*/

/*------------------------------------------------------------------------------
 * librtx.c
 * Thread and task function
 *-----------------------------------------------------------------------------*/
#if (defined WIN32 || defined _WIN64) && !defined UNDER_RTSS
	THREAD	_LIB_EXPORT rtx_beginthread(int (*fct)(void *param), void *param);
	THREAD  _LIB_EXPORT rtx_begin_named_thread(const char *name, int (*fct)(void *param), void *param);
	char_cp _LIB_EXPORT rtx_get_thread_name(THREAD thread);
#endif /* defined WIN32 || defined _WIN64&& !defined UNDER_RTSS */
#ifdef F_ITRON
	THREAD 	_LIB_EXPORT lib_fitron_task_create(void *fct, void *arg, int prio, char *name);
	void 	_LIB_EXPORT lib_fitron_task_destroy();
	THREAD 	_LIB_EXPORT lib_fitron_get_task_id();
	int		_LIB_EXPORT lib_fitron_task_wait(THREAD thr, int timeout);
#endif /* F_ITRON */

/*------------------------------------------------------------------------------
 * libnet.c
 * TCP/IP access functions
 *-----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/* WINDOWS, POSIX, VXWORKS_6x functions		*/
#if defined WIN32 || defined _WIN64 || defined POSIX || defined VXWORKS_6x || defined VXWORKS_5_4
	int 		_LIB_EXPORT net_init(void);
	int 		_LIB_EXPORT net_recv(SOCKET s, char *buf, int len, int flags);
	int 		_LIB_EXPORT net_recvfrom(SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen);
	int 		_LIB_EXPORT net_send(SOCKET s, char *buf, int len, int flags);
	int 		_LIB_EXPORT net_sendto(SOCKET s, char *buf, int len, int flags, const struct sockaddr *to, int tolen);
	int 		_LIB_EXPORT net_socket(int af, int type, int protocol);
	int 		_LIB_EXPORT net_connect(SOCKET s, const struct sockaddr *name, int namelen);
	int 		_LIB_EXPORT net_close(SOCKET s);
	int 		_LIB_EXPORT net_listen(SOCKET s, int backlog);
	int 		_LIB_EXPORT net_accept(SOCKET s, struct sockaddr *addr, int *addrlen);
	int 		_LIB_EXPORT net_bind(SOCKET s, const struct sockaddr *name, int namelen);
	int 		_LIB_EXPORT net_setsockopt(SOCKET s, int level, int optname, const char *optval, int optlen);
	int 		_LIB_EXPORT net_getsockopt(SOCKET s, int level, int optname, char *optval, int *optlen);
	int 		_LIB_EXPORT net_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout);
	HOSTENT* 	_LIB_EXPORT net_gethostbyname(const char* name);
	int 		_LIB_EXPORT net_gethostname(char* name, int size);
	u_long		_LIB_EXPORT net_inet_addr(const char* cp);
	u_short 	_LIB_EXPORT net_ntohs(u_short netshort);
	u_long  	_LIB_EXPORT net_ntohl(u_long netlong);
	u_short 	_LIB_EXPORT net_htons(u_short hostshort);
	u_long  	_LIB_EXPORT net_htonl(u_long hostlong);
	int 		_LIB_EXPORT net_get_last_error();

	#if defined WIN32 || defined _WIN64
		int _LIB_EXPORT net_wsa_fd_is_set(SOCKET s, fd_set FAR *fd);
		#ifndef UNDER_RTSS
			#define __WSAFDIsSet net_wsa_fd_is_set
		#endif /* UNDER_RTSS */
	#else
		int _LIB_EXPORT net_wsa_fd_is_set(SOCKET s, fd_set *fd);
	#endif /* WIN32 || _WIN64*/
#endif /* defined WIN32 || defined _WIN64 || defined POSIX || defined VXWORKS_6x || defined VXWORKS_5_4*/

#if defined F_ITRON
	int 		_LIB_EXPORT net_init(void);
	int 		_LIB_EXPORT net_recv(SOCKET s, char *buf, int len, int flags);
	int 		_LIB_EXPORT net_send(SOCKET s, char *buf, int len, int flags);
	int 		_LIB_EXPORT net_socket(int af, int type, int protocol);
	int 		_LIB_EXPORT net_connect(SOCKET s, const struct sockaddr *name, int namelen);
	int 		_LIB_EXPORT net_close(SOCKET s);
	int 		_LIB_EXPORT net_listen(SOCKET s, int backlog);
	int 		_LIB_EXPORT net_accept(SOCKET s, struct sockaddr *addr, int *addrlen);
	int 		_LIB_EXPORT net_bind(SOCKET s, const struct sockaddr *name, int namelen);
	int 		_LIB_EXPORT net_setsockopt(SOCKET s, int level, int optname, const char *optval, int optlen);
	int 		_LIB_EXPORT net_getsockopt(SOCKET s, int level, int optname, char *optval, int *optlen);
	int 		_LIB_EXPORT net_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout);
	u_short 	_LIB_EXPORT net_ntohs(u_short netshort);
	u_long  	_LIB_EXPORT net_ntohl(u_long netlong);
	u_short 	_LIB_EXPORT net_htons(u_short hostshort);
	u_long  	_LIB_EXPORT net_htonl(u_long hostlong);

//  Not implemented on F_ITRON
//	int			_LIB_EXPORT net_get_last_error();
//	u_long		_LIB_EXPORT net_inet_addr(const char* cp);
//	HOSTENT* 	_LIB_EXPORT net_gethostbyname(const char* name);
//	int 		_LIB_EXPORT net_gethostname(char* name, int size);
#endif /* defined F_ITRON */

/*------------------------------------------------------------------*/
/* FREERTOS functions		*/
#if defined FREERTOS
	int 		_LIB_EXPORT net_init(char *ip_addr, char *net_mask, char *gateway, unsigned char mac_addr[6]);
	int 		_LIB_EXPORT net_recv(SOCKET s, char *buf, int len, int flags);
	int 		_LIB_EXPORT net_recvfrom(SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen);
	int 		_LIB_EXPORT net_send(SOCKET s, char *buf, int len, int flags);
	int 		_LIB_EXPORT net_sendto(SOCKET s, char *buf, int len, int flags, const struct sockaddr *to, int tolen);
	int 		_LIB_EXPORT net_socket(int af, int type, int protocol);
	int 		_LIB_EXPORT net_connect(SOCKET s, const struct sockaddr *name, int namelen);
	int 		_LIB_EXPORT net_close(SOCKET s);
	int 		_LIB_EXPORT net_listen(SOCKET s, int backlog);
	int 		_LIB_EXPORT net_accept(SOCKET s, struct sockaddr *addr, int *addrlen);
	int 		_LIB_EXPORT net_bind(SOCKET s, const struct sockaddr *name, int namelen);
	int 		_LIB_EXPORT net_setsockopt(SOCKET s, int level, int optname, const char *optval, int optlen);
	int 		_LIB_EXPORT net_getsockopt(SOCKET s, int level, int optname, char *optval, int *optlen);
	int 		_LIB_EXPORT net_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout);
	u_long		_LIB_EXPORT net_inet_addr(const char* cp);
	u_short 	_LIB_EXPORT net_ntohs(u_short netshort);
	u_long  	_LIB_EXPORT net_ntohl(u_long netlong);
	u_short 	_LIB_EXPORT net_htons(u_short hostshort);
	u_long  	_LIB_EXPORT net_htonl(u_long hostlong);
	int 		_LIB_EXPORT net_wsa_fd_is_set(SOCKET s, fd_set *fd);
	int			_LIB_EXPORT net_get_last_error();

#endif /* FREERTOS*/


/*------------------------------------------------------------------------------
 * libjni.c
 * Windows-W32 java JNI access function
 *-----------------------------------------------------------------------------*/
#if (defined WIN32 || defined _WIN64) && !defined UNDER_RTSS
	void    	_LIB_EXPORT jni_set_vm(jnivm_p);
	jnivm_p 	_LIB_EXPORT jni_get_vm(void);
	jnienv_p	_LIB_EXPORT jni_get_env(void);
	void    	_LIB_EXPORT jni_detach(void);
	jobject 	_LIB_EXPORT jni_create_object(jnienv_p env, const char *name, const char *sig, ...);
	void    	_LIB_EXPORT jni_out_of_memory_error(jnienv_p env, const char *s);
	void    	_LIB_EXPORT jni_illegal_argument_exception(jnienv_p env, const char *s);
	void    	_LIB_EXPORT jni_illegal_state_exception(jnienv_p env, const char *s);
#endif /* (defined WIN32 || defined _WIN64) && !defined UNDER_RTSS */

/*------------------------------------------------------------------------------
 * libshm.c
 *------------------------------------------------------------------------------*/
#if !defined FREERTOS && !defined VXWORKS_5_4
	int _LIB_EXPORT shm_exist(char *name, void *addr, int size);
	int _LIB_EXPORT shm_create(SHM **shm_obj, char *name, void *addr, dword size);
	int _LIB_EXPORT shm_destroy(SHM *shm_obj);
	int _LIB_EXPORT shm_map(SHM *shm_obj, void **addr);
	int _LIB_EXPORT shm_unmap(SHM *shm_obj, void *addr);
#endif /*!defined FREERTOS && !defined VXWORKS_5_4*/

/*------------------------------------------------------------------------------
 * libprio.c
 * Priority setting functions
 *------------------------------------------------------------------------------*/
/*------------------------------------------*/
/* POSIX priority setting functions			*/
#if defined POSIX
	int _LIB_EXPORT thread_get_prio(pthread_t thr);
	void _LIB_EXPORT thread_set_prio(pthread_t thr, int prio);
	void _LIB_EXPORT thread_set_policy(pthread_t thr, int policy, int prio);
#endif /* POSIX */

/*------------------------------------------*/
/* VXWORKS_5_4 priority setting functions	*/
#ifdef VXWORKS_5_4
	int _LIB_EXPORT task_get_prio(int task);
	void _LIB_EXPORT task_set_prio(int task, int prio);
#endif /*VXWORKS_5_4*/


/*------------------------------------------*/
/* VXWORKS_6x priority setting functions	*/
#ifdef VXWORKS_6x
	int _LIB_EXPORT task_get_prio(int task);
	void _LIB_EXPORT task_set_prio(int task, int prio);
#endif /*VXWORKS_6x*/

/*------------------------------------------------------------------------------
 * libwait.c
 * Waiting synchronisation object functions
 *------------------------------------------------------------------------------*/
 /*------------------------------------------*/
/* POSIX synchronisation object functions	*/
#if defined POSIX
	int 	_LIB_EXPORT event_wait (pthread_mutex_t *mutex, pthread_cond_t *cond, int *state, int *error, int timeout);
	int 	_LIB_EXPORT autoevent_wait (pthread_mutex_t *mutex, pthread_cond_t *cond, int *state, int *error, int timeout);
	int 	_LIB_EXPORT mutex_wait (pthread_mutex_t *mutex, pthread_cond_t *cond, THREAD *tid, int *counter, int *error, int timeout);
	int 	_LIB_EXPORT local_sema_wait (pthread_mutex_t *mutex, pthread_cond_t *cond, int *counter, int *error, int timeout);
	void 	_LIB_EXPORT lib_vxworks_6x_yield();
#endif /* POSIX */

#ifdef UNDER_RTSS
	int		_LIB_EXPORT lib_wait_for_thread (THREAD thr, int timeout);
	void 	_LIB_EXPORT uSleep(long us);
#endif /*UNDER_RTSS*/

/*------------------------------------------*/
/* F_ITRON synchronisation object functions	*/
#ifdef F_ITRON
	int _LIB_EXPORT lib_fitron_edi_init(FJ_ID firstLockIndex,  dword nbLock,
										FJ_ID firstEventIndex, dword nbEvent,
										FJ_ID firstSemaIndex,  dword nbSema);
	int _LIB_EXPORT lib_fitron_edi_exit();
	/* CRITICAL SECTION function */
	void 	_LIB_EXPORT lib_fitron_critical_create(FJ_ID csID, FJ_ID *cs, int *counter, FJ_ID *pid);
	void 	_LIB_EXPORT lib_fitron_critical_destroy(FJ_ID *cs);
	void 	_LIB_EXPORT lib_fitron_critical_enter(FJ_ID *cs, int *counter, FJ_ID *pid);
	void 	_LIB_EXPORT lib_fitron_critical_leave(FJ_ID *cs, int *counter, FJ_ID *pid);

	/* EVENT functions */
	FJ_ID	_LIB_EXPORT lib_fitron_event_create(FJ_ID ev_id, ebool init);
	void 	_LIB_EXPORT lib_fitron_event_destroy(FJ_ID ev_id);
	void 	_LIB_EXPORT lib_fitron_event_set(FJ_ID ev_id);
	void 	_LIB_EXPORT lib_fitron_event_reset(FJ_ID ev_id);
	int 	_LIB_EXPORT lib_fitron_event_wait(FJ_ID ev_id, int timeout);

	/* AUTOEVENT functions */
	FJ_ID 	_LIB_EXPORT lib_fitron_autoevent_create(FJ_ID ev_id, ebool init);
	void 	_LIB_EXPORT lib_fitron_autoevent_destroy(FJ_ID ev_id);
	void 	_LIB_EXPORT lib_fitron_autoevent_set(FJ_ID ev_id);
	void 	_LIB_EXPORT lib_fitron_autoevent_reset(FJ_ID ev_id);
	int 	_LIB_EXPORT lib_fitron_autoevent_wait(FJ_ID ev_id, int timeout);
	
	/* MUTEX functions */
	void 	_LIB_EXPORT lib_fitron_mutex_create(FJ_ID csID, FJ_ID *cs, int *counter, FJ_ID *pid, int init);
	void 	_LIB_EXPORT lib_fitron_mutex_destroy(FJ_ID *cs);
	void 	_LIB_EXPORT lib_fitron_mutex_release(FJ_ID *cs, int *counter, FJ_ID *pid);
	int 	_LIB_EXPORT lib_fitron_mutex_wait(FJ_ID *cs, int *counter, FJ_ID *pid, int timeout);

	/* SEMACOUNT functions */
	FJ_ID 	_LIB_EXPORT lib_fitron_semaphore_create(FJ_ID semaID, dword init, dword max);
	void 	_LIB_EXPORT lib_fitron_semaphore_destroy(FJ_ID semaID);
	void 	_LIB_EXPORT lib_fitron_semaphore_release(FJ_ID semaID);
	int 	_LIB_EXPORT lib_fitron_semaphore_wait(FJ_ID semaID, int timeout);
	void 	_LIB_EXPORT lib_fitron_semaphore_set(FJ_ID *target, FJ_ID *source);

	/* DELAY functions */
	void 	_LIB_EXPORT lib_fitron_yield();
#endif /*F_ITRON*/

/*-----------------------------------------------------*/
/* VXWORKS_6x synchronisation object functions	*/
#if defined VXWORKS_6x
	int _LIB_EXPORT task_wait(int thr, int timeout);
	int _LIB_EXPORT task_get_prio(int task);
	void _LIB_EXPORT task_set_prio(int task, int prio);

	int _LIB_EXPORT event_wait(SEM_ID event, int timeout);
	int _LIB_EXPORT autoevent_wait(SEM_ID event, int timeout);
	int _LIB_EXPORT mutex_wait(SEM_ID event, int timeout);
	int _LIB_EXPORT local_sema_wait_old(SEM_ID mutex, SEM_ID ev, int *counter, int timeout);
	int _LIB_EXPORT local_sema_wait(SEM_ID SEM, int timeout);
	int _LIB_EXPORT critical_enter(SEM_ID event);
	void 	_LIB_EXPORT lib_vxworks_6x_yield();
#endif /*VXWORKS_6x */

/*------------------------------------------------------*/
/* VXWORKS_5_4 Waiting synchronisation object functions	*/
#ifdef VXWORKS_5_4
	int _LIB_EXPORT event_wait(SEM_ID event, int timeout);
	int _LIB_EXPORT autoevent_wait(SEM_ID event, int timeout);
	int _LIB_EXPORT mutex_wait(SEM_ID event, int timeout);
	int _LIB_EXPORT local_sema_wait_old(SEM_ID mutex, SEM_ID ev, int *counter, int timeout);
	int _LIB_EXPORT local_sema_wait(SEM_ID SEM, int timeout);
	int _LIB_EXPORT critical_enter(SEM_ID event);
#endif /*VXWORKS_5_4 */


/*------------------------------------------------------------------------------
 * libzip.c
 * zip/unzip file function (Windows, Posix)
 * unzip from memory to memory (All OS)
 *------------------------------------------------------------------------------*/
int _LIB_EXPORT zip_unzip_buffer(size_t src_size, char *src_buf, size_t dest_size, byte *dest_buf, size_t *unzipped_size);
int _LIB_EXPORT zip_unzip_multiple_file_buffer(size_t src_size, char *src_buf, char *sub_file_name, size_t dest_size, byte *dest_buf, size_t *unzipped_size);
#if !defined NO_DIRECTORY_MANAGEMENT
	int _LIB_EXPORT zip_fw_unzip(char *zipFile, char *extractDir);
	int _LIB_EXPORT zip_fw_get_manifest (char *extractDir, FW_MANIFEST *fw_manifest);
	int _LIB_EXPORT zip_unzip(char *zipFile, char *extractDir, char *flags);
	int _LIB_EXPORT zip_zip(char *zipFile, char *dir, char *flags);
#endif //!defined NO_DIRECTORY_MANAGEMENT

/*------------------------------------------------------------------------------
 * libdir.c (directory search)
 * All OS except RTX & VXWORKS_5_4
 *------------------------------------------------------------------------------*/
#if !defined NO_DIRECTORY_MANAGEMENT
	int _LIB_EXPORT lib_execute_system_cmd(char *cmd);
	int _LIB_EXPORT dir_find_first_file (char *dirName, DIRECTORY_ENTRY *entry);
	int _LIB_EXPORT dir_find_next_file (DIRECTORY_ENTRY *entry);
	int _LIB_EXPORT dir_find_abort();
	int _LIB_EXPORT dir_remove(char *dirName);
	int _LIB_EXPORT dir_mkdir(char *dir_name);
	int _LIB_EXPORT dir_chdir(char *dir_name);
	int _LIB_EXPORT dir_getdir(char *dir_name, int size);
	int _LIB_EXPORT dir_dir_copy(char *srcDirName, char *destDirName);
#endif //!defined NO_DIRECTORY_MANAGEMENT
#if !defined NO_FILE_SYSTEM
	char* _LIB_EXPORT dir_tmpname();
	int _LIB_EXPORT dir_access(char *dirName, int mode);
	int _LIB_EXPORT dir_file_copy(char *srcFileName, char *destFileName);
	int _LIB_EXPORT dir_file_remove(char *fileName);
#endif //!defined NO_FILE_SYSTEM

/*------------------------------------------------------------------------------
 * libdir.c (dll access)
 * Windows only
 *------------------------------------------------------------------------------*/
#if defined FWPOOL_FIND
	int _LIB_EXPORT dir_get_dll_path(char *dll_name, int size, char *dll_path);
	ebool _LIB_EXPORT lib_is_64bits();
	ebool _LIB_EXPORT lib_is_OS_biggerEqual_Vista();
#endif

/*------------------------------------------------------------------------------
 * liberr.c
 * Error generation and display functions
 *------------------------------------------------------------------------------*/
#define CREATE_EDI_ERROR(fctName,errorNr,errorText,comment,axisMask,recNr,cmd,timeout) \
    do {\
        lib_create_error(__FILE__, __LINE__, fctName,errorNr,errorText,comment,axisMask,recNr,cmd,timeout); \
    } while(0)
#define SET_EDI_ERROR_PARAM(axisMask,recNr,cmd,timeout) \
    do {\
        lib_set_error_param(axisMask,recNr,cmd,timeout); \
    } while(0)
#define ADD_EDI_TRACE(fctName,comment) \
    do {\
        lib_add_error_trace(__FILE__, __LINE__, fctName,comment); \
    } while(0)
#define TRACE_OFF() \
    do {\
        lib_trace_off(); \
    } while(0)
#define TRACE_ON() \
    do {\
        lib_trace_on(); \
    } while(0)
#define CREATE_OR_ADD_EDI_ERROR(fctName,errorNr,errorText,comment,axisMask,recNr,cmd,timeout) \
    do {\
        lib_create_or_add_error(__FILE__, __LINE__, fctName,errorNr,errorText,comment,axisMask,recNr,cmd,timeout); \
    } while(0)

int 	_LIB_EXPORT lib_create_error(	char *file,
										int line,
										const char *fctName,
										int errorNr,
										const char *errorText,
										char *comment,
										eint64 axisMask,
										int recNr,
										int cmd,
										int timeout);
int 	_LIB_EXPORT lib_set_error_param(eint64 axisMask,
										int recNr,
										int cmd,
										int timeout);
int 	_LIB_EXPORT lib_add_error_trace(char *file,
										int line,
										const char *fctName,
										char *comment);
int 	_LIB_EXPORT lib_create_or_add_error(char* file,
											int line,
											const char* fctName,
											int errorNr,
											const char* errorText,
											char* comment,
											eint64 axisMask,
											int recNr,
											int cmd,
											int timeout);
int 	_LIB_EXPORT lib_trace_off();
int 	_LIB_EXPORT lib_trace_on();
int 	_LIB_EXPORT lib_get_edi_error_command();
eint64 	_LIB_EXPORT lib_get_edi_error_axismask();
int 	_LIB_EXPORT lib_get_edi_error_record();
int 	_LIB_EXPORT lib_get_edi_error_timeout();
char* 	_LIB_EXPORT lib_get_edi_error_text(int size, char *str);
char *	_LIB_EXPORT lib_get_edi_small_error_text(int size, char *str);
char_cp _LIB_EXPORT lib_translate_edi_error(int code);


/*------------------------------------------------------------------------------
 * liblog.c
 * Logging function (replace printf,sprintf, etc which are not advised on some OS)
 *------------------------------------------------------------------------------*/
int _LIB_EXPORT vsedilog(char *string, const char *format, va_list vargs);
int _LIB_EXPORT sedilog(char *string, const char *format, ...);
int _LIB_EXPORT edilog(const char *format, ...);
int _LIB_EXPORT vedilog(const char *format, va_list vargs);

/*------------------------------------------------------------------------------
 * libstr.c
 * String uppercase function which are not standard
 *------------------------------------------------------------------------------*/
char* _LIB_EXPORT strToUpper(char *src);
char* _LIB_EXPORT strToLower(char *src);
int _LIB_EXPORT strCaseCmp(const char *src1, const char *src2);
int _LIB_EXPORT strnCaseCmp(const char *src1, const char *src2, size_t i);
int   _LIB_EXPORT lib_string_to_version(char *buffer, dword *version);
void  _LIB_EXPORT lib_version_to_string(char *buffer, int max, dword version);
#ifdef VXWORKS_6x
	unsigned long long _LIB_EXPORT strtoull(const char *nptr, char **endptr, int base);
	long long int _LIB_EXPORT strtoll(const char *nptr, char **endptr, int base);
#endif //VXWORKS_6x
#ifdef VXWORKS_5_4
	unsigned long long _LIB_EXPORT strtoull(const char *nptr, char **endptr, int base);
	long long int _LIB_EXPORT strtoll(const char *nptr, char **endptr, int base);
#endif //VXWORKS_5_4
/*------------------------------------------------------------------------------
 * libfile.c
 * File access functions
 *------------------------------------------------------------------------------*/
#ifdef F_ITRON
	EFILE* 	_LIB_EXPORT lib_fitron_file_open(char *name, char *access);
	int     _LIB_EXPORT lib_fitron_file_close(EFILE *pfile);
	int     _LIB_EXPORT lib_fitron_file_printf(EFILE *pfile, char *fmt, ...);
	int 	_LIB_EXPORT lib_fitron_file_vprintf(EFILE *pfile, const char *fmt, va_list args);
//	int      lib_fitron_file_scanf(EFILE *pfile, char *fmt, ...);
	size_t  _LIB_EXPORT lib_fitron_file_write(const void *buf, size_t size, size_t count, EFILE *pfile);
	size_t  _LIB_EXPORT lib_fitron_file_read(const void *buf, size_t size, size_t count, EFILE *pfile);
	int		_LIB_EXPORT lib_fitron_file_seek(EFILE *pfile, long offset, int origin);
	long	_LIB_EXPORT lib_fitron_file_tell(EFILE *pfile);
	char* 	_LIB_EXPORT lib_fitron_fgets(char *s, int n, EFILE *pfile);
	int 	_LIB_EXPORT lib_fitron_feof(EFILE *pfile);
	int 	_LIB_EXPORT lib_fitron_ferror(EFILE *pfile);
#else
	EFILE* 	_LIB_EXPORT lib_file_open(char *name, char *access);
	int     _LIB_EXPORT lib_file_close(EFILE *pfile);
	int     _LIB_EXPORT lib_file_printf(EFILE *pfile, char *fmt, ...);
	int 	_LIB_EXPORT lib_file_vprintf(EFILE *pfile, const char *fmt, va_list args);
	char* 	_LIB_EXPORT lib_file_gets(char *s, int n, EFILE *pfile);
	int		_LIB_EXPORT lib_file_scanf(EFILE *pfile, char *fmt, ...);
	size_t  _LIB_EXPORT lib_file_write(const void *buf, size_t size, size_t count, EFILE *pfile);
	size_t  _LIB_EXPORT lib_file_read(const void *buf, size_t size, size_t count, EFILE *pfile);
	int		_LIB_EXPORT lib_file_seek(EFILE *pfile, long offset, int origin);
	long	_LIB_EXPORT lib_file_tell(EFILE *pfile);
	int 	_LIB_EXPORT lib_file_eof(EFILE *pfile);
	int 	_LIB_EXPORT lib_file_error(EFILE *pfile);
    int     _LIB_EXPORT lib_file_getpos(EFILE *pfile, fpos_t *pos);
    int     _LIB_EXPORT lib_file_setpos(EFILE *pfile, fpos_t *pos);
    int 	_LIB_EXPORT lib_file_flush(EFILE *pfile);
#endif /*F_ITRON*/

/*------------------------------------------------------------------------------
 * libmd5.c
 *------------------------------------------------------------------------------*/
#define MD5_DIGEST_SIZE 4
typedef uint32_t MD5_DIGEST[MD5_DIGEST_SIZE];
void _LIB_EXPORT lib_md5_create(MD5_CONTEXT **ctx);
void _LIB_EXPORT lib_md5_init(MD5_CONTEXT *ctx);
void _LIB_EXPORT lib_md5_update(MD5_CONTEXT *ctx, unsigned char const *buf, unsigned len);
void _LIB_EXPORT lib_md5_final(MD5_CONTEXT *ctx, MD5_DIGEST digest);
void _LIB_EXPORT lib_md5_destroy(MD5_CONTEXT **ctx);
ebool _LIB_EXPORT lib_md5_digest_equal(MD5_DIGEST digest1, MD5_DIGEST digest2);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* _LIB40_H */
