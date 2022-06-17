/*
 * continuous_acquisition.c
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
 * This simple demo program shows how to make a continuous acquisition, using ETEL EDI library set.
 * This example will do a continuous acquisition of some traces on AccurET controllers and UltimET device
 */

/*
 * To run this demo without modification, you must have:
 *  - An AccurET numbered: 0, 1
 *  - an UltimET plugged in the PC
 *  - AccurET and UltimET connected through transnET
 *  - Traces on Accuret1 : ML228, M7, MF31
 *  - Traces on Accuret2 : ML228
 *  - Traces on UltimET  : ML228, M66
 */

/*** libraries ***/

/*
 * Include the standard I/O library.
 */
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Include EDI high level library.
 */
#include <dmd40.h>
#include <dsa40.h>
#include <lib40.h>

#define BUFFER_CONT_TRACE 16000
#define NB_PTS_TO_UPLOAD  1000
#define SAMPLING_TIME     200e-6

#define LOOP_VALUE        50

#define USE_WAIT_DATA     TRUE

void main()
{
    int err, i, k, nb_available_pts, m66;

    DSA_MASTER       *ultimet  = NULL;
    DSA_DRIVE        *accuret1 = NULL;
    DSA_DRIVE        *accuret2 = NULL;
    DSA_DEVICE_GROUP *grp      = NULL;
    DSA_ACQUISITION  *acq      = NULL;
    EFILE            *fLog;

    ebool integrity_status = FALSE;

    char *Accu1_Tr0_filename = "Accuret1Trace0Log.txt";
    ebool Accu1Log0Created   = FALSE;
    char *Accu1_Tr1_filename = "Accuret1Trace1Log.txt";
    ebool Accu1Log1Created   = FALSE;
    char *Accu1_Tr2_filename = "Accuret1Trace2Log.txt";
    ebool Accu1Log2Created   = FALSE;

    char *Accu2_Tr0_filename = "Accuret2Trace0Log.txt";
    ebool Accu2Log0Created   = FALSE;

    char *Ulti_Tr0_filename = "UltimetTrace0Log.txt";
    ebool UltiLog0Created   = FALSE;
    char *Ulti_Tr1_filename = "UltimetTrace1Log.txt";
    ebool UltiLog1Created   = FALSE;

    double acqTimeArrayAccu1Tr0[NB_PTS_TO_UPLOAD];
    eint64 acqValueArrayAccu1Tr0_Int64[NB_PTS_TO_UPLOAD];
    double acqTimeArrayAccu1Tr1[NB_PTS_TO_UPLOAD];
    int    acqValueArrayAccu1Tr1_Int32[NB_PTS_TO_UPLOAD];
    double acqTimeArrayAccu1Tr2[NB_PTS_TO_UPLOAD];
    float  acqValueArrayAccu1Tr2_Flt32[NB_PTS_TO_UPLOAD];

    double acqTimeArrayAccu2Tr0[NB_PTS_TO_UPLOAD];
    eint64 acqValueArrayAccu2Tr0_Int64[NB_PTS_TO_UPLOAD];

    double acqTimeArrayUltiTr0[NB_PTS_TO_UPLOAD];
    eint64 acqValueArrayUltiTr0_Int64[NB_PTS_TO_UPLOAD];
    double acqTimeArrayUltiTr1[NB_PTS_TO_UPLOAD];
    int    acqValueArrayUltiTr1_Int32[NB_PTS_TO_UPLOAD];

    /*--- Create objects -------------------------------------------------------------------*/
    /* create drives/master */
    if (err = dsa_create_master(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }
    if (err = dsa_create_drive(&accuret1)) {
        DSA_EXT_DIAG(err, accuret1);
        goto _error;
    }
    if (err = dsa_create_drive(&accuret2)) {
        DSA_EXT_DIAG(err, accuret2);
        goto _error;
    }

    /* open devices/master */
    if (err = dsa_open_u(ultimet, "etb:ULTIMET:*")) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }
    if (err = dsa_open_u(accuret1, "etb:ULTIMET:0")) {
        DSA_EXT_DIAG(err, accuret1);
        goto _error;
    }
    if (err = dsa_open_u(accuret2, "etb:ULTIMET:1")) {
        DSA_EXT_DIAG(err, accuret2);
        goto _error;
    }

    /* create device group */
    if (err = dsa_create_device_group(&grp, 3)) {
        DSA_EXT_DIAG(err, NULL);
        goto _error;
    }

    /* adding devices to group */
    if (err = dsa_set_group_item(grp, 0, ultimet)) {
        DSA_EXT_DIAG(err, accuret1);
        goto _error;
    }
    if (err = dsa_set_group_item(grp, 1, accuret1)) {
        DSA_EXT_DIAG(err, accuret1);
        goto _error;
    }
    if (err = dsa_set_group_item(grp, 2, accuret2)) {
        DSA_EXT_DIAG(err, accuret2);
        goto _error;
    }

    if (err = dsa_reset_error_ex_s(grp, 0, 5000)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* create acquisition object and set it with group--------------------------------------*/
    /* Must be done once all drives/master are opened */
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
    /* configure trace of Accuret1 to ML228:0 (ML228 is oversampled)*/
    if (err = dsa_acquisition_config_trace(acq,                   // Acquisition object
                                           accuret1,              // Concerned controller
                                           0,                     // Trace index
                                           DMD_TYP_MONITOR_INT64, // Monitoring register
                                           228,                   // Monitoring register 228 (ML228)
                                           0)) {                  // Subindex 0
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }
    /* configure trace of Accuret1 to M7:0 (M7 is not oversampled)*/
    if (err = dsa_acquisition_config_trace(acq,                   // Acquisition object
                                           accuret1,              // Concerned controller
                                           1,                     // Trace index
                                           DMD_TYP_MONITOR_INT32, // Monitoring register
                                           7,                     // Monitoring register 7 (M7)
                                           0)) {                  // Subindex 0
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }
    /* configure trace of Accuret1 to MF31:0 (MF31 is oversampled)*/
    if (err = dsa_acquisition_config_trace(acq,                     // Acquisition object
                                           accuret1,                // Concerned controller
                                           2,                       // Trace index
                                           DMD_TYP_MONITOR_FLOAT32, // Monitoring register
                                           31,                      // Monitoring register 31 (MF31)
                                           0)) {                    // Subindex 0
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* configure trace of Accuret2 to ML228:0 (ML228 is oversampled)*/
    if (err = dsa_acquisition_config_trace(acq,                   // Acquisition object
                                           accuret2,              // Concerned controller
                                           0,                     // Trace index
                                           DMD_TYP_MONITOR_INT64, // Monitoring register
                                           228,                   // Monitoring register 228 (ML228)
                                           0)) {                  // Subindex 0
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* configure trace of Ultimet to ML228:0 (ML228)*/
    if (err = dsa_acquisition_config_trace(acq,                   // Acquisition object
                                           ultimet,               // Concerned controller
                                           0,                     // Trace index
                                           DMD_TYP_MONITOR_INT64, // Monitoring register
                                           228,                   // Monitoring register 228 (ML228)
                                           0)) {                  // Subindex 0
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }
    /* configure trace of Ultimet to M66:0 (M66)*/
    if (err = dsa_acquisition_config_trace(acq,                   // Acquisition object
                                           ultimet,               // Concerned controller
                                           1,                     // Trace index
                                           DMD_TYP_MONITOR_INT32, // Monitoring register
                                           66,                    // Monitoring register 66 (M66)
                                           0)) {                  // Subindex 0
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /*--- Configure continuous trace ------------------------------------------------------------*/
    /* NOTE ----------------------------------------------------------------------------------------------------------- NOTE */
    /* NOTE  The sampling time value could be adapted based on the devices involved in the acquisition.                 NOTE */
    /* NOTE  If an UltimET is present the sampling time will be a multiple of 100us otherwise is is a multiple of 50us  NOTE */
    /* NOTE ----------------------------------------------------------------------------------------------------------- NOTE */
    if (err = dsa_acquisition_continuous_config(acq, BUFFER_CONT_TRACE, SAMPLING_TIME)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /*--- Start continuous acquisition ----------------------------------------------------------*/
    if (err = dsa_acquisition_continuous_acquire(acq)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    for (k = 1; k <= LOOP_VALUE; k++) {
        if (USE_WAIT_DATA) {
            err = dsa_acquisition_continuous_wait_available_values_s(acq, NB_PTS_TO_UPLOAD, 5000);
            if (err) {
                DSA_EXT_DIAG(err, grp);
                goto _error;
            }
            err = dsa_acquisition_continuous_get_nb_available_values(acq, &nb_available_pts);
            if (err) {
                DSA_EXT_DIAG(err, grp);
                goto _error;
            }
            err = dsa_get_register_int32_s(accuret1, DMD_TYP_MONITOR, 66, 0, &m66, 0, 5000);
            if (err) {
                DSA_EXT_DIAG(err, grp);
                goto _error;
            }
            printf("GetVal=%d,M66=%d; ", nb_available_pts, m66);
        }
        else {
            /* Check number of available points */
            do {
                err = dsa_acquisition_continuous_get_nb_available_values(acq, &nb_available_pts);
                if (err) {
                    DSA_EXT_DIAG(err, grp);
                    goto _error;
                }
                err = dsa_get_register_int32_s(accuret1, DMD_TYP_MONITOR, 66, 0, &m66, 0, 5000);
                if (err) {
                    DSA_EXT_DIAG(err, grp);
                    goto _error;
                }
                printf("GetVal=%d,M66=%d; ", nb_available_pts, m66);
            } while (nb_available_pts < NB_PTS_TO_UPLOAD);
        }

        /*--- Upload continuous trace ------------------------------------------------------------*/
        /* WARNING ---------------------------------------------------------------------------------------------------------------------- WARNING */
        /* WARNING  During a continuous acquisition, the buffers of the traces are filled at the same speed for all the devices involved  WARNING */
        /* WARNING  in the acquisition therefore it is MANDATORY that the number of uploaded data is the same for all uploaded traces     WARNING */
        /* WARNING ---------------------------------------------------------------------------------------------------------------------- WARNING */

        /* Upload of first trace of Accuret1 */
        if (err = dsa_acquisition_continuous_upload_int64_trace(acq,                            // Acquisition object
                                                                accuret1,                       // Controller on which trace must be uploaded
                                                                0,                              // Trace index to upload
                                                                NB_PTS_TO_UPLOAD,               // Size of two next tables
                                                                acqTimeArrayAccu1Tr0,           // Table which will contain the times of each acquired value
                                                                acqValueArrayAccu1Tr0_Int64)) { // Table which will contain the acquired values of ML228
            DSA_EXT_DIAG(err, grp);
            goto _error;
        }
        /* Upload of second trace of Accuret1 */
        if (err = dsa_acquisition_continuous_upload_int32_trace(acq,                            // Acquisition object
                                                                accuret1,                       // Controller on which trace must be uploaded
                                                                1,                              // Trace index to upload
                                                                NB_PTS_TO_UPLOAD,               // Size of two next tables
                                                                acqTimeArrayAccu1Tr1,           // Table which will contain the times of each acquired value
                                                                acqValueArrayAccu1Tr1_Int32)) { // Table which will contain the acquired values of M7
            DSA_EXT_DIAG(err, grp);
            goto _error;
        }
        /* Upload of third trace of Accuret1 */
        if (err = dsa_acquisition_continuous_upload_float32_trace(acq,                            // Acquisition object
                                                                  accuret1,                       // Controller on which trace must be uploaded
                                                                  2,                              // Trace index to upload
                                                                  NB_PTS_TO_UPLOAD,               // Size of two next tables
                                                                  acqTimeArrayAccu1Tr2,           // Table which will contain the times of each acquired value
                                                                  acqValueArrayAccu1Tr2_Flt32)) { // Table which will contain the acquired values of MF31
            DSA_EXT_DIAG(err, grp);
            goto _error;
        }

        /* Upload of first trace of Accuret2 */
        if (err = dsa_acquisition_continuous_upload_int64_trace(acq,                            // Acquisition object
                                                                accuret2,                       // Controller on which trace must be uploaded
                                                                0,                              // Trace index to upload
                                                                NB_PTS_TO_UPLOAD,               // Size of two next tables
                                                                acqTimeArrayAccu2Tr0,           // Table which will contain the times of each acquired value
                                                                acqValueArrayAccu2Tr0_Int64)) { // Table which will contain the acquired values of ML228
            DSA_EXT_DIAG(err, grp);
            goto _error;
        }

        /* Upload of first trace of UltimET */
        if (err = dsa_acquisition_continuous_upload_int64_trace(acq,                           // Acquisition object
                                                                ultimet,                       // Controller on which trace must be uploaded
                                                                0,                             // Trace index to upload
                                                                NB_PTS_TO_UPLOAD,              // Size of two next tables
                                                                acqTimeArrayUltiTr0,           // Table which will contain the times of each acquired value
                                                                acqValueArrayUltiTr0_Int64)) { // Table which will contain the acquired values of ML228
            DSA_EXT_DIAG(err, grp);
            goto _error;
        }
        /* Upload of second trace of UltimET */
        if (err = dsa_acquisition_continuous_upload_int32_trace(acq,                           // Acquisition object
                                                                ultimet,                       // Controller on which trace must be uploaded
                                                                1,                             // Trace index to upload
                                                                NB_PTS_TO_UPLOAD,              // Size of two next tables
                                                                acqTimeArrayUltiTr1,           // Table which will contain the times of each acquired value
                                                                acqValueArrayUltiTr1_Int32)) { // Table which will contain the acquired values of M66
            DSA_EXT_DIAG(err, grp);
            goto _error;
        }

        printf("\n\tLoop: %d\n", k);

        /* Save trace of Accuret1 */
        if (!Accu1Log0Created) {
            if (!(fLog = EFOPEN(Accu1_Tr0_filename, "w"))) {
                printf("Error opening LogFile %s\n", Accu1_Tr0_filename);
                exit(-1);
            }
            time_t    _time;
            struct tm newtime;
            char      timeStr[64];

            time(&_time);
            localtime_s(&newtime, &_time);
            asctime_s(timeStr, sizeof timeStr, &newtime);

            EFPRINTF(fLog, "Time counter[ms]: %lld, Trace log started at %s\n", tim_counter(), timeStr);
            EFCLOSE(fLog);
            Accu1Log0Created = TRUE;
        }
        if (!(fLog = EFOPEN(Accu1_Tr0_filename, "a+"))) {
            printf("Unable to append to LogFile %s\n", Accu1_Tr0_filename);
            exit(-1);
        }
        EFPRINTF(fLog, "Time counter[ms]: %lld,\n", tim_counter());
        EFPRINTF(fLog, "Time[sec]\tValue[-] loop: %d\n", k);
        for (i = 0; i < NB_PTS_TO_UPLOAD; i++) {
            EFPRINTF(fLog, "%f\t%d\n", acqTimeArrayAccu1Tr0[i], acqValueArrayAccu1Tr0_Int64[i]);
        }
        EFCLOSE(fLog);

        // Next one
        if (!Accu1Log1Created) {
            if (!(fLog = EFOPEN(Accu1_Tr1_filename, "w"))) {
                printf("Error opening LogFile %s\n", Accu1_Tr1_filename);
                exit(-1);
            }
            time_t    _time;
            struct tm newtime;
            char      timeStr[64];

            time(&_time);
            localtime_s(&newtime, &_time);
            asctime_s(timeStr, sizeof timeStr, &newtime);

            EFPRINTF(fLog, "Time counter[ms]: %lld, Trace log started at %s\n", tim_counter(), timeStr);
            EFCLOSE(fLog);
            Accu1Log1Created = TRUE;
        }
        if (!(fLog = EFOPEN(Accu1_Tr1_filename, "a+"))) {
            printf("Unable to append to LogFile %s\n", Accu1_Tr1_filename);
            exit(-1);
        }
        EFPRINTF(fLog, "Time counter[ms]: %lld,\n", tim_counter());
        EFPRINTF(fLog, "Time[sec]\tValue[-] loop: %d\n", k);
        for (i = 0; i < NB_PTS_TO_UPLOAD; i++) {
            EFPRINTF(fLog, "%f\t%d\n", acqTimeArrayAccu1Tr1[i], acqValueArrayAccu1Tr1_Int32[i]);
        }
        EFCLOSE(fLog);

        // Next one
        if (!Accu1Log2Created) {
            if (!(fLog = EFOPEN(Accu1_Tr2_filename, "w"))) {
                printf("Error opening LogFile %s\n", Accu1_Tr2_filename);
                exit(-1);
            }
            time_t    _time;
            struct tm newtime;
            char      timeStr[64];

            time(&_time);
            localtime_s(&newtime, &_time);
            asctime_s(timeStr, sizeof timeStr, &newtime);

            EFPRINTF(fLog, "Time counter[ms]: %lld, Trace log started at %s\n", tim_counter(), timeStr);
            EFCLOSE(fLog);
            Accu1Log2Created = TRUE;
        }
        if (!(fLog = EFOPEN(Accu1_Tr2_filename, "a+"))) {
            printf("Unable to append to LogFile %s\n", Accu1_Tr2_filename);
            exit(-1);
        }
        EFPRINTF(fLog, "Time counter[ms]: %lld,\n", tim_counter());
        EFPRINTF(fLog, "Time[sec]\tValue[-] loop: %d\n", k);
        for (i = 0; i < NB_PTS_TO_UPLOAD; i++) {
            EFPRINTF(fLog, "%f\t%f\n", acqTimeArrayAccu1Tr2[i], acqValueArrayAccu1Tr2_Flt32[i]);
        }
        EFCLOSE(fLog);

        /* Save trace of Accuret2 */
        if (!Accu2Log0Created) {
            if (!(fLog = EFOPEN(Accu2_Tr0_filename, "w"))) {
                printf("Error opening LogFile %s\n", Accu2_Tr0_filename);
                exit(-1);
            }
            time_t    _time;
            struct tm newtime;
            char      timeStr[64];

            time(&_time);
            localtime_s(&newtime, &_time);
            asctime_s(timeStr, sizeof timeStr, &newtime);

            EFPRINTF(fLog, "Time counter[ms]: %lld, Trace log started at %s\n", tim_counter(), timeStr);
            EFCLOSE(fLog);
            Accu2Log0Created = TRUE;
        }
        if (!(fLog = EFOPEN(Accu2_Tr0_filename, "a+"))) {
            printf("Unable to append to LogFile %s\n", Accu2_Tr0_filename);
            exit(-1);
        }
        EFPRINTF(fLog, "Time counter[ms]: %lld,\n", tim_counter());
        EFPRINTF(fLog, "Time[sec]\tValue[-] loop: %d\n", k);
        for (i = 0; i < NB_PTS_TO_UPLOAD; i++) {
            EFPRINTF(fLog, "%f\t%d\n", acqTimeArrayAccu2Tr0[i], acqValueArrayAccu2Tr0_Int64[i]);
        }
        EFCLOSE(fLog);

        /* Save trace of Ultimet */
        if (!UltiLog0Created) {
            if (!(fLog = EFOPEN(Ulti_Tr0_filename, "w"))) {
                printf("Error opening LogFile %s\n", Ulti_Tr0_filename);
                exit(-1);
            }
            time_t    _time;
            struct tm newtime;
            char      timeStr[64];

            time(&_time);
            localtime_s(&newtime, &_time);
            asctime_s(timeStr, sizeof timeStr, &newtime);

            EFPRINTF(fLog, "Time counter[ms]: %lld, Trace log started at %s\n", tim_counter(), timeStr);
            EFCLOSE(fLog);
            UltiLog0Created = TRUE;
        }
        if (!(fLog = EFOPEN(Ulti_Tr0_filename, "a+"))) {
            printf("Unable to append to LogFile %s\n", Ulti_Tr0_filename);
            exit(-1);
        }
        EFPRINTF(fLog, "Time counter[ms]: %lld,\n", tim_counter());
        EFPRINTF(fLog, "Time[sec]\tValue[-] loop: %d\n", k);
        for (i = 0; i < NB_PTS_TO_UPLOAD; i++) {
            EFPRINTF(fLog, "%f\t%d\n", acqTimeArrayUltiTr0[i], acqValueArrayUltiTr0_Int64[i]);
        }
        EFCLOSE(fLog);

        // Next one
        if (!UltiLog1Created) {
            if (!(fLog = EFOPEN(Ulti_Tr1_filename, "w"))) {
                printf("Error opening LogFile %s\n", Ulti_Tr1_filename);
                exit(-1);
            }
            time_t    _time;
            struct tm newtime;
            char      timeStr[64];

            time(&_time);
            localtime_s(&newtime, &_time);
            asctime_s(timeStr, sizeof timeStr, &newtime);

            EFPRINTF(fLog, "Time counter[ms]: %lld, Trace log started at %s\n", tim_counter(), timeStr);
            EFCLOSE(fLog);
            UltiLog1Created = TRUE;
        }
        if (!(fLog = EFOPEN(Ulti_Tr1_filename, "a+"))) {
            printf("Unable to append to LogFile %s\n", Ulti_Tr1_filename);
            exit(-1);
        }
        EFPRINTF(fLog, "Time counter[ms]: %lld,\n", tim_counter());
        EFPRINTF(fLog, "Time[sec]\tValue[-] loop: %d\n", k);
        for (i = 0; i < NB_PTS_TO_UPLOAD; i++) {
            EFPRINTF(fLog, "%f\t%d\n", acqTimeArrayUltiTr1[i], acqValueArrayUltiTr1_Int32[i]);
        }
        EFCLOSE(fLog);
    }

    /* Stop continuous trace */
    if (err = dsa_acquisition_stop_acquire(acq)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }
    /* Get the integrity of the uploaded data */
    if (err = dsa_acquisition_continuous_get_data_integrity(acq, &integrity_status)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    printf("\nCheck integrity of uploaded data: ");
    if (integrity_status != TRUE) {
        printf("*** INTEGRITY OF DATA IS NOT GUARANTEED ***\n");
    }
    else {
        printf("*** INTEGRITY OF DATA IS OK ***\n");
    }

    /* unreserve all devices for this acquisition---------------------------------------------*/
    if (err = dsa_acquisition_unreserve(acq)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* --- close and destroy ---------------------------------------------------------------*/
    if (err = dsa_destroy_acquisition(&acq)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    if (err = dsa_close(ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
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

    if (err = dsa_destroy(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
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

    printf("Ended.\n");
    _getch();
    return;

    /* Error handler. */
_error:
    /* Destroy the acquisition if not already done. */
    if (dsa_is_valid_acquisition(acq))
        dsa_destroy_acquisition(&acq);

    /* Destroy the group if not already done. */
    if (dsa_is_valid_device_group(grp))
        dsa_destroy(&grp);

    /* Is the ultimet pointer valid ? */
    if (dsa_is_valid_master(ultimet)) {
        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(ultimet, &open);
        if (open) {
            /* Close the connection. */
            dsa_close(ultimet);
        }
    }

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
    dsa_destroy(&ultimet);
    dsa_destroy(&accuret1);
    dsa_destroy(&accuret2);

    _getch();
}
