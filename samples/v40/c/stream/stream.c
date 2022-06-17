/*
 * stream.c
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
 * This sample program show how to make "stream" transfer between PC and device.
 * Stream transfer allows to write or read a large amount of data into/from the device.
 * This type of transfer is mainly used when uploading traces or registers or when
 * downloading firmwares.
 * This sample allows the user to choose the communication bus and the connected devices.
 * No tests are done concerning the validity of the registers'typ. For example, if is not possible to
 * write M registers, but this example doen no validity check
 */

/*** Initialisation steps ***/

/*** standard libraries ***/

#include <conio.h>  /* standard library for getch()*/
#include <ctype.h>  /* standard character conversion library */
#include <math.h>   /* standard math interface; used for random numbers here*/
#include <process.h>/* standard multithreading library       */
#include <stdio.h>  /* the standard I/O library              */
#include <stdlib.h> /* standard general purpose library      */

/*** ETEL libraries ***/
#include <dmd40.h> /* EDI constants definitions             */
#include <dsa40.h> /* EDI high level library version 4.0x.  */
#include <etb40.h>

#define NUM_AXIS 2
ebool       ultimETPresent;
int         nbDevices;
DSA_DEVICE *devices[NUM_AXIS + 1];

/* -------------------------------------------------------------------------------------------------------
 * Error handle function
 * -------------------------------------------------------------------------------------------------------*/
void errorHandle(int a_iErr, DSA_DEVICE_BASE *a_pDevice, const char *a_sErrorMsg)
{
    char strError[1024];
    int  i;

    // System error that we "catch" with a_sErrorMsg parameter, if it is void it means this is an EDI error and
    // in this case a_iErr will give us the error message, this is the else branch scenario.
    if (a_sErrorMsg != "") {
        sprintf(strError, "System Error: ");
        strcat(strError, a_sErrorMsg);
    }

    // EDI error.
    else {
        if (a_iErr < 0) {
            if (dsa_is_valid_device(a_pDevice))
                dsa_sdiag(strError, "StreamingRegisterTransfer", 0, a_iErr, a_pDevice);
            else
                sprintf(strError, "Device is not detected.");
        }

        else
            sprintf(strError, "Undefined Error.");
    }

    // Print the error message in the stadard output error stream.
    fprintf(stderr, strError);

    // Now we close each device (in case it is opened), and we destroy the devices and group.
    // Then we finish the execution with exit(1) --> errors.
    for (i = 0; i < (ultimETPresent ? NUM_AXIS + 1 : NUM_AXIS); i++) {
        if (dsa_is_valid_device(devices[i])) {
            ebool bOpen = FALSE;
            dsa_is_open(devices[i], &bOpen);

            if (bOpen)
                dsa_close(devices[i]);

            dsa_destroy(&(devices[i]));
            devices[i] = NULL;
        }
        else
            devices[i] = NULL;
    }

    exit(1);
}

/* -------------------------------------------------------------------------------------------------------
 * Function which diplays the axis selection menu to execute the register transfer operations.
 * -------------------------------------------------------------------------------------------------------*/
DSA_DEVICE *ChooseDevice()
{
    int i, ichoice;

    do {
        fflush(stdin);
        printf("\n\n\nChoose Axis\n");
        printf("****************\n");
        for (i = 0; i < NUM_AXIS; i++)
            printf("\t%d: Axis %d\n", i, i);
        if (ultimETPresent)
            printf("\t%d: UltimET\n", i);
        printf("\n\t\tYour choice : ");
        fflush(stdout);
        scanf("%d", &ichoice);
    } while ((ichoice < 0) || (ichoice >= nbDevices));

    return devices[ichoice];
}

/* ---------------------------------------------------------------------------------------------------------------------------
 * The user will be able to choose the connection mode of the Drive System: USB, AccurET TCP/IP, Ultimet TCP/IP or Ultimet PCI;
 * and the Drive Connection will be established.
 * --------------------------------------------------------------------------------------------------------------------------*/
