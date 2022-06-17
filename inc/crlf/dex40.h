/*
 * dex40.h
 *
 * Copyright (c) 2009-2021 ETEL SA. All Rights Reserved.
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
 *
 */

/**
 * DEX-Version : 4.09A
 * This header file contains public declarations for the data exchange library.\n
 * This library allows exporting and importing datas from/into a pre-defined file format\n
 * This library is conformed to POSIX 1003.1c 
 * @file dex40.h
 */


#ifndef _DEX40_H
#define _DEX40_H

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

#ifdef F_ITRON
	#include <ctype.h>
	#include <OS_IF/os_api.h>
	#include <File_api.h>
	#include <time.h>
	#include <ComSrc/ComSrc_Pro.h>	
#endif /*F_ITRON */

#ifdef FREERTOS
	#include <FreeRTOS.h>
#endif /*FREERTOS */

/**********************************************************************************************************/
/*** Literals */
/**********************************************************************************************************/
#ifdef VXWORKS_5_4
    #define NO_UNNAMED_STRUCT
#endif    

/**
 * @defgroup DEXAll DEX All functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXExchanger DEX Exchanger object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXGeneralInfo DEX GeneralInfo object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXControllerInfo DEX ControllerInfo object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXMachineConfiguration DEX MachineConfiguration object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXMappingConfiguration DEX MappingConfiguration object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXMappingInfoTable DEX MappingInfoTable object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXMappingDataTable DEX MappingDataTable object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXTimeTraceConfiguration DEX TimeTraceConfiguration object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXTimeTriggerConfiguration DEX TimeTriggerConfiguration object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXTimeAcquisitionConfiguration DEX TimeAcquisitionConfiguration object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXTimeDataTable DEX TimeDataTable object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXFrequencyAcquisitionConfiguration DEX FrequencyAcquisitionConfiguration object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXFrequencyDataTable DEX FrequencyDataTable object access functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXUtility DEX Utility functions
 */
/*@{*/
/*@}*/

/**
 * @defgroup DEXRegisterInfo DEX Register Info functions
 */
/*@{*/
/*@}*/

/**
 * @example exchanger.c
 */

#define DEX_MAX_CHAR_BUFFER			 32			/**< maximal size of character buffer */
#define DEX_MAX_CHAR_USER_BUFFER	 1024		/**< maximal size of user character buffer (used for Transfer function In/Out Registers)*/
#define DEX_MAX_DESCRIPTION_BUFFER	 128		/**< maximal size of character buffer which contains register description*/

#ifdef __cplusplus
	#ifdef ETEL_OO_API		/* defined by the user when he need the Object Oriented interface */
		#define DEX_OO_API
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
	#if defined WIN32 || defined _WIN64
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#define __BYTE_ORDER __LITTLE_ENDIAN			/* define byte order for INTEL processor */
	#endif /*defined WIN32 || defined _WIN64*/

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
	#ifdef VXWORKS_6x
		#define __LITTLE_ENDIAN 1234
		#define __BIG_ENDIAN 4321
		#if (_BYTE_ORDER == _BIG_ENDIAN)		//VXWORKS 6 defines _BIG_ENDIAN or _LITTLE_ENDIAN
			#define __BYTE_ORDER __BIG_ENDIAN
		#else
			#define __BYTE_ORDER __LITTLE_ENDIAN
		#endif
	#endif /*VXWORKS_6x*/

	/*------------------------------*/
	/* F_ITRON Byte order			*/
	#ifdef F_ITRON
		#define __LITTLE_ENDIAN _LITTLE_ENDIAN
		#define __BIG_ENDIAN _BIG_ENDIAN
		#define __BYTE_ORDER _BYTE_ORDER
	#endif /*F_ITRON*/

	/*------------------------------*/
	/* FREERTOS Byte order			*/
	#ifdef FREERTOS
		#define __LITTLE_ENDIAN _LITTLE_ENDIAN
		#define __BIG_ENDIAN _BIG_ENDIAN
		#define __BYTE_ORDER _BYTE_ORDER
	#endif /*FREERTOS*/
	
	/*------------------------------*/
	/* VXWORKS_5_4 Byte order			*/
	#ifdef VXWORKS_5_4
		#define __LITTLE_ENDIAN _LITTLE_ENDIAN
		#define __BIG_ENDIAN _BIG_ENDIAN
		#define __BYTE_ORDER _BYTE_ORDER
	#endif /*VXWORKS_5_4*/

	
#endif /*BYTE_ORDER*/

/*------------------------------*/
/* WINDOWS type modifiers		*/
#if defined WIN32 || defined _WIN64
	#define _DEX_EXPORT __cdecl                          /* function exported by static library */
	#define DEX_CALLBACK __cdecl                         /* client callback function called by library */
#endif /*WIN32 || _WIN64 */

/*------------------------------*/
/* POSIX type modifiers			*/
#if defined POSIX || defined VXWORKS_6x
	#define _DEX_EXPORT                           		/* function exported by library */
	#define DEX_CALLBACK                          		/* client callback function called by library */
#endif /* POSIX  || VXWORKS_6x*/

/*------------------------------*/
/* F_ITRON type modifiers		*/
#ifdef F_ITRON
	#define _DEX_EXPORT __cdecl                          /* function exported by static library */
	#define DEX_CALLBACK __cdecl                         /* client callback function called by library */
#endif /* F_ITRON */

/*------------------------------*/
/* FREERTOS type modifiers		*/
#ifdef FREERTOS
	#define _DEX_EXPORT
	#define DEX_CALLBACK
#endif /* FREERTOS */

/*------------------------------*/
/* VXWORKS_5_4 type modifiers		*/
#ifdef VXWORKS_5_4
	#define _DEX_EXPORT                                  /* function exported by static library */
	#define DEX_CALLBACK                                 /* client callback function called by library */
#endif /* VXWORKS_5_4 */


/*------------------------------------------------------------------------------
 * hidden structures for library clients
 *-----------------------------------------------------------------------------*/
#ifndef DEX_EXCHANGER
	#define DEX_EXCHANGER void
#endif

#ifndef FALSE
	#define FALSE                            (unsigned char)0
#endif
#ifndef TRUE
	#define TRUE                             (unsigned char)1
#endif


/*------------------------------------------------------------------------------
 * error codes - c
 *-----------------------------------------------------------------------------*/
#define DEX_EVERSION                     -2406       /**< version not supported */
#define DEX_EINTERNAL                    -2405       /**< some internal error in the etel software */
#define DEX_EFERROR                      -2404       /**< file access error */
#define DEX_EFSYNTAX                     -2403       /**< file syntax error */
#define DEX_ENOTIMPLEMENTED              -2402       /**< function not yet implemented */
#define DEX_ESYSTEM                      -2401       /**< some system resource return an error */
#define DEX_EBADPARAM                    -2400       /**< one of the parameter is not valid */

/**********************************************************************************************************/
/*- MACROS */
/**********************************************************************************************************/
#define DEX_DIAG(err, exchanger)                      (dex_diag(__FILE__, __LINE__, err, exchanger))
#define DEX_SDIAG(str, err, exchanger)                (dex_sdiag(str, __FILE__, __LINE__, err, exchanger))
#define DEX_FDIAG(output_file_name, err, exchanger)   (dex_fdiag(output_file_name, __FILE__, __LINE__, err, exchanger))

/**********************************************************************************************************/
/*** Types */
/**********************************************************************************************************/
/**
 * @enum DEX_DATA_TYPE
 * Data type which the dll is able to export or import
 */
typedef enum DEX_DATA_TYPE {
    DEX_NO_FILE = -1,									/**< No data type file*/
    DEX_SCALING_FILE = 0,								/**< Scaling data type file*/
    DEX_MAPPING_FILE = 1,								/**< Mapping data type file */
    DEX_TIME_FILE = 2,									/**< Time domain data type file */
    DEX_FREQUENCY_FILE = 3								/**< Frequency domain data type file */
} DEX_DATA_TYPE;


/**
 * @enum DEX_MAPPING_CORRECTION_MODE
 * Available mapping mode
 */
typedef enum DEX_MAPPING_CORRECTION_MODE {
    DEX_MAPPING_CORRECTION_MODE_POSITIVE = 0,					/**< Contains Correction mode positive values*/
    DEX_MAPPING_CORRECTION_MODE_NEGATIVE = 1					/**< Contains Correction mode negative values*/
} DEX_MAPPING_CORRECTION_MODE;

/**
 * @enum DEX_TIME_TRIGGER_TYPE
 * Available Trigger mode
 */
typedef enum DEX_TIME_TRIGGER_TYPE {
    DEX_TIME_TRIGGER_TYPE_IMM = 0,					/**< Trigger type Immediate*/
    DEX_TIME_TRIGGER_TYPE_BOM = 1,					/**< Trigger type Begin Of Movement*/
    DEX_TIME_TRIGGER_TYPE_EOM = 2,					/**< Trigger type End Of Movement*/
    DEX_TIME_TRIGGER_TYPE_POS = 3,					/**< Trigger type Position*/
    DEX_TIME_TRIGGER_TYPE_TV = 4,					/**< Trigger type Trace Value*/
    DEX_TIME_TRIGGER_TYPE_RV = 5,					/**< Trigger type Register value*/
    DEX_TIME_TRIGGER_TYPE_BFS = 6,					/**< Trigger type Bit Field state*/
    DEX_TIME_TRIGGER_TYPE_BFC = 7					/**< Trigger type Bit Field Change*/
} DEX_TIME_TRIGGER_TYPE;

/**
 * @enum DEX_TIME_TRIGGER_EDGE
 * Available Triggering edge
 */
typedef enum DEX_TIME_TRIGGER_EDGE {
    DEX_TIME_TRIGGER_EDGE_RISING = 1,				/**< Trigger Rising Edge*/
    DEX_TIME_TRIGGER_EDGE_FALLING = 2,				/**< Trigger falling Edge*/
    DEX_TIME_TRIGGER_EDGE_BOTH = 3					/**< Trigger Rising and Falling Edge*/
} DEX_TIME_TRIGGER_EDGE;

/**
 * @enum DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE
 * Available Transfer function type for frequency domain data
 */
typedef enum DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE {
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_OPLF =     0,	    /**< Transfer function type Open Position Loop Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_OPL =	    1,	    /**< Transfer function type Open Position Loop*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_CPLF =     2,	    /**< Transfer function type Close Position Loop Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_CPL =	    3,	    /**< Transfer function type Close Position Loop*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_SPLF =     4,	    /**< Transfer function type Sensitivity Position Loop Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_SPL =	    5,	    /**< Transfer function type Sensitivity Position Loop*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_OCLF =     6,	    /**< Transfer function type Open Current Loop Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_OCL =	    7,	    /**< Transfer function type Open Current Loop*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_CCLF =     8,	    /**< Transfer function type Close Current Loop Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_CCL =	    9,	    /**< Transfer function type Close Current Loop*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_SCLF =     10,	    /**< Transfer function type Sensitivity Current Loop Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_SCL =	    11,	    /**< Transfer function type Sensitivity Current Loop*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_MPLPF =	12,	    /**< Transfer function type Mechanical Position loop Position Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_MPLP =		13,	    /**< Transfer function type Mechanical Position loop Position*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_MPLSF =	14,	    /**< Transfer function type Mechanical Position loop Speed Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_MPLS =		15,	    /**< Transfer function type Mechanical Position loop Speed*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_MCLPF =	16,	    /**< Transfer function type Mechanical Current loop Position Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_MCLP =		17,	    /**< Transfer function type Mechanical Current loop Position*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_MCLSF =	18,	    /**< Transfer function type Mechanical Current loop Speed Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_MCLS =		19,	    /**< Transfer function type Mechanical Current loop Speed*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_MCF =	    20,	    /**< Transfer function type Motor Coil Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_MC =	    21,	    /**< Transfer function type Motor Coil*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_SF =	    22,	    /**< Transfer function type Stiffness Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_S =	    23,	    /**< Transfer function type Stiffness*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_R101F =    24,     /**<Transfer function type Reference1 -> Output1 Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_R101 =     25,     /**<Transfer function type Reference1 -> Output1*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_R102F =    26,     /**<Transfer function type Reference1 -> Output2 Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_R102 =     27,     /**<Transfer function type Reference1 -> Output2*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_R201F =    28,     /**<Transfer function type Reference2 -> Output1 Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_R201 =     29,     /**<Transfer function type Reference2 -> Output1*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_R202F =    30,     /**<Transfer function type Reference2 -> Output2 Filtered*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_R202  =    31,     /**<Transfer function type Reference2 -> Output2*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_OFLF_M0 =  32,     /**< Transfer function type Open Force Loop Filtered Mode 0*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_OFL_M0  =  33,     /**< Transfer function type Open Force Loop Mode 0*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_CFLF_M0 =  34,     /**< Transfer function type Closed Force Loop Filtered Mode 0*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_CFL_M0  =  35,     /**< Transfer function type Closed Force Loop Mode 0*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_SFLF_M0 =  36,     /**< Transfer function type Sensitivity Force Loop Filtered Mode 0*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_SFL_M0  =  37,     /**< Transfer function type Sensitivity Force Loop Mode 0*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_PFLF_M0 =  38,     /**< Transfer function type Process Force Loop Filtered Mode 0*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_PFL_M0  =  39,     /**< Transfer function type Process Force Loop Mode 0*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_OFLF_M1 =  40,     /**< Transfer function type Open Force Loop Filtered Mode 1*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_OFL_M1  =  41,     /**< Transfer function type Open Force Loop Mode 1*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_CFLF_M1 =  42,     /**< Transfer function type Closed Force Loop Filtered Mode 1*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_CFL_M1  =  43,     /**< Transfer function type Closed Force Loop Mode 1*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_SFLF_M1 =  44,     /**< Transfer function type Sensitivity Force Loop Filtered Mode 1*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_SFL_M1  =  45,     /**< Transfer function type Sensitivity Force Loop Mode 1*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_PFLF_M1 =  46,     /**< Transfer function type Process Force Loop Filtered Mode 1*/
    DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE_PFL_M1  =  47,     /**< Transfer function type Process Force Loop Mode 1*/
    DEX_FREQUENCY_COHERENCE_FUNCTION_TYPE        =  48      /**< Coherence function type*/
} DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE;

/**
 * @enum DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE
 * Available Acquisition quality type for Frequency domain data
 */
typedef enum DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE {
    DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE_NL = 0,	/**< Acquisition quality Normal Long*/
    DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE_NS = 1,	/**< Acquisition quality Normal Short*/
    DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE_FL = 2,	/**< Acquisition quality Fast Long*/
    DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE_FS = 3,	/**< Acquisition quality Fast Short*/
    DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE_HQ =	4	/**< Acquisition quality High quality*/
} DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE;

/**
 * @enum DEX_FREQUENCY_CURRENT_REFERENCE_NOISE
 * Addition of noise on current reference
 */
typedef enum DEX_FREQUENCY_CURRENT_REFERENCE_NOISE {
    DEX_FREQUENCY_CURRENT_REFERENCE_NOISE_NOT_ADDED = 0,	/**< No noise is added*/
    DEX_FREQUENCY_CURRENT_REFERENCE_NOISE_ADDED = 1     	/**< Noise is added*/
} DEX_FREQUENCY_CURRENT_REFERENCE_NOISE;

