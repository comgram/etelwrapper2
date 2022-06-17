/*
 * etne40.h
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
 * This header file contains public declaration of etne library.\n
 * This library allows enabling access to already connected hardware for other process
 * This library in conformed to POSIX 1003.1c
 * @file etne40.h
 */

#ifndef _ETNE40_H
#define _ETNE40_H

/**********************************************************************************************************/
/*** Libraries */
/**********************************************************************************************************/

/**********************************************************************************************************/
/*** Literals */
/**********************************************************************************************************/
#ifdef __cplusplus
	#ifdef ETEL_OO_API		/* defined by the user when he need the Object Oriented interface */
		#define ETNE_OO_API
	#endif
#endif //__cplusplus

#ifdef __cplusplus
	extern "C" {
#endif //__cplusplus

/**
 * @defgroup ETNEAll ETNE All functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup ETNEErrorsAndWarnings ETNE Errors and warnings
 */
/*@{*/
/*@}*/

/**
 * @defgroup ETNEVersion ETNE Library version access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup ETNELibrary ETNE Connection enabling functions
 */
/*@{*/
/*@}*/

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
	/* VXWORKS_6x Byte order			*/
	#ifdef VXWORKS_6x
		#define __LITTLE_ENDIAN _LITTLE_ENDIAN
		#define __BIG_ENDIAN _BIG_ENDIAN
		#define __BYTE_ORDER _BYTE_ORDER
	#endif /*VXWORKS_6x*/

	/*------------------------------*/
	/* F_ITRON Byte order			*/
	#if defined F_ITRON
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#define __BYTE_ORDER __LITTLE_ENDIAN			
	#endif /*F_ITRON*/

#endif /*BYTE_ORDER*/


/*
 * error codes - c
 */
#ifndef ETNE_OO_API
 
#define ETNE_EPORTMAX                     -704        /**< maximal number of user port exceeded */
#define ETNE_EPRESENT                     -703        /**< an etne or a custom application is already running for this url */
#define ETNE_EINTERNAL                    -702        /**< some internal error in the etel software */
#define ETNE_ESYSTEM                      -701        /**< some system resource return an error */
#define ETNE_EBADPARAM                    -700        /**< one of the parameter is not valid */


#endif /* ETNE_OO_API */

/*
 * type modifiers
 */
#if defined WIN32 || defined _WIN64
	#define _ETNE_EXPORT __cdecl                         /* function exported by static library */
	#define ETNE_CALLBACK __cdecl                         /* client callback function called by library */
#endif /* WIN32 || _WIN64*/

#ifdef QNX4
	#define _ETNE_EXPORT __cdecl                          /* function exported by library */
	#define ETNE_CALLBACK __cdecl                         /* client callback function called by library */
#endif /* QNX4 */

#ifdef POSIX
	#define _ETNE_EXPORT									/* function exported by library */
	#define ETNE_CALLBACK                                   /* client callback function called by library */
#endif /* POSIX */

#ifdef F_ITRON
	#define _ETNE_EXPORT __cdecl                         /* function exported by static library */
	#define ETNE_CALLBACK __cdecl                         /* client callback function called by library */
#endif /* F_ITRON */

#ifdef VXWORKS_6x
	#define _ETNE_EXPORT									/* function exported by library */
	#define ETNE_CALLBACK                                   /* client callback function called by library */
#endif /* VXWORKS_6x */

/*
 * hidden structure for library clients
 */
#ifndef ETB
	#define ETB void
	struct EtbBus { ETB *etb; };
#endif

#ifndef STA
	#define STA void
#endif

/**********************************************************************************************************/
/*** Types */
/**********************************************************************************************************/

/*********************************************************************************************************/
/*** ETN exported functions */
/*********************************************************************************************************/

/*------------------------------------------------------------------------------
 * ETNE Connection handler type
 *-----------------------------------------------------------------------------*/
/*Type of callback function called when host connection of custom port is open or closed*/
typedef void (ETNE_CALLBACK *ETNE_CUSTOM_CONNECT_HANDLER)(ETB *etb, int custom_accept_port, ebool connect, void *user);

/**
 * @addtogroup ETNEAll
 * @{
 */
int _ETNE_EXPORT etne_start_custom(ETB *bus, int custom_accept_port, char *name);
int _ETNE_EXPORT etne_stop_custom(ETB *bus, int custom_accept_port);
int _ETNE_EXPORT etne_is_custom_connected(ETB *bus, int custom_accept_port, ebool *connected);
int _ETNE_EXPORT etne_add_custom_connect_handler(ETB *bus, int custom_accept_port, ETNE_CUSTOM_CONNECT_HANDLER handler, void *user);
int _ETNE_EXPORT etne_remove_custom_connect_handler(ETB *bus, int custom_accept_port);

char_cp _ETNE_EXPORT etne_translate_error (int code);
dword	_ETNE_EXPORT etne_get_version();
dword	_ETNE_EXPORT etne_get_edi_version();
time_t	_ETNE_EXPORT etne_get_build_time();
/** @}*/

void _ETNE_EXPORT etne_connection_clear(int sta_port_idx);
void _ETNE_EXPORT etne_connection_destroy(int sta_port_idx);
int _ETNE_EXPORT etne_connection_init(int sta_port_idx,
                                      ETB *bus,
									  char *name,
									  char *url,
									  int flags,
									  char *baudrate,
									  char *accept_driver_name,
									  int accept_driver_port,
									  int port_properties_driver_port,
									  STA *sta);
int _ETNE_EXPORT etne_accept_destroy(int port_idx);
int _ETNE_EXPORT etne_accept_init(int port_idx, char *accept_driver_name, int accept_driver_port);
#if !defined F_ITRON && !defined VXWORKS_6x
	int _ETNE_EXPORT etne_connection_spy(int sta_port_idx);
	int _ETNE_EXPORT etne_wait_event(int port_idx, int timeout);
#endif//F_ITRON



#ifdef __cplusplus
} /* extern "C" */
#endif