int OpenConnection()
{
    char  sMode[64], sConnection[64], sIP[64];
    char  choice;
    int   err = 0;
    int   i;
    ebool bOpen;

    /* Display menu. We show the connection menu until a valid option is selected. */
    do {
        fflush(stdin);
        printf("\n\n\nChoose Connection Mode\n");
        printf("******************\n");
        printf("\t0: Exit\n");
        printf("\t1: USB\n");
        printf("\t2: AccurET TCP/IP\n");
        printf("\t3: UltimET\n");
        printf("\t4: UltimET TCP/IP\n");

        printf("\n\t\tYour choice : ");
        fflush(stdout);
        scanf("%c", &choice);
    } while ((choice != '1') && (choice != '2') && (choice != '3') && (choice != '4') && (choice != '0'));

    ultimETPresent = TRUE;
    /* Get connection mode */
    switch (choice) {
    /*** USB *********************************************************************/
    case '1':
        ultimETPresent = FALSE;
        strcpy(sMode, "USB");
        break;
    /*** TCP/IP ************************************************************************/
    case '2':
        ultimETPresent = FALSE;
    case '4':
        printf("\n\t\t\tEnter IP : ");
        fflush(stdout);
        scanf("%s", sIP);
        strcpy(sMode, "ETN://");
        strcat(sMode, sIP);
        break;
    /*** ULTIIMET **********************************************************************/
    case '3':
        strcpy(sMode, "ULTIMET");
        break;
    /*** Exit ************************************************************************/
    case '0':
        ultimETPresent = FALSE;
        return 0;
    }

    nbDevices = NUM_AXIS;
    /* Create and open devices */
    for (i = 0; i < NUM_AXIS; i++) {
        if (!devices[i]) {
            if (err = dsa_create_drive(&devices[i]))
                errorHandle(err, devices[i], "");

            bOpen = FALSE;
            if (err = dsa_is_open(devices[i], &bOpen))
                errorHandle(err, devices[i], "");

            if (!bOpen) {
                sprintf(sConnection, "etb:%s:%d", sMode, i);
                if (err = dsa_open_u(devices[i], (const char *)(sConnection)))
                    errorHandle(err, devices[i], "");
            }
        }
    }

    if (ultimETPresent) {
        nbDevices++;
        if (err = dsa_create_master(&devices[NUM_AXIS]))
            errorHandle(err, devices[NUM_AXIS], "");
        bOpen = FALSE;
        if (err = dsa_is_open(devices[NUM_AXIS], &bOpen))
            errorHandle(err, devices[NUM_AXIS], "");
        if (!bOpen) {
            sprintf(sConnection, "etb:%s:*", sMode);
            if (err = dsa_open_u(devices[NUM_AXIS], (const char *)(sConnection)))
                errorHandle(err, devices[NUM_AXIS], "");
        }
    }

    return 1;
}

/* -------------------------------------------------------------------------------------------------------
 * Get parameter from user
 * -------------------------------------------------------------------------------------------------------*/
int get_param_menu(char *sRegParam)
{
    char choice;

    fflush(stdin);
    do {
        printf("\nEnter parameter :\n");
        printf("\tX : User\n");
        printf("\tK : Parameter\n");
        printf("\tM : Monitor\n");
        printf("\tT : Trace\n");
        printf("\tA : Address\n");
        printf("\tL : Lookup\n");
        printf("\tE : Trigger\n");
        printf("\tC : Common\n");
        printf("\n\t\tYour choice : ");
        fflush(stdout);
        scanf("%c", &choice);
    } while ((choice != 'X') && (choice != 'K') && (choice != 'M') && (choice != 'T') && (choice != 'A') && (choice != 'L') && (choice != 'E') &&
             (choice != 'C') && (choice != 'x') && (choice != 'k') && (choice != 'm') && (choice != 't') && (choice != 'a') && (choice != 'l') &&
             (choice != 'e') && (choice != 'c'));

    switch (choice) {
    case 'X':
    case 'x':
        strcpy(sRegParam, "X");
        return DMD_TYP_USER;
    case 'K':
    case 'k':
        strcpy(sRegParam, "K");
        return DMD_TYP_PPK;
    case 'M':
    case 'm':
        strcpy(sRegParam, "M");
        return DMD_TYP_MONITOR;
    case 'T':
    case 't':
        strcpy(sRegParam, "T");
        return DMD_TYP_TRACE;
    case 'A':
    case 'a':
        strcpy(sRegParam, "A");
        return DMD_TYP_ADDRESS;
    case 'L':
    case 'l':
        strcpy(sRegParam, "L");
        return DMD_TYP_LKT;
    case 'E':
    case 'e':
        strcpy(sRegParam, "E");
        return DMD_TYP_TRIGGER;
    case 'C':
    case 'c':
        strcpy(sRegParam, "C");
        return DMD_TYP_COMMON;
    }
    return 0;
}