/**
* @enum DEX_FREQUENCY_IDENTIFICATION_METHOD
* Method defines where in control loop excitation is added and 
 * which signals are used to calculate transfer function.
*/
typedef enum DEX_FREQUENCY_IDENTIFICATION_METHOD {
    DEX_FREQUENCY_IDENTIFICATION_METHOD_2POINT_PROCESS = 0,   /**< Control loop open, excitation at process input*/
    DEX_FREQUENCY_IDENTIFICATION_METHOD_3POINT = 1,           /**< Control loop closed, excitation at process input*/
    DEX_FREQUENCY_IDENTIFICATION_METHOD_2POINT_LOOP = 2       /**< Control loop closed, excitation at reference signal*/
} DEX_FREQUENCY_IDENTIFICATION_METHOD;

/**
* @enum DEX_FREQUENCY_IDENTIFICATION_EXTRA_VALUE
* Type of extra value which can be saved with Identification acquisition
*/
typedef enum DEX_FREQUENCY_IDENTIFICATION_EXTRA_VALUE
{
    DEX_FREQUENCY_IDENTIFICATION_EXTRA_VALUE_CONTACT_FORCE = 0,
    DEX_FREQUENCY_IDENTIFICATION_EXTRA_VALUE_CONTACT_STIFFNESS = 1,
    DEX_FREQUENCY_IDENTIFICATION_EXTRA_VALUE_CONTACT_POSITION = 2,
    DEX_FREQUENCY_IDENTIFICATION_EXTRA_VALUE_LAST
} DEX_FREQUENCY_IDENTIFICATION_EXTRA_VALUE;

/**
* @enum DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER
* Type of the regulation parameter used for the Identification acquisition
*/
typedef enum DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER
{
    DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER_KIF0 = 0,
    DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER_KPF0 = 1,
    DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER_KIF1 = 2,
    DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER_KPF1 = 3,
    DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER_KPFC = 4,
    DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER_KIFC = 5,
    DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER_KDFC = 6,
    DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER_LAST
} DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER;


/**
 * @enum DEX_UNIT_TYPE
 * Unit type of the trace
 */
typedef enum DEX_UNIT_TYPE {
    DEX_LINEAR_UNIT = 0,								/**< Linear unit*/
    DEX_ROTARY_UNIT = 1,								/**< Rotary unit */
} DEX_UNIT_TYPE;

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
			#define STRTOI64(s,e,b)						_strtoi64(s,e,b)
			#define STRTOUI64(s,e,b)					_strtoui64(s,e,b)
		/*------------------------------*/
		/* POSIX, F_ITRON, VXWORKS_6x 64 bits integer		*/
		#elif defined POSIX || defined F_ITRON || defined VXWORKS_6x || defined FREERTOS || defined VXWORKS_5_4
			#define eint64 long long
			#define STRTOI64(s,e,b)						strtoll(s,e,b)
			#define STRTOUI64(s,e,b)					strtoull(s,e,b)
		#endif
	#endif
#endif

/*-------------------------*/
/* DEX_INC_VALUE_TYPE type */
/*-------------------------*/
/**
 * @enum DEX_INC_VALUE_TYPE
 * Types defining all possible increment values
 */
typedef enum DEX_INC_VALUE_TYPE {
    DEX_INC_VALUE_I32 = 0,				/**< Increment value is integer 32 bits*/
    DEX_INC_VALUE_F32 = 32,				/**< Increment value is float 32 bits*/
    DEX_INC_VALUE_I64 = 64,				/**< Increment value is integer 64 bits*/
    DEX_INC_VALUE_F64 = 96				/**< Increment value is float 64 bits*/
} DEX_INC_VALUE_TYPE;

/*-------------------------*/
/* DEX_INC_VALUE structure */
/*-------------------------*/
#ifdef NO_UNNAMED_STRUCT
    #define DEX_INC_VAL(val)  val.v
    #define DEX_INC_PVAL(val)  (&(val->v))
#else
    #define DEX_INC_VAL(val)  val
    #define DEX_INC_PVAL(val)  val
#endif    

typedef struct DEX_INC_VALUE {
	DEX_INC_VALUE_TYPE type;
    #ifndef NO_UNNAMED_STRUCT
        union {
            int i32;
            eint64 i64;
            float f32;
            double f64;
        };
    #else
        union {
            int i32;
            eint64 i64;
            float f32;
            double f64;
        }v;
    #endif        
} DEX_INC_VALUE;

/**********************************************************************************************************/
/*** DEX exported functions */
/**********************************************************************************************************/

/**
 * @addtogroup DEXAll
 * @{
 */

/*----------------------------*/
/* Error management functions */
/*----------------------------*/
const char* _DEX_EXPORT dex_translate_error(int code);
char *		_DEX_EXPORT dex_get_error_text(DEX_EXCHANGER *exchanger, int size, char *str);
char *		_DEX_EXPORT dex_get_full_error_text(DEX_EXCHANGER *exchanger, int size, char *str) ;
int			_DEX_EXPORT dex_diag(char_cp file_name, int line, int err, DEX_EXCHANGER *exchanger);
int			_DEX_EXPORT dex_sdiag(char_p str, char_cp file_name, int line, int err, DEX_EXCHANGER *exchanger);
int			_DEX_EXPORT dex_fdiag(char_cp output_file_name, char_cp file_name, int line, int err, DEX_EXCHANGER *exchanger);

int			_DEX_EXPORT dex_create_trace(char *file, int line, const char *fctName, int errorNr, const char *errorText, char *comment, int data_file_line);
int			_DEX_EXPORT dex_add_trace(char *file, int line, const char *fctName, char *comment);
void		_DEX_EXPORT dex_set_trace_source_line(int data_file_line); 
int			_DEX_EXPORT dex_trace_off();
int			_DEX_EXPORT dex_trace_on();

/*---------------------------------------*/
/* Object creation-destruction functions */
/*---------------------------------------*/
int		_DEX_EXPORT dex_create_exchanger(DEX_EXCHANGER **exchanger);
int		_DEX_EXPORT dex_destroy_exchanger(DEX_EXCHANGER **exchanger);
ebool	_DEX_EXPORT dex_is_valid_exchanger(DEX_EXCHANGER *exchanger);
int		_DEX_EXPORT dex_check_consistency(DEX_EXCHANGER *exchanger);

/*-----------------------*/
/* File access functions */
/*-----------------------*/
int		_DEX_EXPORT dex_import(DEX_EXCHANGER *exchanger, const char* file_name);
int		_DEX_EXPORT dex_export(DEX_EXCHANGER *exchanger, const char* file_name);
int		_DEX_EXPORT dex_set_separator(DEX_EXCHANGER *exchanger, char separator);

/*-------------------*/
/* Utility functions */
/*-------------------*/
dword   _DEX_EXPORT dex_get_version(void);
dword	_DEX_EXPORT dex_get_last_data_format_version();
#ifdef F_ITRON
	int 	_DEX_EXPORT dex_mem_set_edi_heap_id(FJ_ID edi_heap_id);
#endif /* F_ITRON */

/*---------------------------------*/
/* General info exchanger functions */
/*---------------------------------*/
int		_DEX_EXPORT dex_create_general_info(DEX_EXCHANGER *exchanger, DEX_DATA_TYPE type);
int		_DEX_EXPORT dex_get_general_info_data_type(DEX_EXCHANGER *exchanger, DEX_DATA_TYPE *type);
int		_DEX_EXPORT dex_set_general_info_data_format_version(DEX_EXCHANGER *exchanger, dword version);
int		_DEX_EXPORT dex_get_general_info_data_format_version(DEX_EXCHANGER *exchanger, dword *version);
int		_DEX_EXPORT dex_set_general_info_software_name(DEX_EXCHANGER *exchanger, const char *buf);
int		_DEX_EXPORT dex_get_general_info_software_name(DEX_EXCHANGER *exchanger, int buf_size, char *buf);
int		_DEX_EXPORT dex_set_general_info_software_version(DEX_EXCHANGER *exchanger, dword version);
int		_DEX_EXPORT dex_get_general_info_software_version(DEX_EXCHANGER *exchanger, dword * version);
int		_DEX_EXPORT dex_set_general_info_date(DEX_EXCHANGER *exchanger, const char *buf);
int		_DEX_EXPORT dex_get_general_info_date(DEX_EXCHANGER *exchanger, int buf_size, char *buf);
int		_DEX_EXPORT dex_set_general_info_time(DEX_EXCHANGER *exchanger, const char *buf);
int		_DEX_EXPORT dex_get_general_info_time(DEX_EXCHANGER *exchanger, int buf_size, char *buf);
int		_DEX_EXPORT dex_set_general_info_operator(DEX_EXCHANGER *exchanger, const char *buf);
int		_DEX_EXPORT dex_get_general_info_operator(DEX_EXCHANGER *exchanger, int buf_size, char *buf);
int		_DEX_EXPORT dex_set_general_info_customer(DEX_EXCHANGER *exchanger, const char *buf);
int		_DEX_EXPORT dex_get_general_info_customer(DEX_EXCHANGER *exchangerr, int buf_size, char *buf);
int		_DEX_EXPORT dex_set_general_info_project(DEX_EXCHANGER *exchanger, const char *buf);
int		_DEX_EXPORT dex_get_general_info_project(DEX_EXCHANGER *exchanger, int buf_size, char *buf);
int		_DEX_EXPORT dex_set_general_info_description(DEX_EXCHANGER *exchanger, const char *buf);
int		_DEX_EXPORT dex_get_general_info_description(DEX_EXCHANGER *exchanger, int buf_size, char *buf);

/*------------------------------------*/
/* Controller info exchanger functions */
/*------------------------------------*/
int		_DEX_EXPORT dex_create_controller_info(DEX_EXCHANGER *exchanger, int nb_controller);
int		_DEX_EXPORT dex_get_controller_info_nb_controller(DEX_EXCHANGER *exchanger, int *nb_controller);
int		_DEX_EXPORT dex_set_controller_info_name(DEX_EXCHANGER *exchanger, int controller_idx, const char *buf);
int		_DEX_EXPORT dex_get_controller_info_name(DEX_EXCHANGER *exchanger, int controller_idx, int buf_size, char *buf);
int		_DEX_EXPORT dex_set_controller_info_firmware(DEX_EXCHANGER *exchanger, int controller_idx, dword firmware);
int		_DEX_EXPORT dex_get_controller_info_firmware(DEX_EXCHANGER *exchanger, int controller_idx, dword *firmware);
int		_DEX_EXPORT dex_set_controller_info_address(DEX_EXCHANGER *exchanger, int controller_idx, int address);
int		_DEX_EXPORT dex_get_controller_info_address(DEX_EXCHANGER *exchanger, int controller_idx, int *address);
int		_DEX_EXPORT dex_set_controller_info_serial_number(DEX_EXCHANGER *exchanger, int controller_idx, dword serial_number);
int		_DEX_EXPORT dex_get_controller_info_serial_number(DEX_EXCHANGER *exchanger, int controller_idx, dword *serial_number);
int		_DEX_EXPORT dex_set_controller_info_status(DEX_EXCHANGER *exchanger, int controller_idx, dword status);
int		_DEX_EXPORT dex_get_controller_info_status(DEX_EXCHANGER *exchanger, int controller_idx, dword *status);
int		_DEX_EXPORT dex_set_controller_info_type(DEX_EXCHANGER *exchanger, int controller_idx, int type);
int		_DEX_EXPORT dex_get_controller_info_type(DEX_EXCHANGER *exchanger, int controller_idx, int *type);
int		_DEX_EXPORT dex_get_controller_info_index(DEX_EXCHANGER *exchanger, int controller_nr, int *controller_idx);

/*-----------------------------------*/
/* Register info exchanger functions */
/*-----------------------------------*/
int	_DEX_EXPORT dex_create_axis_register_info(DEX_EXCHANGER *exchanger, int axis_idx, int nb_register);
int	_DEX_EXPORT dex_set_axis_register_info_axis_number(DEX_EXCHANGER *exchanger, int axis_idx, int axis_nb);
int	_DEX_EXPORT dex_create_axis_register_info_register(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int nb_sidx);
int	_DEX_EXPORT dex_set_axis_register_info_register_typ(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int reg_typ);
int	_DEX_EXPORT dex_set_axis_register_info_register_idx(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int idx);
int	_DEX_EXPORT dex_set_axis_register_info_register_sidx(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int reg_sidx, DEX_INC_VALUE reg_sidx_inc_val);
int	_DEX_EXPORT dex_set_axis_register_info_register_int32_sidx(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int reg_sidx, int val);
int	_DEX_EXPORT dex_set_axis_register_info_register_int64_sidx(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int reg_sidx, eint64 val);
int	_DEX_EXPORT dex_set_axis_register_info_register_float32_sidx(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int reg_sidx, float val);
int	_DEX_EXPORT dex_set_axis_register_info_register_float64_sidx(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int reg_sidx, double val);
int	_DEX_EXPORT dex_set_axis_register_info_register_conversion_factor(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, double reg_conv_factor);
int	_DEX_EXPORT dex_set_axis_register_info_register_unit(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, char *unit);
int _DEX_EXPORT dex_is_register_info_present(DEX_EXCHANGER *exchanger, ebool *present);
int	_DEX_EXPORT dex_get_register_info_nb_axis(DEX_EXCHANGER *exchanger, int *nb_axis);
int	_DEX_EXPORT dex_get_axis_register_info_axis_nb(DEX_EXCHANGER *exchanger, int axis_idx, int *axis_nb);
int	_DEX_EXPORT dex_get_axis_register_info_axis_idx(DEX_EXCHANGER *exchanger, int axis_nb, int *axis_idx);
int	_DEX_EXPORT dex_get_axis_register_info_nb_register(DEX_EXCHANGER *exchanger, int axis_idx, int *nb_register);
int	_DEX_EXPORT dex_get_axis_register_info_register_typ(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int *etel_reg_typ);
int	_DEX_EXPORT dex_get_axis_register_info_register_idx(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int *etel_reg_idx);
int	_DEX_EXPORT dex_get_axis_register_info_register_conversion_factor(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, double *conv_factor);
int	_DEX_EXPORT dex_get_axis_register_info_register_unit(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int buf_size, char *buf);
int	_DEX_EXPORT dex_get_axis_register_info_register_nb_depth(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int *nb_depth);
int	_DEX_EXPORT dex_get_axis_register_info_register_depth_value(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int reg_sidx, DEX_INC_VALUE *val);
int	_DEX_EXPORT dex_get_axis_register_info_register_depth_int32_value(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int reg_sidx, int *val);
int	_DEX_EXPORT dex_get_axis_register_info_register_depth_int64_value(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int reg_sidx, eint64 *val);
int	_DEX_EXPORT dex_get_axis_register_info_register_depth_float32_value(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int reg_sidx, float *val);
int	_DEX_EXPORT dex_get_axis_register_info_register_depth_float64_value(DEX_EXCHANGER *exchanger, int axis_idx, int reg_idx, int reg_sidx, double *val);
int	_DEX_EXPORT dex_get_register_inc_value(DEX_EXCHANGER *exchanger, int etel_axis_nr, int etel_reg_typ, int etel_reg_idx, int etel_reg_sidx, DEX_INC_VALUE *val);
int	_DEX_EXPORT dex_get_register_inc_int32_value(DEX_EXCHANGER *exchanger, int etel_axis_nr, int etel_reg_typ, int etel_reg_idx, int etel_reg_sidx, int *val);
int	_DEX_EXPORT dex_get_register_inc_int64_value(DEX_EXCHANGER *exchanger, int etel_axis_nr, int etel_reg_typ, int etel_reg_idx, int etel_reg_sidx, eint64 *val);
int	_DEX_EXPORT dex_get_register_inc_float32_value(DEX_EXCHANGER *exchanger, int etel_axis_nr, int etel_reg_typ, int etel_reg_idx, int etel_reg_sidx, float *val);
int	_DEX_EXPORT dex_get_register_inc_float64_value(DEX_EXCHANGER *exchanger, int etel_axis_nr, int etel_reg_typ, int etel_reg_idx, int etel_reg_sidx, double *val);
int	_DEX_EXPORT dex_get_register_iso_value(DEX_EXCHANGER *exchanger, int etel_axis_nr, int etel_reg_typ, int etel_reg_idx, int etel_reg_sidx, double *val);
int	_DEX_EXPORT dex_get_register_global_index(DEX_EXCHANGER *exchanger, int etel_axis_nr, int etel_reg_typ, int etel_reg_idx, int *idx);

