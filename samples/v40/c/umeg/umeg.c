/*
* umeg.c
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
 * This is the example that is referred to in the EDI package User's Manual.
 *
 * This example aims at using the library to control  two axes concurrently via an ULTIMET.
 * The ULTIMET is a ULTIMET light PCI and the controllers are one AccurET 400.
 * If an UltimET light TCP/IP or UltimET advanced is used, only the URL used when the communication
 * is opened must be changed. The remainder of the example will run.
 *
 * A first set of actions aims at getting the drives in an operational state which entails:
 * - establishing the communication with the ULTIMET and the drives connected to it
 * - powering up the drives
 * - performing the homing procedure on each drive.
 *
 * The next set deals with making movements on each axis by first describing how to set
 * the limits and precision windows , and then actually specifiying and starting the
 * movement. The movements will be controlled by the ULTIMET which will
 * execute them  in interpolated mode.
 * In a more elaborate step, one thread will be created to monitor the current position
 * of the motors. The monitoring thread will show the current position of each motor
 * every 100ms. Another will loop indefinitely waiting for a user input: the space bar
 * will immediately stop the movement and set one of the controller�s digital outputs.
 *
 * Finally, it will be shown how to power off the drives.
 *
 * The last portion of the program covers error handling.
 *
 * To run this example without modification, you must have:
 *  - two linear motors and two ETEL position loop controllers set and tuned accordingly.
 *  - a ULTIMET light PCI on a PCI slot of your PC
 *  - a TransnET connection between the ULTIMET and the drives.
 *  - the drives must have the axis numbers 0 and 1.
 *  - the software position limits (KL34 and KL35) must have been set when setting the controllers otherwise
 *    these will be defaulted to zero and nothing will move.
 */

/*** Initialisation steps ***/

/*** standard libraries ***/

#include <Windows.h>
#include <conio.h>  /* standard library for getch()*/
#include <ctype.h>  /* standard character conversion library */
#include <math.h>   /* standard math interface; used for random numbers here*/
#include <process.h>/* standard multithreading library       */
#include <stdio.h>  /* the standard I/O library              */
#include <stdlib.h> /* standard general purpose library      */

/*** ETEL libraries ***/

#include <dmd40.h> /* EDI constants definitions             */
#include <dsa40.h> /* EDI high level library version 3.0x.  */

/*** prototypes ***/

/*
 * The following function define the threads which will run independently.
 * The exact parameter type of theses functions as well as the return value
 * are platform specific.
 * this will be the thread that displays the drive status and the current
 * position of the motor.
 */
static void display_thread(void *param);

/*
 * Synchronisation between main thread and displaying thread:
 * To tell the displaying thread whether to fetch and display the data or not.
 */
int doDisplay = 1; /* 0: don't; 1: do*/

/*
 * The following function is the function which is called back when an
 * asynchronous function call terminates.
 */
static void callback(DSA_DEVICE_GROUP *grp, int err, void *param);
/*
 * Returned error by callback of asynchronous call. 0 if no error
 */
int asyncFunctionError;

/*
 * Synchronisation between main thread ans asynchronous call function
 * True when callback has been called
 */
int callbackCalled;

/*
 *** Main entry point function.***
 */