/* -------------------------------------------------------------------------------------------------------
 * Get parameter type from user
 * -------------------------------------------------------------------------------------------------------*/
int get_type_menu()
{
    char choice;

    fflush(stdin);
    do {
        printf("\nEnter type\n");
        printf("\t1 : Integer 32 bits\n");
        printf("\t2 : Integer 64 bits\n");
        printf("\t3 : Float 32 bits\n");
        printf("\t4 : Float 64 bits\n");
        printf("\n\t\tYour choice : ");
        fflush(stdout);
        scanf("%c", &choice);
    } while ((choice != '1') && (choice != '2') && (choice != '3') && (choice != '4'));

    switch (choice) {
    case '1':
        return 0;
    case '2':
        return DMD_TYP_64_BIT_MASK;
    case '3':
        return DMD_TYP_FLOAT_BIT_MASK;
    case '4':;
        return DMD_TYP_64_BIT_MASK | DMD_TYP_FLOAT_BIT_MASK;
    }
    return 0;
}

/* -------------------------------------------------------------------------------------------------------
 * Upload all specified data and put them into a file
 * -------------------------------------------------------------------------------------------------------*/
void uploadReg(DSA_DEVICE *dev, int parType, const char *sRegParam, int depth, int iStartIdx, char *fileName)
{
    char  buffer[1024];
    char  sErrorMsg[1024];
    int   axisNb;
    int   err;
    int   size = 1024;
    int   iIdx = iStartIdx;
    FILE *f;
    char  choice[2] = "";
    ebool binary;
    DMD  *dmd;
    int   nbDepth;
    int   currentDepth;
    int   startDepth;

    if (err = dsa_etcom_get_etb_axis(dev, &axisNb)) {
        sprintf(sErrorMsg, "Error getting axis number\n");
        errorHandle(-1, dev, sErrorMsg);
    }

    /* Get the number of uploaded depth in the case where all depth are asked */
    if (depth == -1) {
        if (err = dsa_get_dmd_data(dev, &dmd)) {
            sprintf(sErrorMsg, "Error getting dmd\n");
            errorHandle(-1, dev, sErrorMsg);
        }
        nbDepth    = dmd_get_number_of_subindexes(dmd, parType, 0);
        startDepth = 0;

        /* In the case of X, the number of uploaded depth with stream are one less, because depth 0 */
        /* is the same than depth 1 */
        if (parType == DMD_TYP_USER) {
            startDepth = 1;
        }
    }
    else {
        nbDepth    = 1;
        startDepth = depth;
    }

    do {
        printf("\nBinary file (Y if must be re-downloaded using stream) : [Y/N] ? ");
        fflush(stdout);
        scanf("%s", choice);
    } while ((*choice != 'Y') && (*choice != 'y') && (*choice != 'N') && (*choice != 'n'));

    binary = (*choice == 'Y') || (*choice == 'y');

    if (binary) {
        /* Open file */
        if (!(f = fopen(fileName, "wb"))) {
            sprintf(sErrorMsg, "Error creating file %s\n", fileName);
            errorHandle(-1, dev, sErrorMsg);
        }
    }
    else {
        /* Open file */
        if (!(f = fopen(fileName, "w"))) {
            sprintf(sErrorMsg, "Error creating file %s\n", fileName);
            errorHandle(-1, dev, sErrorMsg);
        }
    }

    currentDepth = startDepth;

    /* Upload data in 1024 bytes buffer until no more data are retrieved */
    while ((err = dsa_upload_data_s(dev, buffer, size, 100000)) > 0) {
        if (binary) {
            fwrite(buffer, 1, err, f);
        }
        else {
            switch (parType & (DMD_TYP_64_BIT_MASK | DMD_TYP_FLOAT_BIT_MASK)) {
            case 0: {
                int *p;
                for (p = (int *)buffer; (char *)p - buffer < err; p++) {
                    if (axisNb == 63)
                        fprintf(f, "*%s%d:%d=%d\n", sRegParam, iIdx, currentDepth, *p);
                    else
                        fprintf(f, "%s%d.%d:%d=%d\n", sRegParam, iIdx, axisNb, currentDepth, *p);
                    if (++currentDepth >= nbDepth) {
                        currentDepth = startDepth;
                        iIdx++;
                    }
                }
                break;
            }
            case DMD_TYP_FLOAT_BIT_MASK: {
                float *p;
                for (p = (float *)buffer; (char *)p - buffer < err; p++) {
                    if (axisNb == 63)
                        fprintf(f, "*%sF%d:%d=%f\n", sRegParam, iIdx, currentDepth, *p);
                    else
                        fprintf(f, "%sF%d.%d:%d=%f\n", sRegParam, iIdx, currentDepth, depth, *p);
                    if (++currentDepth >= nbDepth) {
                        currentDepth = startDepth;
                        iIdx++;
                    }
                }
                break;
            }
            case DMD_TYP_64_BIT_MASK: {
                eint64 *p;
                for (p = (eint64 *)buffer; (char *)p - buffer < err; p++) {
                    if (axisNb == 63)
                        fprintf(f, "*%sL%d:%d=%lld\n", sRegParam, iIdx, currentDepth, *p);
                    else
                        fprintf(f, "%sL%d.%d:%d=%lld\n", sRegParam, iIdx, axisNb, currentDepth, *p);
                    if (++currentDepth >= nbDepth) {
                        currentDepth = startDepth;
                        iIdx++;
                    }
                }
                break;
            }
            case DMD_TYP_64_BIT_MASK | DMD_TYP_FLOAT_BIT_MASK: {
                double *p;
                for (p = (double *)buffer; (char *)p - buffer < err; p++) {
                    if (axisNb == 63)
                        fprintf(f, "*%sD%d:%d=%lf\n", sRegParam, iIdx, currentDepth, *p);
                    else
                        fprintf(f, "%sD%d.%d:%d=%lf\n", sRegParam, iIdx, axisNb, currentDepth, *p);
                    if (++currentDepth >= nbDepth) {
                        currentDepth = startDepth;
                        iIdx++;
                    }
                }
                break;
            }
            }
        }
    }
    fclose(f);

    if (err < 0)
        errorHandle(err, dev, "");

    return;
}

