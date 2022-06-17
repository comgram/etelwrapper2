/*
 * sample_3.c
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
 * This third sample program extends sample_2.c and shows how to use
 * multithreading with ETEL's dlls. In this program, we will add a thread to
 * monitor the current position of the drive and a thread for emergency
 * stop. The monitoring thread will show the current position of both axes
 * every 100ms, and the second thread will wait indefinitely for a command from
 * the user: the space key will immediately stop the sequence, which will run
 * indefinitely otherwise.
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

#include <Windows.h>

/* Include the standard I/O library, utilities and character conversion. */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/* Include the platform specific header for thread managment. */
#include <process.h>

/* Include the platform specific header for keyboard input. */
#include <conio.h>

#include <dsa40.h>
/* Include EDI high level library version 3.0x. */

/*** prototypes ***/

/*
 * This utility function is defined to perform a simple xy movement.
 */
static int move_xy(DSA_DRIVE_GROUP *xy, double x, double y);

/*
 * The following two functions define the threads which will run independently.
 * The exact parameter type of theses functions as well as the return value
 * are platform specific.
 */
static void display_thread(void *param);
static void keyboard_thread(void *param);

/*** variables ***/

/*
 * This global variable can be set by any thread to request the end of the
 * program. The main thread will listen to this variable at each cycle.
 */
static ebool abort_req;

/*** functions ***/

/*
 * Entry point function.
 */