/*==============================================================*/
/* Stage mapping and scale mapping specific exchanger functions */
/*==============================================================*/
/*-------------------------------------------------------------------*/
/* Stage mapping and scale mapping configuration exchanger functions */
/*-------------------------------------------------------------------*/
int		_DEX_EXPORT dex_create_machine_configuration(DEX_EXCHANGER *exchanger, int nb_axis);
int		_DEX_EXPORT dex_get_machine_configuration_nb_axis(DEX_EXCHANGER *exchanger, int *nb_axis);
int		_DEX_EXPORT dex_set_machine_configuration_mapping_version(DEX_EXCHANGER *exchanger, dword mapping_version);
int		_DEX_EXPORT dex_get_machine_configuration_mapping_version(DEX_EXCHANGER *exchanger, dword *mapping_version);
int		_DEX_EXPORT dex_set_machine_configuration_machine_type(DEX_EXCHANGER *exchanger, const char *buf);
int		_DEX_EXPORT dex_get_machine_configuration_machine_type(DEX_EXCHANGER *exchanger, int buf_size, char *buf);
int		_DEX_EXPORT dex_set_machine_configuration_machine_serial_number(DEX_EXCHANGER *exchanger, const char *buf);
int		_DEX_EXPORT dex_get_machine_configuration_machine_serial_number(DEX_EXCHANGER *exchanger, int buf_size, char *buf);
int		_DEX_EXPORT dex_set_machine_configuration_mapping_correction_mode(DEX_EXCHANGER *exchanger, DEX_MAPPING_CORRECTION_MODE mapping_correction_mode);
int		_DEX_EXPORT dex_get_machine_configuration_mapping_correction_mode(DEX_EXCHANGER *exchanger, DEX_MAPPING_CORRECTION_MODE *mapping_correction_mode);
int		_DEX_EXPORT dex_set_machine_configuration_axis_name_nr(DEX_EXCHANGER *exchanger, int axis_idx, const char *axis_name, int axis_nr);
int		_DEX_EXPORT dex_get_machine_configuration_axis_name_nr(DEX_EXCHANGER *exchanger, int axis_idx, int axis_name_size, char *axis_name, int *axis_nr);

/*----------------------------------*/
/* Configuration exchanger functions */
/*----------------------------------*/
int		_DEX_EXPORT dex_get_nb_mapping_configuration(DEX_EXCHANGER *exchanger, int *nb_conf);
int		_DEX_EXPORT dex_create_mapping_configuration(DEX_EXCHANGER *exchanger, int conf_idx, int dimension);
int		_DEX_EXPORT dex_get_mapping_configuration_number(DEX_EXCHANGER *exchanger, int conf_idx, int *conf_nr);
int		_DEX_EXPORT dex_set_mapping_configuration_number(DEX_EXCHANGER *exchanger, int conf_idx, int conf_nr);
int		_DEX_EXPORT dex_get_mapping_configuration_correction_dimension(DEX_EXCHANGER *exchanger, int conf_idx, int *correction_dimension);
int		_DEX_EXPORT dex_set_mapping_configuration_corrected_axis(DEX_EXCHANGER *exchanger, int conf_idx, const char *buf);
int		_DEX_EXPORT dex_get_mapping_configuration_corrected_axis(DEX_EXCHANGER *exchanger, int conf_idx, int buf_size, char *buf);
int		_DEX_EXPORT dex_set_mapping_configuration_correction_table(DEX_EXCHANGER *exchanger, int conf_idx, int correction_table_idx);
int		_DEX_EXPORT dex_get_mapping_configuration_correction_table(DEX_EXCHANGER *exchanger, int conf_idx, int *correction_table_idx);
int		_DEX_EXPORT dex_set_mapping_configuration_source_axes(DEX_EXCHANGER *exchanger, int conf_idx, int dimension_idx, const char *buf);
int		_DEX_EXPORT dex_get_mapping_configuration_source_axes(DEX_EXCHANGER *exchanger, int conf_idx, int dimension_idx, int buf_size, char *buf);
int		_DEX_EXPORT dex_set_mapping_configuration_origin(DEX_EXCHANGER *exchanger, int conf_idx, int dimension_idx, double origin);
int		_DEX_EXPORT dex_get_mapping_configuration_origin(DEX_EXCHANGER *exchanger, int conf_idx, int dimension_idx, double *origin);
int		_DEX_EXPORT dex_set_mapping_configuration_source_cyclic_stroke(DEX_EXCHANGER *exchanger, int conf_idx, int dimension_idx, double stroke);
int		_DEX_EXPORT dex_get_mapping_configuration_source_cyclic_stroke(DEX_EXCHANGER *exchanger, int conf_idx, int dimension_idx, double *stroke);
int		_DEX_EXPORT dex_set_mapping_configuration_source_registers(DEX_EXCHANGER *exchanger, int conf_idx, int dimension_idx, int reg_typ, int reg_idx, int reg_sidx);
int		_DEX_EXPORT dex_get_mapping_configuration_source_registers(DEX_EXCHANGER *exchanger, int conf_idx, int dimension_idx, int *reg_typ, int *reg_idx, int *reg_sidx);
int		_DEX_EXPORT dex_get_mapping_configuration_index(DEX_EXCHANGER *exchanger, int conf_nr, int *conf_idx);

/*-------------------------------*/
/* Info table exchanger functions */
/*-------------------------------*/
int		_DEX_EXPORT dex_get_nb_mapping_info_table(DEX_EXCHANGER *exchanger, int *nb_info);
int		_DEX_EXPORT dex_create_mapping_info_table(DEX_EXCHANGER *exchanger, int table_idx, int dimension);
int		_DEX_EXPORT dex_set_mapping_info_table_number(DEX_EXCHANGER *exchanger, int table_idx, int table_number);
int		_DEX_EXPORT dex_get_mapping_info_table_number(DEX_EXCHANGER *exchanger, int table_idx, int *table_number);
int		_DEX_EXPORT dex_get_mapping_info_table_correction_dimension(DEX_EXCHANGER *exchanger, int table_idx, int *correction_dimension);
int		_DEX_EXPORT dex_set_mapping_info_table_data_unit_factor(DEX_EXCHANGER *exchanger, int table_idx, int data_unit_factor);
int		_DEX_EXPORT dex_get_mapping_info_table_data_unit_factor(DEX_EXCHANGER *exchanger, int table_idx, int *data_unit_factor);
int		_DEX_EXPORT dex_set_mapping_info_table_step_size(DEX_EXCHANGER *exchanger, int table_idx, int dimension_idx, double step_size);
int		_DEX_EXPORT dex_get_mapping_info_table_step_size(DEX_EXCHANGER *exchanger, int table_idx, int dimension_idx, double *step_size);
int		_DEX_EXPORT dex_set_mapping_info_table_table_size(DEX_EXCHANGER *exchanger, int table_idx, int dimension_idx, int table_size);
int		_DEX_EXPORT dex_get_mapping_info_table_table_size(DEX_EXCHANGER *exchanger, int table_idx, int dimension_idx, int *table_size);
int		_DEX_EXPORT dex_get_mapping_info_table_index(DEX_EXCHANGER *exchanger, int info_table_nr, int *info_table_idx);

/*-------------------------------*/
/* Data table exchanger functions */
/*-------------------------------*/
int		_DEX_EXPORT dex_get_nb_mapping_data_table(DEX_EXCHANGER *exchanger, int *nb_mapping_data_table);
int		_DEX_EXPORT dex_create_mapping_data_table_buffer(DEX_EXCHANGER *exchanger, int table_idx, int buf_size, byte *buffer);
int		_DEX_EXPORT dex_create_mapping_data_table_buffer_separator(DEX_EXCHANGER *exchanger, int table_idx, int buf_size, byte *buffer, char *separator);
int		_DEX_EXPORT dex_set_mapping_data_table_number(DEX_EXCHANGER *exchanger, int table_idx, int table_number);
int		_DEX_EXPORT dex_get_mapping_data_table_number(DEX_EXCHANGER *exchanger, int table_idx, int *table_number);
int		_DEX_EXPORT dex_get_mapping_data_table_buffer(DEX_EXCHANGER *exchanger, int table_idx, int buf_size, byte *buf);
int		_DEX_EXPORT dex_create_mapping_data_table_1D_array(DEX_EXCHANGER *exchanger, int table_idx, double *values, int nb_column);
int		_DEX_EXPORT dex_get_mapping_data_table_1D_array_size(DEX_EXCHANGER *exchanger, int table_idx, int *nb_column);
int		_DEX_EXPORT dex_get_mapping_data_table_1D_array(DEX_EXCHANGER *exchanger, int table_idx, int buf_size_in_double, double *buf);
int		_DEX_EXPORT dex_create_mapping_data_table_2D_array(DEX_EXCHANGER *exchanger, int table_idx, double *values, int nb_raw, int nb_column);
int		_DEX_EXPORT dex_get_mapping_data_table_2D_array_size(DEX_EXCHANGER *exchanger, int table_idx, int *nb_raw, int *nb_column);
int		_DEX_EXPORT dex_get_mapping_data_table_2D_array(DEX_EXCHANGER *exchanger, int table_idx, int buf_size_in_double, double *buf);
int		_DEX_EXPORT dex_create_mapping_data_table_3D_array(DEX_EXCHANGER *exchanger, int table_idx, double *values, int nb_block, int nb_raw, int nb_column);
int		_DEX_EXPORT dex_get_mapping_data_table_3D_array_size(DEX_EXCHANGER *exchanger, int table_idx, int *nb_block, int *nb_raw, int *nb_column);
int		_DEX_EXPORT dex_get_mapping_data_table_3D_array(DEX_EXCHANGER *exchanger, int table_idx, int buf_size_in_double, double *buf);
int		_DEX_EXPORT dex_create_mapping_data_table_nD_array(DEX_EXCHANGER *exchanger, int table_idx, double *values, int nb_dim, int dim[]);
int		_DEX_EXPORT dex_get_mapping_data_table_double_array_size(DEX_EXCHANGER *exchanger, int table_idx, long *nb_values);
int		_DEX_EXPORT dex_get_mapping_data_table_double_array(DEX_EXCHANGER *exchanger, int table_idx, long buf_size_in_double, double *buf);
int		_DEX_EXPORT dex_set_mapping_data_table_1D_value(DEX_EXCHANGER *exchanger, int table_idx, int column_idx, double value);
int		_DEX_EXPORT dex_get_mapping_data_table_1D_value(DEX_EXCHANGER *exchanger, int table_idx, int column_idx, double *value);
int		_DEX_EXPORT dex_set_mapping_data_table_2D_value(DEX_EXCHANGER *exchanger, int table_idx, int raw_idx, int column_idx, double value);
int		_DEX_EXPORT dex_set_mapping_data_table_2D_values(DEX_EXCHANGER *exchanger, int table_idx, int raw_idx, int buf_size_in_double, double *buffer);
int		_DEX_EXPORT dex_set_mapping_data_table_2D_text_values(DEX_EXCHANGER *exchanger, int table_idx, int raw_idx, int buf_size, byte *buffer);
int		_DEX_EXPORT dex_get_mapping_data_table_2D_value(DEX_EXCHANGER *exchanger, int table_idx, int raw_idx, int column_idx, double *value);
int		_DEX_EXPORT dex_set_mapping_data_table_3D_value(DEX_EXCHANGER *exchanger, int table_idx, int block_idx, int raw_idx, int column_idx, double value);
int		_DEX_EXPORT dex_set_mapping_data_table_3D_values(DEX_EXCHANGER *exchanger, int table_idx, int block_idx, int buf_size_in_double, double *buffer);
int		_DEX_EXPORT dex_set_mapping_data_table_3D_text_values(DEX_EXCHANGER *exchanger, int table_idx, int block_idx, int buf_size, byte *buffer);
int		_DEX_EXPORT dex_get_mapping_data_table_3D_value(DEX_EXCHANGER *exchanger, int table_idx, int block_idx, int raw_idx, int column_idx, double *value);
int		_DEX_EXPORT dex_get_mapping_data_table_index(DEX_EXCHANGER *exchanger, int mapping_data_table_nr, int *mapping_data_table_idx);
int		_DEX_EXPORT dex_get_mapping_data_table_nb_dimension(DEX_EXCHANGER *exchanger, int table_idx, int *nb_dimension);

/*===================================*/
/* Time specific exchanger functions */
/*===================================*/
/*----------------------------------------------*/
/* Time Trace Configuration exchanger functions */
/*----------------------------------------------*/
int	_DEX_EXPORT dex_create_time_trace_configuration(DEX_EXCHANGER *exchanger, int nb_trace);
int	_DEX_EXPORT dex_set_time_trace_configuration_monitoring(DEX_EXCHANGER *exchanger, int trace_idx, int reg_typ, int reg_idx, int reg_sidx, int reg_axis);
int	_DEX_EXPORT dex_set_time_trace_configuration_name(DEX_EXCHANGER *exchanger, int trace_idx, const char* buf);
int	_DEX_EXPORT dex_set_time_trace_configuration_formula(DEX_EXCHANGER *exchanger, int trace_idx, const char* buf);
int	_DEX_EXPORT dex_set_time_trace_configuration_unit(DEX_EXCHANGER *exchanger, int trace_idx, const char* buf);
int	_DEX_EXPORT dex_set_time_trace_configuration_unit_type(DEX_EXCHANGER *exchanger, int trace_idx, DEX_UNIT_TYPE unit_typ);
int	_DEX_EXPORT dex_set_time_trace_configuration_conversion_factor(DEX_EXCHANGER *exchanger, int trace_idx, double factor);
int	_DEX_EXPORT dex_set_time_trace_configuration_acquisition_frequency(DEX_EXCHANGER *exchanger, int trace_idx, int frequency);
int	_DEX_EXPORT dex_get_time_trace_configuration_nb_trace(DEX_EXCHANGER *exchanger, int *nb_trace);
int	_DEX_EXPORT dex_is_time_trace_configuration_formula(DEX_EXCHANGER *exchanger, int trace_idx, ebool *formula);
int	_DEX_EXPORT dex_get_time_trace_configuration_formula(DEX_EXCHANGER *exchanger, int trace_idx, int buf_size, char *buf);
int	_DEX_EXPORT dex_get_time_trace_configuration_monitoring(DEX_EXCHANGER *exchanger, int trace_idx, int *reg_typ, int *reg_idx, int *reg_sidx, int *reg_axis);
int	_DEX_EXPORT dex_get_time_trace_configuration_name(DEX_EXCHANGER *exchanger, int trace_idx, int buf_size, char *buf);
int	_DEX_EXPORT dex_get_time_trace_configuration_unit(DEX_EXCHANGER *exchanger, int trace_idx, int buf_size, char *buf);
int	_DEX_EXPORT dex_get_time_trace_configuration_unit_type(DEX_EXCHANGER *exchanger, int trace_idx, DEX_UNIT_TYPE *unit_typ);
int	_DEX_EXPORT dex_get_time_trace_configuration_conversion_factor(DEX_EXCHANGER *exchanger, int trace_idx, double *factor);
int	_DEX_EXPORT dex_get_time_trace_configuration_acquisition_frequency(DEX_EXCHANGER *exchanger, int trace_idx, int *frequency);

