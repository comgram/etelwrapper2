/*
 * ekd40.h
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
* this header file contains public declaration for etel kernel driver interface
* @library ekd40
*/

#ifndef _EKD40_H_
#define _EKD40_H_

/**********************************************************************************************************/
/*** Libraries */
/**********************************************************************************************************/

/*-----------------------*/
/* Common libraries*/
#include "lib40.h"

/**********************************************************************************************************/
/*** Literals */
/**********************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*------------------------------------------------------------------------------
 * type modifiers
 *-----------------------------------------------------------------------------*/
/*--------------------------------------*/
/* WINDOWS type modifiers				*/
#if defined WIN32 || defined _WIN64
	#define _EKD_EXPORT __cdecl                          /* function exported by the library */
	#define EKD_CALLBACK __cdecl                         /* client callback function called by library */
#endif /* WIN32 || _WIN64*/

/*--------------------------------------*/
/* POSIX-LINUX type modifiers			*/
#if defined POSIX && defined LINUX
	#define HANDLE int
	#define _EKD_EXPORT                           /* function exported by library */
	#define EKD_CALLBACK                          /* client callback function called by library */
#endif /* LINUX */

/*--------------------------------------*/
/* POSIX-QNX6 type modifiers			*/
#if defined POSIX && defined QNX6
	#define HANDLE int
	#define _EKD_EXPORT                           /* function exported by library */
	#define EKD_CALLBACK                          /* client callback function called by library */
#endif /* QNX6 */
/*------------------------------*/
 /* VXWORKS_6x 					*/
 #ifdef VXWORKS_6x
 	#define _EKD_EXPORT
 	#define EKD_CALLBACK		                 /* client callback function called by library */
 #endif /*VXWORKS_6x*/
/*------------------------------*/
/* F_ITRON type modifiers		*/
#ifdef F_ITRON
	#define _EKD_EXPORT __cdecl
	#define EKD_CALLBACK __cdecl                  /* client callback function called by library */
#endif /*F_ITRON*/
 /*------------------------------*/
 /* FREERTOS type modifiers		*/
 #ifdef FREERTOS
 	#define _EKD_EXPORT
 	#define EKD_CALLBACK		                 /* client callback function called by library */
 #endif /*F_ITRON*/

/*------------------------------------------------------------------------------
 * hidden structures for library clients
 *-----------------------------------------------------------------------------*/
#ifndef _EKD_LIB
	#define EKD_IRQ_SEMAPHORE void
	#define EKD_USB_DEVICE void
	#define PCI_DEVICE void
	#define EKD_UA_FPGA_DEVICE void
#endif

/*------------------------------------------------------------------------------
 * Maximal number of USB devices
 *-----------------------------------------------------------------------------*/
#define EKD_USB_MAX_DEVICE 64

/*------------------------------------------------------------------------------
 * ETEL PCI device ID
 *-----------------------------------------------------------------------------*/
#define EKD_PCI_ULIGHT_VENDOR_ID	0x1556
#define EKD_PCI_UA_VENDOR_ID		0x10EE

#define EKD_PCI_DEVICE_ID_ULIGHT_PCI	0x2022			//UltimET PCI
#define EKD_PCI_DEVICE_ID_ULIGHT_PCIE	0x2024			//UltimET PCIe

#define EKD_PCI_DEVICE_ID_UA_PCIE		0xD400			//UltimET Advanced

/*------------------------------------------------------------------------------
 * error codes
 *-----------------------------------------------------------------------------*/
#define EKD_EBADLIBRARYVERSION           -619        /**< Bad WinDriver library version installed */
#define EKD_EBADLIBRARY                  -618        /**< function of external library not found */
#define EKD_ENOLIBRARY                   -617        /**< external library not found */
#define EKD_EBADDRIVER                   -616        /**< wrong version of the installed device driver */
#define EKD_EINTERNAL                    -615        /**< some internal error in the etel software */
#define EKD_ESEMAPHORENOTACTIVE          -614        /**< semaphore is not active */
#define EKD_EEXCEPTION                   -613        /**< an exception has occurred */
#define EKD_EBADSTATE                    -612        /**< bad state */
#define EKD_ENOTIMPLEMENTED              -611        /**< this function is not implemented */
#define EKD_ETIMEOUT                     -610        /**< timeout error */
#define EKD_EOSDETECTION                 -609        /**< troubles in os detection */
#define EKD_ESEMAPHORELIMIT              -608        /**< the semaphore have the max value */
#define EKD_ESLOTNOTFOUND                -607        /**< isa or pci slot not found */
#define EKD_EBADPARAM                    -606        /**< bad parameters */
#define EKD_ESYSTEM                      -605        /**< some system resource return an error */
#define EKD_EVECTORBUSY                  -604        /**< the irq vector is already used */
#define EKD_EMAPPING                     -603        /**< troubles during the mapping or the unmapping */
#define EKD_EDEVIO                       -602        /**< troubles in data transfer with the driver */
#define EKD_EINIT                        -601        /**< initialization troubles */
#define EKD_EOPENDRV                     -600        /**< cannot open the driver */



