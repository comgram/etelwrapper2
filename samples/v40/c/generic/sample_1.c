/*
 * sample_1.c
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
 * This simple demo program show how to make basic drive operations (power on,
 * indexation, movement) on a single drive, using ETEL EDI library set.
 * The program will send a power on and an indexation command, then move the
 * motor near the two limits of the available range, go to the zero position
 * again and power off the drive.
 */

/*
 * To run this demo without modification, you must have:
 *  - a linear motor and an ETEL position loop controller properly configured.
 *  - a USB connection between the drive and the PC.
 *  - the drive must have the axis number 0.
 *  - KL45 must be set to insure that position 0 is inside the valid range.
 *  - KL34 and KL35 must be set properly.
 * If KL34 and KL35 aren't set properly, you can modify this code to set pos_min
 * and pos_max manually.
 */

/*** libraries ***/

/*
 * Include the standard I/O library.
 */
#include <conio.h>
#include <stdio.h>

/*
 * Include EDI high level library version 3.0x.
 */
#include <dsa40.h>

/*** functions ***/

/*
 * Entry point function.
 */
int main(int argc, char *argv[])
{
    /*
     * We need two variables to hold the minimum and maximum position of the
     * motor. All parameters which represent physical dimensions are usually
     * defined in ISO units and use double precision floating point variables.
     */
    double pos_min, pos_max;

    /*
     * We must also define a pointer to a DSA_DRIVE object. This hidden object
     * is defined in the dsa30 library. The client does not need to access
     * the members of this object directly, but need to pass it to various dsa
     * library functions. It can be compared to the FILE structure defined in
     * the standard I/O library.
     * The drv pointer *must* be initialized to NULL before calling the created
     * drive function, otherwise this function will fail.
     */
    DSA_DRIVE *drv = NULL;

    /*
     * Most functions of the library return an error code or 0 if no error
     * occured. We need a variable to store the last error.
     * The error codes are negative values, ranging from -399 to -300 for the
     * dsa30 library.
     */
    int err;

    /*
     * The first operation is to create the drive object. This operation just
     * allocates some memory and OS resources and initializes it. No active
     * communication takes place at this step.
     * The address of the drive object is returned into the pointer passed by
     * reference to the function.
     * If an error occurs, we jump to the error label. It is usually not very
     * nice to use the goto keyword, but it's sometimes the best way for error
     * handling in C.
     */
    if (err = dsa_create_drive(&drv)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    /*
     * The next thing to do is to open the communication with the drive. This
     * is usually done by the dsa_open_u function, which takes the url of the
     * drive as the parameter. At this time, only ETEL-Bus's URLs are recognized.
     * It must take the following form: "etb:<driver>:<axis>"
     * The driver part of the url is the same string that is passed to the
     * ETEL-Bus etb_open low level function. Valid driver string includes:
     *  - "ULTIMET"
     *      for ULTIMET access.
     *  - "ETN://<host>:<port>"
     *      for local or remote access via the TCP/IP protocol to the ETEL
     *      network daemon (etnd), ULTIMET TCPIP, ...
     *       - host is the DNS name of the computer, or it's IP address.
     *         (localhost or 127.0.0.1 usually represent the local computer)
     *       - port is the TCP port number.
     *  - "ETN://<host>"
     *      for access via the TCP/IP protocol to an AccurET  device
     *       - host is the DNS name of the AccurET, or it's IP address.
     *  - "USB"
     *      for access via the USB to an AccurET  device
     *  - "DUMMY:<amsk>:<d_prod>:<d_ver>[:x_prod[:x_ver]]"
     *      for drive "simulation" with
     *       - amsk: bit field of all axes present on the bus (5 means 0 and 2)
     *       - d_prod: drive product number (2 = DSA2P, 4 = DSB2P, see M70)
     *       - d_ver: drive software version (see M72)
     *       - x_prod: extension card product number (see M76)
     *       - x_ver: extension card software version (see M78)
     * This function is likely to fail if the drive is not running, has a bad
     * connection, if the port is already in use, ...
     */
    if (err = dsa_open_u(drv, "etb:USB:0")) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    if (err = dsa_reset_error_ex_s(drv, 0, 10000)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    /*
     * Now we can send commands to the drive. The first thing to do is to
     * put the drive in power on state. This is done by the dsa_power_on_s
     * command. The "_s" at the end of the command indicates that this is a
     * *synchronous* function. Synchronous function are waiting until the
     * end of the operation before returning. All synchronous functions have
     * a timeout parameter as the last parameter. This parameter orders the
     * function to return with a timeout error (DSA_ETIMEOUT) if no response
     * comes from the drive before the end of the specified timeout. This lack
     * of response usually indicates an error in the application, or could
     * result from bad drive parameters. An appropriate timeout value heavily
     * depends on the application. In the power on case, less than 1 second
     * could be appropriate with pulse initialization, but more than 5 seconds
     * could be required with constant current initialization.
     */
    if (err = dsa_power_on_s(drv, 10000)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    /*
     * An indexation command must then be issued in order to find the absolute
     * position of the motor. The "homing start" command will just *start* the
     * homing procedure.
     */
    if (err = dsa_homing_start_s(drv, 10000)) {
        DSA_EXT_DIAG(err, drv);
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
     * As usually, a timeout must be provided, but for request operation,
     * which are always executed in one cycle in the drive, it is recommended
     * to ask for the default timeout. This special timeout value is
     * automatically adjusted to the communication channel used.
     */
    if (err = dsa_get_min_soft_position_limit_s(drv, &pos_min, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }
    if (err = dsa_get_max_soft_position_limit_s(drv, &pos_max, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    /*
     * Now we will wait for the end of the indexation procedure before continuing
     * this program. This is done with the "wait movement" command. The only
     * required parameter is the timeout, and its correct value is application
     * dependent. A conservative value of 1 minute is given here, to
     * accommodate most situations.
     */
    if (err = dsa_wait_movement_s(drv, 60000)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    if (err = dsa_set_profile_velocity_s(drv, 0, 2.0, 1000)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    if (err = dsa_set_profile_acceleration_s(drv, 0, 10.0, 1000)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    /*
     * The next command will start a movement near (95%) the first limit. This
     * is done with the "set target position" function, which is the equivalent
     * of the standard POS function in the drive. The function takes a subindex
     * parameter as it's second argument. If a zero is written in the subindex,
     * the movement will start immediately. A value of 1 to 3 in the subindex
     * just prepares the movement, which will then start with the "new set point"
     * command.
     * The "set target position" command will just *start* the movement, which
     * means that the operation will just need one or two milliseconds (on the
     * ETEL-Bus-Lite) to be executed by the drive, and then the response will
     * come back quickly to the master. In that case, the timeout value that
     * need to be supplied must only take the communication time from the pc
     * to the drive into account. In this situation, a special value can be
     * used, DSA_DEF_TIMEOUT, which ask the function to use a default timeout
     * value adjusted to the communication channel used.
     * After the command, we will again wait for the end of the motion. The
     * timeout value has been fixed to 1 minute for each of the following
     * movement waiting functions.
     */
    if (err = dsa_set_target_position_s(drv, 0, pos_min * 0.95 + pos_max * 0.05, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }
    if (err = dsa_wait_movement_s(drv, 60000)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    /*
     * Now we move to the other limit, followed by a move to the zero position,
     * before putting the motor in power off state.
     */

    /*
     * Move to 95% of the second limit.
     */
    if (err = dsa_set_target_position_s(drv, 0, pos_min * 0.05 + pos_max * 0.95, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }
    if (err = dsa_wait_movement_s(drv, 60000)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    /*
     * Move to 95% of the second limit.
     */
    if (err = dsa_set_target_position_s(drv, 0, pos_min * 0.05 + pos_max * 0.95, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }
    if (err = dsa_wait_movement_s(drv, 60000)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    /*
     * Power off the motor.
     */
    if (err = dsa_power_off_s(drv, 60000)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    /*
     * The first thing to do in order to cleanly terminate the program is to
     * close the communication to the drive.
     */
    if (err = dsa_close(drv)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    /*
     * The last operation is to release the memory and resources used by the
     * drive object. In this case, this operation is not required because we
     * are at the end of the program and the OS will free theses resources
     * itself as soon as the program terminates. After freeing the resources,
     * the function will reassign NULL to the drive pointer.
     */
    if (err = dsa_destroy(&drv)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    /*
     * Everything seems successful, we can exit now.
     */
    printf("success.\n");
    _getch();
    return 0;

    /*
     * Now comes the error handler. Because we want to implement
     * only one common error handler, we will have to check the
     * state of the program to know how to terminate the program
     * in the best way.
     */

_error:
    /*
     * The first test checks if the drv pointer itself is valid.
     * An invalid pointer would mean that the dsa_create_drive() function
     * has failed or that memory corruption occured.
     */
    if (dsa_is_valid_drive(drv)) {

        /*
         * Now we will check if the connection has been established. A closed
         * connection would indicate that dsa_open_u() failed.
         * We don't care about the returned value of this function because we
         * are only interested in the original error that occured. However,
         * the open flag has to be initialized to false to properly handle
         * the possibility of a failure in this function.
         */
        ebool open = 0;
        dsa_is_open(drv, &open);
        if (open) {

            /*
             * Now it's time to stop the motor. the dsa_quick_stop_s()
             * function will bypass any other command pending into the
             * drive. If the motor is already stopped, this operation doesn't
             * have any effect.
             */
            dsa_quick_stop_s(drv, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);

            /*
             * When the motor has stopped, a power off is done .
             */
            dsa_wait_movement_s(drv, 60000);
            dsa_power_off_s(drv, 60000);

            /*
             * At this point, we can close the communication.
             */
            dsa_close(drv);
        }

        /*
         * And finally, we release all resources to the OS.
         */
        dsa_destroy(&drv);
    }

    _getch();
    return 1;
}
