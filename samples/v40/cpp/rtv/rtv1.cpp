/*
 * rtv1.cpp
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
 * This sample program show how to read realtime value on TransnET without
 * irq synchronisation, this means that the software will :
 * Configure UltimET to put
 *		M50 : Digital input
 *		ML7 : real position realtime value
 *		MF31 : Real force Iq measured
 * of the AccurET into Realtime slot on TransnET.
 * The example will then read these value asynchronously.
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
#include <dsa40.h>

/*** functions ***/

/*
 * Entry point function.
 */
int main(int argc, char *argv[])
{
    DsaMaster ultimet; // Create UltimET object
    DsaDrive  drv;     // Create AccurET object

    try {
        printf("Opening communication\n");
        ultimet.open("etb:ULTIMET:*"); // Open UltimET device
        drv.open("etb:ULTIMET:0");     // Open AccurET device

        printf("Creating RTV objects\n");
        DsaRTVData m50rtv(drv, DmdTyp::MONITOR, 50, 0);          // Create read RTVData object on M50
        DsaRTVData ml7rtv(drv, DmdTyp::MONITOR_INT64, 7, 0);     // Create read RTVData object on ML7
        DsaRTVData mf31rtv(drv, DmdTyp::MONITOR_FLOAT32, 31, 0); // Create read RTVData object on MF31

        printf("Reading RTV objects\n");
        printf("M50 0x%x\n", m50rtv.readInt32());   // Read M50 Realtime object and print it out
        printf("ML7 %lld\n", ml7rtv.readInt64());   // Read ML70 Realtime object and print it out
        printf("MF31 %f\n", mf31rtv.readFloat32()); // Read MF31 Realtime object and print it out

        printf("Closing communication\n");
        drv.close();     // Close AccurET object
        ultimet.close(); // Close UltimET object

        /*
         * Everything seems succesfull, we can exit now.
         */
        printf("success.\n");
        return 0;
    }
    catch (DsaException exc) {
        ebool open = false;
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