/**********************************************************************************************************/
/*** Types */
/**********************************************************************************************************/
/*------------------------------------------------------------------------------
 * EKD BUS enum type
 *-----------------------------------------------------------------------------*/
typedef enum {
    EKD_BUS_UNDEFINED = -1,
    EKD_BUS_INTERNAL,
    EKD_BUS_ISA,
    EKD_BUS_EISA,
    EKD_BUS_MICROCHANNEL,
    EKD_BUS_TURBOCHANNEL,
    EKD_BUS_PCI,
    EKD_BUS_VME,
    EKD_BUS_NUBUS,
    EKD_BUS_PCMCIA,
    EKD_BUS_C,
    EKD_BUS_MPI,
    EKD_BUS_MPSA,
    EKD_BUS_PROCESSORINTERNAL,
    EKD_BUS_INTERNALPOWER,
    EKD_BUS_PNPISA,
	EKD_BUS_PPC_PER_BUS,
	EKD_BUS_PPC_DSGAT2_BUS,
	EKD_MAX_BUS_TYPE			/* Maximum value of bus type */
} EKD_BUS_TYPE;


/*------------------------------------------------------------------------------
 * EKD IRQ_MODE type
 *-----------------------------------------------------------------------------*/
typedef enum {
	EKD_IRQ_LEVEL_SENSITIVE,
	EKD_IRQ_LATCHED
} EKD_IRQ_MODE;


/*------------------------------------------------------------------------------
 * EKD PCI SLOT type
 *-----------------------------------------------------------------------------*/
typedef union {
	struct {
		dword device_number:5;
        dword function_number:3;
        dword reserved:24;
    } bits;
    dword as_dword;
} EKD_PCI_SLOT;


/*------------------------------------------------------------------------------
 * EKD PCI PlugAndPlay type
 *-----------------------------------------------------------------------------*/
typedef struct {
    word vendor_id;
    word device_id;
    word command;
    word status;
    byte revision_id;
    byte prog_if;
    byte SubClass;
    byte BaseClass;
    byte CacheLineSize;
    byte LatencyTimer;
    byte HeaderType;
    byte BIST;
    union {
         struct {
             dword BaseAddresses[6];
             dword Reserved1[2];
             dword ROMBaseAddress;
             dword Reserved2[2];
             byte InterruptLine;
             byte InterruptPin;
             byte MinimumGrant;
             byte MaximumLatency;
         } type0;
     } u;
     byte device_specific[192];
} EKD_PCI_PNP_INFO;


/*------------------------------------------------------------------------------
 * EKD PCI DEVICE type
 *-----------------------------------------------------------------------------*/
#ifndef FREERTOS
typedef struct {
    PCI_DEVICE *device;
    size_t address[6];			//Use of size_t which is dependant of memory model (32 or 64 bits)
	int isrCalled;
} EKD_PCI_DEVICE;
#endif

/*------------------------------------------------------------------------------
 * EKD KERNEL CALLBACK type
 *-----------------------------------------------------------------------------*/
typedef void (EKD_CALLBACK *EKD_KERNEL_CALLBACK)(void *param);

/*********************************************************************************************************/
/*** EKD exported functions */
/*********************************************************************************************************/
char_cp _EKD_EXPORT ekd_translate_error(int code);
int	_EKD_EXPORT ekd_get_driver_version(dword *drv_version);
dword _EKD_EXPORT ekd_get_version(void);
dword _EKD_EXPORT ekd_get_edi_version(void);
time_t _EKD_EXPORT ekd_get_build_time(void);
void _EKD_EXPORT ekd_close(void);