/*------------------------------------------------*/
/* Time Trigger Configuration exchanger functions */
/*------------------------------------------------*/
int	_DEX_EXPORT dex_create_time_trigger_configuration(DEX_EXCHANGER *exchanger);
int	_DEX_EXPORT dex_set_time_trigger_type(DEX_EXCHANGER *exchanger, DEX_TIME_TRIGGER_TYPE trigger_type);
int	_DEX_EXPORT dex_set_time_trigger_BOM_axis_nb(DEX_EXCHANGER *exchanger, int axis_nb);
int	_DEX_EXPORT dex_set_time_trigger_BOM_interpolation_group(DEX_EXCHANGER *exchanger, int ipol_group);
int	_DEX_EXPORT dex_set_time_trigger_EOM_axis_nb(DEX_EXCHANGER *exchanger, int axis_nb);
int	_DEX_EXPORT dex_set_time_trigger_EOM_interpolation_group(DEX_EXCHANGER *exchanger, int ipol_group);
int	_DEX_EXPORT dex_set_time_trigger_POS_axis_nb(DEX_EXCHANGER *exchanger, int axis_nb);
int	_DEX_EXPORT dex_set_time_trigger_POS_value(DEX_EXCHANGER *exchanger, eint64 inc, double iso_to_inc_conversion_factor);
int	_DEX_EXPORT dex_set_time_trigger_POS_unit(DEX_EXCHANGER *exchanger, const char *unit);
int	_DEX_EXPORT dex_set_time_trigger_POS_edge(DEX_EXCHANGER *exchanger, DEX_TIME_TRIGGER_EDGE edge);
int	_DEX_EXPORT dex_set_time_trigger_TV_trace_idx(DEX_EXCHANGER *exchanger, int trace_idx);
int	_DEX_EXPORT dex_set_time_trigger_TV_value(DEX_EXCHANGER *exchanger, DEX_INC_VALUE inc, double iso_to_inc_conversion_factor);
int	_DEX_EXPORT dex_set_time_trigger_TV_unit(DEX_EXCHANGER *exchanger, const char *unit);
int	_DEX_EXPORT dex_set_time_trigger_TV_edge(DEX_EXCHANGER *exchanger, DEX_TIME_TRIGGER_EDGE edge);
int	_DEX_EXPORT dex_set_time_trigger_RV_register(DEX_EXCHANGER *exchanger, int reg_typ, int reg_idx, int reg_sidx, int axis);
int	_DEX_EXPORT dex_set_time_trigger_RV_value(DEX_EXCHANGER *exchanger, DEX_INC_VALUE inc, double iso_to_inc_conversion_factor);
int	_DEX_EXPORT dex_set_time_trigger_RV_unit(DEX_EXCHANGER *exchanger, const char *unit);
int	_DEX_EXPORT dex_set_time_trigger_RV_edge(DEX_EXCHANGER *exchanger, DEX_TIME_TRIGGER_EDGE edge);
int	_DEX_EXPORT dex_set_time_trigger_BFS_register(DEX_EXCHANGER *exchanger, int reg_typ, int reg_idx, int reg_sidx, int axis);
int	_DEX_EXPORT dex_set_time_trigger_BFS_low(DEX_EXCHANGER *exchanger, DEX_INC_VALUE inc);
int	_DEX_EXPORT dex_set_time_trigger_BFS_high(DEX_EXCHANGER *exchanger, DEX_INC_VALUE inc);
int	_DEX_EXPORT dex_set_time_trigger_BFC_register(DEX_EXCHANGER *exchanger, int reg_typ, int reg_idx, int reg_sidx, int axis);
int	_DEX_EXPORT dex_set_time_trigger_BFC_rising_edge(DEX_EXCHANGER *exchanger, DEX_INC_VALUE inc);
int	_DEX_EXPORT dex_set_time_trigger_BFC_falling_edge(DEX_EXCHANGER *exchanger, DEX_INC_VALUE inc);
int	_DEX_EXPORT dex_get_time_trigger_type(DEX_EXCHANGER *exchanger, DEX_TIME_TRIGGER_TYPE *trigger_type);
int	_DEX_EXPORT dex_get_time_trigger_BOM_axis_nb(DEX_EXCHANGER *exchanger, int *axis_nb);
int	_DEX_EXPORT dex_get_time_trigger_BOM_interpolation_group(DEX_EXCHANGER *exchanger, int *ipol_group);
int	_DEX_EXPORT dex_get_time_trigger_EOM_axis_nb(DEX_EXCHANGER *exchanger, int *axis_nb);
int	_DEX_EXPORT dex_get_time_trigger_EOM_interpolation_group(DEX_EXCHANGER *exchanger, int *ipol_group);
int	_DEX_EXPORT dex_get_time_trigger_POS_axis_nb(DEX_EXCHANGER *exchanger, int *axis_nb);
int	_DEX_EXPORT dex_get_time_trigger_POS_value(DEX_EXCHANGER *exchanger, eint64 *inc, double *iso_to_inc_conversion_factor);
int	_DEX_EXPORT dex_get_time_trigger_POS_unit(DEX_EXCHANGER *exchanger, int buf_size, char *buf);
int	_DEX_EXPORT dex_get_time_trigger_POS_edge(DEX_EXCHANGER *exchanger, DEX_TIME_TRIGGER_EDGE *edge);
int	_DEX_EXPORT dex_get_time_trigger_TV_trace_idx(DEX_EXCHANGER *exchanger, int *trace_idx);
int	_DEX_EXPORT dex_get_time_trigger_TV_value(DEX_EXCHANGER *exchanger, DEX_INC_VALUE *inc, double *iso_to_inc_conversion_factor);
int	_DEX_EXPORT dex_get_time_trigger_TV_unit(DEX_EXCHANGER *exchanger, int buf_size, char *buf);
int	_DEX_EXPORT dex_get_time_trigger_TV_edge(DEX_EXCHANGER *exchanger, DEX_TIME_TRIGGER_EDGE *edge);
int	_DEX_EXPORT dex_get_time_trigger_RV_register(DEX_EXCHANGER *exchanger, int *reg_typ, int *reg_idx, int *reg_sidx, int *axis);
int	_DEX_EXPORT dex_get_time_trigger_RV_value(DEX_EXCHANGER *exchanger, DEX_INC_VALUE *inc, double *iso_to_inc_conversion_factor);
int	_DEX_EXPORT dex_get_time_trigger_RV_unit(DEX_EXCHANGER *exchanger, int buf_size, char *buf);
int	_DEX_EXPORT dex_get_time_trigger_RV_edge(DEX_EXCHANGER *exchanger, DEX_TIME_TRIGGER_EDGE *edge);
int	_DEX_EXPORT dex_get_time_trigger_BFS_register(DEX_EXCHANGER *exchanger, int *reg_typ, int *reg_idx, int *reg_sidx, int *axis);
int	_DEX_EXPORT dex_get_time_trigger_BFS_low(DEX_EXCHANGER *exchanger, DEX_INC_VALUE *inc);
int	_DEX_EXPORT dex_get_time_trigger_BFS_high(DEX_EXCHANGER *exchanger, DEX_INC_VALUE *inc);
int	_DEX_EXPORT dex_get_time_trigger_BFC_register(DEX_EXCHANGER *exchanger, int *reg_typ, int *reg_idx, int *reg_sidx, int *axis);
int	_DEX_EXPORT dex_get_time_trigger_BFC_rising_edge(DEX_EXCHANGER *exchanger, DEX_INC_VALUE *inc);
int	_DEX_EXPORT dex_get_time_trigger_BFC_falling_edge(DEX_EXCHANGER *exchanger, DEX_INC_VALUE *inc);

/*----------------------------------------------------*/
/* Time Acquisition Configuration exchanger functions */
/*----------------------------------------------------*/
int	_DEX_EXPORT dex_create_time_acquisition_configuration(DEX_EXCHANGER *exchanger);
int	_DEX_EXPORT dex_set_time_acquisition_nb_points(DEX_EXCHANGER *exchanger, int nb_points);
int	_DEX_EXPORT dex_set_time_acquisition_frequency(DEX_EXCHANGER *exchanger, int frequency);
int	_DEX_EXPORT dex_set_time_acquisition_trigger_delay(DEX_EXCHANGER *exchanger, double delay);
int	_DEX_EXPORT dex_get_time_acquisition_nb_points(DEX_EXCHANGER *exchanger, int *nb_points);
int	_DEX_EXPORT dex_get_time_acquisition_frequency(DEX_EXCHANGER *exchanger, int *frequency);
int	_DEX_EXPORT dex_get_time_acquisition_trigger_delay(DEX_EXCHANGER *exchanger, double *delay);

/*-------------------------------------*/
/* Time Data Table exchanger functions */
/*-------------------------------------*/
int	_DEX_EXPORT dex_create_time_data_table(DEX_EXCHANGER *exchanger, int nb_row, int nb_col);
int	_DEX_EXPORT dex_set_time_data_table_values(DEX_EXCHANGER *exchanger, int nb_vals, DEX_INC_VALUE vals[]);
int	_DEX_EXPORT dex_set_time_data_table_col_values(DEX_EXCHANGER *exchanger, int col_idx, int nb_vals, DEX_INC_VALUE vals[]);
int	_DEX_EXPORT dex_set_time_data_table_row_values(DEX_EXCHANGER *exchanger, int row_idx, int nb_vals, DEX_INC_VALUE vals[]);
int	_DEX_EXPORT dex_set_time_data_table_value(DEX_EXCHANGER *exchanger, int row_idx, int col_idx, DEX_INC_VALUE vals);
int	_DEX_EXPORT dex_get_time_data_table_values(DEX_EXCHANGER *exchanger, int nb_vals, DEX_INC_VALUE vals[]);
int	_DEX_EXPORT dex_get_time_data_table_col_values(DEX_EXCHANGER *exchanger, int col_idx, int nb_vals, DEX_INC_VALUE vals[]);
int	_DEX_EXPORT dex_get_time_data_table_row_values(DEX_EXCHANGER *exchanger, int row_idx, int nb_vals, DEX_INC_VALUE vals[]);
int	_DEX_EXPORT dex_get_time_data_table_value(DEX_EXCHANGER *exchanger, int row_idx, int col_idx, DEX_INC_VALUE *val);


/*========================================*/
/* Frequency specific exchanger functions */
/*========================================*/
/*---------------------------------------------*/
/* Frequancy Configuration exchanger functions */
/*---------------------------------------------*/
int	_DEX_EXPORT dex_create_frequency_acquisition_configuration(DEX_EXCHANGER *exchanger, int nb_transfer_function);
int	_DEX_EXPORT dex_set_frequency_axis_nb(DEX_EXCHANGER *exchanger, int axis_nb);
int	_DEX_EXPORT dex_set_frequency_unit_type(DEX_EXCHANGER *exchanger, DEX_UNIT_TYPE unit_type);
int	_DEX_EXPORT dex_set_frequency_acquisition_nb_points(DEX_EXCHANGER *exchanger, int nb_points);
int	_DEX_EXPORT dex_set_frequency_transfer_function_type(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE transfer_function_type);
int	_DEX_EXPORT dex_set_frequency_transfer_function_io_registers(DEX_EXCHANGER *exchanger, int transfer_function_idx, char *io_registers);
int	_DEX_EXPORT dex_set_frequency_acquisition_quality_type(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE acquisition_quality_type);
int	_DEX_EXPORT dex_set_frequency_current_reference_noise(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_CURRENT_REFERENCE_NOISE current_reference_noise);
int	_DEX_EXPORT dex_set_frequency_identification_method(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_IDENTIFICATION_METHOD identification_method);
int	_DEX_EXPORT dex_set_frequency_identification_extra_value(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_IDENTIFICATION_EXTRA_VALUE extra_value_type, double extra_value);
int	_DEX_EXPORT dex_set_frequency_identification_regulation_parameter_value(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER regulation_parameter_type, double regulation_parameter_value);
int	_DEX_EXPORT dex_set_frequency_transfer_function_gain_unit(DEX_EXCHANGER *exchanger, int transfer_function_idx, char *gain_unit);
int	_DEX_EXPORT dex_set_frequency_transfer_function_phase_unit(DEX_EXCHANGER *exchanger, int transfer_function_idx, char *phase_unit);

int	_DEX_EXPORT dex_get_frequency_axis_number(DEX_EXCHANGER *exchanger, int *axis_nb);
int	_DEX_EXPORT dex_get_frequency_unit_type(DEX_EXCHANGER *exchanger, DEX_UNIT_TYPE *unit_type);
int	_DEX_EXPORT dex_get_frequency_acquisition_nb_points(DEX_EXCHANGER *exchanger, int *nb_points);
int	_DEX_EXPORT dex_get_frequency_nb_transfer_function(DEX_EXCHANGER *exchanger, int *nb_transfer_function);
int	_DEX_EXPORT dex_get_frequency_transfer_function_type(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE *transfer_function_type);
int	_DEX_EXPORT dex_get_frequency_transfer_function_io_registers(DEX_EXCHANGER *exchanger, int transfer_function_idx, char *io_registers);
int	_DEX_EXPORT dex_get_frequency_acquisition_quality_type(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE *acquisition_quality_type);
int	_DEX_EXPORT dex_get_frequency_current_reference_noise(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_CURRENT_REFERENCE_NOISE *current_reference_noise);
int	_DEX_EXPORT dex_get_frequency_identification_method(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_IDENTIFICATION_METHOD *identification_method);
int	_DEX_EXPORT dex_get_frequency_identification_extra_value(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_IDENTIFICATION_EXTRA_VALUE extra_value_type, double *extra_value);
int	_DEX_EXPORT dex_get_frequency_identification_regulation_parameter_value(DEX_EXCHANGER *exchanger, int transfer_function_idx, DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER regulation_parameter_type, double *regulation_parameter_value);
int	_DEX_EXPORT dex_get_frequency_transfer_function_gain_unit(DEX_EXCHANGER *exchanger, int transfer_function_idx, char *gain_unit);
int	_DEX_EXPORT dex_get_frequency_transfer_function_phase_unit(DEX_EXCHANGER *exchanger, int transfer_function_idx, char *phase_unit);

/*------------------------------------------*/
/* Frequency Data Table exchanger functions */
/*------------------------------------------*/
int	_DEX_EXPORT dex_create_frequency_data_table(DEX_EXCHANGER *exchanger, int nb_row, int nb_col);
int	_DEX_EXPORT dex_set_frequency_data_table_values(DEX_EXCHANGER *exchanger, int nb_vals, double vals[]);
int	_DEX_EXPORT dex_set_frequency_data_table_col_values(DEX_EXCHANGER *exchanger, int col_idx, int nb_vals, double vals[]);
int	_DEX_EXPORT dex_set_frequency_data_table_row_values(DEX_EXCHANGER *exchanger, int row_idx, int nb_vals, double vals[]);
int	_DEX_EXPORT dex_set_frequency_data_table_value(DEX_EXCHANGER *exchanger, int row_idx, int col_idx, double vals);
int	_DEX_EXPORT dex_get_frequency_data_table_values(DEX_EXCHANGER *exchanger, int nb_vals, double vals[]);
int	_DEX_EXPORT dex_get_frequency_data_table_col_values(DEX_EXCHANGER *exchanger, int col_idx, int nb_vals, double vals[]);
int	_DEX_EXPORT dex_get_frequency_data_table_row_values(DEX_EXCHANGER *exchanger, int row_idx, int nb_vals, double vals[]);
int	_DEX_EXPORT dex_get_frequency_data_table_value(DEX_EXCHANGER *exchanger, int row_idx, int col_idx, double *val);

