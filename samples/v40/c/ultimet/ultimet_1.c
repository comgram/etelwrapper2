/*
 * ultimet_1.c
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
 * This simple demo program shows how to make basic interpolated movements
 * on a X-Y axes system using ETEL's EDI libraries and the ULTIMET multi-axis
 * board. The program will switch the motors on, send an indexation command,
 * make a simple movement, switch to the intepolation mode and interpolate a
 * G-code movement.
 */

/*
 * To run this demo without modification, you must have:
 *  - two linear motors with one AccurET controllers configured.
 *  - one ULTIMET multi-axis board on the PCI bus, configured like this
 *  - a TransnET connection between the drives and the ULTIMET.
 *  - the drives must have the axis number 0 and 1.
 *  - KL45 must be set to insure that position 0 is inside the valid range.
 *  - KL34 and KL35 must be set properly.
 */

/*** libraries ***/

/*
 * Include the standard I/O library.
 */
#include <conio.h>
#include <stdio.h>

/*
 * Include EDI high level library version 3.0x
 */
#include <dsa40.h>

/*** functions ***/

/*
 * Entry point function.
 */
int main(int argc, char *argv[])
{
    /* Define two drive objects. */
    DSA_DRIVE *x = NULL;
    DSA_DRIVE *y = NULL;

    /* Define a ULTIMET object. This object represents a multi-axis board */
    DSA_MASTER *ultimet = NULL;

    /*
     * Define an interpolation group. This group can be used like a normal
     * device or drive group. All drives within this group can also be
     * interpolated.
     */
    DSA_IPOL_GROUP *igrp = NULL;

    /* Definition of some global variable. */
    int err;

    /* Create the drive and ULTIMET objects. */
    if (err = dsa_create_drive(&x)) {
        DSA_EXT_DIAG(err, x);
        goto _error;
    }
    if (err = dsa_create_drive(&y)) {
        DSA_EXT_DIAG(err, y);
        goto _error;
    }
    if (err = dsa_create_master(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /* Create the interpolation group object and set it up. */
    if (err = dsa_create_ipol_group(&igrp, 2)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_set_group_item(igrp, 0, x)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_set_group_item(igrp, 1, y)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Associate the ULTIMET to the interpolation group. To make interpolated
     * movement (like lines, circles and PVT), you need a multi-axis
     * board. When you ask for an interpolated movement on a group of axes, the
     * dsa30 library will use the ULTIMET associated with the group.
     */
    if (err = dsa_set_master(igrp, ultimet)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Create connections. To have access to a ULTIMET board on the PCI bus, you
     * must specify the following string as url:
     *     etb:ULTIMET:<axis_number>
     * For the ULTIMET, you must specify "*" as an axis number.
     * During the opening procedure, you can force the hardware reset of the
     * ULTIMET and the drives. To do that, you must specify the following url:
     *     etb:ULTIMET,<reset_params>:<axis_number>
     * where <reset_params> can take the following values:
     *     r     to reset the ULTIMET
     *     x     to reset all the drives
     *     rx    to reset the ULTIMET and all the drives.
     */
    if (err = dsa_open_u(x, "etb:ULTIMET:0")) {
        DSA_EXT_DIAG(err, x);
        goto _error;
    }
    if (err = dsa_open_u(y, "etb:ULTIMET:1")) {
        DSA_EXT_DIAG(err, y);
        goto _error;
    }
    if (err = dsa_open_u(ultimet, "etb:ULTIMET:*")) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /* Reset error of both axes */
    if (err = dsa_reset_error_ex_s(igrp, 0, 10000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /* Reset error of ultimet */
    if (err = dsa_reset_error_ex_s(ultimet, 0, 10000)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /* Power on both axes. */
    if (err = dsa_power_on_s(igrp, 10000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /* Start indexation on both axes. */
    if (err = dsa_homing_start_s(igrp, 10000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /* Wait for the end of the movement of both axes. */
    if (err = dsa_wait_movement_s(igrp, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /* Make a movement to a specified point. */
    if (err = dsa_set_target_position_s(x, 0, 0.01, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, x);
        goto _error;
    }
    if (err = dsa_set_target_position_s(y, 0, 0.02, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, y);
        goto _error;
    }

    /*
     * Wait for the end of the movement on both axes. Here, we use the
     * interpolation group like a normal group. The ULTIMET associated with this
     * group isn't taken into account.
     */
    if (err = dsa_wait_movement_s(igrp, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Now we enter in interpolated mode. In this mode the position of the
     * motors is controlled by the multi-axis board. This is right only for
     * drives that are specified in the interpolation group (igrp). When you
     * are in this mode, you cannot use the dsa_set_target_position_x()
     * functions on the drives specified in the igrp. You must use the
     * dsa_ipol_xxx() functions instead to define the interpolated path. The
     * interpolator (the ULTIMET) will drive the motors along this path.
     * Actually, the X-Y system is in the following absolute position (in
     * meters):
     *     x = 0.01, y = 0.02
     * When you enter into interpolated mode, the actual position becomes the
     * reference postition for the interpolation. For the interpolation
     * functions (dsa_ipol_xxx()), all coordinates are given in absolute from
     * this reference point. Practically, after the call to dsa_ipol_begin_s()
     * you are in the point:
     *     x = 0.0, y = 0.0
     * When you leave the interpolated mode, you come back to the previous
     * reference system.
     */
    if (err = dsa_ipol_begin_s(igrp, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Define the maximum tengential speed and acceleration that the
     * interpolator must perform during the interpolation.
     * The tangential speed is set to 0.05 m/s.
     * The tangential acceleration and deceleration are set to 0.1 m/(s*s).
     */
    if (err = dsa_ipol_tan_velocity_s(igrp, 0.05, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_ipol_tan_acceleration_s(igrp, 0.1, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_ipol_tan_deceleration_s(igrp, 0.1, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Add a linear segment to the interpolation path. After this function,
     * the interpolator must directly start the movement along this segment
     * and stop it at the end of the segment.
     */
    if (err = dsa_ipol_line_2d_s(igrp, -0.005, 0.001, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Add another linear segment to the interpolation path.
     */
    if (err = dsa_ipol_line_2d_s(igrp, 0.005, 0.001, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Start the concatenation of the interpolated segments. In this mode
     * all segments are processed at constant speed, without stoping between
     * segments. This means that when the interpolator processes the
     * previous segment, it does not decrease the speed at the end of the
     * segment. It jumps to the next segment without speed change.
     */
    if (err = dsa_ipol_begin_concatenation_s(igrp, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Add a circular segment to the interpolation path. This segment is
     * defined by the destination point (second and third parameters) and
     * the center of the arc (fourth and fifth parameters) in the X-Y plane.
     * The circular segment is processed in the counter-clockwise direction.
     */
    if (err = dsa_ipol_circle_ccw_c2d_s(igrp, 0.008, 0.002, 0.005, 0.006, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Add some segments to the interpolation path.
     */
    if (err = dsa_ipol_line_2d_s(igrp, 0.012, 0.005, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_ipol_circle_cw_c2d_s(igrp, 0.019, -0.002, 0.015, 0.001, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_ipol_line_2d_s(igrp, 0.016, -0.006, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Stop the concatenated mode. After this command, the interpolator stops
     * the movement at the end of the previous segment and between each
     * following segments. Practically, the interpolator decreases
     * the speed in order to arrive at the end of the segment with a null
     * speed. If you don't call dsa_ipol_begin_concatenation_s(), the
     * interpolator must restart the movement at the beginning of the next
     * segment, stop at its end, restart at the beginning of the next one and
     * so on for each following segments.
     */
    if (err = dsa_ipol_end_concatenation_s(igrp, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Add a new segment to the trajectory path.
     */
    if (err = dsa_ipol_circle_ccw_c2d_s(igrp, 0.007, -0.003, 0.012, -0.003, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Go back to the concatenation mode. In this way, the interpolator appends
     * to the next segment without stoping the movement
     * between them.
     */
    if (err = dsa_ipol_begin_concatenation_s(igrp, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Add a few segments to the trajectory path.
     */
    if (err = dsa_ipol_circle_cw_c2d_s(igrp, 0.003, -0.003, 0.005, -0.003, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Stop concatenation.
     */
    if (err = dsa_ipol_end_concatenation_s(igrp, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Just a few segments again.
     */
    if (err = dsa_ipol_line_2d_s(igrp, -0.005, -0.003, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_ipol_line_2d_s(igrp, -0.007, -0.005, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Wait for the end of the interpolated movement.
     */
    if (err = dsa_ipol_wait_movement_s(igrp, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Leave the interpolation mode and come back to the previous reference
     * system.
     */
    if (err = dsa_ipol_end_s(igrp, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Power off the motors.
     */
    if (err = dsa_power_off_s(igrp, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /* Close all connections. */
    if (err = dsa_close(x)) {
        DSA_EXT_DIAG(err, x);
        goto _error;
    }
    if (err = dsa_close(y)) {
        DSA_EXT_DIAG(err, y);
        goto _error;
    }
    if (err = dsa_close(ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /* We should then destroy the devices and group objects. */
    if (err = dsa_destroy(&igrp)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_destroy(&x)) {
        DSA_EXT_DIAG(err, x);
        goto _error;
    }
    if (err = dsa_destroy(&y)) {
        DSA_EXT_DIAG(err, y);
        goto _error;
    }
    if (err = dsa_destroy(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /* Everything seems successful, we can exit now. */
    printf("success.\n");
    return 0;

    /* Error handler. */
_error:
    /* Leave the interpolation mode. */
    if (dsa_is_ipol_in_progress(igrp))
        dsa_ipol_end_s(igrp, DSA_DEF_TIMEOUT);

    /* Destroy the group if not already done. */
    if (dsa_is_valid_ipol_group(igrp))
        dsa_destroy(&igrp);

    /* Is the drive pointer valid ? */
    if (dsa_is_valid_drive(x)) {

        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(x, &open);
        if (open) {

            /* Stop all movements. */
            dsa_quick_stop_s(x, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);

            /* When the motor has stopped, a power off is done. */
            dsa_wait_movement_s(x, 60000);
            dsa_power_off_s(x, 10000);

            /* Close the connection. */
            dsa_close(x);
        }

        /* And finally, release all resources to the OS. */
        dsa_destroy(&x);
    }

    /* Same operations for the second drive */
    if (dsa_is_valid_drive(y)) {

        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(y, &open);
        if (open) {

            /* Stop all movements. */
            dsa_quick_stop_s(y, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);

            /* When the motor has stopped, a power off is done. */
            dsa_wait_movement_s(y, 60000);
            dsa_power_off_s(y, 10000);

            /* Close the connection */
            dsa_close(y);
        }

        /* And finally, release all resources to the OS. */
        dsa_destroy(&y);
    }

    /* Same operations for the ULTIMET */
    if (dsa_is_valid_master(ultimet)) {

        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(ultimet, &open);
        if (open) {

            /* Close the connection */
            dsa_close(ultimet);
        }

        /* And finally, release all resources to the OS. */
        dsa_destroy(&ultimet);
    }

    return 1;
}
