/*
 * etne_sample.c
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

/***************************************************************************************
 * This software is an example which show the way to implement the following things:
 *		1)	The way to open several connection on an UltimET PCI using ETNE server
 *		2)	How to use the new DSA_EXT_DIAG for extended diagnostic in case of error
 *
 *	Remark: The EDI package must be greater or equal to 4.00A !!!

***************************************************************************************/

#include <Windows.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dsa40.h"
#include "etne40.h"

#define SHORT_TIMEOUT       2000
#define MEDIUM_TIMEOUT      10000
#define LONG_TIMEOUT        60000

/* ETNE definition */
#define HOST_CONNEXION_PORT 1146             /* The port allowing a host connexion through UltimET*/
#define HOST_CONNEXION_NAME "CUSTOMER DEBUG" /* The name of the connexion which will be displayed in the ComET host list*/

/****** IMPORTANT NOTICE ******
THE 2nd LINE OF port.properties IN THE BIN FOLDER OF COMET NEEDS TO MODIFY LIKETHIS:

port.number=6

THE FOLLOWING LINES NEED TO BE ADDED AT THE BOTTOM IN THE port.properties IN THE BIN FOLDER OF COMET:

#parameters of port 5
port.5.name=CUSTOMER DEBUG
port.5.driver=etn://localhost:1146
port.5.protocol=ETB
port.5.accept.0.driver=ETN:0
port.5.start=auto

****** END OF IMPORTANT NOTICE ******/

/* Static variables */
static ETB *etb = NULL;

void etne_stop(void)
{
    etne_stop_custom(etb, HOST_CONNEXION_PORT);
    printf("Debug port has closed.\n");
    printf("Communication with ComET is loss if already connected!\n");
}

void deploy_status(DSA_DEVICE *dev, DSA_STATUS *stat)
{
    if (dsa_is_valid_drive(dev)) {
        printf("Drive status: SW1:0x%x, SW2:0x%x\n", stat->raw.sw1, stat->raw.sw2);
        /*
                //This piece of code will show the status in expanded way
                printf("\tPOWER ON: %1d\n", stat->drive.power_on);
                printf("\tPRESENT: %1d\n",  stat->drive.present);
                printf("\tMOVING: %1d\n",   stat->drive.moving);
                printf("\tIN WINDOW: %1d\n", stat->drive.in_window);
                printf("\tSEQUENCE ON: %1d\n", stat->drive.sequence);
                printf("\tERROR: %1d\n", stat->drive.error);
                printf("\tTRACE ON: %1d\n", stat->drive.trace);
                printf("\tSEQUENCE THREAD ON: %1d\n", stat->drive.sequence_thread);
                printf("\tWARNING: %1d\n", stat->drive.warning);
                printf("\tSAVE_POS: %1d\n", stat->drive.save_pos);
                printf("\tBREAKPOINT: %1d\n", stat->drive.breakpoint);
                printf("\tUSER-BITS: 0x%x\n", stat->drive.user);
        */
    }
    else if (dsa_is_valid_master(dev)) {
        printf("UltimET status: SW1:0x%x, SW2:0x%x\n", stat->raw.sw1, stat->raw.sw2);
        /*
                //This piece of code will show the status in expanded way
                printf("\tPRESENT: %1d\n", stat->master.present);
                printf("\tMOVING: %1d\n", stat->master.moving);
                printf("\tSEQUENCE ON: %1d\n", stat->master.sequence);
                printf("\tERROR: %1d", stat->master.error);
                printf("\tTRACE ON: %1d\n", stat->master.trace);
                printf("\tIPOL GRP 0 MOVING: %1d\n", stat->master.ipol0_moving);
                printf("\tIPOL GRP 1 MOVING: %1d\n", stat->master.ipol1_moving);
                printf("\tSEQUENCE THREAD ON: %1d\n", stat->ultimet.sequence_thread);
                printf("\tWARNING: %1d\n", stat->master.warning);
                printf("\tTRANSNET BROKEN: %1d\n", stat->master.transnet_broken);
                printf("\tBREAKPOINT: %1d\n", stat->master.breakpoint);
                printf("\tUSER-BITS: 0x%x\n", stat->master.user);
        */
    }
}

int main(int argc, char *argv[])
{
    int         err;
    ebool       open;
    DSA_DRIVE  *axis    = NULL;
    DSA_MASTER *ultimET = NULL;
    DSA_STATUS  status  = {sizeof(DSA_STATUS)};

    /* Create devices */
    printf("Drive & UltimET creation.\n");
    if (err = dsa_create_drive(&axis)) {
        DSA_EXT_DIAG(err, axis);
        goto _error;
    }
    if (err = dsa_create_master(&ultimET)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("Drive & UltimET are create.\n");

    /* Open connection */
    printf("Open connections in progress.\n");
    if (err = dsa_open_u(axis, "etb:UltimET:0")) {
        DSA_EXT_DIAG(err, axis);
        goto _error;
    }
    printf("..........");
    if (err = dsa_open_u(ultimET, "etb:UltimET:*")) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }
    printf("..........");
    printf("\nConnections are open.\n");

    /* Prepare the debug port with ETNE */
    /* Get the communication bus */
    if (err = dsa_get_etb_bus(ultimET, &etb)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }
    /* Start the thread allowing the host connection */
    if (err = etne_start_custom(etb, HOST_CONNEXION_PORT, HOST_CONNEXION_NAME)) {
        goto _error;
    }
    /*
     * Attach a routine which will be called at application exit.
     * It is safe to do this, because etne_stop_custom must be done in any case
     * This garantees the etne_stop_custom to be called, even on a process crash
     */
    atexit(etne_stop);

    printf("Debug communication port for ComET on UltimET is create.\n");
    printf("Now, you could connect ComET on \"CUSTOMER DEBUG\" port.\n");

    while (!_kbhit()) {
        // Print out UltimET Status
        if (err = dsa_get_status(ultimET, &status))
            DSA_EXT_DIAG(err, ultimET);
        else
            deploy_status(ultimET, &status);

        // Print out Drive Status
        if (err = dsa_get_status(axis, &status)) {
            DSA_EXT_DIAG(err, axis);
        }
        else
            deploy_status(axis, &status);
        printf("\n\nPress a key to stop process...\n");
        Sleep(5000);
    }
    /* Close the connection. */
    dsa_close(axis);

    /****** IMPORTANT NOTICE ******
     *	Before closing the last  connection,
     *	it's important to close etb connection on ETNE!!
     ****** END OF IMPORTANT NOTICE ******/
    etne_stop_custom(etb, HOST_CONNEXION_PORT);
    printf("Debug port has closed.\n");
    printf("Communication with ComET is loss if already connected!\n"); /* Close the connection. */

    /* Close the connection. */
    dsa_close(ultimET);

    /* Finally, release all resources to the OS. */
    dsa_destroy(&axis);
    dsa_destroy(&ultimET);
    return 0;

_error:
    etne_stop_custom(etb, HOST_CONNEXION_PORT);
    /* Is the drive pointer valid ? */
    if (dsa_is_valid_drive(axis)) {
        /* Is the drive open ? */
        open = 0;
        dsa_is_open(axis, &open);
        if (open) {
            /* Close the connection. */
            dsa_close(axis);
        }
        dsa_destroy(&axis);
    }
    if (dsa_is_valid_master(ultimET)) {
        /* Is the drive open ? */
        open = 0;
        dsa_is_open(ultimET, &open);
        if (open) {
            /* Close the connection. */
            dsa_close(ultimET);
        }
        dsa_destroy(&ultimET);
    }
}
