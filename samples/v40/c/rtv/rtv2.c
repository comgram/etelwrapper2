/*
 * rtv2.c
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
 * This sample program show how to read realtime value on TransnET with
 * irq synchronisation, this means that the software will :
 * Configure UltimET to put
 *		M50 : Digital input
 *		ML7 : real position realtime value
 *		MF31 : Real force Iq measured
 * of the AccurET into Realtime slot on TransnET.
 * The example will then read these value synchronously when DSA_RTV_HANDLER will be called
 */

/*
 * To run this demo without modification, you must have:
 *  - a PCI-UltimET board plugged into your PC
 *  - an AccurET device 0 connected to UltimET through TransnET
 *  - Eventually a connected and setted motor
 */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#include <dmd40.h>
#include <dsa40.h>
#include <lib40.h>

// RTV handler
// This handler will be called by EDI kernel at the rate specified by dsa_start_rtv_handler
// In our case, this handler will be called each second for first attached handler
// and each 2 seconds for second attached handler
// There are no object in write_table object
static void DSA_CALLBACK
rtv_handler(DSA_MASTER *ultimet, int nr, int nb_read, DSA_RTV_DATA *read_table[], int nb_write, DSA_RTV_DATA *write_table[], void *user)
{
    int    err;
    int    reg_typ, reg_idx, reg_sidx;
    int    i32Val;
    eint64 i64Val;
    float  f32Val;
    double f64Val;
    int    i;
    DMD   *dmd = NULL;

    // Get the dmd of ultimET object
    if (err = dsa_get_dmd_data(ultimet, &dmd)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    // Read all values specified in read_table parameters
    printf("rtv_handler nr %d called...Read...\n", nr);
    for (i = 0; i < nb_read; i++) {
        // Get the kind of DSA_RTV_DATA read object
        if (err = dsa_get_register_typ_idx_sidx_rtv(read_table[i], &reg_typ, &reg_idx, &reg_sidx)) {
            DSA_EXT_DIAG(err, ultimet);
            goto _error;
        }

        printf("\t%s%d RTV value:", dmd_get_type_text(dmd, DMD_TEXT_MNEMONIC, reg_typ), reg_idx);
        // DSA_RTV_DATA read object is an integer 32 bits
        if (dsa_is_int32_rtv(read_table[i])) {
            // read RTV value in a loop, because the read can return DSA_RTVREADSYNCRO error which
            // means that the read has access the RTV value exactly while the active RTV buffer has
            // switcheed
            do {
                if (err = dsa_read_rtv_int32(read_table[i], &i32Val)) {
                    if (err == DSA_ERTVREADSYNCRO)
                        continue;
                    DSA_EXT_DIAG(err, ultimet);
                    goto _error;
                }
            } while (err);
            printf("0x%04x\n", i32Val);
        }
        // DSA_RTV_DATA read object is an integer 64 bits
        else if (dsa_is_int64_rtv(read_table[i])) {
            // read RTV value in a loop, because the read can return DSA_RTVREADSYNCRO error which
            // means that the read has access the RTV value exactly while the active RTV buffer has
            // switcheed
            do {
                if (err = dsa_read_rtv_int64(read_table[i], &i64Val)) {
                    if (err == DSA_ERTVREADSYNCRO)
                        continue;
                    DSA_EXT_DIAG(err, ultimet);
                    goto _error;
                }
            } while (err);
            printf("%lld\n", i64Val);
        }
        // DSA_RTV_DATA read object is a float 32 bits
        else if (dsa_is_float32_rtv(read_table[i])) {
            // read RTV value in a loop, because the read can return DSA_RTVREADSYNCRO error which
            // means that the read has access the RTV value exactly while the active RTV buffer has
            // switcheed
            do {
                if (err = dsa_read_rtv_float32(read_table[i], &f32Val)) {
                    if (err == DSA_ERTVREADSYNCRO)
                        continue;
                    DSA_EXT_DIAG(err, ultimet);
                    goto _error;
                }
            } while (err);
            printf("%012.6f\n", f32Val);
        }
        // DSA_RTV_DATA read object is a float 64 bits
        else if (dsa_is_float64_rtv(read_table[i])) {
            // read RTV value in a loop, because the read can return DSA_RTVREADSYNCRO error which
            // means that the read has access the RTV value exactly while the active RTV buffer has
            // switcheed
            do {
                if (err = dsa_read_rtv_float64(read_table[i], &f64Val)) {
                    if (err == DSA_ERTVREADSYNCRO)
                        continue;
                    DSA_EXT_DIAG(err, ultimet);
                    goto _error;
                }
            } while (err);
            printf("%012.6f\n", f64Val);
        }
    }

_error:
    return;
}

int main()
{
#define NB_RTV       3
#define M50_RTV_IDX  0
#define ML7_RTV_IDX  1
#define MF31_RTV_IDX 2

    DSA_MASTER   *ultimet = NULL; // ULTIMET object
    DSA_DRIVE    *drv     = NULL; // ACCURET object
    int           err     = 0;
    DSA_RTV_DATA *read_table[NB_RTV];
    int           i;

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
    for (i = 0; i < NB_RTV; i++)
        read_table[i] = NULL;

    if (err = dsa_create_rtv_read(drv, DMD_TYP_MONITOR, 50, 0, &read_table[M50_RTV_IDX])) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    if (err = dsa_create_rtv_read(drv, DMD_TYP_MONITOR_INT64, 7, 0, &read_table[ML7_RTV_IDX])) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }
    if (err = dsa_create_rtv_read(drv, DMD_TYP_MONITOR_FLOAT32, 31, 0, &read_table[MF31_RTV_IDX])) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    // Start DSA_RTV_HANDLER handlers
    printf("Start RTV handlers...Press a key to stop handlers...\n");

    // Configure and start first handler.
    // This first handler (parameter <nr> = 0) will be called each
    // second (parameter <rate> = 10000 = 10000 * 100 us = 1 second)
    // We pass rtv_table as user parameter. This user parameter will be
    // transferred to the handler as is. This allow to have the RTV_DATA object in handler.
    if (err = dsa_start_rtv_handler(ultimet, 0, 10000, rtv_handler, NB_RTV, read_table, 0, NULL, NULL)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    // This second handler (parameter <nr> = 1) will be called each
    // 2 seconds (parameter <rate> = 20000 = 20000 * 100 us = 2 seconds)
    // We pass rtv_table as user parameter. This user parameter will be
    // transferred to the handler as is. This allow to have the RTV_DATA object in handler.
    if (err = dsa_start_rtv_handler(ultimet, 1, 20000, rtv_handler, NB_RTV, read_table, 0, NULL, NULL)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    while (!_kbhit()) {
        SLEEP(500);
    }
    _getch();

    // Stop first handler
    if (err = dsa_stop_rtv_handler(ultimet, 0)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    // Stop second handler
    if (err = dsa_stop_rtv_handler(ultimet, 1)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    /* Success. */
    printf("success.\n");

/* Error handler. */
_error:
    if (dsa_is_valid_master(ultimet)) {
        ebool active = 0;
        dsa_get_rtv_handler_activity(ultimet, 0, &active);
        if (active)
            dsa_stop_rtv_handler(ultimet, 0);
        dsa_get_rtv_handler_activity(ultimet, 1, &active);
        if (active)
            dsa_stop_rtv_handler(ultimet, 1);
    }

    /* Destroy the rtv_data objects */
    for (i = 0; i < NB_RTV; i++) {
        if (dsa_is_valid_rtv(read_table[i]))
            dsa_destroy_rtv(&read_table[i]);
    }

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