//USB functions
#if (defined WIN32 || defined _WIN64) && !defined UNDER_RTSS
		int _EKD_EXPORT ekd_usb_get_present_serial_number(char serial_number[][16], int *size);
		int	_EKD_EXPORT ekd_usb_create_device (EKD_USB_DEVICE **dev);
		int	_EKD_EXPORT ekd_usb_destroy_device (EKD_USB_DEVICE **dev);
		int	_EKD_EXPORT ekd_usb_open (EKD_USB_DEVICE *dev, char* serial_number);
		int	_EKD_EXPORT ekd_usb_write (EKD_USB_DEVICE *dev, void *buffer, dword size);
		int	_EKD_EXPORT ekd_usb_read (EKD_USB_DEVICE *dev, void *buffer, dword size);
		int	_EKD_EXPORT ekd_usb_reset (EKD_USB_DEVICE *dev);
		int	_EKD_EXPORT ekd_usb_purge (EKD_USB_DEVICE *dev);
		int	_EKD_EXPORT ekd_usb_close (EKD_USB_DEVICE *dev);
		int	_EKD_EXPORT ekd_usb_data_available (EKD_USB_DEVICE *dev, ebool *data_available);
		int	_EKD_EXPORT ekd_usb_set_timeout (EKD_USB_DEVICE *dev, dword read_timeout, dword write_timeout);
		char* _EKD_EXPORT ekd_usb_get_serial_number (EKD_USB_DEVICE *dev);
		int	_EKD_EXPORT ekd_usb_wait_for_data(EKD_USB_DEVICE *dev, int timeout);
		int _EKD_EXPORT ekd_usb_load_library();
		int _EKD_EXPORT ekd_usb_unload_library();
		int _EKD_EXPORT ekd_usb_simul_event(EKD_USB_DEVICE *dev);
#endif /* (defined WIN32 || defined _WIN64) && !defined UNDER_RTSS*/

#if defined LINUX
		int _EKD_EXPORT ekd_usb_get_present_serial_number(char serial_number[][16], int *size);
		int	_EKD_EXPORT ekd_usb_create_device (EKD_USB_DEVICE **dev);
		int	_EKD_EXPORT ekd_usb_destroy_device (EKD_USB_DEVICE **dev);
		int	_EKD_EXPORT ekd_usb_open (EKD_USB_DEVICE *dev, char* serial_number);
		int	_EKD_EXPORT ekd_usb_write (EKD_USB_DEVICE *dev, void *buffer, dword size);
		int	_EKD_EXPORT ekd_usb_read (EKD_USB_DEVICE *dev, void *buffer, dword size);
		int	_EKD_EXPORT ekd_usb_reset (EKD_USB_DEVICE *dev);
		int	_EKD_EXPORT ekd_usb_purge (EKD_USB_DEVICE *dev);
		int	_EKD_EXPORT ekd_usb_close (EKD_USB_DEVICE *dev);
		int	_EKD_EXPORT ekd_usb_data_available (EKD_USB_DEVICE *dev, ebool *data_available);
		int	_EKD_EXPORT ekd_usb_set_timeout (EKD_USB_DEVICE *dev, dword read_timeout, dword write_timeout);
		char* _EKD_EXPORT ekd_usb_get_serial_number (EKD_USB_DEVICE *dev);
		int	_EKD_EXPORT ekd_usb_wait_for_data(EKD_USB_DEVICE *dev, int timeout);
		int _EKD_EXPORT ekd_usb_load_library();
		int _EKD_EXPORT ekd_usb_unload_library();
		int _EKD_EXPORT ekd_usb_simul_event(EKD_USB_DEVICE *dev);
#endif /* LINUX */

//UltimET PCI functions
#if defined WIN32 || defined _WIN64 || defined LINUX || defined QNX6 || defined F_ITRON || defined VXWORKS_6x
	int	_EKD_EXPORT ekd_ultimet_load_library(dword vendor_id, int nb_devices, dword device_id_list[], word device_idx);
	int	_EKD_EXPORT ekd_ultimet_unload_library();
	int _EKD_EXPORT ekd_ultimet_irq_disable(EKD_PCI_DEVICE *dev);
	int _EKD_EXPORT ekd_ultimet_irq_enable(EKD_PCI_DEVICE *dev, dword offset0, dword offset1, dword offset2, dword offset3, dword offset4, dword offset5);
	int _EKD_EXPORT ekd_ultimet_device_close(EKD_PCI_DEVICE *dev);
	int _EKD_EXPORT ekd_ultimet_device_open(EKD_PCI_DEVICE *dev, word deviceIdx);
	int _EKD_EXPORT ekd_ultimet_device_open_ex(EKD_PCI_DEVICE *dev, dword vendor_id, int nb_devices, dword device_ids[], word device_idx);
	int _EKD_EXPORT ekd_ultimet_wait_for_irq(EKD_PCI_DEVICE *dev, int timeout);
	int _EKD_EXPORT ekd_ultimet_release_irq(EKD_PCI_DEVICE *dev);
	int _EKD_EXPORT ekd_ultimet_get_events(EKD_PCI_DEVICE *dev, dword *evt0, dword *evt1, dword *evt2, dword *evt3, dword *evt4, dword *evt5);
#endif /* defined WIN32 || defined _WIN64 || defined LINUX || defined QNX6 || defined F_ITRON */