/*
 * ETNE constants - c++
 */
#ifdef ETNE_OO_API

/*
 * ETNE exception - c++
 */
class EtneException {
	friend class Etne;

	/* error codes - c++ */
public:
    enum {EBADPARAM = -700 };                       /* one of the parameter is not valid */
    enum {EINTERNAL = -702 };                       /* some internal error in the etel software */
    enum {EPORTMAX = -704 };                        /* maximal number of user port exceeded */
    enum {EPRESENT = -703 };                        /* an etne or a custom application is already running for this url */
    enum {ESYSTEM = -701 };                         /* some system resource return an error */


	/* exception code */
private:
    int code;

	/* constructor */
protected:
    EtneException(int e) { code = e; };

	/* translate error code */
public:
    static char_cp translate(int code) {
        return etne_translate_error(code);
    }

    /* get error description */
public:
    int getCode() {
		return code;
	}
    const char *getText() {
		return translate(code);
	}
};

/*
* generate exceptions from error codes
*/
#define ERRCHK(a) do { int _err = (a); if(_err) throw EtneException(_err); } while(0)

/**
 * Etne class - C++
 */
class Etne {
	/* constructors */
public:
	Etne(void) {
	}
public:
	/* functions */
    void startCustom(EtbBus etb, int custom_accept_port, char *name) {
        ERRCHK(etne_start_custom(*(ETB **)&etb, custom_accept_port, name));
	}
    void stopCustom(EtbBus etb, int custom_accept_port) {
        ERRCHK(etne_stop_custom(*(ETB **)&etb, custom_accept_port));
	}
    ebool isCustomConnected(EtbBus etb, int custom_accept_port) {
        ebool connected = FALSE;
        ERRCHK(etne_is_custom_connected(*(ETB **)&etb, custom_accept_port, &connected));
        return connected;
	}
    void addCustomConnectHandler(EtbBus etb, int custom_accept_port, ETNE_CUSTOM_CONNECT_HANDLER handler, void *user) {
        ERRCHK(etne_add_custom_connect_handler(*(ETB **)&etb, custom_accept_port, handler, user));
    }
    void removeCustomConnectHandler(EtbBus etb, int custom_accept_port) {
        ERRCHK(etne_remove_custom_connect_handler(*(ETB **)&etb, custom_accept_port));
    }
	static dword getVersion() {
		return etne_get_version();
	}
	static dword getEdiVersion() {
		return etne_get_edi_version();
	}
	static time_t getBuildTime() {
		return etne_get_build_time();
	}
};

#undef ERRCHK
#endif /* ETNE_OO_API */


#endif /* _ETNE40_H */