/* -------------------------------------------------------------------------------------------------------
 * Download all register data from file in our device
 * -------------------------------------------------------------------------------------------------------*/
void downloadReg(DSA_DEVICE *pDev, char *fileName)
{
    char  buffer[1024];
    char  sErrorMsg[1024];
    int   size = 1024;
    int   err  = 0;
    int   nb;
    ebool bCopyFlag;
    FILE *f;

    /* Open file for read */
    if (!(f = fopen(fileName, "rb"))) {
        sprintf(sErrorMsg, "Error creating file %s\n", fileName);
        errorHandle(-1, pDev, sErrorMsg);
    }

    bCopyFlag = FALSE;
    /* Read data from file in a buffer of 1024 bytes until end of file is reached */
    while (nb = (int)fread(buffer, 1, size, f)) {
        /* read Error */
        if (nb == -1) {
            fclose(f);
            sprintf(sErrorMsg, "Error reading from file.\n");
            errorHandle(-1, pDev, sErrorMsg);
        }
        /* Download data */
        if (err = dsa_download_data_s(pDev, buffer, nb, 10000)) {
            fclose(f);
            errorHandle(err, pDev, "");
        }
    }

    /* terminate download process */
    if (err = dsa_download_data_s(pDev, NULL, 0, 10000)) {
        fclose(f);
        errorHandle(err, pDev, "");
    }

    fclose(f);

    return;
}

/* -------------------------------------------------------------------------------------------------------
 * Get all input need to upload parameters from user and upload parameter
 * -------------------------------------------------------------------------------------------------------*/
