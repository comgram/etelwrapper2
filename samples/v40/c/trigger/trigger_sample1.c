/*
 * Trigger_sample.c
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

/*** libraries ***/
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dsa40.h"

/*** functions ***/

// trigger_output_setting allow to reserve output for triggers and the trigger basis (real or theoretical position)
int trigger_output_setting(DSA_DRIVE *axis)
{

    int err = 0;

    /************************ Position type selection *******************************/
    // K336 = 0 for trigger based on real position (ML7)
    if (err = dsa_trigger_set_position_type_s(axis, DSA_TRIGGER_TYPE_REAL_POSITION, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }
    /************************ Output selection *******************************/
    // K359:0 = 0x1 for DOUT used
    if (err = dsa_trigger_set_dout_mask_s(axis, 0x01, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    // C359:1 = 0x1 for FDOUT1 axis used
    if (err = dsa_trigger_set_fdout_mask_s(axis, 0x01, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    return 0;
}
//-------------------------------------------------------------------------//

// trigger_combi_setting allow to select the output adress for the combi mode
int trigger_combi_setting(DSA_DRIVE *axis)
{

    int err = 0;

    /******************** Outputs mask setting (combi) ****************************/

    // K320 = 0x01 Output1 will be set with combi n�0
    if (err = dsa_trigger_set_combi_dout_mask_s(axis, DSA_TRIGGER_COMBI_0, 0x01, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    // K321 = 0x1000 Output1 will be reset with combi n�1
    if (err = dsa_trigger_set_combi_dout_mask_s(axis, DSA_TRIGGER_COMBI_1, 0x10000, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    return 0;
}
//-------------------------------------------------------------------------//

// trigger_pulse_generator_setting allow:
//	- select the output adress for the pulse generator mode
//	- set pulse delay
//	- set pulse widTH
//	- set pulse quantity
int trigger_pulse_generator_setting(DSA_DRIVE *axis)
{

    int err = 0;

    /******************** Pulse generator setting*************************/
    // K340 = 0x0 for DOUT not used with PG
    if (err = dsa_trigger_set_pulse_generator_dout_mask_s(axis, DSA_TRIGGER_PG_1, 0x00, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    // K340:1 = 0x1 for FDOUT1 used with PG
    if (err = dsa_trigger_set_pulse_generator_fdout_mask_s(axis, DSA_TRIGGER_PG_1, 0x01, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    // K342 = 0 Time before the first pulse
    if (err = dsa_trigger_set_pulse_generator_delay_s(axis, DSA_TRIGGER_PG_1, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    // K343 = 2000000 Pulse time at logical state '1'. K343 Unit=10ns (minimum 2)
    if (err = dsa_trigger_set_pulse_generator_pulse_width_s(axis, DSA_TRIGGER_PG_1, 200000, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    // K344 = 10 ms Pulse time at logical state '0'. K344 Unit=10ns (minimum 2)
    if (err = dsa_trigger_set_pulse_generator_interval_s(axis, DSA_TRIGGER_PG_1, 0.01, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    // K345 = 3 Pulse number (0 = infinite pulse number)
    if (err = dsa_trigger_set_pulse_generator_number_s(axis, DSA_TRIGGER_PG_1, 3, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    return 0;
}
//-------------------------------------------------------------------------//

// trigger_combi_event_setting is the command ELx.<axis> = Target (start of the trigger), Type (positive or negative trigger), Combi number
int trigger_combi_event_setting(DSA_DRIVE *axis, int trigger_event_number, double trigger_start_on_position, long trigger_direction, long trigger_combi_number)
{

    int err = 0;

    /*********************** Event definition ********************************/
    // EVENT N�: activate the standard mode @ trigger_start_on_position combi n�1
    // EL0:0 = trigger_start_on_position
    // EL0:1 = trigger_direction (1L for positiv, 2L for negativ) EL0 Type = trigger_direction
    // EL0:2 = trigger_combi_number  start combi n�trigger_combi_number
    // EL0:3 = 0L Pulse generator  is not used
    if (err = dsa_trigger_set_standard_mode_event_s(axis,
                                                    trigger_event_number,
                                                    trigger_start_on_position,
                                                    trigger_direction,
                                                    trigger_combi_number,
                                                    DSA_TRIGGER_ACTION_NO_PULSE,
                                                    DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    return 0;
}
//-------------------------------------------------------------------------//

// trigger_pulse_generator_event_setting is the command ELx.<axis> = Target (start of the trigger), Type (positive or negative trigger), 16L (combi not used),
// PG (pulse generator selection), delta P (distance between pulse generation)
int trigger_pulse_generator_event_setting(DSA_DRIVE *axis,
                                          int        trigger_event_number,
                                          double     trigger_start_on_position,
                                          long       trigger_direction,
                                          long       generator_used,
                                          double     delta_position)
{

    int err = 0;

    /*********************** Event definition ********************************/
    // EVENT N�: activate the standard mode @ trigger_start_on_position combi n�1
    // EL0:0 = trigger_start_on_position
    // EL0:1 = trigger_direction (1L for positiv, 2L for negativ) EL0 Type = trigger_direction
    if (err = dsa_trigger_set_incremental_mode_event_s(axis,
                                                       trigger_event_number,
                                                       trigger_start_on_position,
                                                       trigger_direction,
                                                       generator_used,
                                                       delta_position,
                                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    return 0;
}
//-------------------------------------------------------------------------//

// trigger_start_event is function TRE.<axis> = P1 (first event to be started), P2 (number of event to be executed)
int trigger_start_event(DSA_DRIVE *axis, long trigger_event_number_begin, long trigger_event_quantity_execution)
{

    int err = 0;

    /*********************** Event starter ********************************/
    // Use the command n�104 (TRE.<axis>=P1,P2) to start the event
    if (err = dsa_trigger_enable_ex_s(axis, FALSE, trigger_event_number_begin, trigger_event_quantity_execution, FALSE, FALSE, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    return 0;
}
//-------------------------------------------------------------------------//

// trigger_stop_event is TRE.<axis> = -1 (disable triggers)
int trigger_stop_event(DSA_DRIVE *axis)
{

    int err = 0;

    if (err = dsa_trigger_disable_s(axis, FALSE, FALSE, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axis);
        printf("Error number is %d", err);
        return err;
    }

    return 0;
}
//-------------------------------------------------------------------------//

// Main function:
//	- create drive object (2 axis)
//	- open communication with UltimET PCI
//	- trigger initalisation (2 combi + 2 pulse generator)
//	- start trigger event
//	- start a 2 forward-back movement on axis0
//	- close communication
//	- destroy object
//	- error handling label
int main(int argc, char *argv[])
{

    int err = 0;
    int i;

    /*********** Define pointer on object ***********/
    DSA_DRIVE      *axisX   = NULL;
    DSA_DRIVE      *axisY   = NULL;
    DSA_MASTER     *ultimet = NULL;
    DSA_IPOL_GROUP *igrp    = NULL;

    /********** Create drive object ***********/
    if (err = dsa_create_drive(&axisX)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }

    if (err = dsa_create_drive(&axisY)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }

    if (err = dsa_create_master(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /*----------------------------------------------------------------*/
    /*--------------------- Open commnication ------------------------*/
    /*----------------------------------------------------------------*/

    /********** Open communication on axis  ***********/
    if (err = dsa_open_u(ultimet, "etb:ETN://172.22.10.216:*")) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    if (err = dsa_open_u(axisX, "etb:ETN://172.22.10.216:0")) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }

    if (err = dsa_open_u(axisY, "etb:ETN://172.22.10.216:1")) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }

    /******************** Restet error ******************/
    if (err = dsa_reset_error_ex_s(axisX, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }

    if (err = dsa_reset_error_ex_s(axisY, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }

    if (err = dsa_reset_error_ex_s(ultimet, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /********** Power on on axis0 ***********/
    if (err = dsa_power_on_s(axisX, 10000)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }

    /********** Indexation of axis0 ***********/
    if (err = dsa_homing_start_s(axisX, 12000)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }

    /********** WTM of axis0 ***********/
    if (err = dsa_wait_movement_s(axisX, 60000)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }

    /********** Trigger initalisation on axisX ***********/
    trigger_output_setting(axisX);

    trigger_combi_setting(axisX);
    trigger_pulse_generator_setting(axisX);

    trigger_combi_event_setting(axisX, 0, 0.01, DSA_TRIGGER_POSITIVE, DSA_TRIGGER_COMBI_0);
    trigger_combi_event_setting(axisX, 1, 0.02, DSA_TRIGGER_POSITIVE, DSA_TRIGGER_COMBI_1);
    trigger_pulse_generator_event_setting(axisX, 2, 0.04, DSA_TRIGGER_NEGATIVE, DSA_TRIGGER_ACTION_START_PG1, -0.02);
    trigger_pulse_generator_event_setting(axisX, 3, 0.03, DSA_TRIGGER_NEGATIVE, DSA_TRIGGER_ACTION_STOP_PG1, 0.0);

    for (i = 0; i < 2; i++) {
        trigger_start_event(axisX, 0, 4);

        if (err = dsa_start_profiled_movement_ex_s(axisX, 0, 0.07, 0.5, 0.5, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }

        if (err = dsa_wait_movement_s(axisX, 60000)) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }

        if (err = dsa_start_profiled_movement_ex_s(axisX, 0, -0.065, 0.5, 0.5, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }

        if (err = dsa_wait_movement_s(axisX, 60000)) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }
    }

    trigger_stop_event(axisX);

    if (err = dsa_power_off_s(axisX, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }

    /*----------------------------------------------------------------*/
    /*--------------------- Close communication-----------------------*/
    /*----------------------------------------------------------------*/

    if (err = dsa_close(axisX)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }

    if (err = dsa_close(axisY)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }

    if (err = dsa_close(ultimet)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }
    /*----------------------------------------------------------------*/
    /*----------------- Drive & group destruction --------------------*/
    /*----------------------------------------------------------------*/

    if (err = dsa_destroy(&axisX)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }

    if (err = dsa_destroy(&axisY)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }

    if (err = dsa_destroy(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /*----------------------------------------------------------------*/
    /*---------------------   Error handling  ------------------------*/
    /*----------------------------------------------------------------*/

_error:

    // Is the drive pointer valid ?
    if (dsa_is_valid_drive(axisX)) {

        // Is the drive open
        ebool open = 0;
        dsa_is_open(axisX, &open);
        if (open) {

            // Stop all movements
            dsa_quick_stop_s(axisX, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);

            // When the motor has stopped, a power off is done
            dsa_wait_movement_s(axisX, 60000);
            dsa_power_off_s(axisX, 10000);

            // Close the connection
            dsa_close(axisX);
        }

        // And finally, release all resources to the OS
        dsa_destroy(&axisX);
    }

    // Same operations for the second drive
    if (dsa_is_valid_drive(axisY)) {

        // Is the drive open ?
        ebool open = 0;
        dsa_is_open(axisY, &open);
        if (open) {

            // Stop all movements
            dsa_quick_stop_s(axisY, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);

            // When the motor has stopped, a power off is done
            dsa_wait_movement_s(axisY, 60000);
            dsa_power_off_s(axisY, 10000);

            // Close the connection
            dsa_close(axisY);
        }

        // And finally, release all resources to the OS
        dsa_destroy(&axisY);
    }

    // Same operations for the UltimET
    if (dsa_is_valid_master(ultimet)) {

        // Is the drive open ?
        ebool open = 0;
        dsa_is_open(ultimet, &open);
        if (open) {

            // Close the connection
            dsa_close(ultimet);
        }

        // And finally, release all resources to the OS
        dsa_destroy(&ultimet);
    }

    getchar();

    return 1;
}
//-------------------------------------------------------------------------//