/*
 * sample_1.cpp
 *
 * Copyright (c) 1997-2020 ETEL SA. All Rights Reserved.
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
//#include <stdio.h>

/*
 * Include EDI high level library version 2.0xx.
 * To enable the object oriented interface, you must define ETEL_OO_API before
 * include the header file. You can specify this define in the linker
 * or by putting a simple #define like in this sample.
 * If you dont'n define ETEL_OO_API, you can used the normal C API.
 */

#include <conio.h>

#define ETEL_OO_API
#include <dex40.h>
#include <dmd40.h>
#include <dsa40.h>
#include <esc40.h>
#include <etb40.h>
#include <lib40.h>
#include <tra40.h>

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
     * The first operation is to create the drive object. This operation is
     * made by define a DsaDrive object. This object is defined in the dsa40c
     * library header file. The client need to call it methods to perform
     * operations on the drive.
     */
    DsaDrive drv;

    /*
     * Most methods of the library objects throw an exception when a error
     * occured. You can catch these exceptions by a try statement.
     */
    try {

        /*
         * The next thing to do is to open the communication with the drive. This
         * is usually done by the open method, which takes the url of the drive as
         * the parameter. At this time, only etel-bus URLs are recognized.
         * It must be of the form: "etb:<driver>:<axis>".
         * The driver part of the url is the same string that is passed to the
         * etel-bus etb_open low level function. valid driver string includes:
         *  - "ETN://<host>:<port>"
         *      for local or remote access through ETEL network daemon (etnd).
         *		 - host is the DNS name of the computer, or it's IP address.
         *		   (localhost or 127.0.0.1 usually represent the local computer)
         *		 - port is the TCP port number.
         *  - "ULTIMET"
         *      for local access through ETEL ULTIMET PCI board.
         * This function is likely to fail (to throw a exception) if the drive is
         * not running, has a bad connection, if the port is already in use, ...
         */
        drv.open("etb:USB:0");

        /*
         * Reset the eventual error of the drive
         */
        drv.resetErrorEx(0, 2000);

        /*
         * Now we can send commands to the drive. The first thing to do is to
         * put the drive in power on state. this is done by the powerOn() method.
         * In this example we perform this operation synchronously. Synchronous
         * operations are waiting until the end of the operation before returning.
         * All syncronous functions have a timeout parameter as the last parameter.
         * This parameter order the function to throw a timeout exception if no
         * respose comes from the drive before the end of the specified timeout.
         * This lack of response usually indicates an error in the application,
         * or could result from bad drive parameters. an appropriate timeout value
         * heavily depends of the application. In the power on case, more than 8
         * seconds could be required with constant current initialisation.
         * The timeout parameter is given in milliseconds.
         */
        drv.powerOn(10000);

        /*
         * An inxation command must then be issued in order to find the absolute
         * position of the motor. The "homing start" command will just *start* the
         * homing procedure, which mean that the operation will just need one or
         * two milliseconds to execute on the drive, and then the respond will
         * come back quickly to the master. In that case, the timeout value that
         * need to be supplied must only take into account the communication time
         * from the pc to the drive. In this situation, you can omit the timeout
         * parameter. In this way you ask the function to use a default timeout
         * value adjusted to the communication channel used.
         */
        drv.homingStart(10000); // check out this procedure

        /*
         * During the indexation procedure, we can read the minimum and maximum
         * position allowed for the motor. This is done by calling the function
         * "get min/max soft position limit". The first argument indicates
         * to the function what kind of value must be returned: the current value,
         * the default value, the maximum or minimum value, or the conversion id.
         * some of theses operations (getting min, max or default values) does
         * not involve any communication. As usually, a timeout must be provided,
         * but for request operation, which are always executed in one cycle in
         * the drive, it is recommended to ask for the default timeout by
         * omitting the timeout parameter.
         */
        pos_min = drv.getMinSoftPositionLimit(Dsa::GET_CURRENT);
        pos_max = drv.getMaxSoftPositionLimit(Dsa::GET_CURRENT);

        /*
         * Now we will wait the end of the indexation procedure before continuing
         * this program. this is done with the "wait movement" command. the only
         * required parameter is the timeout, and his correct value is application
         * dependent. a conservative value of 10 minutes is given here, to
         * accomodate most situations.
         */
        drv.waitMovement(600000);

        /*
         * The next command will set the profile velocity
         */
        drv.setProfileVelocity(0, 1.0);

        /*
         * The next command will set the profile acceleration
         */
        drv.setProfileAcceleration(0, 1.0);

        /*
         * The next command will start a movement near (95%) the first limit. this
         * is done with the "set target position" function, which is the equivalent
         * of the standard POS function in the drive. The function takes a subindex
         * parameter as it's first argument. If a zero is written in the subindex,
         * the movement will start immediatly. A value of 1 to 3 in the subindex
         * just prepare the movement, which will then start with the "new setpoint"
         * command. After the command, we will again wait for the end of motion.
         * The timeout value has been fixed to 1 minute for each of the following
         * movements.
         */
        drv.setTargetPosition(0, pos_min * 0.95 + pos_max * 0.05);
        drv.waitMovement(60000);

        /*
         * Now we move to the other limit, followed by a move to the zero position, before
         * putting the motor in power off state.
         */

        /*
         * Move to 95% of the second limit.
         */
        drv.setTargetPosition(0, pos_min * 0.05 + pos_max * 0.95);
        drv.waitMovement(60000);

        /*
         * Move to 95% of the second limit.
         */
        drv.setTargetPosition(0, pos_min * 0.05 + pos_max * 0.95);
        drv.waitMovement(60000);

        /*
         * Power off the motor.
         */
        drv.powerOff(60000);

        /*
         * The first thing to do in order to cleanly terminates the program is to
         * close the communication to the drive. This operation is also required
         * if an error has occured after a successful "dsa open" function.
         */
        drv.close();

        /*
         * Everything seems succesfull, we can exit now.
         */
        fprintf(stderr, "success.\n");
        return 0;
    }
    catch (DsaException exc) {
        drv.diag(__FILE__, __LINE__, exc.getCode());
        /*
         * now comes the error handler. because we want to implements
         * only one common error handler, we will have to check the
         * state of the program to know how to terminate the program
         * the better way.
         */

        /*
         * first, we check if the connection has been established. A closed
         * connection would indicates that open() failed. To properly handle
         * the possibility of a failure in this function, we must put the
         * function in a try statement.
         */
        ebool open = false;
        try {
            open = drv.isOpen();
        }
        catch (DsaException) {
        }

        if (open) {

            /*
             * At this point, if the motor is moving, we need to halt it before
             * closing the connection.
             */

            /*
             * First we halt the motor. The "quick stop" function will bypass
             * any other command pending into the drive.
             */
            try {
                drv.quickStop(Dsa::QS_PROGRAMMED_DEC, Dsa::QS_BYPASS | Dsa::QS_STOP_SEQUENCE);
            }
            catch (DsaException) {
            }

            /* when the motor has stopped, */
            try {
                drv.waitMovement(600000);
            }
            catch (DsaException) {
            }
            /* a power off is done */
            try {
                drv.powerOff(10000);
            }
            catch (DsaException) {
            }

            /*
             * Finally, we can close the communication.
             */
            try {
                drv.close();
            }
            catch (DsaException) {
            }
        }

        /*
         * Now we will print a small message describing the error which occured.
         * The DsaException object contain the error code and a short text
         * description of the error. The main function will return with return
         * code 1 to indicates an error.
         */
        fprintf(stderr, "error %d: %s.\n", exc.getCode(), exc.getText());
        return 1;
    }
}
