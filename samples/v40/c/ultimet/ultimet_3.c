/*
 * ultimet_3.c
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

/*
 * This is a simple program that illustrates the working principle of the command DMD_CMD_SET_RANGE (126.)
 * This command is available on UltimET prodcts only.
 * The aim of this function is to set in one command a specific depth of several contiguous registers. This command
 * can be used for X, XL, XF, XD or L registers only
 * For X, XF and L registers, the maximum number of values is 127.
 * For XL and XD registers, the maximum number of values is 100.
 * This command has to be sent by calling dsa_execute_command_x_s(). Please see the EDI HTML documentation for further explanations about this function.
 * The number of parameters to send has to be equal to the number of effective values plus one. This additional parameter has to be first set to specify the
 * type of register, the depth and the index used by the command.
 */

/*
 * To run this demo without modification, you must have:
 *  - one ULTIMET multi-axis board on the PCI bus or an UltimET TCP/IP
 */

/*** Libraries ***/

#include <conio.h>   /* standard library for getch()*/
#include <ctype.h>   /* standard character conversion library */
#include <math.h>    /* standard math interface; used for random numbers here */
#include <process.h> /* standard multithreading library       */
#include <stdio.h>   /* the standard I/O library              */
#include <stdlib.h>  /* standard general purpose library      */

#include <dmd40.h> /* EDI constants definitions             */
#include <dsa40.h> /* EDI high level library version 3.0x.  */
#include <lib40.h> /* EDI constants definitions             */

/*** Defines ***/

#define TEST_TIMEOUT 5000
#define NB_PARAM     80 // values+first param. For 102 values, set 103. Always X values + 1 param to specify the register --> min value of NB_PARAM: 2
// from 1+1 to 127+1 for X, XF and L registers
// from 1+1 to 100+1 for XL and XD registers

/*** Function ***/

