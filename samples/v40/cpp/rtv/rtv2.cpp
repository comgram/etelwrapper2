/*
 * rtv2.cpp
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
 * This sample program show how to read realtime value on TransnET with
 * irq synchronisation, this means that the software will :
 * Configure UltimET to put
 *		ML7 : real position realtime value
 * of the AccurET into Realtime slot on TransnET.
 * The example will then read these value synchronously when DSA_RTV_HANDLER will be called
 */

/*** libraries ***/

/*
 * Include EDI high level library version 3.0xx.
 * To enable the object oriented interface, you must define ETEL_OO_API before
 * include the header file. You can specify this define in the linker
 * or by putting a simple #define like in this sample.
 * If you dont'n define ETEL_OO_API, you can used the normal C API.
 */

#include <conio.h>

#define ETEL_OO_API
#include <dmd40.h>
#include <dsa40.h>
#include <lib40.h>

// RTV handler
// This handler will be called by EDI kernel at the rate specified by dsa_start_rtv_handler
// In our case, this handler will be called each second for first attached handler
// and each 2 seconds for second attached handler
// There are no object in write_table object
static void rtv_handler(DSA_DEVICE_BASE *dev, int nr, int nb_read, DSA_RTV_DATA *read_table[], int nb_write, DSA_RTV_DATA *write_table[], void *user)
{
    DsaMaster ultimet(dev);
    int       regTyp, regIdx, regSidx;
    int       i32Val;
    eint64    i64Val;
    float     f32Val;
    double    f64Val;
    int       i;

    // Get the dmd of ultimET object
    try {
        // Read all values specified in read_table parameters
        printf("rtv_handler nr %d called...Read...\n", nr);
        for (i = 0; i < nb_read; i++) {
            DsaRTVData rtv(read_table[i]);
            // Get the kind of DSA_RTV_DATA read object
            regTyp  = rtv.getRegisterTyp();
            regIdx  = rtv.getRegisterIdx();
            regSidx = rtv.getRegisterSidx();

            printf("\t%s%d RTV value:", Dmd::getDefaultTypeText(regTyp), regIdx);
            // DSA_RTV_DATA read object is an integer 32 bits
            if (rtv.isInt32()) {
                // read RTV value in a loop, because the read can return DSA_RTVREADSYNCRO error which
                // means that the read has access the RTV value exactly while the active RTV buffer has
                // switcheed
                for (;;) {
                    try {
                        i32Val = rtv.readInt32();
                        break;
                    }
                    catch (DsaException exc) {
                        if (exc.getCode() == DsaException::ERTVREADSYNCRO)
                            continue;
                        throw exc;
                    }
                }
                printf("0x%04x\n", i32Val);
            }
            // DSA_RTV_DATA read object is an integer 64 bits
            else if (rtv.isInt64()) {
                // read RTV value in a loop, because the read can return DSA_RTVREADSYNCRO error which
                // means that the read has access the RTV value exactly while the active RTV buffer has
                // switcheed
                for (;;) {
                    try {
                        i64Val = rtv.readInt64();
                        break;
                    }
                    catch (DsaException exc) {
                        if (exc.getCode() == DsaException::ERTVREADSYNCRO)
                            continue;
                        throw exc;
                    }
                }
                printf("%lld\n", i64Val);
            }
            // DSA_RTV_DATA read object is a float 32 bits
            else if (rtv.isFloat32()) {
                // read RTV value in a loop, because the read can return DSA_RTVREADSYNCRO error which
                // means that the read has access the RTV value exactly while the active RTV buffer has
                // switcheed
                for (;;) {
                    try {
                        f32Val = rtv.readFloat32();
                        break;
                    }
                    catch (DsaException exc) {
                        if (exc.getCode() == DsaException::ERTVREADSYNCRO)
                            continue;
                        throw exc;
                    }
                }
                printf("%012.6f\n", f32Val);
            }
            // DSA_RTV_DATA read object is a float 64 bits
            else if (rtv.isFloat64()) {
                // read RTV value in a loop, because the read can return DSA_RTVREADSYNCRO error which
                // means that the read has access the RTV value exactly while the active RTV buffer has
                // switcheed
                for (;;) {
                    try {
                        f64Val = rtv.readFloat64();
                        break;
                    }
                    catch (DsaException exc) {
                        if (exc.getCode() == DsaException::ERTVREADSYNCRO)
                            continue;
                        throw exc;
                    }
                }
                printf("%012.6f\n", f64Val);
            }
        }
    }
    catch (DsaException exc) {
        ultimet.diag(__FILE__, __LINE__, exc.getCode());
    }
}

/*
 * Entry point function.
 */
int main(int argc, char *argv[])
{
    DsaMaster ultimet; // Create UltimET object
    DsaDrive  drv;     // Create AccurET object

    try {
        printf("Opening communication\n");
        ultimet.open("etb:ULTIMET:*"); // Open AccurET device
        drv.open("etb:ULTIMET:0");     // Open AccurET device

        DsaRTVData *readTable[3];
        DsaRTVData  m50rtv(drv, DmdTyp::MONITOR_INT32, 50, 0);    // Create read RTVData object on M50
        DsaRTVData  ml7rtv(drv, DmdTyp::MONITOR_INT64, 7, 0);     // Create read RTVData object on ML7
        DsaRTVData  mf31rtv(drv, DmdTyp::MONITOR_FLOAT32, 31, 0); // Create read RTVData object on MF31

        readTable[0] = &m50rtv;
        readTable[1] = &ml7rtv;
        readTable[2] = &mf31rtv;

        // Start DSA_RTV_HANDLER handlers
        printf("Start RTV handler...Press a key to stop handler...\n");

        // Configure and start handler.
        // This first handler (parameter <nr> = 0) will be called each
        // second (parameter <rate> = 10000 = 10000 * 100 us = 1 second)
        // We pass m17rtv C datastructure as user parameter. This user parameter will be
        // transferred to the handler as is. This allow to have the RTV_DATA object in handler.
        ultimet.startRTVHandler(0, 10000, rtv_handler, 3, readTable, 0, NULL, NULL);

        while (!_kbhit()) {
            SLEEP(500);
        }
        _getch();

        printf("Stopping Handler\n");
        // Stop handler
        ultimet.stopRTVHandler(0);

        printf("Closing communication\n");
        drv.close();     // Close AccurET
        ultimet.close(); // Close UltimET

        /*
         * Everything seems succesfull, we can exit now.
         */
        printf("success.\n");
        return 0;
    }
    catch (DsaException exc) {
        ebool open   = false;
        ebool active = false;
        drv.diag(__FILE__, __LINE__, exc.getCode());
        try {
            open = drv.isOpen();
        }
        catch (DsaException) {
        }

        if (open) {
            try {
                drv.close();
            }
            catch (DsaException) {
            }
        }
        try {
            active = ultimet.isRTVHandlerActive(0);
        }
        catch (DsaException) {
        }
        if (active)
            try {
                ultimet.stopRTVHandler(0);
            }
            catch (DsaException) {
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
        fprintf(stderr, "error %d: %s.\n", exc.getCode(), exc.getText());
        return 1;
    }
}