/**@}*/

#ifdef __cplusplus
	} /* extern "C" */
#endif

/**********************************************************************************************************/
/*- C++ WRAPPER CLASSES */
/**********************************************************************************************************/

/*------------------------------------------------------------------------------
 * DEX constants - c++
 *-----------------------------------------------------------------------------*/

#ifdef DEX_OO_API

class Dex;
class DexException;
class DexExchanger;

/*------------------------------------------------------------------------------
 * generate exceptions from error codes
 *-----------------------------------------------------------------------------*/
#define DEX_ERRCHK(a) do { int _err = (a); if(_err) throw DexException(_err); } while(0)


/*------------------------------------------------------------------------------
 * DexException Class - c++
 *-----------------------------------------------------------------------------*/
class DexException {
    friend class Dex;
    friend class DexExchanger;

    /* error codes - c++ */
public:
	    enum {EBADPARAM = -2400 };                      /* one of the parameter is not valid */
    enum {EFERROR = -2404 };                        /* file access error */
    enum {EFSYNTAX = -2403 };                       /* file syntax error */
    enum {EINTERNAL = -2405 };                      /* some internal error in the etel software */
    enum {ENOTIMPLEMENTED = -2402 };                /* function not yet implemented */
    enum {ESYSTEM = -2401 };                        /* some system resource return an error */
    enum {EVERSION = -2406 };                       /* version not supported */




    /* exception code */
	private:
		int code;

    /* constructor */
	public:
		DexException(int e) { code = e; };

