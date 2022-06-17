/*
 * ultimet_2.c
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
 * This is a simple program that use the ULTIMET multi-axis board to perform
 * an interpolated movememt on a X-Y axes system with PVT commands.
 * PVT means Position - Velocity - Time. A movement is defined by the position
 * of the destination point (in x,y,z and theta coordinates), the velocity at
 * the destination point, and the time to do the displacement.
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
#include <stdio.h>

/*
 * Include EDI high level library version 3.0x.
 */
#include <dsa40.h>

/*
 * Include another EDI library to make some defines available.
 */
#include <dmd40.h>

/*** functions ***/

/*
 * Entry point function.
 */
int main(int argc, char *argv[])
{
    /* Define two drive objects. */
    DSA_DRIVE *x = NULL;
    DSA_DRIVE *y = NULL;

    /* Define a ULTIMET object. */
    DSA_MASTER *ultimet = NULL;

    /* Define a group that can be interpolated. */
    DSA_IPOL_GROUP *igrp = NULL;

    /*
     * Define vectors used to specify the destination point and velocity of a
     * PVT segment. Be careful to always set the size of the structures.
     */
    DSA_VECTOR destination = {sizeof(DSA_VECTOR)};
    DSA_VECTOR velocity    = {sizeof(DSA_VECTOR)};

    /* Define some global variable. */
    double time;
    int    err;

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
        DSA_EXT_DIAG(err, y);
        goto _error;
    }
    if (err = dsa_set_master(igrp, ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /* Create connections. */
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
    if (err = dsa_wait_movement_s(igrp, 600000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /* Make a movement to a specified point. */
    if (err = dsa_set_target_position_s(x, 0, 0.0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, x);
        goto _error;
    }
    if (err = dsa_set_target_position_s(y, 0, -0.03, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, y);
        goto _error;
    }
    if (err = dsa_wait_movement_s(igrp, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Start of the trajectory
     */
    printf("Start of the trajectory\n");

    /*
     * Now we enter into interpolated mode.
     */
    if (err = dsa_ipol_begin_s(igrp, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * PVT command is defined by its destination point, the velocity at this
     * point, and the time of the displacement.
     */

    /*
     * Define the first PVT destination point, velocity and time:
     * x    = 10.0e-3 m
     * vx   = 50e-3 m/s
     * y    = 0 m
     * vy   = 0 m/s
     * time = 200.0e-3 s
     */
    destination.val.dim.x = 10.0e-3;
    destination.val.dim.y = 0;

    velocity.val.dim.x = 50.0e-3;
    velocity.val.dim.y = 0;

    time = 200.0e-3;

    /*
     * Add this PVT segment to the trajectory path. After this function call,
     * the interpolator will directly start the movement along this segment
     * and the next ones.
     */
    if (err = dsa_ipol_pvt_s(igrp, &destination, &velocity, time, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Define the 2nd PVT destination point, velocity and time:
     * x    = 110.0e-3 m
     * vx   = 50e-3 m/s
     * y    = 0 m
     * vy   = 0 m/s
     * time = 2.0 s
     */
    destination.val.dim.x = 110.0e-3;
    destination.val.dim.y = 0;

    velocity.val.dim.x = 50.0e-3;
    velocity.val.dim.y = 0;

    time = 2.0;

    /*
     * Add this PVT segment to the trajectory path.
     */
    if (err = dsa_ipol_pvt_s(igrp, &destination, &velocity, time, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*
     * Define and add some other PVT segments:
     */
    destination.val.dim.x = 110.0e-3;
    destination.val.dim.y = 0;
    velocity.val.dim.x    = 0;
    velocity.val.dim.y    = 50.0e-3;
    time                  = 1.0;
    if (err = dsa_ipol_pvt_s(igrp, &destination, &velocity, time, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    destination.val.dim.x = 110.0e-3;
    destination.val.dim.y = 100.0e-3;
    velocity.val.dim.x    = 0;
    velocity.val.dim.y    = 50.0e-3;
    time                  = 2.0;
    if (err = dsa_ipol_pvt_s(igrp, &destination, &velocity, time, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    destination.val.dim.x = 110.0e-3;
    destination.val.dim.y = 100.0e-3;
    velocity.val.dim.x    = -50.0e-3;
    velocity.val.dim.y    = 0;
    time                  = 1.0;
    if (err = dsa_ipol_pvt_s(igrp, &destination, &velocity, time, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    destination.val.dim.x = 10.0e-3;
    destination.val.dim.y = 100.0e-3;
    velocity.val.dim.x    = -50.0e-3;
    velocity.val.dim.y    = 0;
    time                  = 2.0;
    if (err = dsa_ipol_pvt_s(igrp, &destination, &velocity, time, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    destination.val.dim.x = 10.0e-3;
    destination.val.dim.y = 100.0e-3;
    velocity.val.dim.x    = 0;
    velocity.val.dim.y    = -50.0e-3;
    time                  = 1.0;
    if (err = dsa_ipol_pvt_s(igrp, &destination, &velocity, time, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    destination.val.dim.x = 10.0e-3;
    destination.val.dim.y = 0;
    velocity.val.dim.x    = 0;
    velocity.val.dim.y    = -50.0e-3;
    time                  = 2.0;
    if (err = dsa_ipol_pvt_s(igrp, &destination, &velocity, time, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    destination.val.dim.x = 10.0e-3;
    destination.val.dim.y = -10e-3;
    velocity.val.dim.x    = 0;
    velocity.val.dim.y    = 0;
    time                  = 200.0e-3;
    if (err = dsa_ipol_pvt_s(igrp, &destination, &velocity, time, 60000)) {
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
     * Leave the interpolation mode.
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

    /* Destroy the objects. */
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

            /* Close the connection. */
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
