/*
 * sample_2.c
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
 * This second sample program extends sample_1.c and shows how to use the
 * library to control multiple axes in once. It will move a x/y table
 * to random positions.
 */

/*
 * To run this demo without modification, you must have:
 *  - two linear motor with one AccurET controller configured.
 *  - a USB connection between the drive and the PC.
 *  - the drives must have the axes number 0 and 1.
 *  - KL45 must be set to insure that position 0 is inside the valid range.
 *  - KL34 and KL35 must be set properly.
 */

/*** libraries ***/

/*
 * Include the standard I/O library and utility library (for rand()).
 */
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Include EDI high level library version 3.0x.
 */
#include <dsa40.h>

/*** prototypes ***/

/*
 * This utility function is defined to perform a simple xy movement.
 */
static int move_xy(DSA_DRIVE_GROUP *xy, double x, double y);

/*** functions ***/

/*
 * Entry point function.
 */
int main(int argc, char *argv[])
{
    /* Stores the min/max position of each axis. */
    double pos_min[2], pos_max[2];

    /*
     * Now, we must define two pointers on DSA_DRIVE objects, for each axis.
     * We must also define a pointer to a DSA_DRIVE_GROUP object. This hidden
     * object is also defined in the dsa30 library, and can be considered as
     * a sort of array containing other DSA_DRIVE objects. The client can set,
     * change and retrieve the different drives belonging to this object
     * through some accessor functions. As with DSA_DRIVE, the drv pointer
     * *must* be initialized to NULL before calling the created group function,
     * otherwise this function will fail.
     */
    DSA_DRIVE       *drv[2] = {NULL, NULL};
    DSA_DRIVE_GROUP *grp    = NULL;

    /* Defines some global variables. */
    int err;
    int i;

    /* Create two drive objects. */
    for (i = 0; i < 2; i++) {
        if (err = dsa_create_drive(drv + i)) {
            DSA_EXT_DIAG(err, drv[i]);
            goto _error;
        }
    }

    /*
     * Now, we should create the groupe object. The size for the group must
     * be given at the creation time, and cannot currently be changed afterwards.
     * In our case, we want to put two drives in this group.
     */
    if (err = dsa_create_drive_group(&grp, 2)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /*
     * After creating the group, it should be filled with the required drives.
     * The second argument in the dsa_set_group_item() function is the
     * position inside the array where the drive object must be set.
     * In place of dsa_set_group_item(), you can call dsa_add_group_item()
     * that need only two arguments: the group and the drive to put on in.
     */
    for (i = 0; i < 2; i++) {
        if (err = dsa_set_group_item(grp, i, drv[i])) {
            DSA_EXT_DIAG(err, drv[i]);
            goto _error;
        }
    }

    /*
     * Now, we must establish the connection by calling the dsa_open_u()
     * function for each drive, as previously done. Note that the first call
     * in the dsa_open_u() function for a specific bus will take more time
     * to be performed than subsequent ones, because the underlaying ETB_BUS
     * object used to access the drives is shared by all DSA_DRIVE objects
     * and the first call to dsa_open_u() will have to initialize and open it.
     */
    if (err = dsa_open_u(drv[0], "etb:USB:0")) {
        DSA_EXT_DIAG(err, drv[0]);
        goto _error;
    }
    if (err = dsa_open_u(drv[1], "etb:USB:1")) {
        DSA_EXT_DIAG(err, drv[1]);
        goto _error;
    }

    /*
     * Now we can send commands to both drives at the same time by giving
     * the group object instead of the one of the drive object. The next call
     * is two time more efficient than calling successively the fonction on
     * each drive.
     */
    if (err = dsa_power_on_s(grp, 10000)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* Start indexation on both axes. */
    if (err = dsa_homing_start_s(grp, 10000)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /*
     * We must now read the minimum and maximum values of each axis, but this
     * can obviously not been done with the group object, because every axis
     * of the group can have different values. In fact, none of the getter
     * functions (dsa_get_xxx) will accept a DSA_DRIVE_GROUP as its first
     * parameter.
     */
    for (i = 0; i < 2; i++) {
        if (err = dsa_get_min_soft_position_limit_s(drv[i], &pos_min[i], DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, drv[i]);
            goto _error;
        }
        if (err = dsa_get_max_soft_position_limit_s(drv[i], &pos_max[i], DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, drv[i]);
            goto _error;
        }
    }

    /* Wait for the end of the movement of both axes. */
    if (err = dsa_wait_movement_s(grp, 60000)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* Set speed of both axes. */
    if (err = dsa_set_profile_velocity_s(grp, 0, 1.0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* Set acceleration of both axes. */
    if (err = dsa_set_profile_acceleration_s(grp, 0, 1.0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /*
     * The main part of the program comes here, but we have created a separated
     * function called move_xy() to perform a movement and wait until its end
     * before returning. Here we will just generate the random coordinates and
     * pass them to the move_xy() function. After 100 iterations, we will jump
     * to the (0,0) position and stop the program.
     */
    for (i = 0; i < 10; i++) {
        double x = pos_min[0] + (rand() * (pos_max[0] - pos_min[0])) / RAND_MAX;
        double y = pos_min[1] + (rand() * (pos_max[1] - pos_min[1])) / RAND_MAX;
        if (err = move_xy(grp, x, y))
            goto _error;
    }
    if (err = move_xy(grp, 0.0, 0.0))
        goto _error;

    /* Power off the motor. */
    if (err = dsa_power_off_s(grp, 60000)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* Close both connections. */
    for (i = 0; i < 2; i++) {
        if (err = dsa_close(drv[i])) {
            DSA_EXT_DIAG(err, drv[i]);
            goto _error;
        }
    }

    /* We should then destroy the group object. */
    if (err = dsa_destroy(&grp)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* We should then destroy the drive objects. */
    for (i = 0; i < 2; i++) {
        if (err = dsa_destroy(drv + i)) {
            DSA_EXT_DIAG(err, drv[i]);
            goto _error;
        }
    }

    /* Success. */
    printf("success.\n");
    _getch();
    return 0;

    /* Error handler. */
_error:
    /*
     * First destroy the group if not already done, it is no more useful.
     */
    if (dsa_is_valid_drive_group(grp))
        dsa_destroy(&grp);

    /*
     * Then check the status of each drive object individually.
     */
    for (i = 0; i < 2; i++) {

        /* Is the drive pointer valid ? */
        if (dsa_is_valid_drive(drv[i])) {

            /* Is the drive open ? */
            ebool open = 0;
            dsa_is_open(drv[i], &open);
            if (open) {

                /*
                 * We can check if a motor is switched on by reading the first
                 * bit (the lsb) of M60. To do that, we use the
                 * dsa_get_register_int32_s() function. The variable that stores the
                 * value returned by dsa_get_register_int32_s() has to be initialized
                 * correctly in order to properly handle the possibility of a
                 * failure in the request.
                 */
                int m60 = 1;
                dsa_get_register_int32_s(drv[i],          /* the drive object */
                                         3,               /* 3 = M register */
                                         60,              /* index */
                                         0,               /* subindex */
                                         &m60,            /* returned value */
                                         DSA_GET_CURRENT, /* kind of value */
                                         DSA_DEF_TIMEOUT  /* default timeout */
                );
                if (m60 & 0x00000001) {
                    /* Stop all movements. */
                    dsa_quick_stop_s(drv[i], DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);
                    /* When the motor has stopped, a power off is done. */
                    dsa_wait_movement_s(drv[i], 60000);
                    dsa_power_off_s(drv[i], 60000);
                }

                /* Close the connection. */
                dsa_close(drv[i]);
            }

            /*
             * Finally, release all resources to the OS.
             */
            dsa_destroy(drv + i);
        }
    }

    _getch();
    return 1;
}

/*
 * move_xy() function.
 */
static int move_xy(DSA_DRIVE_GROUP *xy, double x, double y)
{
    /* The common variables must be defined. */
    int err;
    int i;

    /*
     * The first thing to do is to retrieve the individual DSA_DRIVE contained
     * in the xy group. We will need them because we want to assign different
     * positions to each of them. These drive objects could be passed with
     * additional parameters, but we want to keep the design clean.
     */
    DSA_DRIVE *drv[2];
    printf("Move %f %f\n", x, y);
    for (i = 0; i < 2; i++) {
        if (err = dsa_get_group_item(xy, i, drv + i)) {
            DSA_EXT_DIAG(err, xy);
            goto _error;
        }
    }

    /*
     * We can now set the target position for both axes, but this time, we will
     * not write it into subindex zero, because we want the movement to start
     * synchronously on both axes. Instead, we will first write the movement in
     * the first buffer (subindex 1) and start both movements with another
     * function. This is the recommended method when doing multi-axis movement.
     */
    for (i = 0; i < 2; i++) {
        if (err = dsa_set_target_position_s(drv[i], 1, (i ? y : x), DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, xy);
            goto _error;
        }
    }

    /*
     * The dsa_new_setpoint_s() function order the drive to use the information
     * contained in one of its buffer to start a new movement. The second
     * parameter of the method specifies which buffer to use (1-3 currently
     * defined) while the third param is a mask and should contain the logical
     * sum of all the following constants, specifying which information must
     * be fetched from the buffer. The constants include:
     *  - 0x0001: use the target position of the given buffer
     *  - 0x0002: use the profile velocity of the given buffer
     *  - 0x0004: use the profile acceleration of the given buffer
     *  - ...
     * In our case, we just use the buffer to store the new target position.
     * We will then use 0x0001 for the mask.
     * This function is applied to the whole group, which means that if the two
     * controllers are on the same bus, the two motors will start at the same
     * time, regardless of what happens on the PC.
     * With Turbo-ETEL-Bus (TEB), you can synchronize all drives together. In
     * this case, the movements really start exactly at the same time (within
     * one microsecond). However, in this example, we communicate with the
     * drives through the ETEL-Bus (ETB). With this bus, the two motors
     * will start within a delay of one millisecond.
     */
    if (err = dsa_new_setpoint_s(xy, 1, 0x0001, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, xy);
        goto _error;
    }

    /* Wait for the end of the movement. */
    if (err = dsa_wait_movement_s(xy, 60000)) {
        DSA_EXT_DIAG(err, xy);
        goto _error;
    }

    /* Success. */
    return 0;

    /* Error handler. */
_error:
    /* Simply return the error code to the caller. */
    return err;
}