#if (defined WIN32 || defined _WIN64) && !defined UNDER_RTSS
	int _EKD_EXPORT ekd_ultimet_wait_for_cyclic_irq(EKD_PCI_DEVICE *dev, int cyclic_rtv_idx, int timeout);
	int _EKD_EXPORT ekd_ultimet_wait_for_rtv_cyclic_irq(EKD_PCI_DEVICE *dev, int timeout);
	int _EKD_EXPORT ekd_ultimet_wait_for_status_change_or_command_acknowledge_irq(EKD_PCI_DEVICE *dev, int timeout);
	int _EKD_EXPORT ekd_ultimet_release_status_change_or_command_acknowledge_irq(EKD_PCI_DEVICE *dev);
	int _EKD_EXPORT ekd_ultimet_release_rtv_cyclic_irq(EKD_PCI_DEVICE *dev);
	int _EKD_EXPORT ekd_ultimet_release_cyclic_irq(EKD_PCI_DEVICE *dev, int cyclic_rtv_idx);
#endif /* (defined WIN32 || defined _WIN64) && !defined UNDER_RTSS */

#if defined UNDER_RTSS
	int _EKD_EXPORT ekd_ultimet_enable_cyclic_irq(EKD_PCI_DEVICE *dev, int idx, void *etb, void(*handler)(void *, int, void*), void *user_param);
	int _EKD_EXPORT ekd_ultimet_disable_cyclic_irq(EKD_PCI_DEVICE *dev, int idx);
#endif /* defined UNDER_RTSS */

#if defined FREERTOS
	int _EKD_EXPORT ekd_ua_fpga_device_create(EKD_UA_FPGA_DEVICE **dev);
	int _EKD_EXPORT ekd_ua_fpga_device_destroy(EKD_UA_FPGA_DEVICE **dev);
	int _EKD_EXPORT ekd_ua_fpga_irq_cleanup();
	int _EKD_EXPORT ekd_ua_fpga_irq_disable(EKD_UA_FPGA_DEVICE *dev);
	int _EKD_EXPORT ekd_ua_fpga_irq_enable(EKD_UA_FPGA_DEVICE *dev, volatile dword *global_irq_mask_addr, volatile dword *global_irq_addr, dword cmd_stream_bit_in_global_irq_mask, dword status_bit_in_global_irq_mask, dword rtv_bit_in_global_irq_mask, dword cyclic_bit_in_global_irq_mask, dword csm_cmd_bit_in_global_irq_mask, dword file_cmd_bit_in_global_irq_mask);
	int _EKD_EXPORT ekd_ua_fpga_device_close(EKD_UA_FPGA_DEVICE *dev);
	int _EKD_EXPORT ekd_ua_fpga_device_open(EKD_UA_FPGA_DEVICE *dev);
	int _EKD_EXPORT ekd_ua_fpga_wait_for_cyclic_irq(EKD_UA_FPGA_DEVICE *dev, int cyclic_rtv_idx, int timeout);
	int _EKD_EXPORT ekd_ua_fpga_wait_for_rtv_cyclic_irq(EKD_UA_FPGA_DEVICE *dev, int timeout);
	int _EKD_EXPORT ekd_ua_fpga_wait_for_status_change_or_command_acknowledge_irq(EKD_UA_FPGA_DEVICE *dev, int timeout);
	int _EKD_EXPORT ekd_ua_fpga_wait_for_csm_cmd_irq(EKD_UA_FPGA_DEVICE *dev, int timeout);
	int _EKD_EXPORT ekd_ua_fpga_release_status_change_or_command_acknowledge_irq(EKD_UA_FPGA_DEVICE *dev);
	int _EKD_EXPORT ekd_ua_fpga_release_rtv_cyclic_irq(EKD_UA_FPGA_DEVICE *dev);
	int _EKD_EXPORT ekd_ua_fpga_release_cyclic_irq(EKD_UA_FPGA_DEVICE *dev, int cyclic_rtv_idx);
	int _EKD_EXPORT ekd_ua_fpga_release_csm_cmd_irq(EKD_UA_FPGA_DEVICE *dev);
	int _EKD_EXPORT ekd_ua_fpga_set_debug_shm_ptr(EKD_UA_FPGA_DEVICE *dev, void *isr_dbg_shm);
	int _EKD_EXPORT ekd_ua_fpga_set_etb_waiting_thread_id(EKD_UA_FPGA_DEVICE *dev, THREAD cmd_and_status_waiting_thread,  THREAD cyclic_rtv_waiting_thread, THREAD cyclic_waiting_thread[]);
	int _EKD_EXPORT ekd_ua_fpga_set_csm_cmd_waiting_thread_id(EKD_UA_FPGA_DEVICE *dev, THREAD csm_cmd_waiting_thread);
#endif /* (defined WIN32 || defined _WIN64) && !defined UNDER_RTSS */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _EKD40_H_ */