int main(int argc, char *argv[])
{
    /* Stores the min/max position of each axis. */
    double pos_min[2], pos_max[2];

    /* Definition of the drive and the group objects. */
    DSA_DRIVE       *drv[2] = {NULL, NULL};
    DSA_DRIVE_GROUP *grp    = NULL;

    /* Definition of some global variable. */
    int err;
    int i;

    /* Create two drive objects. */
    for (i = 0; i < 2; i++) {
        if (err = dsa_create_drive(drv + i)) {
            DSA_EXT_DIAG(err, drv[i]);
            goto _error;
        }
    }

    /* Create the group object and set it up. */
    if (err = dsa_create_drive_group(&grp, 2)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }
    for (i = 0; i < 2; i++) {
        if (err = dsa_set_group_item(grp, i, drv[i])) {
            DSA_EXT_DIAG(err, grp);
            goto _error;
        }
    }

    /* Create connections. */
    if (err = dsa_open_u(drv[0], "etb:USB:0")) {
        DSA_EXT_DIAG(err, drv[0]);
        goto _error;
    }
    if (err = dsa_open_u(drv[1], "etb:USB:1")) {
        DSA_EXT_DIAG(err, drv[1]);
        goto _error;
    }

    /*
     * We will start the two threads before sending any command to the
     * controllers. The mechanism for starting the two thread is platform
     * dependant.
     */
    if (_beginthread(display_thread, 0, grp) <= 0) {
        err = DSA_ESYSTEM;
        printf("ERROR at file %s, line %d %s\n", __FILE__, __LINE__, dsa_translate_edi_error(err));
        goto _error;
    }
    if (_beginthread(keyboard_thread, 0, grp) <= 0) {
        err = DSA_ESYSTEM;
        printf("ERROR at file %s, line %d %s\n", __FILE__, __LINE__, dsa_translate_edi_error(err));
        goto _error;
    }

    /* Power on both axes. */
    if (err = dsa_power_on_s(grp, 10000)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* Start the indexation on both axes. */
    if (err = dsa_homing_start_s(grp, 10000)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    /* Retrieve minimum / maximum positions. */
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
     * Main program loop - the program will run indefinitely until the ESC or
     * SPACE key will be hit by the user.
     */
    while (!abort_req) {
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

    /* We should then destroy the group and the drive object. */
    if (err = dsa_destroy(&grp)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }
    for (i = 0; i < 2; i++) {
        if (err = dsa_destroy(drv + i)) {
            DSA_EXT_DIAG(err, drv[i]);
            goto _error;
        }
    }

    /* Success. */
    printf("\nsuccess.\n");
    _getch();
    return 0;

    /* Error handler. */
_error:
    /* Destroy the group if not already done. */
    if (dsa_is_valid_drive_group(grp))
        dsa_destroy(&grp);

    /* Check the status of each drive object individually. */
    for (i = 0; i < 2; i++) {

        /* Is the drive pointer valid ? */
        if (dsa_is_valid_drive(drv[i])) {

            /* Is the drive open ? */
            ebool open = 0;
            dsa_is_open(drv[i], &open);
            if (open) {

                /* Stop movements. */
                dsa_quick_stop_s(drv[i], DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);

                /* When the motor has stopped, a power off is done. */
                dsa_wait_movement_s(drv[i], 60000);
                dsa_power_off_s(drv[i], 60000);

                /* Close the connection. */
                dsa_close(drv[i]);
            }

            /* Finally, release all resources to the OS. */
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

    /* Get individual drive objects. */
    DSA_DRIVE *drv[2];
    for (i = 0; i < 2; i++) {
        if (err = dsa_get_group_item(xy, i, drv + i)) {
            DSA_EXT_DIAG(err, drv[i]);
            goto _error;
        }
    }

    /* Set target positions. */
    for (i = 0; i < 2; i++) {
        if (err = dsa_set_target_position_s(drv[i], 1, (i ? y : x), DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, drv[i]);
            goto _error;
        }
    }

    /* Start movement. */
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

/*
 * Display thread function.
 */
static void display_thread(void *param)
{
    /* the common variables must be defined */
    int counter = 0;
    int err;
    int i;

    /*
     * Casting of parameter to a group of pointer.
     */
    DSA_DRIVE_GROUP *grp = (DSA_DRIVE_GROUP *)param;

    /* Get individual drive objects. */
    DSA_DRIVE *drv[2];
    for (i = 0; i < 2; i++) {
        if (err = dsa_get_group_item(grp, i, drv + i)) {
            DSA_EXT_DIAG(err, drv[i]);
            goto _error;
        }
    }

    /*
     * Start an infinite loop. This function will be stopped by the OS only
     * when the whole process will end.
     */
    for (; !abort_req;) {
        /*
         * Each 100 ms, we will display the status and the position of each
         * drive. As usually, the status structure must be initialized before
         * using it.
         */
        DSA_STATUS status[] = {sizeof(DSA_STATUS), sizeof(DSA_STATUS)};
        double     pos[2];

        /* Get the position of each axis. */
        for (i = 0; i < 2; i++) {
            int err;
            if (err = dsa_get_position_actual_value_s(drv[i], &pos[i], DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
                DSA_EXT_DIAG(err, drv[i]);
                goto _error;
            }
        }

        /*
         * Get the drive status. Theses functions don't generate any activity
         * on the communication channel. The status of each drive is always
         * kept up-to-date in the process memory, which means that theses
         * function are very effective and could be often called without
         * degrading performances.
         */
        for (i = 0; i < 2; i++) {
            int err;
            if (err = dsa_get_status(drv[i], status + i)) {
                DSA_EXT_DIAG(err, drv[i]);
                goto _error;
            }
        }

        /*
         * We can now print the status string on the display.
         */
        printf("%04d: ", ++counter);
        for (i = 0; i < 2; i++) {
            printf("AXIS %d: %c%c%c/%20.4f mm",
                   i,
                   status[i].drive.moving ? 'M' : '-',
                   status[i].drive.warning ? 'W' : '-',
                   status[i].drive.error ? 'E' : '-',
                   pos[i] * 1.0E3);
            printf((i == 0) ? ", " : "\r");
        }

        /*
         * We can now stop for 100ms - the following call is platform dependant.
         */
        {
            Sleep(100);
        }
    }

    if (abort_req) {
        return;
    }
    /* Error handler. */
_error:

    /* Ask the main thread to stop. */
    abort_req = 1;

    /* Print the first error that occured. */
    printf("error in display (%d it)\n", counter);
}

/*
 * Keyboard thread function.
 */
static void keyboard_thread(void *param)
{
    /*
     * Casting of parameter to a group of pointer.
     */
    DSA_DRIVE_GROUP *grp = (DSA_DRIVE_GROUP *)param;

    /*
     * Infinite loop which waits for the keyboards inputs.
     */
    for (; !abort_req;) {

        /*
         * Get a character in a system dependent way.
         */
        char c = tolower(_getch());

        /*
         * Handle the key.
         */
        switch (c) {
        case '\33':
            /*
             * Pressing the ESC key will stop the process cleanly at the end
             * of the cycle. This is done by signaling the event to the main
             * process thread.
             */
            abort_req = 1;
            break;
        case ' ':
            /*
             * Pressing the SPACE key will generate an "emergency stop" and
             * stop the system immediatly, without waiting for the end of the
             * cycle. We don't check the error code because if the function
             * fails, there is no other action that could be taken.
             */
            abort_req = 1;
            dsa_quick_stop_s(grp, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);
            break;
        }
    }
}