void main(void)
{
    char              url[64];
    int               err = 0;
    int               j   = 0;
    int               reg_idx;
    int               reg_depth;
    int               firstValue_int    = 0;
    long              firstValue_long   = 0;
    float             firstValue_float  = 0;
    double            firstValue_double = 0;
    DSA_COMMAND_PARAM param[NB_PARAM]; // a pointer to the parameter array, for ETCOM record
    DSA_MASTER       *UltimET = NULL;

    strcpy(url, "etb:ultimet:*"); // UltimET PCI

    printf("\n**** INIT ****\n");

    printf("\nNumber of effective values to write to the registers = %d\n", NB_PARAM - 1);
    printf("This value can be changed by modifying the macro NB_PARAM\n");

    if (err = dsa_create_master(&UltimET)) {
        printf("\nCreating ultimet failed with error %d", err);
        DSA_EXT_DIAG(err, UltimET);
        goto _error;
    }
    printf("Creating UltimET: DONE\n");

    /************************************************ OPEN ************************************************/
    printf("Opening the UltimET...\n");
    if (err = dsa_open_u(UltimET, url)) {
        printf("\nOpening failed with error %d", err);
        DSA_EXT_DIAG(err, UltimET);
        goto _error;
    }

    printf("Communication with the UltimET done\n");

    if (err = dsa_reset_error_ex_s(UltimET, 0, 1000)) {
        printf("\nFailed to reset UltimET !!!!\n");
        DSA_EXT_DIAG(err, UltimET);
        goto _error;
    }

    printf("\n**** SEND ETCOM RECORD ****\n");

    /************************************************ X REGISTER ************************************************/

    // preparation of the ETCOM record
    printf("\nX register\n");

    firstValue_int = 10; // first value to set;
    printf("First value = %d\n", firstValue_int);

    // preparation of the ETCOM record
    reg_idx        = 4;            // index of the register
    reg_depth      = 1 << 16;      // fixed depth for the registers. Depth 0: not writable !
    param[0].typ   = DMD_TYP_USER; // DMD_TYP_USER --> X register
    param[0].conv  = 0;            // no conversion needed
    param[0].val.i = (0x40000000) | reg_depth | reg_idx;
    // Here above: the function set_multi in the FW automatically increments the address of the memory used to store the values, hence we only set the first
    // index and depth

    for (j = 1; j < NB_PARAM; j++) { // we specify the values to set
        param[j].typ   = DMD_TYP_IMMEDIATE_INT32;
        param[j].conv  = 0;
        param[j].val.i = firstValue_int + j - 1;
    }
    printf("reg_idx = %d\n", reg_idx);
    printf("reg_depth = %d\n", reg_depth >> 16);

    // sending of the command DMD_CMD_SET_RANGE
    if (err = dsa_execute_command_x_s(UltimET, DMD_CMD_SET_RANGE, param, NB_PARAM, FALSE, TRUE, TEST_TIMEOUT)) {
        DSA_EXT_DIAG(err, UltimET);
        goto _error;
    }

    printf("Function successfully executed for X registers\n");

    /************************************************ XL REGISTER ************************************************/

    // preparation of the ETCOM record
    printf("\nXL register\n");

    firstValue_long = 11; // first value to set;
    printf("First value = %ld\n", firstValue_long);

    reg_idx        = 6;                  // index of the register
    reg_depth      = 2 << 16;            // fixed depth for the registers. Depth 0: not writable !
    param[0].typ   = DMD_TYP_USER_INT64; // DMD_TYP_USER --> XL register
    param[0].conv  = 0;                  // no conversion needed
    param[0].val.i = (0x40000000) | reg_depth | reg_idx;
    // Here above: the function set_multi in the FW automatically increments the address of the memory used to store the values, hence we only set the first
    // index and depth

    for (j = 1; j < NB_PARAM; j++) { // we specify the values to set
        param[j].typ     = DMD_TYP_IMMEDIATE_INT64;
        param[j].conv    = 0;
        param[j].val.i64 = firstValue_long + j - 1;
    }
    printf("reg_idx = %d\n", reg_idx);
    printf("reg_depth = %d\n", reg_depth >> 16);

    // sending of the command 126
    if (err = dsa_execute_command_x_s(UltimET, DMD_CMD_SET_RANGE, param, NB_PARAM, FALSE, TRUE, TEST_TIMEOUT)) {
        DSA_EXT_DIAG(err, UltimET);
        goto _error;
    }

    printf("Function successfully executed for XL registers\n");

    /************************************************ XF REGISTER ************************************************/

    // preparation of the ETCOM record
    printf("\nXF register\n");

    firstValue_float = 12; // first value to set;
    printf("First value = %f\n", firstValue_float);

    reg_idx        = 6;                    // index of the register
    reg_depth      = 2 << 16;              // fixed depth for the registers. Depth 0: not writable !
    param[0].typ   = DMD_TYP_USER_FLOAT32; // DMD_TYP_USER_FLOAT32 --> XF register
    param[0].conv  = 0;                    // no conversion needed
    param[0].val.i = (0x40000000) | reg_depth | reg_idx;
    // Here above: the function set_multi in the FW automatically increments the address of the memory used to store the values, hence we only set the first
    // index and depth

    for (j = 1; j < NB_PARAM; j++) { // we specify the values to set
        param[j].typ   = DMD_TYP_IMMEDIATE_FLOAT32;
        param[j].conv  = 0;
        param[j].val.f = firstValue_float + j - 1;
    }
    printf("reg_idx = %d\n", reg_idx);
    printf("reg_depth = %d\n", reg_depth >> 16);

    // sending of the command 126
    if (err = dsa_execute_command_x_s(UltimET, DMD_CMD_SET_RANGE, param, NB_PARAM, FALSE, TRUE, TEST_TIMEOUT)) {
        DSA_EXT_DIAG(err, UltimET);
        goto _error;
    }

    printf("Function successfully executed for XF registers\n");

    /************************************************ XD REGISTER ************************************************/

    // preparation of the ETCOM record
    printf("\nXD register\n");

    firstValue_double = 13; // first value to set;
    printf("First value = %lf\n", firstValue_double);

    reg_idx        = 5;                    // index of the register
    reg_depth      = 3 << 16;              // fixed depth for the registers. Depth 0: not writable !
    param[0].typ   = DMD_TYP_USER_FLOAT64; // DMD_TYP_USER_FLOAT64 --> XD register
    param[0].conv  = 0;                    // no conversion needed
    param[0].val.i = (0x40000000) | reg_depth | reg_idx;
    // Here above: the function set_multi in the FW automatically increments the address of the memory used to store the values, hence we only set the first
    // index and depth

    for (j = 1; j < NB_PARAM; j++) { // we specify the values to set
        param[j].typ   = DMD_TYP_IMMEDIATE_FLOAT64;
        param[j].conv  = 0;
        param[j].val.d = firstValue_double + j - 1;
    }
    printf("reg_idx = %d\n", reg_idx);
    printf("reg_depth = %d\n", reg_depth >> 16);

    // sending of the command 126
    if (err = dsa_execute_command_x_s(UltimET, DMD_CMD_SET_RANGE, param, NB_PARAM, FALSE, TRUE, TEST_TIMEOUT)) {
        DSA_EXT_DIAG(err, UltimET);
        goto _error;
    }

    printf("Function successfully executed for XD registers\n");

    /************************************************ L REGISTER ************************************************/

    printf("\n**** L register ****\n");

    firstValue_int = 14; // first value to set

    printf("First value = %d\n", firstValue_int);

    // preparation of the ETCOM record
    reg_idx        = 10;          // index of the register
    reg_depth      = 0 << 16;     // fixed depth for the registers
    param[0].typ   = DMD_TYP_LKT; // DMD_TYP_LKT: L register
    param[0].conv  = 0;           // no conversion needed
    param[0].val.i = (0x40000000) | reg_depth | reg_idx;
    // Here above: the function set_multi in the FW automatically increments the address of the memory used to store the values, hence we only set the first
    // index and depth

    for (j = 1; j < NB_PARAM; j++) { // we specify the values to set. NB_PARAM
        param[j].typ   = DMD_TYP_IMMEDIATE_INT32;
        param[j].conv  = 0;
        param[j].val.i = firstValue_int + j - 1;
    }
    printf("reg_idx = %d\n", reg_idx);
    printf("reg_depth = %d\n", reg_depth >> 16);

    // sending of the command 126
    if (err = dsa_execute_command_x_s(UltimET, DMD_CMD_SET_RANGE, param, NB_PARAM, FALSE, TRUE, TEST_TIMEOUT)) {
        DSA_EXT_DIAG(err, UltimET);
        goto _error;
    }

    printf("Function successfully executed for L registers\n");

    /************************************************ CLOSE ************************************************/

    if (err = dsa_close(UltimET)) {
        printf("\nClosing failed with error %d", err);
        DSA_EXT_DIAG(err, UltimET);
        goto _error;
    }

    printf("\nFile closed\n");

    dsa_destroy(&UltimET);

    printf("\nTest finished\n");
    while (!_kbhit()) {
    }

    return;

_error:
    printf("\n\nExecution failed !!!!\n");
    if (err = dsa_close(UltimET)) {
        printf("\nClosing failed with error %d", err);
        DSA_EXT_DIAG(err, UltimET);
    }

    dsa_destroy(&UltimET);

    while (!_kbhit()) {
    }
}