int main(int argc, char *argv[])
{
    /*** Initialisation steps ***/

    /*
     * We are going to be making some loops, so we need a variable to count them.
     */
    int loops = 0;
    /*
     * Most functions of the library return an error code or 0 if no error
     * occured. We need a variable to store the last error.
     * The error codes are negative values, ranging from -399 to -300 for the
     * dsa40 library.
     */
    int err; /* initialised to a value that is NOT*/
             /* returned by the libray functions	*/
    /*
     * We need two variables to hold the minimum and maximum position of the
     * motor. We are going to make movements within these bounds.
     * All parameters which represent physical dimensions are usually
     * defined in ISO units and use double precision floating point variables.
     */
    double pos_min[2], pos_max[2];
    /*
     * Moving distances will also be expressed in relation to the total ramge of
     * motion defined by the above limits.
     */
    double range_of_motion[2];

    /*** Creating the objects ***/

    /*
     * We must also define a pointer to a DSA_DRIVE object for each drive. This "hidden"
     * object is defined in the dsa40 library. The client does not need to access
     * the members of this object directly, but need to pass it to various dsa
     * library functions. It can be compared to the FILE structure defined in
     * the standard I/O library.
     * The pointers *must* be initialized to NULL before calling the created
     * drive function, otherwise this function will fail.
     */
    DSA_DRIVE *axisX = NULL;
    DSA_DRIVE *axisY = NULL;

/*
 * For programming purposes we define here the constants for the axis numbers
 */
#define AXIS_NB_X 0
#define AXIS_NB_Y 1

    /*
     * The same goes for the object representing the ULTIMET
     */
    DSA_MASTER *ultimet = NULL;

    /* When the ULTIMET manages interpolated movements, the commands must be sent to what
     * is called an interpolation group. This interpolation group can be used like a
     * normal device or drive group. All drives within this group can also be interpolated.
     * So, we must also define a pointer to a DSA_IPOL_GROUP (like a DSA_DRIVE_GROUP)
     * object. This hidden object is also defined in the dsa40 library, and can be
     * considered as a sort of array containing other DSA_DRIVE objects. The client can
     * set, change and retrieve the different drives belonging to this object through
     * some accessor functions. But you might also simply want to create a group
     * (a DSA_DRIVE_GROUP) just to send one single same command to several drives or
     * ULTIMET. See � Groups ofthe User's Manual for more details on groups.
     * As with DSA_DRIVE, the grp pointer  must be initialized to NULL before calling
     * the created group function, otherwise this function will fail.
     */
    DSA_IPOL_GROUP *igrp = NULL;

    /* Once the pointers have been created, the actual objects themselves have to be
     * created and initialised:
     */

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

    /* Now, we also have to create the interpolation group object. The size for the
     * group must be given at the creation time. In our case, we want to put two
     * drives in this group.
     */
    if (err = dsa_create_ipol_group(&igrp, 2)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    /* After creating the group, it has to be filled with the required drives.
       The interpolation group requests the respect of the position of the axis in the group.
       X axis must be at position 0, Y axis at position 1, Z axis at position 3, Theta axis at position 4
     */
    if (err = dsa_set_group_item(igrp, 0, axisX)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }
    if (err = dsa_set_group_item(igrp, 2, axisY)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }

    /* Now associate the ULTIMET to the interpolation group. When you ask for an
     * interpolated movement on a group of axes, the dsa40 library will use the ULTIMET
     * associated with the group.
     */
    if (err = dsa_set_master(igrp, ultimet)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*** Opening the communication ***/

    /* Now we are ready to open the communication channels.
     * In our exemple, we have to write the following lines to establish the
     * communication. See � on Establishing the communication in the Users's Manual for
     * more details on the syntax involved with opening the communication.
     */

    if (err = dsa_open_u(axisX, "etb:ULTIMET:0")) { /* Motor axis X is on drive number 0   */
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }
    if (err = dsa_open_u(axisY, "etb:ULTIMET:1")) { /* Motor axis Y is on drive number 1   */
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }
    if (err = dsa_open_u(ultimet, "etb:ULTIMET:*")) { /* For a ULTIMET, the node number is "*" */
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /*** Powering up the motors ***/
    /* Different devices take different times to completely startup which means that in
     * a system that includes different types of devices, such as some position
     * controllers and a position motion controller, some devices will be ready when
     * others are not. This can have the effect of putting in error a device that expects
     * another to be there but that hasn�t finished starting up. This is why, when all
     * devices have finished powering up, it is recommended to start the application by
     * a general �reset error� command
     */
    if (err = dsa_reset_error_ex_s(igrp, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_reset_error_ex_s(ultimet, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /* Now we can send commands to the drive. The first thing to do is to put the drive
     * in power on state. This is done by the dsa_power_on_s command. The "_s" at the end
     * of the command indicates that this is a �synchronous� function. Synchronous function
     * wait until the end of the operation before returning. All synchronous functions have
     * a timeout parameter as the last parameter. This parameter orders the  function to
     * return with a timeout error (DSA_ETIMEOUT) if no response  comes from the drive
     * before the end of the specified timeout. This lack of response usually indicates an
     * error in the application, or could  result from bad drive parameters. An appropriate
     * timeout value heavily  depends on the application and the command issued. In the
     * �power on� case, less than 1 second  could be appropriate with pulse initialization,
     * but more than 5 seconds could be required with constant current initialization.
     * Here we can do this in two ways: either switch the power on in each motor
     * individually:
     */
    if (err = dsa_power_on_s(axisX, 10000)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }
    if (err = dsa_power_on_s(axisY, 10000)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }
    /* or, since we have created a group - primarily for interpolation purposes, but it
     * can also be used any where a group is expected - send the command to the group in
     * general, the DLL taking upon itself to dispatch it to all the members of the group:
     */
    /*
    if (err = dsa_power_on_s(igrp, 10000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }*/

    /*** Homing ***/

    /* Next step in readying the controllers and motors for operation is performing the
     * homing procedure in order to find the reference for the motor�s absolute position.
     * Once  again, this can be done on each axis individually:
     */
    /*
    if (err = dsa_homing_start_s(axisX, 10000)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }
    if (err = dsa_homing_start_s(axisY, 10000)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }
    */
    /* or on the group as a whole:
     */
    if (err = dsa_homing_start_s(igrp, 10000)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }

    /*
     * During the indexation procedure, we can read the minimum and maximum
     * position allowed for the motor. This is done by calling the function
     * "get min/max soft position limit". This function writes the data at the
     * address provided in the second argument. The third argument indicates
     * to the function what kind of value must be returned: the current value,
     * the default value, the maximum or minimum value,... Some of these
     * operations (getting min, max or default values) do not involve any
     * communication.
     * As usual, a timeout must be provided, but for request operation,
     * which are always executed in one cycle in the drive, it is recommended
     * to ask for the default timeout. This special timeout value is
     * automatically adjusted to the communication channel used.
     */
    if (err = dsa_get_min_soft_position_limit_s(axisX, &pos_min[AXIS_NB_X], DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }
    if (err = dsa_get_max_soft_position_limit_s(axisX, &pos_max[AXIS_NB_X], DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }
    range_of_motion[AXIS_NB_X] = pos_max[AXIS_NB_X] - pos_min[AXIS_NB_X];

    if (err = dsa_get_min_soft_position_limit_s(axisY, &pos_min[AXIS_NB_Y], DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }
    if (err = dsa_get_max_soft_position_limit_s(axisY, &pos_max[AXIS_NB_Y], DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }
    range_of_motion[AXIS_NB_Y] = pos_max[AXIS_NB_Y] - pos_min[AXIS_NB_Y];

    /* Now, before we can make other movements, we must wait until it is terminated. To this
     * end, we wait until the movement is finished, on both axis:
     */
    if (err = dsa_wait_movement_s(igrp, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*** Monitoring data ***/
    /*
     * We will start the thread once the controllers are homed.
     * The mechanism for starting the threads is platform
     * dependant.
     * Notice here a slightly different way to handle the error code.
     * This is because _beginthread is a C library call that does not
     * return known EDI error codes, so if there is a system error creating
     * the thread, it is translated into an EDI system error and processed for display.
     */
    if (_beginthread(display_thread, 0, igrp) <= 0) {
        err = DSA_ESYSTEM;
        printf("ERROR at file %s, line %d %s\n", __FILE__, __LINE__, dsa_translate_edi_error(err));
        goto _error;
    }

    /*** Movements ***/

    /*** -- Defining and making simple movements ***/

    /* A position controlled movement can be defined by up to four parameters. The speed,
     * acceleration and jerk time describing the movement profile can be specified.
     * The movement is started when setting the target position
     */
    {
        double profileSpeed = 0.5;  /* m/s (or turns/secs) for rotary motors */
        double profileAcc   = 10.;  /* m/s^2 or rad/s^2 */
        double jerkTime     = 0.01; /* seconds */

        if (err = dsa_set_profile_velocity_s(axisX, 0, profileSpeed, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }

        if (err = dsa_set_profile_acceleration_s(axisX, 0, profileAcc, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }

        if (err = dsa_set_jerk_time_s(axisX, 0, jerkTime, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }

        /* If the values aren't specified, the values set when tuning the controller are used,
         * (or the default ones if they were not changed at that time).
         */
    }

    /* Making motors simply go to a given position can be as straight forward as calling
     * dsa_set_target_position_s() which is the equivalent of the controller�s POS
     * command : it sets the value of the target position and starts the movement.
     */

    {
        double targetPositionX = pos_min[AXIS_NB_X] + (rand() * (range_of_motion[AXIS_NB_X])) / RAND_MAX; /* meters */
        double targetPositionY = pos_min[AXIS_NB_Y] + (rand() * (range_of_motion[AXIS_NB_Y])) / RAND_MAX; /* meters */

        if (err = dsa_set_target_position_s(axisX, 0, targetPositionX, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }
        if (err = dsa_set_target_position_s(axisY, 0, targetPositionY, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, axisY);
            goto _error;
        }
        /*
         * The above calls will just *start* the movement, which
         * means that the operation will just need one or two milliseconds (on the
         * ETEL-Bus-Lite, much less on the TEB) to be executed by the drive, and then control will be
         * handed back immediately to the application. In that case, the timeout value that
         * need to be supplied must only take the communication time from the pc
         * to the drive into account. In this situation, a special value can be
         * used, DSA_DEF_TIMEOUT, which ask the function to use a default timeout
         * value adjusted to the communication channel used.
         * After the command however, we will again wait for the end of the motion. The
         * timeout value has been fixed to 1 minute for each of the following
         * movement waiting functions; this obviously depends on the requested move.
         */
        if (err = dsa_wait_movement_s(igrp, 60000)) {
            DSA_EXT_DIAG(err, igrp);
            goto _error;
        }
    }
    /* In the above example, the two motors start as soon as they receive the command
     * (index �0� indicated by the second parameter) and not synchronously.
     * If we wanted to make the drive start simultaneously, we would send the target position
     * at index 1 in the controllers, for instance , and then indicated for the whole group
     * we have a new setpoint to be executed with dsa_new_setpoint_s():
     */
    {
        double targetPositionX = pos_min[AXIS_NB_X] + (rand() * (range_of_motion[AXIS_NB_X])) / RAND_MAX; /* meters */
        double targetPositionY = pos_min[AXIS_NB_Y] + (rand() * (range_of_motion[AXIS_NB_Y])) / RAND_MAX; /* meters */

        if (err = dsa_set_target_position_s(axisX, 1, targetPositionX, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }
        if (err = dsa_set_target_position_s(axisY, 1, targetPositionY, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, axisY);
            goto _error;
        }
        if (err = dsa_new_setpoint_s(igrp, 1, DSA_STA_POS, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }
        /* The third parameter tells the controller that it is the position that has changed.
         * One can also changed the speed and accelerations and jerk time  and then ask the
         * controller to take them into account with the dsa_new_setpoint_s() by using the
         * constants DSA_STA_SPD, DSA_STA_ACC, DSA_STA_JRK combined in an OR expression.
         */
        if (err = dsa_wait_movement_s(igrp, 60000)) {
            DSA_EXT_DIAG(err, igrp);
            goto _error;
        }
    }

    /*** Making interpolated movements ***/

    /*** -- Defining the movements ***/
    /*
     * Now we enter in interpolated mode. In this mode the position of the
     * motors is controlled by the multi-axis board. This applies only for
     * drives that are specified in the interpolation group (igrp). When you
     * are in this mode, you cannot use the dsa_set_target_position_x()
     * functions on the drives specified in the igrp. You must use the
     * dsa_ipol_xxx() functions instead to define the interpolated path. The
     * interpolator (the ULTIMET) will drive the motors along this path.
     * When you enter into interpolated mode, the actual position becomes the
     * reference postition for the interpolation. For the interpolation
     * functions (dsa_ipol_xxx()), all coordinates are given in absolute from
     * this reference point. Practically, after the call to dsa_ipol_begin_s()
     * you are at the point with coordinates:
     *     x = 0.0, y = 0.0
     * When you leave the interpolated mode, you come back to the previous
     * reference system.
     *
     * At present, the X-Y system is in a random absolute (encoder related)
     * position (in meters). For the sake of simplicity, we will first move to the center
     * of the motion ranges of both motors, and start interpolation mode from
     * there: it will be our (0,0) reference for interpolated movements.
     */

    /*
     * Move to center of both motor motion ranges
     */
    if (err = dsa_set_target_position_s(axisX, 1, (pos_min[AXIS_NB_X] + pos_max[AXIS_NB_X]) / 2.0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }
    if (err = dsa_set_target_position_s(axisY, 1, (pos_min[AXIS_NB_Y] + pos_max[AXIS_NB_Y]) / 2.0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }
    if (err = dsa_new_setpoint_s(igrp, 1, DSA_STA_POS, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }
    if (err = dsa_wait_movement_s(igrp, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    /*
     * Now start interpolation mode
     */
    if (err = dsa_ipol_begin_s(igrp, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /* In this mode, it is the maximum tangential speed (in m/s) and accelerations
     *(in m/s^2) that must be defined:
     * Here, the tangential speed is set to 0.05 m/s;
     * and the tangential acceleration and deceleration are set to 0.1 m/(s*s).
     */
    if (err = dsa_ipol_tan_velocity_s(igrp, 0.5, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_ipol_tan_acceleration_s(igrp, 1.0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_ipol_tan_deceleration_s(igrp, 1.0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*** Making interpolated movements ***/

    /* At this point we can, for example, start the trajectory with a linear segment.
     * The following function defines and starts the movement from the previous end of
     * segment and stops the system at the end of the specified segment.
     */
    {
        double endPointX = range_of_motion[AXIS_NB_X] / 4.0;
        double endPointY = range_of_motion[AXIS_NB_Y] / 4.0;

        if (err = dsa_ipol_line_2d_s(igrp, endPointX, endPointY, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, igrp);
            goto _error;
        }
        /* After this we can keep on specifying other lines by another call to the same
         * function:
         */
        endPointX = range_of_motion[AXIS_NB_X] / 4.0;
        endPointY = -range_of_motion[AXIS_NB_Y] / 4.0;
        if (err = dsa_ipol_line_2d_s(igrp, endPointX, endPointY, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, igrp);
            goto _error;
        }
        /* or draw a portion of a circle with a call like:*/
        if (err = dsa_ipol_circle_cw_c2d_s(igrp,
                                           0.0,                               /* arc end X corrdinate    */
                                           0.0,                               /* arc end Y corrdinate    */
                                           range_of_motion[AXIS_NB_X] / 8.0,  /* arc center X coordinate */
                                           -range_of_motion[AXIS_NB_Y] / 8.0, /* arc center Y coordinate */
                                           DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, igrp);
            goto _error;
        }
        /* which will draw an arc ending at the position given by the 2nd and 3rd parameters,
         * (back to center of reference frame, in our case)
         * centered on the point with the coordinates given by the 4th and 5th parameters
         * (centerX=range_of_motion[AXIS_NB_X]/8.0; centerY=range_of_motion[AXIS_NB_Y]/8.0)
         *(with respect to the point at which the system was when entering the interpolation
         * mode, remember).
         */
    }

    /* Up until now the system will have stopped at the end of each segment. To make a
     * continuous movement, we have to indicate that we want to start concatenation of
     * the segments: BUT YOU ONLY SPECIFY THIS AFTER THE FIRST SEGMENT HAS BEEN STARTED
     */

    /* Now, in this mode all segments are processed at constant speed, without stopping
     * between segments. This means that when the interpolator processes the one segment,
     * it does not decrease the speed at the end of the segment. It jumps to the next
     * segment without speed change. Now, the following calls will generate a movement along
     * a trajectory at a constant speed:
     */
    {
        double endPointX = -range_of_motion[AXIS_NB_X] / 8.0;
        double endPointY = range_of_motion[AXIS_NB_Y] / 8.0;

        if (err = dsa_ipol_line_2d_s(igrp, endPointX, endPointY, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, igrp);
            goto _error;
        }
        /*
         * Now specifiy that next segment has to be concatenated.
         */
        if (err = dsa_ipol_begin_concatenation_s(igrp, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, igrp);
            goto _error;
        }
        /*
         * A now the circle and last segment back to the center:
         */
        if (err = dsa_ipol_circle_ccw_c2d_s(igrp,
                                            endPointX,                         /* arc end X corrdinate    */
                                            -endPointY,                        /* arc end Y corrdinate    */
                                            -range_of_motion[AXIS_NB_X] / 4.0, /* arc center X coordinate */
                                            0.0,                               /* arc center Y coordinate */
                                            DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, igrp);
            goto _error;
        }
        if (err = dsa_ipol_line_2d_s(igrp, 0.0, 0.0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, igrp);
            goto _error;
        }
    }

    /* Once the concatenated segments section is terminated, you can revert to giving
     * individual segments again, in between which the ULTIMET will drive the system to
     * a stop, and a restart:
     */
    if (err = dsa_ipol_end_concatenation_s(igrp, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /** add more interpolated individual segments here **/

    /* When  the system no longer needs to perform interpolated movements, the
     * interpolation mode is exited by calling first :
     */
    if (err = dsa_ipol_wait_movement_s(igrp, 60000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    /* to wait for all segments to be finished, and then:*/
    if (err = dsa_ipol_end_s(igrp, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    /*to leave the interpolation mode and come back to the previous reference system.*/

    /*
     * End of movements: power motors off:
     */
    if (err = dsa_power_off_s(igrp, 10000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /*** Reading and writing of registers (generic functions) ***/

    /*
     * Stop display thread while we are going to save and reset, otherwise we will
     * get errors.
     */
    doDisplay = 0;
    /*
     * Here we read a user register (type X) and increment its value, to be saved
     * and read again for the examples purpose in the next two portions of code.
     */
    {
        long val = 0;

        if (err = dsa_get_register_int32_s(axisX,           /* grp: destination device */
                                           DMD_TYP_USER,    /* typ: DMD constant 1:X user register */
                                           2,               /* idx: register index */
                                           0,               /* sidx: register subindex */
                                           &val,            /* value: register value */
                                           DSA_GET_CURRENT, /* kind: actual device value */
                                           DSA_DEF_TIMEOUT  /* timeout: default timeout */
                                           )) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }
        printf("\nUser register read from controller X2 = %d\n", val);
        val++;

        if (err = dsa_set_register_int32_s(axisX,          /* grp: destination device */
                                           DMD_TYP_USER,   /* typ: DMD constant 1:X user register */
                                           2,              /* idx: register index */
                                           0,              /* sidx: register subindex */
                                           val,            /* value: register value */
                                           DSA_DEF_TIMEOUT /* timeout: default timeout */
                                           )) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }
        printf("\nUser register before sav/reset X2 = %d\n", val);
    }

    /*** Sending commands : Saving and resetting ***/

    /* The values of parameters that have been only modified are not stored in permanent
     * storage on the devices which means that when they are reset the parameters revert
     * back to the stored values. To effectively store modified values, you have to
     * explicitely save them by calling  dsa_save_parameters_s() which is the equivalent
     * of the SAV command a the controller.
     * Sending a SAV  command to a device will cause it to �disappear� on the ETEL device
     * network and to re-appear in error. So it is possible that the device itself does not
     * have time to acknowledge the SAV command. So it is advised to not check the error
     * code returned by this function...
     */
    dsa_save_parameters_s(axisX, DSA_PARAM_SAVE_X_PARAMS, 10000);

    /* The SAV command takes quite a while to execute hence the large timeout value for the
     * last parameter.
     */

    /* Sending a SAV  command to a device will cause it to �disappear� on the ETEL device
     * network and to re-appear in error. So before you can issue any other commands, you
     * must wait (quite some time, as the last parameter of dsa_wait_status_equal_s()
     * shows) until the drives are present again:
     */
    {
        /* setup status masks */
        DSA_STATUS status_checkbits      = {sizeof(DSA_STATUS)};
        DSA_STATUS status_checkstates    = {sizeof(DSA_STATUS)};
        status_checkbits.drive.present   = 1;
        status_checkstates.drive.present = 1;

        /* wait that the controllers are present */
        if (err = dsa_wait_status_equal_s(axisX, &status_checkbits, &status_checkstates, NULL, 30000)) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
            ;
        }
    }
    /* For the new  values to be effectively taken into account the drive has to be reset.
     * There is no specific function for the RSD (reset) command so you have to send it
     * �manually�:
     */
    dsa_execute_command_d_s(igrp,                /* grp: destination device */
                            DMD_CMD_RESET_DRIVE, /* cmd: constant number of the RSD command (88) */
                            DMD_TYP_IMMEDIATE,   /* typ: the parameter to the command is a value*/
                            255,                 /* par : the value always passed to the RSD command*/
                            0,                   /* fast: 1 : true = fast command */
                            0,                   /* ereport: 0 false = don't report drive errors */
                            DSA_DEF_TIMEOUT      /* timeout: 10 secs */
    );
    /* The RSD command also takes quite a while to execute.
     * Also, like SAV command, because the controller resets, it does not have the
     * time to send the acknowledgement that it has received and executed this command.
     * This is why the call to dsa_execute_command_d_s() for the RSD command nearly
     * always returns an error indicating an absence of command acknowledge. This is the
     * one case where it is advised NOT to test the returned error code and why the above
     * call is a bit different from the previous ones. Also this is why you don't have to
     * put a big timeout value. However, you do have to wait a while for the system to
     * come up again:
     */
    Sleep(60000);

    /* Note: here we have reset all the drives in the network. Only the drive on which the
     * SAV was done really needs to be reset. Here it is done because the group includes
     * the ULTIMET and that means that the communication will be lost and the following lines
     * example shows how to reestablish it
     * The proper way to do this, is to first close the communication with all devices,
     * and then re-open it:

    /* --Close communication.
     * You don�t need to destroy the objects; these will be used again when
     * reopenning the communication
     */
    dsa_close(axisX);
    dsa_close(axisY);
    dsa_close(ultimet);

    /* --Reopen communication*/
    if (err = dsa_open_u(axisX, "etb:ULTIMET:0")) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }
    if (err = dsa_open_u(axisY, "etb:ULTIMET:1")) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }
    if (err = dsa_open_u(ultimet, "etb:ULTIMET:*")) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /*** Reading and writing of registers ***/

    /* We will do this here just to show that we have
     *   a) regained access to the drive
     *   b) actually changed and saved the register
     */
    {
        long val;

        if (err = dsa_get_register_int32_s(axisX,           /* grp: destination device */
                                           DMD_TYP_USER,    /* typ: DMD constant 1:X user register */
                                           2,               /* idx: register index */
                                           0,               /* sidx: register subindex */
                                           &val,            /* value: register value */
                                           DSA_GET_CURRENT, /* kind: actual device value */
                                           DSA_DEF_TIMEOUT  /* timeout: default timeout */
                                           )) {
            DSA_EXT_DIAG(err, axisX);
            goto _error;
        }

        printf("User register after sav/reset X2 = %d\n", val);
    }

    /*** Asynchronous function ***/

    /* Reset errors before power on */
    if (err = dsa_reset_error_ex_s(igrp, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    if (err = dsa_reset_error_ex_s(ultimet, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /* Do an asynchronous power on */
    /* Handler will be called once power on function is finished */
    /* Meanwhile, the application can do something, in this case displaying "*" */
    /* to indicate power on process in-progress status */

    /* Initialisation of main thread - callback function synchronous variable */
    callbackCalled = 0;

    /* asynchronous power on */
    /* This function will return BEFORE the controllers terminate the PWR=1 function */
    /* call function will be called once the controllers has terminated PWR=1 function */
    printf("\nPower on in progress ");
    fflush(stdout);
    if (err = dsa_power_on_a(igrp, callback, (void *)1)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /* Wait that callback function is called */
    while (!callbackCalled) {
        printf("*");
        fflush(stdout);
        Sleep(100);
    }

    if (asyncFunctionError) {
        DSA_EXT_DIAG(asyncFunctionError, igrp);
        goto _error;
    }

    /* Do a homing followed by an asynchronous wait on end of movement */
    /* Because even synchronous homing comes back immediately, it is no */
    /* sense to do an asynchronous homing. On the other hand, it is advised */
    /* to do an asynchronous dsa_wait_movement and meanwhile displaying the */
    /* homing process in-progress status */
    /* to do an asnyhronous wait
    /* Handler will be called once power on function is finished */
    /* Meanwhile, the application can do something, in this case displaying "*" */

    /* asynchronous power on */
    /* This function will return BEFORE the controllers terminate the PWR=1 function */
    /* call function will be called once the controllers has terminated PWR=1 function */
    printf("\nHoming in progress ");
    fflush(stdout);
    if (err = dsa_homing_start_s(igrp, 10000)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /* Initialisation of main thread - callback function synchronous variable */
    callbackCalled = 0;
    if (err = dsa_wait_movement_a(igrp, callback, (void *)2)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }

    /* Wait that callback function is called */
    while (!callbackCalled) {
        printf("*");
        fflush(stdout);
        Sleep(100);
    }

    if (asyncFunctionError) {
        DSA_EXT_DIAG(asyncFunctionError, igrp);
        goto _error;
    }

    /*** Application termination ***/

    /*
     * When we have arrived at the end of our application, we have to go through a couple
     * of termination steps for a proper closeout of our program.
     */

    /* Application termination operations take place in the reverse order of those
     * described in at startup which are creating the objects and opening the communication.
     * These two functions assign resources to the operating system. During the creation of
     * an object or a group, memory is assigned. During the opening of the communication,
     * the use of the serial port, the interruption line or other resources specific to the
     * communication bus used are assigned. These resources must be given back to the system
     * as soon as there are not used any more. It is normally done just before leaving the
     * application.
     * The DSA library has the dsa_close() function to close the communication and the
     * dsa_destroy() function to destroy an object or a group. The closing must be done
     * before the destruction.
     * So, first, the connections must be closed:
     * -connection to the ULTIMET:
     */
    if (err = dsa_close(ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }
    /* - connections to both drives:*/
    if (err = dsa_close(axisX)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }
    if (err = dsa_close(axisY)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }
    /* Then, just like the objects were created at the beginning of the application,
     *the memory alloted to them must be released to be made available to the system again:
     * -group object:
     */
    if (err = dsa_destroy(&igrp)) {
        DSA_EXT_DIAG(err, igrp);
        goto _error;
    }
    /* - ultimet object: */
    if (err = dsa_destroy(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }
    /* - and the drive objects */
    if (err = dsa_destroy(&axisX)) {
        DSA_EXT_DIAG(err, axisX);
        goto _error;
    }
    if (err = dsa_destroy(&axisY)) {
        DSA_EXT_DIAG(err, axisY);
        goto _error;
    }

    printf("Application correctly ended. Press a key to continue\n");
    _getch();
    /* All is well that ends well.*/
    return 0;

    /**************************
     * Now the error handler...
     **************************/
_error:
    /*
     * Stop access to the drive while stopping, otherwise we will
     * get errors.
     */
    doDisplay = 0;

    /* Destroy the group if not already done. */
    if (dsa_is_valid_drive_group(igrp)) {
        /* Leave the interpolation mode. */
        if (dsa_is_ipol_in_progress(igrp))
            dsa_ipol_end_s(igrp, DSA_DEF_TIMEOUT);
        dsa_destroy(&igrp);
    }

    /* The same has to be done with the ultimet and the drives. For each drive, if the object
     * pointer is valid, we check if we can still communicate with it to stop all movements
     * and then to power it off and finally close the communication. Afterwards, the object
     * itself can be destroyed
     */

    /* axis X */
    if (dsa_is_valid_drive(axisX)) {

        /* Is the communication open ? */
        ebool open = 0;
        dsa_is_open(axisX, &open);
        if (open) {

            /* Stop movements. */
            dsa_quick_stop_s(axisX, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);

            /* When the motor has stopped, a power off is done. */
            dsa_wait_movement_s(axisX, 60000);
            dsa_power_off_s(axisX, 60000);

            /* Close the connection. */
            dsa_close(axisX);
        }

        /* Finally, release the associated resources to the OS. */
        dsa_destroy(&axisX);
    }
    /* axis Y */
    if (dsa_is_valid_drive(axisY)) {

        /* Is the communication open ? */
        ebool open = 0;
        dsa_is_open(axisY, &open);
        if (open) {

            /* Stop movements. */
            dsa_quick_stop_s(axisY, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);

            /* When the motor has stopped, a power off is done. */
            dsa_wait_movement_s(axisY, 60000);
            dsa_power_off_s(axisY, 60000);

            /* Close the connection. */
            dsa_close(axisY);
        }

        /* Finally, release the associated resources to the OS. */
        dsa_destroy(&axisY);
    }

    /* The ULTIMET also has to be shutdown:*/
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

    printf("Application ended with error. Press a key to continue\n");
    _getch();
    /* Exit indicating an error */
    return -1;
} /* end main() */

/*** Monitoring data ***/

/*
 * Display thread function.
 */
static void display_thread(void *param)
{
    /* the common variables must be defined */
    int counter = 0;
    int err     = 0;
    int i;

    /*
     * Casting of parameter to a group of pointer.
     */
    DSA_IPOL_GROUP *grp = (DSA_IPOL_GROUP *)param;

    /* Get individual drive objects. */
    DSA_DRIVE  *drv[2];
    DSA_MASTER *ultimet;
    for (i = 0; i < 2; i++) {
        if (err = dsa_get_group_item(grp, i, &drv[i])) {
            DSA_EXT_DIAG(err, NULL);
            goto _error;
        }
    }
    if (err = dsa_get_master(grp, &ultimet)) {
        DSA_EXT_DIAG(err, NULL);
        goto _error;
    }

    /*
     * Start an infinite loop. This function will be stopped by the OS only
     * when the whole process will end.
     */
    for (;;) {

        /*
         * Each 100 ms, we will display the status and the position of each
         * drive. As usually, the status structure must be initialized before
         * using it.
         */
        DSA_STATUS status[]       = {sizeof(DSA_STATUS), sizeof(DSA_STATUS)};
        DSA_STATUS ultimet_status = {sizeof(DSA_STATUS)};
        double     pos[2];

        pos[0] = pos[1] = 0.0;

        /* stop display thread */
        if (!doDisplay) {
            break;
        }

        /*
         * Get the drive status. Theses functions don't generate any activity
         * on the communication channel. The status of each drive is always
         * kept up-to-date in the process memory, which means that theses
         * function are very effective and could be often called without
         * degrading performances.
         */
        if (err = dsa_get_status(ultimet, &ultimet_status)) {
            DSA_EXT_DIAG(err, ultimet);
            goto _error;
        }
        for (i = 0; i < 2; i++) {
            if (err = dsa_get_status(drv[i], &status[i])) {
                DSA_EXT_DIAG(err, drv[i]);
                goto _error;
            }
            /* Get the position of each axis. */
            if (err = dsa_get_position_actual_value_s(drv[i], &pos[i], DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
                DSA_EXT_DIAG(err, drv[i]);
                goto _error;
            }
        }

        /*
         * We can now print the status string on the display.
         */
        printf("%04d: ", ++counter);
        printf("ULTIMET: %c%c%c, ",
               ultimet_status.master.moving ? 'M' : '-',
               ultimet_status.master.warning ? 'W' : '-',
               ultimet_status.master.error ? 'E' : '-');

        for (i = 0; i < 2; i++) {
            printf("AXIS %d: %c%c%c [%12.4fmm]",
                   i,
                   status[i].drive.moving ? 'M' : '-',
                   status[i].drive.warning ? 'W' : '-',
                   status[i].drive.error ? 'E' : '-',
                   pos[i] * 1000.0);
            printf((i == 0) ? ", " : "\r");
        }

        /*
         * We can now stop for 100ms - the following call is platform dependant.
         */
        Sleep(100);
    }

    printf("\ndisplay thread ended\n");
    return;

    /* Error handler. */
_error:

    /* Print the first error that occured. */
    printf("error in display (%d it)\n", counter);
}

/*
 * callback function.
 * This function will be called once asynchronous call is terminated
 */
void callback(DSA_DEVICE_GROUP *grp, int err, void *param)
{
    /* The asynchronous function has terminated with an error */
    if (err) {
        asyncFunctionError = err;
        printf("\n-> callback function called with error %d and parameter %lld\n", err, (long long)(param));
    }
    /* The asynchronous function has terminated without error */
    else {
        asyncFunctionError = 0;
        printf("\n-> callback function called with parameter %lld\n", (long long)(param));
    }
    callbackCalled = 1;
}