void upload_parameter(DSA_DEVICE *pDev)
{
    char fileName[512];
    int  parType;
    int  parDepth;
    int  startIdx;
    int  endIdx;
    int  err;
    char sRegParam[2] = "";
    char sAxis[2]     = "";
    char sRegIdx[5]   = "";

    /* get needed inputs from user */
    printf("Enter file name : ");
    fflush(stdout);
    scanf("%s", fileName);
    parType = get_param_menu(sRegParam);
    parType |= get_type_menu();
    printf("\nEnter parameter depth : ");
    fflush(stdout);
    scanf("%d", &parDepth);
    printf("\nEnter start index : ");
    fflush(stdout);
    scanf("%d", &startIdx);
    printf("\nEnter end index (included): ");
    fflush(stdout);
    scanf("%d", &endIdx);

    if (err = dsa_start_upload_register_s(pDev, parType, startIdx, endIdx, parDepth, 1000))
        errorHandle(err, pDev, "");

    uploadReg(pDev, parType, (const char *)sRegParam, parDepth, startIdx, fileName);

    return;
}

/* -------------------------------------------------------------------------------------------------------
 * Get all input need to download parameters from user and download parameter
 * -------------------------------------------------------------------------------------------------------*/
void download_parameter(DSA_DEVICE *pDev)
{
    char fileName[512];
    int  parType;
    int  parDepth;
    int  startIdx;
    int  endIdx;
    int  err;

    char sRegParam[2] = "";

    /* get needed inputs from user */
    printf("Enter file name : ");
    fflush(stdout);
    scanf("%s", fileName);
    parType = get_param_menu(sRegParam);
    parType |= get_type_menu();
    printf("\nEnter parameter depth : ");
    fflush(stdout);
    scanf("%d", &parDepth);
    printf("\nEnter start index : ");
    fflush(stdout);
    scanf("%d", &startIdx);
    printf("\nEnter end index : ");
    fflush(stdout);
    scanf("%d", &endIdx);

    /* start download parameters */
    if (err = dsa_start_download_register_s(pDev, parType, startIdx, endIdx, parDepth, 1000)) {
        errorHandle(err, pDev, "");
    }
    /* download parameter file */
    downloadReg(pDev, fileName);

    return;
}

/* -------------------------------------------------------------------------------------------------------
 *
 * -------------------------------------------------------------------------------------------------------*/
void RegisterTransfer()
{
    char        choice;
    DSA_DEVICE *device;

    /* Display menu */
    for (;;) {
        do {
            fflush(stdin);
            printf("\n\n\nChoose Action:\n");
            printf("******************\n");
            printf("\t0: Exit\n");
            printf("\t1: Upload Registers to file.\n");
            printf("\t2: Download Registers from file.\n");

            printf("\n\t\tYour choice : ");
            fflush(stdout);
            scanf("%c", &choice);
        } while ((choice != '1') && (choice != '2') && (choice != '0'));
        printf("\n");

        switch (choice) {
        // Upload Registers to file
        case '1':
            device = ChooseDevice();
            upload_parameter(device);
            break;

        // Download Registers from file
        case '2':
            device = ChooseDevice();
            download_parameter(device);
            break;

        // Exit
        case '0':
            return;
        }
    }
}

/* -------------------------------------------------------------------------------------------------------
 * Close connection and destroy devices & group
 * -------------------------------------------------------------------------------------------------------*/

void CloseConnection()
{
    int err, i;

    for (i = 0; i < nbDevices; i++) {
        if (dsa_is_valid_device(devices[i])) {
            /* Is the communication open ? */
            ebool bOpen = FALSE;

            if (err = dsa_is_open(devices[i], &bOpen))
                errorHandle(err, devices[i], "");

            if (bOpen)
                if (err = dsa_close(devices[i]))
                    errorHandle(err, devices[i], "");

            if (err = dsa_destroy(&(devices[i])))
                errorHandle(err, devices[i], "");

            devices[i] = NULL;
        }
        else
            devices[i] = NULL;
    }

    return;
}
/* -------------------------------------------------------------------------------------------------------
 * Main Function
 * -------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    for (;;) {
        /* Choose Connection Mode & open Drive Connection.*/
        if (!OpenConnection())
            exit(0);

        /* Execute the upload or download of registers */
        RegisterTransfer();

        /* Close connection and destroy devices*/
        CloseConnection();
    }
}