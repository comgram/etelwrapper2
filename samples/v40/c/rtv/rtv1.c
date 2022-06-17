/*
 * rtv1.c
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
 * This sample program show how to read realtime value on TransnET without
 * irq synchronisation, this means that the software will :
 * Configure UltimET to put
 *		M50 : Digital input
 *		ML7 : real position realtime value
 *		MF31 : Real force Iq measured
 * of the AccurET into Realtime slot on TransnET.
 * The example will then read these value asynchronously.
 */

/*
 * To run this demo without modification, you must have:
 *  - a PCI-UltimET board plugged into your PC
 *  - an AccurET device 0 connected to UltimET through TransnET
 *  - Eventually a connected and setted motor
 */

#include <stdio.h>
#include <stdlib.h>

#include <dsa40.h>

int main()
{
    DSA_MASTER   *ultimet  = NULL; // ULTIMET object
    DSA_DRIVE    *drv      = NULL; // ACCURET object
    int           err      = 0;
    DSA_RTV_DATA *M50_rtv  = NULL; // Realtime slot object referring to M50 value
    DSA_RTV_DATA *ML7_rtv  = NULL; // Realtime slot object referring to ML7 value
    DSA_RTV_DATA *MF31_rtv = NULL; // Realtime slot object referring to MF31 value
    int           m50;             // Will contain read M50 value
    eint64        ml7;             // Will contain read ML7 value
    float         mf31;            // Will contain read MF31 value

    printf("Creating UltimET and AccurET objects...\n");
    // Create one ultimet object
    if (err = dsa_create_master(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    if (err = dsa_create_drive(&drv)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    printf("Opening of the communication...\n");
    // Connections with UltimET
    if (err = dsa_open_u(ultimet, "etb:ULTIMET:*")) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    if (err = dsa_open_u(drv, "etb:ULTIMET:0")) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    printf("Create RTV read objects...\n");

    if (err = dsa_create_rtv_read(drv, DMD_TYP_MONITOR, 50, 0, &M50_rtv)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    if (err = dsa_create_rtv_read(drv, DMD_TYP_MONITOR_INT64, 7, 0, &ML7_rtv)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }
    if (err = dsa_create_rtv_read(drv, DMD_TYP_MONITOR_FLOAT32, 31, 0, &MF31_rtv)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    // read RTV value in a loop, because the read can return DSA_RTVREADSYNCRO error which
    // means that the read has access the RTV value exactly while the active RTV buffer has
    // switcheed
    printf("Read M50 RTV value...\n");
    do {
        if (err = dsa_read_rtv_int32(M50_rtv, &m50)) {
            if (err == DSA_ERTVREADSYNCRO)
                continue;
            DSA_EXT_DIAG(err, drv);
            goto _error;
        }
    } while (err);
    printf("->RTV M50 : 0x%x\n", m50);

    printf("Read ML7 RTV value...\n");
    do {
        if (err = dsa_read_rtv_int64(ML7_rtv, &ml7)) {
            if (err == DSA_ERTVREADSYNCRO)
                continue;
            DSA_EXT_DIAG(err, drv);
            goto _error;
        }
    } while (err);
    printf("->ML7 : %lld\n", ml7);

    printf("Read MF31 RTV value...\n");
    do {
        if (err = dsa_read_rtv_float32(MF31_rtv, &mf31)) {
            if (err == DSA_ERTVREADSYNCRO)
                continue;
            DSA_EXT_DIAG(err, drv);
            goto _error;
        }
    } while (err);
    printf("->MF31 : %f\n", mf31);

    /* Success. */
    printf("success.\n");

/* Error handler. */
_error:
    /* Destroy the rtv_data objects */
    if (dsa_is_valid_rtv(M50_rtv))
        dsa_destroy_rtv(&M50_rtv);

    if (dsa_is_valid_rtv(ML7_rtv))
        dsa_destroy_rtv(&ML7_rtv);

    if (dsa_is_valid_rtv(MF31_rtv))
        dsa_destroy_rtv(&MF31_rtv);

    /* Is the drive pointer valid ? */
    if (dsa_is_valid_drive(drv)) {
        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(drv, &open);
        if (open) {
            /* Close the connection. */
            dsa_close(drv);
        }
        /* Finally, release all resources to the OS. */
        dsa_destroy(&drv);
    }

    /* Is the drive pointer valid ? */
    if (dsa_is_valid_master(ultimet)) {
        /* Is the ultimet open ? */
        ebool open = 0;
        dsa_is_open(ultimet, &open);
        if (open) {
            /* Close the connection. */
            dsa_close(ultimet);
        }
        /* Finally, release all resources to the OS. */
        dsa_destroy(&ultimet);
    }

    printf("ended.\n");
    return err;
}