    /* translate error code */
	public: 
		static const char* translate(int code) {
			return dex_translate_error(code);
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

/*------------------------------------------------------------------------------
 * Dex Class - c++
 *-----------------------------------------------------------------------------*/
class Dex {
	public:
		static dword getVersion() {return dex_get_version();}
		static dword getLastDataFormatVersion() {return dex_get_last_data_format_version();}
};

/*------------------------------------------------------------------------------
 * DexExchanger Class - c++
 *-----------------------------------------------------------------------------*/
class DexExchanger {
	protected:
		DEX_EXCHANGER *exchanger;

	/* constructors */
	public:
		DexExchanger() {
			exchanger = NULL;
			DEX_ERRCHK(dex_create_exchanger(&exchanger));
		}
	public:
#if defined WIN32 || defined _WIN64
	#if _MSC_VER >= 1900		
		~DexExchanger(void) noexcept(false) {
	#else /* _MSC_VER >= 1900*/
		~DexExchanger(void) {
	#endif /* _MSC_VER >= 1900*/
#else
		~DexExchanger(void) {
#endif /*defined WIN32 || defined _WIN64*/
			DEX_ERRCHK(dex_destroy_exchanger(&exchanger));
		}

	public:
		void importFile(const char *fileName){
			DEX_ERRCHK(dex_import(exchanger, fileName));
		}
		void exportFile(const char* fileName) {
			DEX_ERRCHK(dex_export(exchanger, fileName));
		}
		void setSeparator(char separator) {
			DEX_ERRCHK(dex_set_separator(exchanger, separator));
		}

		void createGeneralInfo(DEX_DATA_TYPE type) {
			DEX_ERRCHK(dex_create_general_info(exchanger, type));
		}
		DEX_DATA_TYPE getGeneralInfoDataType() {
			DEX_DATA_TYPE type;
			DEX_ERRCHK(dex_get_general_info_data_type(exchanger, &type));
			return type;
		}
		void setGeneralInfoDataFormatVersion(dword version) {
			DEX_ERRCHK(dex_set_general_info_data_format_version(exchanger, version));
		}
		dword getGeneralInfoDataFormatVersion() {
			dword version = 0;
			DEX_ERRCHK(dex_get_general_info_data_format_version(exchanger, &version));
			return version;
		}
		void setGeneralInfoDate(const char* date) {
			DEX_ERRCHK(dex_set_general_info_date(exchanger, date));
		}
		void getGeneralInfoDate(int strSize, char *str) {
			DEX_ERRCHK(dex_get_general_info_date(exchanger, strSize, str));
		}
		void setGeneralInfoTime(const char* time) {
			DEX_ERRCHK(dex_set_general_info_time(exchanger, time));
		}
		void getGeneralInfoTime(int strSize, char *str) {
			DEX_ERRCHK(dex_get_general_info_time(exchanger, strSize, str));
		}
		void setGeneralInfoSoftwareName(const char* str) {
			DEX_ERRCHK(dex_set_general_info_software_name(exchanger, str));
		}
		void getGeneralInfoSoftwareName(int strSize, char *str) {
			DEX_ERRCHK(dex_get_general_info_software_name(exchanger, strSize, str));
		}
		void setGeneralInfoSoftwareVersion(dword version) {
			DEX_ERRCHK(dex_set_general_info_software_version(exchanger, version));
		}
		dword getGeneralInfoSoftwareVersion() {
			dword version = 0;
			DEX_ERRCHK(dex_get_general_info_software_version(exchanger, &version));
			return version;
		}
		void setGeneralInfoOperator(const char* str) {
			DEX_ERRCHK(dex_set_general_info_operator(exchanger, str));
		}
		void getGeneralInfoOperator(int strSize, char *str) {
			DEX_ERRCHK(dex_get_general_info_operator(exchanger, strSize, str));
		}
		void setGeneralInfoCustomer(const char* str) {
			DEX_ERRCHK(dex_set_general_info_customer(exchanger, str));
		}
		void getGeneralInfoCustomer(int strSize, char *str) {
			DEX_ERRCHK(dex_get_general_info_customer(exchanger, strSize, str));
		}
		void setGeneralInfoProject(const char *str) {
			DEX_ERRCHK(dex_set_general_info_project(exchanger, str));
		}
		void getGeneralInfoProject(int strSize, char *str) {
			DEX_ERRCHK(dex_get_general_info_project(exchanger, strSize, str));
		}
		void setGeneralInfoDescription(const char *str) {
			DEX_ERRCHK(dex_set_general_info_description(exchanger, str));
		}
		void getGeneralInfoDescription(int strSize, char *str) {
			DEX_ERRCHK(dex_get_general_info_description(exchanger, strSize, str));
		}
		void createControllerInfo(int nbController) {
			DEX_ERRCHK(dex_create_controller_info(exchanger, nbController));
		}
		int getControllerInfoControllerNumber() {
			int number = 0;
			DEX_ERRCHK(dex_get_controller_info_nb_controller(exchanger, &number));
			return number;
		}
		void setControllerInfoName(int controllerIdx, const char* buffer) {
			DEX_ERRCHK(dex_set_controller_info_name(exchanger, controllerIdx, buffer));
		}
		void getControllerInfoName(int controllerIdx, int strSize, char *str) {
			DEX_ERRCHK(dex_get_controller_info_name(exchanger, controllerIdx, strSize, str));
		}
		void setControllerInfoFirmware(int controllerIdx, dword firmwareVersion) {
			DEX_ERRCHK(dex_set_controller_info_firmware(exchanger, controllerIdx, firmwareVersion));
		}
		dword getControllerInfoFirmware(int controllerIdx) {
			dword number = 0;
			DEX_ERRCHK(dex_get_controller_info_firmware(exchanger, controllerIdx, &number));
			return number;
		}
		void setControllerInfoAddress(int controllerIdx, int address) {
			DEX_ERRCHK(dex_set_controller_info_address(exchanger, controllerIdx, address));
		}
		int getControllerInfoAddress(int controllerIdx) {
			int number = 0;
			DEX_ERRCHK(dex_get_controller_info_address(exchanger, controllerIdx, &number));
			return number;
		}
		void setControllerInfoSerialNumber(int controllerIdx, dword serialNumber) {
			DEX_ERRCHK(dex_set_controller_info_serial_number(exchanger, controllerIdx, serialNumber));
		}
		void setControllerInfoType(int controllerIdx, int type) {
			DEX_ERRCHK(dex_set_controller_info_type(exchanger, controllerIdx, type));
		}
		dword getControllerSerialNumber(int controllerIdx) {
			dword number = 0;
			DEX_ERRCHK(dex_get_controller_info_serial_number(exchanger, controllerIdx, &number));
			return number;
		}
		void setControllerInfoStatus(int controllerIdx, dword status) {
			DEX_ERRCHK(dex_set_controller_info_status(exchanger, controllerIdx, status));
		}
		dword getControllerStatus(int controllerIdx) {
			dword number = 0;
			DEX_ERRCHK(dex_get_controller_info_status(exchanger, controllerIdx, &number));
			return number;
		}
		int getControllerType(int controllerIdx) {
			int type = 0;
			DEX_ERRCHK(dex_get_controller_info_type(exchanger, controllerIdx, &type));
			return type;
		}
		int getControllerInfoIndex(int controllerNr) {
			int idx = 0;
			DEX_ERRCHK(dex_get_controller_info_index(exchanger, controllerNr, &idx));
			return idx;
		}
		void createMachineConfiguration(int nbAxis) {
			DEX_ERRCHK(dex_create_machine_configuration(exchanger, nbAxis));
		}
		int getMachineConfigurationNbAxis() {
			int number = 0;
			DEX_ERRCHK(dex_get_machine_configuration_nb_axis(exchanger, &number));
			return number;
		}
		void setMachineConfigurationMappingVersion(dword version) {
			DEX_ERRCHK(dex_set_machine_configuration_mapping_version(exchanger, version));
		}
		dword getMachineConfigurationMappingVersion() {
			dword number = 0;
			DEX_ERRCHK(dex_get_machine_configuration_mapping_version(exchanger, &number));
			return number;
		}
		void setMachineConfigurationMachineType(const char* machineType) {
			DEX_ERRCHK(dex_set_machine_configuration_machine_type(exchanger, machineType));
		}
		void getMachineConfigurationMachineType(int strSize, char *str) {
			DEX_ERRCHK(dex_get_machine_configuration_machine_type(exchanger, strSize, str));
		}
		void setMachineConfigurationMachineSerialNumber(const char* machineSerialNumber) {
			DEX_ERRCHK(dex_set_machine_configuration_machine_serial_number(exchanger, machineSerialNumber));
		}
		void getMachineConfigurationMachineSerialNumber(int strSize, char *str) {
			DEX_ERRCHK(dex_get_machine_configuration_machine_serial_number(exchanger, strSize, str));
		}
		void setMachineConfigurationMappingCorrectionMode(DEX_MAPPING_CORRECTION_MODE mode) {
			DEX_ERRCHK(dex_set_machine_configuration_mapping_correction_mode(exchanger, mode));
		}
		DEX_MAPPING_CORRECTION_MODE getMachineConfigurationMappingCorrectionMode() {
			DEX_MAPPING_CORRECTION_MODE mode;
			DEX_ERRCHK(dex_get_machine_configuration_mapping_correction_mode(exchanger, &mode));
			return mode;
		}
		void setMachineConfigurationAxisNameNumber(int axisIdx, const char*axisName, int axisNumber) {
			DEX_ERRCHK(dex_set_machine_configuration_axis_name_nr(exchanger, axisIdx, axisName, axisNumber));
		}
		void getMachineConfigurationAxisName(int axisIdx, int strSize, char *str) {
			int number = 0;
			DEX_ERRCHK(dex_get_machine_configuration_axis_name_nr(exchanger, axisIdx, strSize, str, &number));
		}
		int getMachineConfigurationAxisNumber(int axisIdx) {
			char s[33];
			int number = 0;
			DEX_ERRCHK(dex_get_machine_configuration_axis_name_nr(exchanger, axisIdx, 33, s, &number));
			return number;
		}
		int getNbMappingConfiguration() {
			int number = 0;
			DEX_ERRCHK(dex_get_nb_mapping_configuration(exchanger, &number));
			return number;
		}
		void createMappingConfiguration(int configIdx, int dimension) {
			DEX_ERRCHK(dex_create_mapping_configuration(exchanger, configIdx, dimension));
		}
		int getMappingConfigurationNumber(int configIdx) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_configuration_number(exchanger, configIdx, &number));
			return number;
		}
		void setMappingConfigurationNumber(int configIdx, int confNr) {
			DEX_ERRCHK(dex_set_mapping_configuration_number(exchanger, configIdx, confNr));
		}
		int getMappingConfigurationCorrectionDimension(int configIdx) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_configuration_correction_dimension(exchanger, configIdx, &number));
			return number;
		}
		void setMappingConfigurationCorrectedAxis(int configIdx, const char* buf) {
			DEX_ERRCHK(dex_set_mapping_configuration_corrected_axis(exchanger, configIdx, buf));
		}
		void getMappingConfigurationCorrectedAxis(int configIdx, int strSize, char *str) {
			DEX_ERRCHK(dex_get_mapping_configuration_corrected_axis(exchanger, configIdx, strSize, str));
		}
		void setMappingConfigurationCorrectionTable(int configIdx, int correctionTableIndex) {
			DEX_ERRCHK(dex_set_mapping_configuration_correction_table(exchanger, configIdx, correctionTableIndex));
		}
		int getMappingConfigurationCorrectionTable(int configIdx) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_configuration_correction_table(exchanger, configIdx, &number));
			return number;
		}
		void setMappingConfigurationSourceAxes(int configIdx, int dimensionIdx, const char* positionInput) {
			DEX_ERRCHK(dex_set_mapping_configuration_source_axes(exchanger, configIdx, dimensionIdx, positionInput));
		}
		void getMappingConfigurationSourceAxes(int configIdx, int dimensionIdx, int strSize, char *str) {
			DEX_ERRCHK(dex_get_mapping_configuration_source_axes(exchanger, configIdx, dimensionIdx, strSize, str));
		}
		void setMappingConfigurationSourceRegisters(int configIdx, int dimensionIdx, int regTyp, int regIdx, int regSidx) {
			DEX_ERRCHK(dex_set_mapping_configuration_source_registers(exchanger, configIdx, dimensionIdx, regTyp, regIdx, regSidx));
		}
		int getMappingConfigurationSourceRegistersTyp(int configIdx, int dimensionIdx) {
			int regTyp = 0;
			int dummy = 0;

			DEX_ERRCHK(dex_get_mapping_configuration_source_registers(exchanger, configIdx, dimensionIdx, &regTyp, &dummy, &dummy));
			return regTyp;
		}
		int getMappingConfigurationSourceRegistersIdx(int configIdx, int dimensionIdx) {
			int regIdx = 0;
			int dummy = 0;

			DEX_ERRCHK(dex_get_mapping_configuration_source_registers(exchanger, configIdx, dimensionIdx, &dummy, &regIdx, &dummy));
			return regIdx;
		}
		int getMappingConfigurationSourceRegistersSidx(int configIdx, int dimensionIdx) {
			int regSidx = 0;
			int dummy = 0;

			DEX_ERRCHK(dex_get_mapping_configuration_source_registers(exchanger, configIdx, dimensionIdx, &dummy, &dummy, &regSidx));
			return regSidx;
		}
		void setMappingConfigurationOrigin(int configIdx, int dimensionIdx, double origin) {
			DEX_ERRCHK(dex_set_mapping_configuration_origin(exchanger, configIdx, dimensionIdx, origin));
		}
		double getMappingConfigurationOrigin(int configIdx, int dimensionIdx) {
			double number = 0;
			DEX_ERRCHK(dex_get_mapping_configuration_origin(exchanger, configIdx, dimensionIdx, &number));
			return number;
		}
		void setMappingConfigurationSourceCyclicStroke(int configIdx, int dimensionIdx, double stroke) {
			DEX_ERRCHK(dex_set_mapping_configuration_source_cyclic_stroke(exchanger, configIdx, dimensionIdx, stroke));
		}
		double getMappingConfigurationSourceCyclicStroke(int configIdx, int dimensionIdx) {
			double number = 0;
			DEX_ERRCHK(dex_get_mapping_configuration_source_cyclic_stroke(exchanger, configIdx, dimensionIdx, &number));
			return number;
		}
		int getMappingConfigurationIndex(int configNr) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_configuration_index(exchanger, configNr, &number));
			return number;
		}
		int getNbMappingInfoTable() {
			int number = 0;
			DEX_ERRCHK(dex_get_nb_mapping_info_table(exchanger, &number));
			return number;
		}
		void createMappingInfoTable(int tableIdx, int dimension) {
			DEX_ERRCHK(dex_create_mapping_info_table(exchanger, tableIdx, dimension));
		}
		int getMappingInfoTableCorrectionDimension(int tableIdx) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_info_table_correction_dimension(exchanger, tableIdx, &number));
			return number;
		}
		void setMappingInfoTableNumber(int tableIdx, int tableNr) {
			DEX_ERRCHK(dex_set_mapping_info_table_number(exchanger, tableIdx, tableNr));
		}
		int getMappingInfoTableNumber(int tableIdx) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_info_table_number(exchanger, tableIdx, &number));
			return number;
		}
		void setMappingInfoTableDataUnitfactor(int tableIdx, int dataUnitFactor) {
			DEX_ERRCHK(dex_set_mapping_info_table_data_unit_factor(exchanger, tableIdx, dataUnitFactor));
		}
		int getMappingInfoTableDataUnitFactor(int tableIdx) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_info_table_data_unit_factor(exchanger, tableIdx, &number));
			return number;
		}
		void setMappingInfoTableStepSize(int tableIdx, int dimensionIdx, double stepSize) {
			DEX_ERRCHK(dex_set_mapping_info_table_step_size(exchanger, tableIdx, dimensionIdx, stepSize));
		}
		double getMappingInfoTableStepSize(int tableIdx, int dimensionIdx) {
			double number = 0;
			DEX_ERRCHK(dex_get_mapping_info_table_step_size(exchanger, tableIdx, dimensionIdx, &number));
			return number;
		}
		void setMappingInfoTableTableSize(int tableIdx, int dimensionIdx, int tableSize) {
			DEX_ERRCHK(dex_set_mapping_info_table_table_size(exchanger, tableIdx, dimensionIdx, tableSize));
		}
		int getMappingInfoTableTableSize(int tableIdx, int dimensionIdx) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_info_table_table_size(exchanger, tableIdx, dimensionIdx, &number));
			return number;
		}
		int getMappingInfoTableIndex(int MappingInfoTableNr) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_info_table_index(exchanger, MappingInfoTableNr, &number));
			return number;
		}
		int getNbMappingDataTable() {
			int number = 0;
			DEX_ERRCHK(dex_get_nb_mapping_data_table(exchanger, &number));
			return number;
		}
		void setMappingDataTableNumber(int tableIdx, int tableNr) {
			DEX_ERRCHK(dex_set_mapping_data_table_number(exchanger, tableIdx, tableNr));
		}
		int getMappingDataTableNumber(int tableIdx) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_data_table_number(exchanger, tableIdx, &number));
			return number;
		}
		void createMappingDataTableBuffer(int tableIdx, int size, byte *buffer) {
			DEX_ERRCHK(dex_create_mapping_data_table_buffer(exchanger, tableIdx, size, buffer));
		}
		void createMappingDataTableBufferSeparator(int tableIdx, int size, byte *buffer, char *separator) {
			DEX_ERRCHK(dex_create_mapping_data_table_buffer_separator(exchanger, tableIdx, size, buffer, separator));
		}
		void getMappingDataTableBuffer(int tableIdx, int size, byte *buffer) {
			DEX_ERRCHK(dex_get_mapping_data_table_buffer(exchanger, tableIdx, size, buffer));
		}
		void createMappingDataTable1DArray(int tableIdx, double *vals, int columnNumber) {
			DEX_ERRCHK(dex_create_mapping_data_table_1D_array(exchanger, tableIdx, vals, columnNumber));
		}
		void getMappingDataTable1DSize(int tableIdx, int *column) {
			DEX_ERRCHK(dex_get_mapping_data_table_1D_array_size(exchanger, tableIdx, column));
		}
		void getMappingDataTable1DArray(int tableIdx, int sizeInDouble, double* val) {
			DEX_ERRCHK(dex_get_mapping_data_table_1D_array(exchanger, tableIdx, sizeInDouble, val));
		}
		void createMappingDataTable2DArray(int tableIdx, double *vals, int rawNumber, int columnNumber) {
			DEX_ERRCHK(dex_create_mapping_data_table_2D_array(exchanger, tableIdx, vals, rawNumber, columnNumber));
		}
		void getMappingDataTable2DSize(int tableIdx, int *raw, int *column) {
			DEX_ERRCHK(dex_get_mapping_data_table_2D_array_size(exchanger, tableIdx, raw, column));
		}
		void getMappingDataTable2DArray(int tableIdx, int sizeInDouble, double* val) {
			DEX_ERRCHK(dex_get_mapping_data_table_2D_array(exchanger, tableIdx, sizeInDouble, val));
		}
		void createMappingDataTable3DArray(int tableIdx, double *vals, int blockNumber, int rawNumber, int columnNumber) {
			DEX_ERRCHK(dex_create_mapping_data_table_3D_array(exchanger, tableIdx, vals, blockNumber, rawNumber, columnNumber));
		}
		void getMappingDataTable3DSize(int tableIdx, int *nbBlock, int *raw, int *column) {
			DEX_ERRCHK(dex_get_mapping_data_table_3D_array_size(exchanger, tableIdx, nbBlock, raw, column));
		}
		void getMappingDataTable3DArray(int tableIdx, int sizeInDouble, double* val) {
			DEX_ERRCHK(dex_get_mapping_data_table_3D_array(exchanger, tableIdx, sizeInDouble, val));
		}
		void getMappingDataTableDoubleArraySize(int tableIdx, long *nbValues) {
			DEX_ERRCHK(dex_get_mapping_data_table_double_array_size(exchanger, tableIdx, nbValues));
		}
		void getMappingDataTableDoubleArray(int tableIdx, int sizeInDouble, double* val) {
			DEX_ERRCHK(dex_get_mapping_data_table_double_array(exchanger, tableIdx, sizeInDouble, val));
		}
		void createMappingDataTablenDArray(int tableIdx, double *vals, int nbDims, int dimensionSize[]) {
			DEX_ERRCHK(dex_create_mapping_data_table_nD_array(exchanger, tableIdx, vals, nbDims, dimensionSize));
		}
		void setMappingDataTable1DValue(int tableIdx, int columnIdx, double value) {
			DEX_ERRCHK(dex_set_mapping_data_table_1D_value(exchanger, tableIdx, columnIdx, value));
		}
		double getMappingDataTable1DValue(int tableIdx, int columnIdx) {
			double value = 0;
			DEX_ERRCHK(dex_get_mapping_data_table_1D_value(exchanger, tableIdx, columnIdx, &value));
			return value;
		}
		void setMappingDataTable2DValue(int tableIdx, int rawIdx, int columnIdx, double value) {
			DEX_ERRCHK(dex_set_mapping_data_table_2D_value(exchanger, tableIdx, rawIdx, columnIdx, value));
		}
		void setMappingDataTable2DValues(int tableIdx, int rawIdx, int sizeInDouble, double* val) {
			DEX_ERRCHK(dex_set_mapping_data_table_2D_values(exchanger, tableIdx, rawIdx, sizeInDouble, val));
		}
		void setMappingDataTable2DTextValues(int tableIdx, int rawIdx, int size, byte* val) {
			DEX_ERRCHK(dex_set_mapping_data_table_2D_text_values(exchanger, tableIdx, rawIdx, size, val));
		}
		double getMappingDataTable2DValue(int tableIdx, int rawIdx, int columnIdx) {
			double value = 0;
			DEX_ERRCHK(dex_get_mapping_data_table_2D_value(exchanger, tableIdx, rawIdx, columnIdx, &value));
			return value;
		}
		void setMappingDataTable3DValue(int tableIdx, int blockIdx, int rawIdx, int columnIdx, double value) {
			DEX_ERRCHK(dex_set_mapping_data_table_3D_value(exchanger, tableIdx, blockIdx, rawIdx, columnIdx, value));
		}
		double getMappingDataTable3DValue(int tableIdx, int blockIdx, int rawIdx, int columnIdx) {
			double value = 0;
			DEX_ERRCHK(dex_get_mapping_data_table_3D_value(exchanger, tableIdx, blockIdx, rawIdx, columnIdx, &value));
			return value;
		}
		void setMappingDataTable3DValues(int tableIdx, int blockIdx, int sizeInDouble, double* val) {
			DEX_ERRCHK(dex_set_mapping_data_table_3D_values(exchanger, tableIdx, blockIdx, sizeInDouble, val));
		}
		void setMappingDataTable3DTextValues(int tableIdx, int blockIdx, int size, byte* val) {
			DEX_ERRCHK(dex_set_mapping_data_table_3D_text_values(exchanger, tableIdx, blockIdx, size, val));
		}
		int getMappingDataTableIndex(int dataTableNr) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_data_table_index(exchanger, dataTableNr, &number));
			return number;
		}
		int getMappingDataTableNbDimension(int tableIdx) {
			int number = 0;
			DEX_ERRCHK(dex_get_mapping_data_table_nb_dimension(exchanger, tableIdx, &number));
			return number;
		}
		void createTimeTraceConfiguration(int nbTrace) {
			DEX_ERRCHK(dex_create_time_trace_configuration(exchanger, nbTrace));
		}
		void setTimeTraceConfigurationMonitoring(int traceIdx, int regTyp, int regIdx, int regSidx, int regAxis) {
			DEX_ERRCHK(dex_set_time_trace_configuration_monitoring(exchanger, traceIdx, regTyp, regIdx, regSidx, regAxis));
		}
		void setTimeTraceConfigurationName(int traceIdx, const char* buf) {
			DEX_ERRCHK(dex_set_time_trace_configuration_name(exchanger, traceIdx, buf));
		}
		void setTimeTraceConfigurationFormula(int traceIdx, const char* buf) {
			DEX_ERRCHK(dex_set_time_trace_configuration_formula(exchanger, traceIdx, buf));
		}
		void setTimeTraceConfigurationUnit(int traceIdx, const char* buf) {
			DEX_ERRCHK(dex_set_time_trace_configuration_unit(exchanger, traceIdx, buf));
		}
		void setTimeTraceConfigurationUnitType(int traceIdx, DEX_UNIT_TYPE unitTyp) {
			DEX_ERRCHK(dex_set_time_trace_configuration_unit_type(exchanger, traceIdx, unitTyp));
		}
		void setTimeTraceConfigurationConversionFactor(int traceIdx, double convFactor) {
			DEX_ERRCHK(dex_set_time_trace_configuration_conversion_factor(exchanger, traceIdx, convFactor));
		}
		void setTimeTraceConfigurationAcquisitionFrequency(int traceIdx, int frequency) {
			DEX_ERRCHK(dex_set_time_trace_configuration_acquisition_frequency(exchanger, traceIdx, frequency));
		}
		int getTimeTraceConfigurationNbTrace() {
			int val;
			DEX_ERRCHK(dex_get_time_trace_configuration_nb_trace(exchanger, &val));
			return val;
		}
		ebool isTimeTraceConfigurationFormula(int traceIdx)
		{
			ebool formula;
			DEX_ERRCHK(dex_is_time_trace_configuration_formula(exchanger, traceIdx, &formula));
			return formula;
		}
		void getTimeTraceConfigurationFormula(int traceIdx, int strSize, char *str) {
			DEX_ERRCHK(dex_get_time_trace_configuration_formula(exchanger, traceIdx, strSize, str));
		}
		int getTimeTraceConfigurationMonitoringTyp(int traceIdx) {
			int val;
			DEX_ERRCHK(dex_get_time_trace_configuration_monitoring(exchanger, traceIdx, &val, NULL, NULL, NULL));
			return val;
		}
		int getTimeTraceConfigurationMonitoringIdx(int traceIdx) {
			int val;
			DEX_ERRCHK(dex_get_time_trace_configuration_monitoring(exchanger, traceIdx, NULL, &val, NULL, NULL));
			return val;
		}
		int getTimeTraceConfigurationMonitoringSidx(int traceIdx) {
			int val;
			DEX_ERRCHK(dex_get_time_trace_configuration_monitoring(exchanger, traceIdx, NULL, NULL, &val, NULL));
			return val;
		}
		int getTimeTraceConfigurationMonitoringAxis(int traceIdx) {
			int val;
			DEX_ERRCHK(dex_get_time_trace_configuration_monitoring(exchanger, traceIdx, NULL, NULL, NULL, &val));
			return val;
		}
		void getTimeTraceConfigurationName(int traceIdx, int strSize, char *str) {
			DEX_ERRCHK(dex_get_time_trace_configuration_name(exchanger, traceIdx, strSize, str));
		}
		void getTimeTraceConfigurationUnit(int traceIdx, int strSize, char *str) {
			DEX_ERRCHK(dex_get_time_trace_configuration_unit(exchanger, traceIdx, strSize, str));
		}
		DEX_UNIT_TYPE getTimeTraceConfigurationUnitType(int traceIdx) {
			DEX_UNIT_TYPE typ;
			DEX_ERRCHK(dex_get_time_trace_configuration_unit_type(exchanger, traceIdx, &typ));
			return typ;
		}
		double getTimeTraceConfigurationConversionFactor(int traceIdx) {
			double val;
			DEX_ERRCHK(dex_get_time_trace_configuration_conversion_factor(exchanger, traceIdx, &val));
			return val;
		}
		int getTimeTraceConfigurationFrequency(int traceIdx) {
			int val;
			DEX_ERRCHK(dex_get_time_trace_configuration_acquisition_frequency(exchanger, traceIdx, &val));
			return val;
		}
		void createTimeTriggerConfiguration() {
			DEX_ERRCHK(dex_create_time_trigger_configuration(exchanger));
		}
		void setTimeTriggerType(DEX_TIME_TRIGGER_TYPE triggerType) {
			DEX_ERRCHK(dex_set_time_trigger_type(exchanger, triggerType));
		}
		void setTimeTriggerBOMAxisNb(int axisNb) {
			DEX_ERRCHK(dex_set_time_trigger_BOM_axis_nb(exchanger, axisNb));
		}
		void setTimeTriggerBOMInterpolationGroup(int ipolGroup) {
			DEX_ERRCHK(dex_set_time_trigger_BOM_interpolation_group(exchanger, ipolGroup));
		}
		void setTimeTriggerEOMAxisNb(int axisNb) {
			DEX_ERRCHK(dex_set_time_trigger_EOM_axis_nb(exchanger, axisNb));
		}
		void setTimeTriggerEOMInterpolationGroup(int ipolGroup) {
			DEX_ERRCHK(dex_set_time_trigger_EOM_interpolation_group(exchanger, ipolGroup));
		}
		void setTimeTriggerPOSAxisNb(int axisNb) {
			DEX_ERRCHK(dex_set_time_trigger_POS_axis_nb(exchanger, axisNb));
		}
		void setTimeTriggerPOSValue(eint64 inc, double incToIsoConversionFactor) {
			DEX_ERRCHK(dex_set_time_trigger_POS_value(exchanger, inc, incToIsoConversionFactor));
		}
		void setTimeTriggerPOSUnit(const char* unit) {
			DEX_ERRCHK(dex_set_time_trigger_POS_unit(exchanger, unit));
		}
		void setTimeTriggerPOSEdge(DEX_TIME_TRIGGER_EDGE edge) {
			DEX_ERRCHK(dex_set_time_trigger_POS_edge(exchanger, edge));
		}
		void setTimeTriggerTVTraceIdx(int traceIdx) {
			DEX_ERRCHK(dex_set_time_trigger_TV_trace_idx(exchanger, traceIdx));
		}
		void setTimeTriggerTVValue(DEX_INC_VALUE inc, double incToIsoConversionFactor) {
			DEX_ERRCHK(dex_set_time_trigger_TV_value(exchanger, inc, incToIsoConversionFactor));
		}
		void setTimeTriggerTVUnit(const char* unit) {
			DEX_ERRCHK(dex_set_time_trigger_TV_unit(exchanger, unit));
		}
		void setTimeTriggerTVEdge(DEX_TIME_TRIGGER_EDGE edge) {
			DEX_ERRCHK(dex_set_time_trigger_TV_edge(exchanger, edge));
		}
		void setTimeTriggerRVRegister(int regTyp, int regIdx, int regSidx, int regAxis) {
			DEX_ERRCHK(dex_set_time_trigger_RV_register(exchanger, regTyp, regIdx, regSidx, regAxis));
		}
		void setTimeTriggerRVValue(DEX_INC_VALUE inc, double incToIsoConversionFactor) {
			DEX_ERRCHK(dex_set_time_trigger_RV_value(exchanger, inc, incToIsoConversionFactor));
		}
		void setTimeTriggerRVUnit(const char* unit) {
			DEX_ERRCHK(dex_set_time_trigger_RV_unit(exchanger, unit));
		}
		void setTimeTriggerRVEdge(DEX_TIME_TRIGGER_EDGE edge) {
			DEX_ERRCHK(dex_set_time_trigger_RV_edge(exchanger, edge));
		}
		void setTimeTriggerBFSRegister(int regTyp, int regIdx, int regSidx, int regAxis) {
			DEX_ERRCHK(dex_set_time_trigger_BFS_register(exchanger, regTyp, regIdx, regSidx, regAxis));
		}
		void setTimeTriggerBFSLow(DEX_INC_VALUE inc) {
			DEX_ERRCHK(dex_set_time_trigger_BFS_low(exchanger, inc));
		}
		void setTimeTriggerBFSHigh(DEX_INC_VALUE inc) {
			DEX_ERRCHK(dex_set_time_trigger_BFS_high(exchanger, inc));
		}
		void setTimeTriggerBFCRegister(int regTyp, int regIdx, int regSidx, int regAxis) {
			DEX_ERRCHK(dex_set_time_trigger_BFC_register(exchanger, regTyp, regIdx, regSidx, regAxis));
		}
		void setTimeTriggerBFCRisingEdge(DEX_INC_VALUE inc) {
			DEX_ERRCHK(dex_set_time_trigger_BFC_rising_edge(exchanger, inc));
		}
		void setTimeTriggerBFCFallingEdge(DEX_INC_VALUE inc) {
			DEX_ERRCHK(dex_set_time_trigger_BFC_falling_edge(exchanger, inc));
		}
		DEX_TIME_TRIGGER_TYPE getTimeTriggerType() {
			DEX_TIME_TRIGGER_TYPE val;
			DEX_ERRCHK(dex_get_time_trigger_type(exchanger, &val));
			return val;
		}
		int getTimeTriggerBOMAxisNb() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_BOM_axis_nb(exchanger, &val));
			return val;
		}
		int getTimeTriggerBOMInterpolationGroup() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_BOM_interpolation_group(exchanger, &val));
			return val;
		}
		int getTimeTriggerEOMAxisNb() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_EOM_axis_nb(exchanger, &val));
			return val;
		}
		int getTimeTriggerEOMInterpolationGroup() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_EOM_interpolation_group(exchanger, &val));
			return val;
		}
		int getTimeTriggerPOSAxisNb() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_POS_axis_nb(exchanger, &val));
			return val;
		}
		eint64 getTimeTriggerPOSValue() {
			eint64 val;
			DEX_ERRCHK(dex_get_time_trigger_POS_value(exchanger, &val, NULL));
			return val;
		}
		double getTimeTriggerPOSConversionFactor() {
			double val;
			DEX_ERRCHK(dex_get_time_trigger_POS_value(exchanger, NULL, &val));
			return val;
		}
		void getTimeTriggerPOSUnit(int strSize, char *str) {
			DEX_ERRCHK(dex_get_time_trigger_POS_unit(exchanger, strSize, str));
		}
		DEX_TIME_TRIGGER_EDGE getTimeTriggerPOSEdge() {
			DEX_TIME_TRIGGER_EDGE val;
			DEX_ERRCHK(dex_get_time_trigger_POS_edge(exchanger, &val));
			return val;
		}
		int getTimeTriggerTVTraceIdx() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_TV_trace_idx(exchanger, &val));
			return val;
		}
		void getTimeTriggerTVValue(DEX_INC_VALUE *val) {
			DEX_ERRCHK(dex_get_time_trigger_TV_value(exchanger, val, NULL));
		}
		double getTimeTriggerTVConversionFactor() {
			double val;
			DEX_ERRCHK(dex_get_time_trigger_TV_value(exchanger, NULL, &val));
			return val;
		}
		void getTimeTriggerTVUnit(int strSize, char *str) {
			DEX_ERRCHK(dex_get_time_trigger_TV_unit(exchanger, strSize, str));
		}
		DEX_TIME_TRIGGER_EDGE getTimeTriggerTVEdge() {
			DEX_TIME_TRIGGER_EDGE val;
			DEX_ERRCHK(dex_get_time_trigger_TV_edge(exchanger, &val));
			return val;
		}
		int getTimeTriggerRVRegisterTyp() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_RV_register(exchanger, &val, NULL, NULL, NULL));
			return val;
		}
		int getTimeTriggerRVRegisterIdx() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_RV_register(exchanger, NULL, &val, NULL, NULL));
			return val;
		}
		int getTimeTriggerRVRegisterSidx() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_RV_register(exchanger, NULL, NULL, &val, NULL));
			return val;
		}
		int getTimeTriggerRVRegisterAxis() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_RV_register(exchanger, NULL, NULL, NULL, &val));
			return val;
		}
		void getTimeTriggerRVValue(DEX_INC_VALUE *val) {
			DEX_ERRCHK(dex_get_time_trigger_RV_value(exchanger, val, NULL));
		}
		double getTimeTriggerRVConversionFactor() {
			double val;
			DEX_ERRCHK(dex_get_time_trigger_RV_value(exchanger, NULL, &val));
			return val;
		}
		void getTimeTriggerRVUnit(int strSize, char *str) {
			DEX_ERRCHK(dex_get_time_trigger_RV_unit(exchanger, strSize, str));
		}
		DEX_TIME_TRIGGER_EDGE getTimeTriggerRVEdge() {
			DEX_TIME_TRIGGER_EDGE val;
			DEX_ERRCHK(dex_get_time_trigger_RV_edge(exchanger, &val));
			return val;
		}
		int getTimeTriggerBFSRegisterTyp() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_BFS_register(exchanger, &val, NULL, NULL, NULL));
			return val;
		}
		int getTimeTriggerBFSRegisterIdx() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_BFS_register(exchanger, NULL, &val, NULL, NULL));
			return val;
		}
		int getTimeTriggerBFSRegisterSidx() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_BFS_register(exchanger, NULL, NULL, &val, NULL));
			return val;
		}
		int getTimeTriggerBFSRegisterAxis() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_BFS_register(exchanger, NULL, NULL, NULL, &val));
			return val;
		}
		void getTimeTriggerBFSLow(DEX_INC_VALUE *val) {
			DEX_ERRCHK(dex_get_time_trigger_BFS_low(exchanger, val));
		}
		void getTimeTriggerBFSHigh(DEX_INC_VALUE *val) {
			DEX_ERRCHK(dex_get_time_trigger_BFS_high(exchanger, val));
		}
		int getTimeTriggerBFCRegisterTyp() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_BFC_register(exchanger, &val, NULL, NULL, NULL));
			return val;
		}
		int getTimeTriggerBFCRegisterIdx() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_BFC_register(exchanger, NULL, &val, NULL, NULL));
			return val;
		}
		int getTimeTriggerBFCRegisterSidx() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_BFC_register(exchanger, NULL, NULL, &val, NULL));
			return val;
		}
		int getTimeTriggerBFCRegisterAxis() {
			int val;
			DEX_ERRCHK(dex_get_time_trigger_BFC_register(exchanger, NULL, NULL, NULL, &val));
			return val;
		}
		void getTimeTriggerBFCRisingEdge(DEX_INC_VALUE *val) {
			DEX_ERRCHK(dex_get_time_trigger_BFC_rising_edge(exchanger, val));
		}
		void getTimeTriggerBFCFallingEdge(DEX_INC_VALUE *val) {
			DEX_ERRCHK(dex_get_time_trigger_BFC_falling_edge(exchanger, val));
		}
		void createTimeAcquisitionConfiguration() {
			DEX_ERRCHK(dex_create_time_acquisition_configuration(exchanger));
		}
		void setTimeAcquisitionNbPoints(int nbPoints) {
			DEX_ERRCHK(dex_set_time_acquisition_nb_points(exchanger, nbPoints));
		}
		void setTimeAcquisitionFrequency(int frequency) {
			DEX_ERRCHK(dex_set_time_acquisition_frequency(exchanger, frequency));
		}
		void setTimeAcquisitionDelay(double delay) {
			DEX_ERRCHK(dex_set_time_acquisition_trigger_delay(exchanger, delay));
		}
		int getTimeAcquisitionNbPoints() {
			int val;
			DEX_ERRCHK(dex_get_time_acquisition_nb_points(exchanger, &val));
			return val;
		}
		int getTimeAcquisitionFrequency() {
			int val;
			DEX_ERRCHK(dex_get_time_acquisition_frequency(exchanger, &val));
			return val;
		}
		double getTimeAcquisitionTriggerDelay() {
			double val;
			DEX_ERRCHK(dex_get_time_acquisition_trigger_delay(exchanger, &val));
			return val;
		}
		void createTimeDataTable(int nbRows, int nbCols) {
			DEX_ERRCHK(dex_create_time_data_table(exchanger, nbRows, nbCols));
		}
		void setTimeDataTableValues(int nbVals, DEX_INC_VALUE vals[]) {
			DEX_ERRCHK(dex_set_time_data_table_values(exchanger, nbVals, vals));
		}
		void setTimeDataTableColValues(int colIdx, int nbVals, DEX_INC_VALUE vals[]) {
			DEX_ERRCHK(dex_set_time_data_table_col_values(exchanger, colIdx, nbVals, vals));
		}
		void setTimeDataTableRowValues(int rowIdx, int nbVals, DEX_INC_VALUE vals[]) {
			DEX_ERRCHK(dex_set_time_data_table_row_values(exchanger, rowIdx, nbVals, vals));
		}
		void setTimeDataTableValue(int rowIdx, int colIdx, DEX_INC_VALUE val) {
			DEX_ERRCHK(dex_set_time_data_table_value(exchanger, rowIdx, colIdx, val));
		}
		void getTimeDataTableValues(int nbVals, DEX_INC_VALUE vals[]) {
			DEX_ERRCHK(dex_get_time_data_table_values(exchanger, nbVals, vals));
		}
		void getTimeDataTableColValues(int colIdx, int nbVals, DEX_INC_VALUE vals[]) {
			DEX_ERRCHK(dex_get_time_data_table_col_values(exchanger, colIdx, nbVals, vals));
		}
		void getTimeDataTableRowValues(int rowIdx, int nbVals, DEX_INC_VALUE vals[]) {
			DEX_ERRCHK(dex_get_time_data_table_row_values(exchanger, rowIdx, nbVals, vals));
		}
		void getTimeDataTableValue(int rowIdx, int colIdx, DEX_INC_VALUE *val) {
			DEX_ERRCHK(dex_get_time_data_table_value(exchanger, rowIdx, colIdx, val));
		}
		void createFrequencyAcquisitionConfiguration(int nbTransferFunction) {
			DEX_ERRCHK(dex_create_frequency_acquisition_configuration(exchanger, nbTransferFunction));
		}
		void setFrequencyAxisNb(int axisNumber) {
			DEX_ERRCHK(dex_set_frequency_axis_nb(exchanger, axisNumber));
		}
		void setFrequencyUnitType(DEX_UNIT_TYPE unitType) {
			DEX_ERRCHK(dex_set_frequency_unit_type(exchanger, unitType));
		}
		void setFrequencyNbPoints(int nbPoints) {
			DEX_ERRCHK(dex_set_frequency_acquisition_nb_points(exchanger, nbPoints));
		}
		void setFrequencyTransferFunctionType(int transferFunctionIdx, DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE transferFunctionType) {
			DEX_ERRCHK(dex_set_frequency_transfer_function_type(exchanger, transferFunctionIdx, transferFunctionType));
		}
		void setFrequencyTransferFunctionIoRegisters(int transferFunctionIdx, char *ioRegisters) {
			DEX_ERRCHK(dex_set_frequency_transfer_function_io_registers(exchanger, transferFunctionIdx, ioRegisters));
		}
		void setFrequencyAcquisitionQualityType(int transferFunctionIdx, DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE qualityType) {
			DEX_ERRCHK(dex_set_frequency_acquisition_quality_type(exchanger, transferFunctionIdx, qualityType));
		}      
		void setFrequencyCurrentReferenceNoise(int transferFunctionIdx, DEX_FREQUENCY_CURRENT_REFERENCE_NOISE currentReferenceNoise) {
			DEX_ERRCHK(dex_set_frequency_current_reference_noise(exchanger, transferFunctionIdx, currentReferenceNoise));
		}
		void setFrequencyIdentificationMethod(int transferFunctionIdx, DEX_FREQUENCY_IDENTIFICATION_METHOD identificationMethod) {
			DEX_ERRCHK(dex_set_frequency_identification_method(exchanger, transferFunctionIdx, identificationMethod));
		}
		void setFrequencyIdentificationExtraValue(int transferFunctionIdx, DEX_FREQUENCY_IDENTIFICATION_EXTRA_VALUE extraValueType, double extraValue) {
			DEX_ERRCHK(dex_set_frequency_identification_extra_value(exchanger, transferFunctionIdx, extraValueType, extraValue));
		}
		void setFrequencyIdentificationRegulationParameterValue(int transferFunctionIdx, DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER regulationParameterType, double regulationParameterValue) {
			DEX_ERRCHK(dex_set_frequency_identification_regulation_parameter_value(exchanger, transferFunctionIdx, regulationParameterType, regulationParameterValue));
		}
		void setFrequencyTransferFunctionGainUnit(int transferFunctionIdx, char *gain_unit) {
			DEX_ERRCHK(dex_set_frequency_transfer_function_gain_unit(exchanger, transferFunctionIdx, gain_unit));
		}
		void setFrequencyTransferFunctionPhaseUnit(int transferFunctionIdx, char *phase_unit) {
			DEX_ERRCHK(dex_set_frequency_transfer_function_phase_unit(exchanger, transferFunctionIdx, phase_unit));
		}		
		
		int getFrequencyAxisNb() {
			int val;
			DEX_ERRCHK(dex_get_frequency_axis_number(exchanger, &val));
			return val;
		}
		DEX_UNIT_TYPE getFrequencyUnitType() {
			DEX_UNIT_TYPE val;
			DEX_ERRCHK(dex_get_frequency_unit_type(exchanger, &val));
			return val;
		}
		int getFrequencyAcquisitionNbPoints() {
			int val;
			DEX_ERRCHK(dex_get_frequency_acquisition_nb_points(exchanger, &val));
			return val;
		}
		int getFrequencyNbTransferFunction() {
			int val;
			DEX_ERRCHK(dex_get_frequency_nb_transfer_function(exchanger, &val));
			return val;
		}
		DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE getFrequencyTransferFunctionType(int transferFunctionIdx) {
			DEX_FREQUENCY_TRANSFER_FUNCTION_TYPE val;
			DEX_ERRCHK(dex_get_frequency_transfer_function_type(exchanger, transferFunctionIdx, &val));
			return val;
		}
		void getFrequencyTransferFunctionIoRegisters(int transferFunctionIdx, char *ioRegisters) {
			DEX_ERRCHK(dex_get_frequency_transfer_function_io_registers(exchanger, transferFunctionIdx, ioRegisters));
		}
		DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE getFrequencyAcquisitionQualityType(int transferFunctionIdx) {
			DEX_FREQUENCY_ACQUISITION_QUALITY_TYPE val;
			DEX_ERRCHK(dex_get_frequency_acquisition_quality_type(exchanger, transferFunctionIdx, &val));
			return val;
		}
		DEX_FREQUENCY_CURRENT_REFERENCE_NOISE getFrequencyCurrentReferenceNoise(int transferFunctionIdx) {
			DEX_FREQUENCY_CURRENT_REFERENCE_NOISE val;
			DEX_ERRCHK(dex_get_frequency_current_reference_noise(exchanger, transferFunctionIdx, &val));
			return val;
		}
		DEX_FREQUENCY_IDENTIFICATION_METHOD getFrequencyIdentificationMethod(int transferFunctionIdx) {
			DEX_FREQUENCY_IDENTIFICATION_METHOD val;
			DEX_ERRCHK(dex_get_frequency_identification_method(exchanger, transferFunctionIdx, &val));
			return val;
		}
		double getFrequencyIdentificationExtraValue(int transferFunctionIdx, DEX_FREQUENCY_IDENTIFICATION_EXTRA_VALUE extraValueType) {
            double val;
			DEX_ERRCHK(dex_get_frequency_identification_extra_value(exchanger, transferFunctionIdx, extraValueType, &val));
            return val;
		} 
		double getFrequencyIdentificationRegulationParameterValue(int transferFunctionIdx, DEX_FREQUENCY_IDENTIFICATION_REGULATION_PARAMETER regulationParameterType) {
            double val;
			DEX_ERRCHK(dex_get_frequency_identification_regulation_parameter_value(exchanger, transferFunctionIdx, regulationParameterType, &val));
            return val;
		}
		void getFrequencyTransferFunctionGainUnit(int transferFunctionIdx, char *gain_unit) {
			DEX_ERRCHK(dex_get_frequency_transfer_function_gain_unit(exchanger, transferFunctionIdx, gain_unit));
		}
		void getFrequencyTransferFunctionPhaseUnit(int transferFunctionIdx, char *phase_unit) {
			DEX_ERRCHK(dex_get_frequency_transfer_function_phase_unit(exchanger, transferFunctionIdx, phase_unit));
		}
		void createFrequencyDataTable(int nbRows, int nbCols) {
			DEX_ERRCHK(dex_create_frequency_data_table(exchanger, nbRows, nbCols));
		}
		void setFrequencyDataTableValues(int nbVals, double vals[]) {
			DEX_ERRCHK(dex_set_frequency_data_table_values(exchanger, nbVals, vals));
		}
		void setFrequencyDataTableColValues(int colIdx, int nbVals, double vals[]) {
			DEX_ERRCHK(dex_set_frequency_data_table_col_values(exchanger, colIdx, nbVals, vals));
		}
		void setFrequencyDataTableRowValues(int rowIdx, int nbVals, double vals[]) {
			DEX_ERRCHK(dex_set_frequency_data_table_row_values(exchanger, rowIdx, nbVals, vals));
		}
		void setFrequencyDataTableValue(int rowIdx, int colIdx, double val) {
			DEX_ERRCHK(dex_set_frequency_data_table_value(exchanger, rowIdx, colIdx, val));
		}
		void getFrequencyDataTableValues(int nbVals, double vals[]) {
			DEX_ERRCHK(dex_get_frequency_data_table_values(exchanger, nbVals, vals));
		}
		void getFrequencyDataTableColValues(int colIdx, int nbVals, double vals[]) {
			DEX_ERRCHK(dex_get_frequency_data_table_col_values(exchanger, colIdx, nbVals, vals));
		}
		void getFrequencyDataTableRowValues(int rowIdx, int nbVals, double vals[]) {
			DEX_ERRCHK(dex_get_frequency_data_table_row_values(exchanger, rowIdx, nbVals, vals));
		}
		double getFrequencyDataTableValue(int rowIdx, int colIdx) {
			double value;
			DEX_ERRCHK(dex_get_frequency_data_table_value(exchanger, rowIdx, colIdx, &value));
			return value;
		}

		void createAxisRegisterInfo(int axisIdx, int nbRegisters) {
			DEX_ERRCHK(dex_create_axis_register_info(exchanger, axisIdx, nbRegisters));
		}

		void setAxisRegisterInfoAxisNumber(int axisIdx, int axisNb) {
			DEX_ERRCHK(dex_set_axis_register_info_axis_number(exchanger, axisIdx, axisNb));
		}

		void createAxisRegisterInfoRegister(int axisIdx, int regIdx, int nbSidx) {
			DEX_ERRCHK(dex_create_axis_register_info_register(exchanger, axisIdx, regIdx, nbSidx));
		}

		void setAxisRegisterInfoRegisterTyp(int axisIdx, int regIdx, int regTyp) {
			DEX_ERRCHK(dex_set_axis_register_info_register_typ(exchanger, axisIdx, regIdx, regTyp));
		}

		void setAxisRegisterInfoRegisterIdx(int axisIdx, int regIdx, int etelRegIdx) {
			DEX_ERRCHK(dex_set_axis_register_info_register_idx(exchanger, axisIdx, regIdx, etelRegIdx));
		}

		void setAxisRegisterInfoRegisterSidx(int axisIdx, int regIdx, int regSidx, DEX_INC_VALUE val) {
			DEX_ERRCHK(dex_set_axis_register_info_register_sidx(exchanger, axisIdx, regIdx, regSidx, val));
		}
		void setAxisRegisterInfoRegisterInt32Sidx(int axisIdx, int regIdx, int regSidx, int val) {
			DEX_ERRCHK(dex_set_axis_register_info_register_int32_sidx(exchanger, axisIdx, regIdx, regSidx, val));
		}
		void setAxisRegisterInfoRegisterInt64Sidx(int axisIdx, int regIdx, int regSidx, eint64 val) {
			DEX_ERRCHK(dex_set_axis_register_info_register_int64_sidx(exchanger, axisIdx, regIdx, regSidx, val));
		}
		void setAxisRegisterInfoRegisterFloat32Sidx(int axisIdx, int regIdx, int regSidx, float val) {
			DEX_ERRCHK(dex_set_axis_register_info_register_float32_sidx(exchanger, axisIdx, regIdx, regSidx, val));
		}
		void setAxisRegisterInfoRegisterFloat64Sidx(int axisIdx, int regIdx, int regSidx, double val) {
			DEX_ERRCHK(dex_set_axis_register_info_register_float64_sidx(exchanger, axisIdx, regIdx, regSidx, val));
		}
		void setAxisRegisterInfoRegisterConversionFactor(int axisIdx, int regIdx, double convFactor) {
			DEX_ERRCHK(dex_set_axis_register_info_register_conversion_factor(exchanger, axisIdx, regIdx, convFactor));
		}

		void setAxisRegisterInfoRegisterUnit(int axisIdx, int regIdx, char *unit) {
			DEX_ERRCHK(dex_set_axis_register_info_register_unit(exchanger, axisIdx, regIdx, unit));
		}

		int getRegisterInfoNbAxis() {
			int nbAxis;
			DEX_ERRCHK(dex_get_register_info_nb_axis(exchanger, &nbAxis));
			return nbAxis;
		}

		ebool isRegisterInfoPresent() {
			ebool present;
			DEX_ERRCHK(dex_is_register_info_present(exchanger, &present));
			return present;
		}

		int getAxisRegisterInfoAxisNb(int axisIdx) {
			int axisNb;
			DEX_ERRCHK(dex_get_axis_register_info_axis_nb(exchanger, axisIdx, &axisNb));
			return axisNb;
		}

		int getAxisRegisterInfoAxisIdx(int axisNb) {
			int axisIdx;
			DEX_ERRCHK(dex_get_axis_register_info_axis_idx(exchanger, axisNb, &axisIdx));
			return axisIdx;
		}

		int getAxisRegisterInfoNbRegister(int axisIdx) {
			int nbRegister;
			DEX_ERRCHK(dex_get_axis_register_info_nb_register(exchanger, axisIdx, &nbRegister));
			return nbRegister;
		}

		int getAxisRegisterInfoRegisterTyp(int axisIdx, int regIdx) {
			int etelRegTyp;
			DEX_ERRCHK(dex_get_axis_register_info_register_typ(exchanger, axisIdx, regIdx, &etelRegTyp));
			return etelRegTyp;
		}

		int getAxisRegisterInfoRegisterIdx(int axisIdx, int regIdx) {
			int etelRegIdx;
			DEX_ERRCHK(dex_get_axis_register_info_register_idx(exchanger, axisIdx, regIdx, &etelRegIdx));
			return etelRegIdx;
		}

		double getAxisRegisterInfoRegisterConversionFactor(int axisIdx, int regIdx) {
			double convFactor;
			DEX_ERRCHK(dex_get_axis_register_info_register_conversion_factor(exchanger, axisIdx, regIdx, &convFactor));
			return convFactor;
		}

		void getAxisRegisterInfoRegisterUnit(int axisIdx, int regIdx, int strSize, char *str) {
			DEX_ERRCHK(dex_get_axis_register_info_register_unit(exchanger, axisIdx, regIdx, strSize, str));
		}

		int getAxisRegisterInfoRegisterNbDepth(int axisIdx, int regIdx) {
			int nbDepth;
			DEX_ERRCHK(dex_get_axis_register_info_register_nb_depth(exchanger, axisIdx, regIdx, &nbDepth));
			return nbDepth;
		}

		void getAxisRegisterInfoRegisterDepthValue(int axisIdx, int regIdx, int regSidx, DEX_INC_VALUE *val) {
			DEX_ERRCHK(dex_get_axis_register_info_register_depth_value(exchanger, axisIdx, regIdx, regSidx, val));
		}
		int getAxisRegisterInfoRegisterDepthInt32Value(int axisIdx, int regIdx, int regSidx) {
			int val;
			DEX_ERRCHK(dex_get_axis_register_info_register_depth_int32_value(exchanger, axisIdx, regIdx, regSidx, &val));
			return val;
		}
		eint64 getAxisRegisterInfoRegisterDepthInt64Value(int axisIdx, int regIdx, int regSidx) {
			eint64 val;
			DEX_ERRCHK(dex_get_axis_register_info_register_depth_int64_value(exchanger, axisIdx, regIdx, regSidx, &val));
			return val;
		}
		float getAxisRegisterInfoRegisterDepthFloat32Value(int axisIdx, int regIdx, int regSidx) {
			float val;
			DEX_ERRCHK(dex_get_axis_register_info_register_depth_float32_value(exchanger, axisIdx, regIdx, regSidx, &val));
			return val;
		}
		double getAxisRegisterInfoRegisterDepthFloat64Value(int axisIdx, int regIdx, int regSidx) {
			double val;
			DEX_ERRCHK(dex_get_axis_register_info_register_depth_float64_value(exchanger, axisIdx, regIdx, regSidx, &val));
			return val;
		}
		void getAxisRegisterInfoRegisterIncValue(int etelAxisNb, int etelRegTyp, int etelRegIdx, int etelRegSidx, DEX_INC_VALUE *val) {
			DEX_ERRCHK(dex_get_register_inc_value(exchanger, etelAxisNb, etelRegTyp, etelRegIdx, etelRegSidx, val));
		}
		int getAxisRegisterInfoRegisterIncInt32Value(int etelAxisNb, int etelRegTyp, int etelRegIdx, int etelRegSidx) {
			int val;
			DEX_ERRCHK(dex_get_register_inc_int32_value(exchanger, etelAxisNb, etelRegTyp, etelRegIdx, etelRegSidx, &val));
			return val;
		}
		eint64 getAxisRegisterInfoRegisterIncInt64Value(int etelAxisNb, int etelRegTyp, int etelRegIdx, int etelRegSidx) {
			eint64 val;
			DEX_ERRCHK(dex_get_register_inc_int64_value(exchanger, etelAxisNb, etelRegTyp, etelRegIdx, etelRegSidx, &val));
			return val;
		}
		float getAxisRegisterInfoRegisterIncFloat32Value(int etelAxisNb, int etelRegTyp, int etelRegIdx, int etelRegSidx) {
			float val;
			DEX_ERRCHK(dex_get_register_inc_float32_value(exchanger, etelAxisNb, etelRegTyp, etelRegIdx, etelRegSidx, &val));
			return val;
		}
		double getAxisRegisterInfoRegisterIncFloat64Value(int etelAxisNb, int etelRegTyp, int etelRegIdx, int etelRegSidx) {
			double val;
			DEX_ERRCHK(dex_get_register_inc_float64_value(exchanger, etelAxisNb, etelRegTyp, etelRegIdx, etelRegSidx, &val));
			return val;
		}

		double getAxisRegisterInfoRegisterIsoValue(int etelAxisNb, int etelRegTyp, int etelRegIdx, int etelRegSidx) {
			double val;
			DEX_ERRCHK(dex_get_register_iso_value(exchanger, etelAxisNb, etelRegTyp, etelRegIdx, etelRegSidx, &val));
			return val;
		}
		int getRegisterInfoRegisterGlobalIndex(int etelAxisNb, int etelRegTyp, int etelRegIdx) {
			int val;
			DEX_ERRCHK(dex_get_register_global_index(exchanger, etelAxisNb, etelRegTyp, etelRegIdx, &val));
			return val;
		}

		void checkConsistency() {
			DEX_ERRCHK(dex_check_consistency(exchanger));
		}
		void diag(char_cp file_name, int line, int err) {
			dex_diag(file_name, line, err, exchanger);
		}
		void sdiag(char_p str, char_cp file_name, int line, int err) {
			dex_sdiag(str, file_name, line, err, exchanger);
		}
		void fdiag(char_cp output_file_name, char_cp file_name, int line, int err) {
			dex_fdiag(output_file_name, file_name, line, err, exchanger);
		}
};
#endif /* DEX_OO_API */

#endif //_DEX40_H
