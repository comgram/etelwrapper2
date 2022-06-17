/*
 * rtv3.c
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
 * This sample program show how to setup slave-to-slave communication using RTV.
 * - Configure Axis 0 to put ML1 value into a slot
 * - Configure Axis 1 to read slot value into ML450
 * - Move Drive 0 and monitor ML450 of drive 1 during move
 * This is an example. Slave to slave communication can be made without RTV if the
 * two axes are part of the same AccurET
 */

/*
 * To run this demo without modification, you must have:
 *  - a PCI-UltimET board plugged into your PC
 *  - an AccurET axis 0 connected to UltimET through TransnET with a connected and setted motor
 *  - an AccurET axis 1 connected to UltimET through TransnET
 */

#include <Windows.h>
#include <process.h>/* standard multithreading library       */
#include <stdio.h>
#include <stdlib.h>

#include <dmd40.h>
#include <dsa40.h>

static void display_thread(void *param);
/*
 * Synchronisation between main thread and displaying thread:
 * To tell the displaying thread whether to fetch and display the data or not.
 */
int doDisplay = 1; /* 0: don't; 1: do*/

/*
 * Display thread function.
 */
static void display_thread(void *param)
{
    DSA_DEVICE_GROUP *grp = (DSA_DEVICE_GROUP *)param;
    DSA_DRIVE        *drv0;
    DSA_DRIVE        *drv1;
    int               err     = 0;
    int               counter = 0;

    /* Get individual drive objects. */
    if (err = dsa_get_group_item(grp, 0, &drv0)) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }
    if (err = dsa_get_group_item(grp, 1, &drv1)) {
        DSA_EXT_DIAG(err, drv1);
        goto _error;
    }

    // Starts an infinite loop which will be stopped by global-variable doDisplay set to 0
    for (;;) {
        // Each 100 ms, we will display the status and the position of the drive
        DSA_STATUS status0 = {sizeof(DSA_STATUS)};
        DSA_STATUS status1 = {sizeof(DSA_STATUS)};
        eint64     pos     = 0;

        /* stop display thread */
        if (!doDisplay)
            break;

        // Get the drive0 status.
        if (err = dsa_get_status(drv0, &status0)) {
            DSA_EXT_DIAG(err, drv0);
            goto _error;
        }
        // Get the drive1 status.
        if (err = dsa_get_status(drv1, &status1)) {
            DSA_EXT_DIAG(err, drv1);
            goto _error;
        }

        // Get the ML450 register of the axis
        if (err = dsa_get_register_int64_s(drv1, DMD_TYP_MONITOR_INT64, 450, 0, &pos, 0, 5000)) {
            DSA_EXT_DIAG(err, drv0);
            goto _error;
        }

        // print status and position
        printf("%04d: ", ++counter);
        printf("AXIS 0 : %c%c%c AXIS 1 : %c%c%c [ML450: %16lld inc]\r",
               status0.drive.moving ? 'M' : '-',
               status0.drive.warning ? 'W' : '-',
               status0.drive.error ? 'E' : '-',
               status1.drive.moving ? 'M' : '-',
               status1.drive.warning ? 'W' : '-',
               status1.drive.error ? 'E' : '-',
               pos);

        // Sleep 100 ms
        Sleep(100);
    }

    printf("\ndisplay thread ended\n");
    return;

    /* Error handler. */
_error:

    /* Print the first error that occured. */
    printf("error in display (%d it)\n", counter);
}

