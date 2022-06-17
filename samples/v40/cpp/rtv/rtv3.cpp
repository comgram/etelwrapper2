/*
 * rtv3.cpp
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

/*** libraries ***/

#include <conio.h>
#include <process.h>
#include <windows.h>

/*
 * Include EDI high level library version 3.0xx.
 * To enable the object oriented interface, you must define ETEL_OO_API before
 * include the header file. You can specify this define in the linker
 * or by putting a simple #define like in this sample.
 * If you dont'n define ETEL_OO_API, you can used the normal C API.
 */

#define ETEL_OO_API
#include <dmd40.h>
#include <dsa40.h>

/*
 * Synchronisation between main thread and displaying thread:
 * To tell the displaying thread whether to fetch and display the data or not.
 */
int doDisplay = 1; /* 0: don't; 1: do*/

static void display_thread(void *param)
{
    DSA_DEVICE_BASE *c_grp  = (DSA_DEVICE_BASE *)param;
    DSA_DRIVE       *c_drv0 = NULL;
    DSA_DRIVE       *c_drv1 = NULL;

    // Create a C++ objects from C object.
    DsaDriveGroup grp(c_grp);
    dsa_get_group_item(c_grp, 0, &c_drv0);
    DsaDrive drv0(c_drv0);

    dsa_get_group_item(c_grp, 1, &c_drv1);
    DsaDrive drv1(c_drv1);
    try {
        int counter = 0;

        // Starts an infinite loop which will be stopped by global-variable doDisplay set to 0
        for (;;) {
            /* stop display thread */
            if (!doDisplay)
                break;

            // Each 100 ms, we will display the status and the position of the drive
            DsaStatus status0 = drv0.getStatus();
            DsaStatus status1 = drv1.getStatus();
            eint64    pos     = drv1.getRegisterInt64(DmdTyp::MONITOR_INT64, 450, 0, 0, 5000);

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
    }
    catch (DsaException exc) {
        grp.diag(__FILE__, __LINE__, exc.getCode());
        printf("Thread error %d: %s.\n", exc.getCode(), exc.getText());
    }

    printf("\ndisplay thread ended\n");
}

/*
 * Entry point function.
 */
int main(int argc, char *argv[])
{
    DsaMaster     ultimet; // Create UltimET object
    DsaDrive      drv0;    // Create AccurET object
    DsaDrive      drv1;    // Create AccurET object
    DsaDriveGroup grp(drv0, drv1);

    try {
        // ---------------------------------------------------
        // Opens communication on UltimeT and on both AccurETs
        // ---------------------------------------------------
        printf("Opening communication\n");

        // Open UltimET device
        ultimet.open("etb:ULTIMET:*");

        // Open AccurET devices
        drv0.open("etb:ULTIMET:0");
        drv1.open("etb:ULTIMET:1");

        // Reset errors on both AccurET
        printf("Reset Axis 0,1 errors...\n");
        grp.resetErrorEx(0, 5000);

        // ----------------------------------------------------
        // Configures Slave-to-slave communication through slot
        // ----------------------------------------------------
        printf("Get a free 64 bits slot from UltimET...\n");

        // Reserve a slot from UltimET.
        // DsaRTVSlot must be declared once ultimET has been opened.
        DsaRTVSlot slot(ultimet, DsaRTVSlot::SLOT_64BIT);

        // We make a new try-catch block to be able to unassign DsaRTVSlot on errors
        try {
            // Assign ML1 of axis 0 to reserved slot
            // Value of ML1 of axis 0 will be copied into slot
            slot.assignRegisterToSlot(drv0, DmdTyp::MONITOR_INT64, 1, 0, 5000);

            // Assign slot to ML450 of axis 1
            // Value of slot will be copied into ML450 of axis 1
            slot.assignSlotToRegister(drv1, DmdTyp::MONITOR_INT64, 450, 0, 5000);

            // Power On and Home axis 0
            printf("Power On Axis 0...\n");
            drv0.powerOn(5000);
            printf("Home Axis 0...\n");
            drv0.homingStart(5000);
            drv0.waitMovement(30000);

            // Starts display thread which will display axis 0 position
            // by displaying ML450 of axis 1
            printf("Move axis 0 and starts display threads...\n");
            _beginthread(display_thread, 0, grp.getDsaStructure());

            // Move axis 0 to 0
            drv0.setTargetPosition(0, 0, 5000);
            drv0.waitMovement(30000);

            // Wait 5 seconds
            Sleep(5000);

            // Move axis 0 to 1.0
            drv0.setTargetPosition(0, 1.0, 5000);
            drv0.waitMovement(30000);

            // Wait 5 seconds
            Sleep(5000);

            // Stops display thread
            doDisplay = 0;

            // Unassign slot
            slot.unassignSlotToRegister(drv1, DmdTyp::MONITOR_INT64, 450, 0, 5000);
            slot.unassignRegisterToSlot(drv0, DmdTyp::MONITOR_INT64, 1, 0, 5000);
        }
        catch (DsaException exc) {
            drv0.diag(__FILE__, __LINE__, exc.getCode());
            // Error management
            doDisplay = 0;
            slot.unassignSlotToRegister(drv1, DmdTyp::MONITOR_INT64, 450, 0, 5000);
            slot.unassignRegisterToSlot(drv0, DmdTyp::MONITOR_INT64, 1, 0, 5000);
        }

        // Close communication
        drv0.close();
        drv1.close();
        ultimet.close();

        // Success.
        printf("success.\n");
        _getch();
    }
    catch (DsaException exc) {
        grp.diag(__FILE__, __LINE__, exc.getCode());
        // Error management
        doDisplay  = 0;
        ebool open = false;
        try {
            open = drv0.isOpen();
        }
        catch (DsaException) {
        }
        if (open) {
            try {
                drv0.close();
            }
            catch (DsaException) {
            }
        }

        try {
            open = drv1.isOpen();
        }
        catch (DsaException) {
        }
        if (open) {
            try {
                drv1.close();
            }
            catch (DsaException) {
            }
        }

        try {
            open = ultimet.isOpen();
        }
        catch (DsaException) {
        }

        if (open) {
            try {
                ultimet.close();
            }
            catch (DsaException) {
            }
        }
        printf("error %d: %s.\n", exc.getCode(), exc.getText());
        _getch();
        return 1;
    }
}
