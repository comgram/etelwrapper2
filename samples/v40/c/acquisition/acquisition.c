/*
 * acquisition.c
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
 * This simple demo program shows how to make acquisition, using ETEL EDI library set.
 * This example will do an acquisition of one trace on 2 Accuret controllers connected to an UltimET
 */

/*
 * To run this demo without modification, you must have:
 *  - 2 linear motor and an AccurET properly configured.
 *  - an UltimET plugged in the PC
 *  - AccurET and UltimET connected through transnET
 */

/*** libraries ***/

/*
 * Include the standard I/O library.
 */
#include <conio.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Include EDI high level library version 3.0x.
 */
#include <dmd40.h>
#include <dsa40.h>
#include <lib40.h>

int printConfig(FILE *f, DSA_DEVICE *dev, int realNbPoints, double realTime)
{
    DMD   *dmd = NULL;
    int    err;
    long   m121_0, m121_1, m121_2, m121_3;
    long   m122_0, m122_1, m122_2, m122_3;
    long   m123;
    long   m124;
    long   m125;
    long   m126_0, m126_1;
    eint64 ml127_0, ml127_1;
    long   m128;
    long   m129;
    long   m130;
    long   m131;
    long   m132;
    long   m133;
    eint64 ml134;
    long   m135;
    long   m136;

    long   k121_0, k121_1, k121_2, k121_3;
    long   k122_0, k122_1, k122_2, k122_3;
    long   k123;
    long   k124;
    long   k125;
    long   k126_0, k126_1;
    long   k127_0, k127_1;
    float  kf127_0, kf127_1;
    eint64 kl127_0, kl127_1;
    double kd127_0, kd127_1;
    long   k128;
    long   k129;

    int nb;

    if (err = dsa_get_dmd_data(dev, &dmd)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_etcom_get_etb_axis(dev, &nb)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    fprintf(f, "PROGRAMMED CONFIG of %s %d:\n", dmd_translate_drv_product(dmd_get_drv_product(dmd)), nb);
    if (err = dsa_quick_register_int32_request_s(dev, 2, 121, 0, &k121_0, 2, 121, 1, &k121_1, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 2, 121, 2, &k121_2, 2, 121, 3, &k121_3, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 2, 122, 0, &k122_0, 2, 122, 1, &k122_1, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 2, 122, 2, &k122_2, 2, 122, 3, &k122_3, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 2, 123, 0, &k123, 2, 124, 0, &k124, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 2, 125, 0, &k125, 2, 126, 0, &k126_0, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 2, 126, 1, &k126_1, 2, 128, 0, &k128, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 2, 129, 1, &k129, 2, 129, 0, &k129, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 2, 129, 1, &k129, 2, 129, 0, &k129, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 2, 127, 0, &k127_0, 2, 127, 1, &k127_1, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int64_request_s(dev, 34, 127, 0, &kl127_0, 34, 127, 1, &kl127_1, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_float32_request_s(dev, 66, 127, 0, &kf127_0, 66, 127, 1, &kf127_1, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_float64_request_s(dev, 98, 127, 0, &kd127_0, 98, 127, 1, &kd127_1, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    fprintf(f, "\tTRACE 0: %s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, k121_0), k122_0 & 0xFFFF, k122_0 >> 16 & 0xFF);
    fprintf(f, "\tTRACE 1:%s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, k121_1), k122_1 & 0xFFFF, k122_1 >> 16 & 0xFF);
    fprintf(f, "\tTRACE 2:%s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, k121_2), k122_2 & 0xFFFF, k122_2 >> 16 & 0xFF);
    fprintf(f, "\tTRACE 3:%s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, k121_3), k122_3 & 0xFFFF, k122_3 >> 16 & 0xFF);
    switch (k125) {
    case DSA_ACQUISITION_TRIG_IMMEDIATE:
        fprintf(f, "\tTRIG Immediate\n");
        break;
    case DSA_ACQUISITION_TRIG_START_MOVE:
        fprintf(f, "\tTRIG Start Move\n");
        fprintf(f, "\t\tIpol Group %d\n", k126_0);
        break;
    case DSA_ACQUISITION_TRIG_END_MOVE:
        fprintf(f, "\tTRIG End Move\n");
        fprintf(f, "\t\tIpol Group %d\n", k126_0);
        break;
    case DSA_ACQUISITION_TRIG_POSITION:
        fprintf(f, "\tTRIG Position\n");
        fprintf(f, "\t\tPosition %lld\n", kl127_0);
        fprintf(f, "\t\tEdge %d\n", k124);
        break;
    case DSA_ACQUISITION_TRIG_TRACE:
        fprintf(f, "\tTRIG Trace Index\n");
        fprintf(f, "\t\tTrace Index %d\n", k126_0);
        fprintf(f, "\t\tTrace int32 Value %d\n", k127_0);
        fprintf(f, "\t\tTrace float32 Value %f\n", kf127_0);
        fprintf(f, "\t\tTrace eint64 Value %lld\n", kl127_0);
        fprintf(f, "\t\tTrace float64 Value %f\n", kd127_0);
        fprintf(f, "\t\tEdge %d\n", k124);
        break;
    case DSA_ACQUISITION_TRIG_EXTERNAL:
        fprintf(f, "\tTRIG External\n");
        break;
    case DSA_ACQUISITION_TRIG_REGISTER:
        fprintf(f, "\tTRIG Register\n");
        fprintf(f, "\t\tRegister: %s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, k126_0), k126_1 & 0xFFFF, k126_1 >> 16 & 0xFF);
        fprintf(f, "\t\tRegister int32 Value %d\n", k127_0);
        fprintf(f, "\t\tRegister float32 Value %f\n", kf127_0);
        fprintf(f, "\t\tRegister eint64 Value %lld\n", kl127_0);
        fprintf(f, "\t\tRegister float64 Value %f\n", kd127_0);
        fprintf(f, "\t\tEdge %d\n", k124);
        break;
    case DSA_ACQUISITION_TRIG_BIT_FIELD_STATE:
        fprintf(f, "\tTRIG Bit Field State\n");
        fprintf(f, "\t\tRegister: %s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, k126_0), k126_1 & 0xFFFF, k126_1 >> 16 & 0xFF);
        fprintf(f, "\t\tLow State 0x%llx\n", kl127_0);
        fprintf(f, "\t\tHigh State 0x%llx\n", kl127_1);
        break;
    case DSA_ACQUISITION_TRIG_BIT_FIELD_EDGE:
        fprintf(f, "\tTRIG Bit Field change\n");
        fprintf(f, "\t\tRegister: %s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, k126_0), k126_1 & 0xFFFF, k126_1 >> 16 & 0xFF);
        fprintf(f, "\t\tRising Edge 0x%llx\n", kl127_0);
        fprintf(f, "\t\tFalling edge 0x%llx\n", kl127_1);
        break;
    }

    fprintf(f, "\n");
    fprintf(f, "\tK123 : %d\n", k123);
    fprintf(f, "\tK128 : %d\n", k128);
    fprintf(f, "\tDelay %d\n", k129);
    fprintf(f, "\tComputed Real Nb Points %d\n", realNbPoints);
    fprintf(f, "\tComputed Real Time %f\n", realTime);

    fprintf(f, "\n");
    fprintf(f, "READ CONFIG of %s %d:\n", dmd_translate_drv_product(dmd_get_drv_product(dmd)), nb);
    if (err = dsa_quick_register_int32_request_s(dev, 3, 121, 0, &m121_0, 3, 121, 1, &m121_1, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 3, 121, 2, &m121_2, 3, 121, 3, &m121_3, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 3, 122, 0, &m122_0, 3, 122, 1, &m122_1, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 3, 122, 2, &m122_2, 3, 122, 3, &m122_3, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 3, 123, 0, &m123, 3, 124, 0, &m124, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 3, 125, 0, &m125, 3, 126, 0, &m126_0, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 3, 126, 1, &m126_1, 3, 128, 0, &m128, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 3, 129, 1, &m129, 3, 130, 0, &m130, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 3, 131, 1, &m131, 3, 132, 0, &m132, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 3, 133, 1, &m133, 3, 135, 0, &m135, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int32_request_s(dev, 3, 136, 1, &m136, 3, 136, 0, &m136, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int64_request_s(dev, 67, 127, 0, &ml127_0, 67, 127, 1, &ml127_1, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    if (err = dsa_quick_register_int64_request_s(dev, 67, 134, 0, &ml134, 67, 134, 0, &ml134, NULL, 1000)) {
        DSA_EXT_DIAG(err, dev);
        goto _error;
    }
    fprintf(f, "\tTRACE 0: %s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, m121_0), m122_0 & 0xFFFF, m122_0 >> 16 & 0xFF);
    fprintf(f, "\tTRACE 1:%s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, m121_1), m122_1 & 0xFFFF, m122_1 >> 16 & 0xFF);
    fprintf(f, "\tTRACE 2:%s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, m121_2), m122_2 & 0xFFFF, m122_2 >> 16 & 0xFF);
    fprintf(f, "\tTRACE 3:%s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, m121_3), m122_3 & 0xFFFF, m122_3 >> 16 & 0xFF);
    switch (m125) {
    case DSA_ACQUISITION_TRIG_IMMEDIATE:
        fprintf(f, "\tTRIG Immediate\n");
        break;
    case DSA_ACQUISITION_TRIG_START_MOVE:
        fprintf(f, "\tTRIG Start Move\n");
        fprintf(f, "\t\tIpol Group %d\n", m126_0);
        break;
    case DSA_ACQUISITION_TRIG_END_MOVE:
        fprintf(f, "\tTRIG End Move\n");
        fprintf(f, "\t\tIpol Group %d\n", m126_0);
        break;
    case DSA_ACQUISITION_TRIG_POSITION:
        fprintf(f, "\tTRIG Position\n");
        fprintf(f, "\t\tPosition %lld\n", ml127_0);
        fprintf(f, "\t\tEdge %d\n", m124);
        break;
    case DSA_ACQUISITION_TRIG_TRACE:
        fprintf(f, "\tTRIG Trace Index\n");
        fprintf(f, "\t\tTrace Index %d\n", m126_0);
        fprintf(f, "\t\tTrace Value %lld\n", ml127_0);
        fprintf(f, "\t\tEdge %d\n", m124);
        break;
    case DSA_ACQUISITION_TRIG_EXTERNAL:
        fprintf(f, "\tTRIG External\n");
        break;
    case DSA_ACQUISITION_TRIG_REGISTER:
        fprintf(f, "\tTRIG Register\n");
        fprintf(f, "\t\tRegister: %s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, m126_0), m126_1 & 0xFFFF, m126_1 >> 16 & 0xFF);
        fprintf(f, "\t\tRegister Value %lld\n", ml127_0);
        fprintf(f, "\t\tEdge %d\n", m124);
        break;
    case DSA_ACQUISITION_TRIG_BIT_FIELD_STATE:
        fprintf(f, "\tTRIG Bit Field State\n");
        fprintf(f, "\t\tRegister: %s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, m126_0), m126_1 & 0xFFFF, m126_1 >> 16 & 0xFF);
        fprintf(f, "\t\tLow State 0x%llx\n", ml127_0);
        fprintf(f, "\t\tHigh State 0x%llx\n", ml127_1);
        break;
    case DSA_ACQUISITION_TRIG_BIT_FIELD_EDGE:
        fprintf(f, "\tTRIG Bit Field change\n");
        fprintf(f, "\t\tRegister: %s%d:%d\n", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, m126_0), m126_1 & 0xFFFF, m126_1 >> 16 & 0xFF);
        fprintf(f, "\t\tRising Edge 0x%llx\n", ml127_0);
        fprintf(f, "\t\tFalling edge 0x%llx\n", ml127_1);
        break;
    }

    fprintf(f, "\n");
    fprintf(f, "\tM123 : %d\n", m123);
    fprintf(f, "\tM128 : %d\n", m128);
    fprintf(f, "\tDelay %d\n", m129);
    fprintf(f, "\tM130 : 0x%x\n", m130);
    fprintf(f, "\tM131 : %d\n", m131);
    fprintf(f, "\tM132 : %d\n", m132);
    fprintf(f, "\tM133 : %d\n", m133);
    fprintf(f, "\tML134 : %lld\n", ml134);
    fprintf(f, "\tM135 : %d\n", m135);
    fprintf(f, "\tM136 : %d\n", m136);

_error:
    return err;
}

/*** functions ***/

/* To run this example you must have the following configuration : */
/* EDI package EDI3.00A or above */
/* a ULTIMET
/* a ACCURET controllers Numbered 0 */
/* a ACCURET controller numbered 2 and 3 */
void main()
{
    int              err;
    DSA_DRIVE       *accuret1           = NULL;
    DSA_DRIVE       *accuret2           = NULL;
    DSA_DRIVE_GROUP *grp                = NULL;
    DSA_ACQUISITION *acq                = NULL;
    int              accuret1_nb_points = 0;
    int              accuret2_nb_points = 0;
    double          *accuret1_times = NULL, *accuret2_times = NULL;
    double          *accuret1_values = NULL, *accuret2_values = NULL;
    FILE            *f;
    int              i, j;
    double           real_time;

    /*--- Create objects -------------------------------------------------------------------*/
    /* create drives */
    if (err = dsa_create_drive(&accuret1)) {
        DSA_EXT_DIAG(err, accuret1);
        goto _error;
    }
    if (err = dsa_create_drive(&accuret2)) {
        DSA_EXT_DIAG(err, accuret2);
        goto _error;
    }

    /* create drive group */
    if (err = dsa_create_drive_group(&grp, 2)) {
        DSA_EXT_DIAG(err, NULL);
        goto _error;
    }

    /* adding drives to group */
    if (err = dsa_set_group_item(grp, 0, accuret1)) {
        DSA_EXT_DIAG(err, accuret1);
        goto _error;
    }
    if (err = dsa_set_group_item(grp, 1, accuret2)) {
        DSA_EXT_DIAG(err, accuret2);
        goto _error;
    }

    /* open drives */
    if (err = dsa_open_u(accuret1, "etb:ULTIMET:0")) {
        DSA_EXT_DIAG(err, accuret1);
        goto _error;
    }

    if (err = dsa_open_u(accuret2, "etb:ULTIMET:1")) {
        DSA_EXT_DIAG(err, accuret2);
        goto _error;
    }

    /* create acquisition object and set it with group--------------------------------------*/
    /* Must be done once all drives are opened */
    if (err = dsa_create_acquisition(&acq, grp)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* reserve all devices for this acquisition---------------------------------------------*/
    if (err = dsa_acquisition_reserve(acq)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /*--- Configure traces -----------------------------------------------------------------*/
    /* configure first trace of accuret1 to M7:0 (M7 is not oversampled)*/
    if (err = dsa_acquisition_config_trace(acq,             // Acquisition object
                                           accuret1,        // Concerned controller
                                           0,               // Trace index
                                           DMD_TYP_MONITOR, // Monitoring register
                                           7,               // Monitoring register 7 (M7)
                                           0)) {            // Subindex 0
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* configure first trace of accuret2 to MF31:0 (MF31 is oversampled)*/
    if (err = dsa_acquisition_config_trace(acq,                     // Acquisition object
                                           accuret2,                // Concerned controller
                                           0,                       // Trace index
                                           DMD_TYP_MONITOR_FLOAT32, // Monitoring register
                                           31,                      // Monitoring register 31 (M31)
                                           0)) {                    // Subindex 0
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /*--- Configure the trigger ------------------------------------------------------------*/
    /* Trigger comes from accuret1, immediately, other parameters are not significant in immediate mode */
    if (err = dsa_acquisition_config_immediate_trigger(acq,         // Acquisition object
                                                       accuret1)) { // Triggered controller
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /*--- Configure frequency --------------------------------------------------------------*/
    /* configure frequency of acquisition to 1001 points, 0.5 seconds */
    if (err = dsa_acquisition_config_with_nb_points_and_total_time(acq,                                  // Acquisition object
                                                                   1001,                                 // Number of points
                                                                   0.5,                                  // Total time of acquisition (0.5 seconds)
                                                                   DSA_ACQUISITION_SYNCHRO_MODE_NONE)) { // Synchronisation mode normal
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /*--- Start acquisition ----------------------------------------------------------------*/
    /* timeout should be at least as big as total time of acquisition specified by dsa_acquisition_config_frequency*/
    if (err = dsa_acquisition_acquire_s(acq,   // Acquisition object
                                        -1)) { // Timeout : should be in any case greater than total time of acquisition
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /*--- Get number of points which must be uploaded to be able to allocate beig enough table ---*/
    /* Get number of point of first trace of Accuret1 and allocate tables for upload */
    if (err = dsa_acquisition_get_trace_real_nb_points(acq,                    // Acquisition object
                                                       accuret1,               // Controller on which the trace will be acquired
                                                       0,                      // Trace index of concerned trace
                                                       &accuret1_nb_points)) { // Pointer on double which will receive number of acquired points
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }
    if (err = dsa_acquisition_get_real_total_time(acq, &real_time)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }
    accuret1_times  = (double *)calloc(accuret1_nb_points, sizeof(double));
    accuret1_values = (double *)calloc(accuret1_nb_points, sizeof(double));

    /* Get number of point of first trace of Accuret2 and allocate tables for upload */
    if (err = dsa_acquisition_get_trace_real_nb_points(acq,                    // Acquisition object
                                                       accuret2,               // Controller on which the trace will be acquired
                                                       0,                      // Trace index of concerned trace
                                                       &accuret2_nb_points)) { // Pointer on double which will receive number of acquired points
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }
    accuret2_times  = (double *)calloc(accuret2_nb_points, sizeof(double));
    accuret2_values = (double *)calloc(accuret2_nb_points, sizeof(double));

    /*--- Upload ---------------------------------------------------------------------------*/
    /* Upload of first trace of Accuret1 */
    /* Will use conversion function corresponding to first trace of Accuret1 (M7) */
    if (err = dsa_acquisition_upload_trace(acq,                // Acquisition object
                                           accuret1,           // Controller on which trace must be uploaded
                                           0,                  // Trace index to upload
                                           accuret1_nb_points, // Size of two next tables
                                           accuret1_times,     // Table which will contain the times of each acquired time
                                           accuret1_values,    // Table which will contain the acquired values of M7
                                           0)) {               // The conversion function to use for the acquired values (0 : none => increment values)
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* Upload of first trace of Accuert2 */
    /* Will use conversion function corresponding to first trace of Accuert2 (MF31) */
    if (err = dsa_acquisition_upload_trace(acq,                // Acquisition object
                                           accuret2,           // Controller on which trace must be uploaded
                                           0,                  // Trace index to upload
                                           accuret2_nb_points, // Size of two next tables
                                           accuret2_times,     // Table which will contain the times of each acquired time
                                           accuret2_values,    // Table which will contain the acquired values of M7
                                           0)) {               // The conversion function to use for the acquired values (0 : none => increment values)
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* unreserve all devices for this acquisition---------------------------------------------*/
    if (err = dsa_acquisition_unreserve(acq)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /*--- Writes results in file------------------------------------------------------------*/
    f = fopen("Results.txt", "w");
    fprintf(f, "Configuration\n");
    if (accuret1_nb_points)
        printConfig(f, accuret1, accuret1_nb_points, real_time);
    if (accuret2_nb_points)
        printConfig(f, accuret2, accuret2_nb_points, real_time);
    fprintf(f, "\n\nTime; Accuret1 Value; Accuret2 Value\n");
    for (i = 0, j = 0; (i < accuret1_nb_points) || (j < accuret2_nb_points);) {
        if ((i < accuret1_nb_points) && (j < accuret2_nb_points)) {
            if (accuret1_times[i] == accuret2_times[j]) {
                fprintf(f, "%.9lf ;%lf ;%lf ;\n", accuret1_times[i], accuret1_values[i], accuret2_values[j]);
                i++;
                j++;
            }
            else if (accuret1_times[i] < accuret2_times[j]) {
                fprintf(f, "%.9lf ;%lf ; ;\n", accuret1_times[i], accuret1_values[i]);
                i++;
            }
            else if (accuret1_times[i] > accuret2_times[j]) {
                fprintf(f, "%.9lf ; ;%lf ;\n", accuret2_times[j], accuret2_values[j]);
                j++;
            }
        }
        else if (i < accuret1_nb_points) {
            fprintf(f, "%.9lf ;%lf ; ;\n", accuret1_times[i], accuret1_values[i]);
            i++;
        }
        else {
            fprintf(f, "%.9lf ; ;%lf ;\n", accuret2_times[j], accuret2_values[j]);
            j++;
        }
    }
    fclose(f);

    /* --- close and destroy ---------------------------------------------------------------*/
    if (err = dsa_destroy_acquisition(&acq)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    if (err = dsa_close(accuret1)) {
        DSA_EXT_DIAG(err, accuret1);
        goto _error;
    }
    if (err = dsa_close(accuret2)) {
        DSA_EXT_DIAG(err, accuret2);
        goto _error;
    }

    if (err = dsa_destroy(&grp)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    if (err = dsa_destroy(&accuret1)) {
        DSA_EXT_DIAG(err, accuret1);
        goto _error;
    }

    if (err = dsa_destroy(&accuret2)) {
        DSA_EXT_DIAG(err, accuret2);
        goto _error;
    }

    free(accuret1_times);
    free(accuret2_times);
    free(accuret1_values);
    free(accuret2_values);

    printf("success.\n");
    _getch();
    return;

    /* Error handler. */
_error:
    /* Destroy the acquisition if not already done. */
    if (dsa_is_valid_acquisition(acq))
        dsa_destroy_acquisition(&acq);

    /* Destroy the group if not already done. */
    if (dsa_is_valid_drive_group(grp))
        dsa_destroy(&grp);

    /* Is the accuret1 pointer valid ? */
    if (dsa_is_valid_drive(accuret1)) {
        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(accuret1, &open);
        if (open) {
            /* Close the connection. */
            dsa_close(accuret1);
        }
    }

    /* Is the accuret2 pointer valid ? */
    if (dsa_is_valid_drive(accuret2)) {
        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(accuret2, &open);
        if (open) {
            /* Close the connection. */
            dsa_close(accuret2);
        }
    }

    /* Finally, release all resources to the OS. */
    dsa_destroy(&accuret1);
    dsa_destroy(&accuret2);

    if (accuret1_times)
        free(accuret1_times);
    if (accuret2_times)
        free(accuret2_times);
    if (accuret1_values)
        free(accuret1_values);
    if (accuret2_values)
        free(accuret2_values);
    _getch();
}