int main()
{
    DSA_MASTER       *ultimET = NULL; // UltimET object
    DSA_DRIVE        *drv0    = NULL; // ACCURET object
    DSA_DRIVE        *drv1    = NULL; // ACCURET object
    DSA_DEVICE_GROUP *grp     = NULL; // Group of AccurET
    DSA_RTV_SLOT     *slot    = NULL; // Slot object
    int               err     = 0;

    // Creates devices objects
    printf("Creating UltimET and AccurET objects...\n");
    if (err = dsa_create_master(&ultimET)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }
    if (err = dsa_create_drive(&drv0)) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }
    if (err = dsa_create_drive(&drv1)) {
        DSA_EXT_DIAG(err, drv1);
        goto _error;
    }

    // Create group of AccurET
    if (err = dsa_create_device_group(&grp, 2)) {
        DSA_EXT_DIAG(err, NULL);
        goto _error;
    }

    if (err = dsa_set_group_item(grp, 0, drv0)) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }

    if (err = dsa_set_group_item(grp, 1, drv1)) {
        DSA_EXT_DIAG(err, drv1);
        goto _error;
    }

    // Opening communication
    printf("Opening of the communication...\n");
    if (err = dsa_open_u(ultimET, "etb:ULTIMET:*")) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }
    if (err = dsa_open_u(drv0, "etb:ULTIMET:0")) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }
    if (err = dsa_open_u(drv1, "etb:ULTIMET:1")) {
        DSA_EXT_DIAG(err, drv1);
        goto _error;
    }

    printf("Reset Axis 0,1 errors...\n");
    if (err = dsa_reset_error_ex_s(grp, 0, 5000)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    // Configure Slave-to-slave communication through slot
    printf("Get a free 64 bits slot from UltimET...\n");
    if (err = dsa_get_64bit_rtv0_slot(ultimET, &slot)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("Assign ML1 from axis 0 to reserved slot...\n");
    if (err = dsa_assign_register_to_slot_s(drv0, DMD_TYP_MONITOR_INT64, 1, 0, slot, 5000)) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }

    printf("Assign slot to ML450 of axis 1...\n");
    if (err = dsa_assign_slot_to_register_s(drv1, slot, DMD_TYP_MONITOR_INT64, 450, 0, 5000)) {
        DSA_EXT_DIAG(err, drv1);
        goto _error;
    }

    // Move axis 0 and display ML450 of axis 1 in a second thread
    printf("Power On Axis 0...\n");
    if (err = dsa_power_on_s(drv0, 5000)) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }
    printf("Home Axis 0...\n");
    if (err = dsa_homing_start_s(drv0, 5000)) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }
    if (err = dsa_wait_movement_s(drv0, 30000)) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }

    // Starts display thread
    printf("Move axis 0 and starts display threads...\n");
    if (_beginthread(display_thread, 0, grp) <= 0) {
        err = DSA_ESYSTEM;
        printf("ERROR at file %s, line %d %s\n", __FILE__, __LINE__, dsa_translate_edi_error(err));
        goto _error;
    }

    if (err = dsa_set_target_position_s(drv0, 0, 1.5, 5000)) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }

    if (err = dsa_wait_movement_s(drv0, 30000)) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }

    Sleep(5000);

    if (err = dsa_set_register_int64_s(drv0, DMD_TYP_PPK_INT64, 210, 0, 10000000, 5000)) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }

    if (err = dsa_wait_movement_s(drv0, 30000)) {
        DSA_EXT_DIAG(err, drv0);
        goto _error;
    }
    Sleep(5000);

    /* Success. */
    printf("success.\n");

/* Error handler. */
_error : {
    int err1;

    doDisplay = 0;

    if (err1 = dsa_unassign_slot_to_register_s(drv1, slot, DMD_TYP_MONITOR_INT64, 450, 0, 5000))
        DSA_EXT_DIAG(err1, drv1);
    if (err1 = dsa_unassign_register_to_slot_s(drv0, DMD_TYP_MONITOR_INT64, 1, 0, slot, 5000))
        DSA_EXT_DIAG(err1, drv0);
    if (err1 = dsa_free_64bit_rtv_slot(&slot))
        DSA_EXT_DIAG(err1, NULL);

    if (dsa_is_valid_drive_group(grp)) {
        if (err1 = dsa_destroy(&grp))
            DSA_EXT_DIAG(err1, grp);
    }

    if (dsa_is_valid_drive(drv0)) {
        ebool open = 0;
        if (err1 = dsa_is_open(drv0, &open))
            DSA_EXT_DIAG(err1, drv0);
        if (open) {
            if (err1 = dsa_close(drv0))
                DSA_EXT_DIAG(err1, drv0);
        }
        if (err1 = dsa_destroy(&drv0))
            DSA_EXT_DIAG(err1, drv0);
    }
    if (dsa_is_valid_drive(drv1)) {
        ebool open = 0;
        if (err1 = dsa_is_open(drv1, &open))
            DSA_EXT_DIAG(err1, drv1);
        if (open) {
            if (err1 = dsa_close(drv1))
                DSA_EXT_DIAG(err1, drv1);
        }
        if (err1 = dsa_destroy(&drv1))
            DSA_EXT_DIAG(err1, drv1);
    }
    if (dsa_is_valid_master(ultimET)) {
        ebool open = 0;
        if (err1 = dsa_is_open(ultimET, &open))
            DSA_EXT_DIAG(err1, ultimET);
        if (open) {
            if (err1 = dsa_close(ultimET))
                DSA_EXT_DIAG(err1, ultimET);
        }
        if (err1 = dsa_destroy(&ultimET))
            DSA_EXT_DIAG(err1, ultimET);
    }
}

    if (err)
        printf("ended with error %d\n", err);
    else
        printf("ended without error\n");
}
