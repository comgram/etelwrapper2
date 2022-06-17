/*
 * mapping1.c
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
 * This sample program show how to :
 *	-	download mapping file into a group of drives
 *	-	upload data stored in a group of device into a mapping file
 *	-	activate mapping of a group ofdrive
 *	-	deactivate mapping of a group of drive
 *	-	check mapping activity of a group of drive
 */

/*
 * To run this demo without modification, you must have:
 *  - a PCI-UltimET board plugged into your PC
 *  - one or many AccurET connected to UltimET through TransnET
 * If you want to run this example without UltimET, you must change the URL
 * It is advised to download first mapping files provided with the example before
 * uploading data from drives. There are 2 mapping files provided with the example:
 * - map2D.txt which is a correct mapping file downloadable into 2 AccurET number 0 and 1.
 * - map3D.txt which is a correct mapping file downloadable into 3 AccurET number 0, 1 and 2.
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
#include <dmd40.h>
#include <dsa40.h> /* EDI high level library version 3.0x.  */

/* The default url */
#define DEFAULT_URL_FORMAT "etb:ultimet:%d" // Transnet url
//#define DEFAULT_URL_FORMAT "etb:USB:%d"									//USB url
//#define DEFAULT_URL_FORMAT "etb:etn://172.22.10.101:1129:%d"				//TCP-IP url

/*
 * Upload function.
 * This function ask from which drives the upload must be done.
 * All specified drives must contain the mapping datas corresponding
 * to the same mapping configuration. While upload process, a check will be done.
 * Thus it is advised to call first download_mapping and download a mapping file
 * provided with the example.
 * The function ask also the file name into which the uploaded datas must be stored
 */
int upload_mapping()
{
    char             filename[512]; // The file name entered by the user
    int              axis_number;   // The number of axis participating to upload
    int              nr;            // A help variable containing the axis number
    DSA_DRIVE       *drvs[64];      // An array containing all devices participating to upload
    DSA_DRIVE_GROUP *grp = NULL;    // A group of devices participating to upload
    char             url[32];       // The url used by dsa_open_u function
    int              err;           // A variable containing the function call error
    int              i;             // A loop counter

    // Initialisation
    memset(drvs, 0, sizeof(drvs));

    // Ask the user to enter the number of axis participating to upload
    printf("\n");
    printf("Enter number of axes participating to upload : ");
    scanf("%d", &axis_number);

    // Create the group of device
    if (err = dsa_create_drive_group(&grp, axis_number)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    // Ask the drive number ,create and open the devices. Stored them in array and put them into device group
    for (i = 0; i < axis_number; i++) {
        printf("\tEnter %d. axis number : ", i);
        scanf("%d", &nr);
        drvs[i] = NULL;
        if (err = dsa_create_drive(&drvs[i])) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
        sprintf(url, DEFAULT_URL_FORMAT, nr);
        if (err = dsa_open_u(drvs[i], url)) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
        if (err = dsa_add_group_item(grp, drvs[i])) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
    }

    // Ask the user to enter file name into which uploaded data will be stored
    printf("\n");
    printf("Enter file name to upload to : ");
    scanf("%s", filename);

    // Upload and store datas into file
    if (err = dsa_stage_mapping_upload(grp, filename)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

// Error management
_error : {
    ebool open;
    if (dsa_is_valid_drive_group(grp))
        dsa_destroy(&grp);
    for (i = 0; i < axis_number; i++) {
        dsa_is_open(drvs[i], &open);
        if (open)
            dsa_close(drvs[i]);
        if (dsa_is_valid_drive(drvs[i]))
            dsa_destroy(&drvs[i]);
    }
    return err;
}
}

/*
 * Download function.
 * The function ask the mapping file name from which mapping datas must be read.
 * It then import the mapping file into memory by calling the dex_import. This is done
 * first to know which are the concerned axes. The function then opens the devices
 * and create a group of devices.
 * The function then call the dsa_stage_mapping_download function which downloads the
 * datas into the group of devices.
 * All drives specified in mapping file must be present.
 * There are 2 mapping files provided with the example:
 * - map2D.txt which is a correct mapping file downloadable into 2 AccurET number 0 and 1.
 * - map3D.txt which is a correct mapping file downloadable into 3 AccurET number 0, 1 and 2.
 */
int download_mapping()
{
    char             filename[512]; // The file name entered by the user
    int              axis_number;   // The number of axes specified in mapping file
    int              nr;            // A help variable containing the axis number
    DSA_DRIVE       *drvs[64];      // An array containing all devices participating to download
    DSA_DRIVE_GROUP *grp = NULL;    // A group containing all devices participating to download
    char             url[32];       // The url used by dsa_open_u function
    int              err;           // A variable containing the function call error
    int              i;             // A loop counter

    // Initialisation
    memset(drvs, 0, sizeof(drvs));

    // Ask the user to enter the number of axis participating to download
    printf("\n");
    printf("Enter number of axes participating to download : ");
    scanf("%d", &axis_number);

    // Create the group of device
    if (err = dsa_create_drive_group(&grp, axis_number)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    // Ask the drive number ,create and open the devices. Stored them in array and put them into device group
    for (i = 0; i < axis_number; i++) {
        printf("\tEnter %d. axis number : ", i);
        scanf("%d", &nr);
        drvs[i] = NULL;
        if (err = dsa_create_drive(&drvs[i])) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
        sprintf(url, DEFAULT_URL_FORMAT, nr);
        if (err = dsa_open_u(drvs[i], url)) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
        if (err = dsa_add_group_item(grp, drvs[i])) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
    }

    // Ask the user to enter the name of the file from which the data must be downloaded
    printf("\n");
    printf("Enter file name to download : ");
    scanf("%s", filename);

    // Download the mapping file into devices
    if (err = dsa_stage_mapping_download(grp, filename)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

// Error management
_error : {
    ebool open;
    if (dsa_is_valid_drive_group(grp))
        dsa_destroy(&grp);
    for (i = 0; i < axis_number; i++) {
        dsa_is_open(drvs[i], &open);
        if (open)
            dsa_close(drvs[i]);
        if (dsa_is_valid_drive(drvs[i]))
            dsa_destroy(&drvs[i]);
    }
    return err;
}
}

/*
 * Activate function.
 * The function ask first which are the concerned axes. The function then opens the devices
 * and create a group of devices.
 * The function then call the dsa_stage_mapping_activate function which activate the pre-downloaded mapping
 * All drives specified in mapping file must be present.
 */
int activate_mapping()
{
    int              axis_number; // The number of axes specified in mapping file
    int              nr;          // A help variable containing the axis number
    DSA_DRIVE       *drvs[64];    // An array containing all devices participating to activation
    DSA_DRIVE_GROUP *grp = NULL;  // A group containing all devices participating to download
    char             url[32];     // The url used by dsa_open_u function
    int              err;         // A variable containing the function call error
    int              i;           // A loop counter

    // Initialisation
    memset(drvs, 0, sizeof(drvs));

    // Ask the user to enter the number of axis participating to mapping
    printf("\n");
    printf("Enter number of axes participating to mapping : ");
    scanf("%d", &axis_number);

    // Create the group of device
    if (err = dsa_create_drive_group(&grp, axis_number)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    // Ask the drive number ,create and open the devices. Stored them in array and put them into device group
    for (i = 0; i < axis_number; i++) {
        printf("\tEnter %d. axis number : ", i);
        scanf("%d", &nr);
        drvs[i] = NULL;
        if (err = dsa_create_drive(&drvs[i])) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
        sprintf(url, DEFAULT_URL_FORMAT, nr);
        if (err = dsa_open_u(drvs[i], url)) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
        if (err = dsa_add_group_item(grp, drvs[i])) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
    }

    // Activate the pre-downloaded mapping
    if (err = dsa_stage_mapping_activate(grp)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

// Error management
_error : {
    ebool open;
    if (dsa_is_valid_drive_group(grp))
        dsa_destroy(&grp);
    for (i = 0; i < axis_number; i++) {
        dsa_is_open(drvs[i], &open);
        if (open)
            dsa_close(drvs[i]);
        if (dsa_is_valid_drive(drvs[i]))
            dsa_destroy(&drvs[i]);
    }
    return err;
}
}

/*
 * Deactivate function.
 * The function ask first which are the concerned axes. The function then opens the devices
 * and create a group of devices.
 * The function then call the dsa_stage_mapping_deactivate function which deactivate the pre-downloaded mapping
 * All drives referred by same mapping must be present.
 */
int deactivate_mapping()
{
    int              axis_number; // The number of axes specified in mapping file
    int              nr;          // A help variable containing the axis number
    DSA_DRIVE       *drvs[64];    // An array containing all devices participating to download
    DSA_DRIVE_GROUP *grp = NULL;  // A group containing all devices participating to download
    char             url[32];     // The url used by dsa_open_u function
    int              err;         // A variable containing the function call error
    int              i;           // A loop counter

    // Initialisation
    memset(drvs, 0, sizeof(drvs));

    // Ask the user to enter the number of axis participating to mapping
    printf("\n");
    printf("Enter number of axes participating to mapping : ");
    scanf("%d", &axis_number);

    // Create the group of device
    if (err = dsa_create_drive_group(&grp, axis_number)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    // Ask the drive number ,create and open the devices. Stored them in array and put them into device group
    for (i = 0; i < axis_number; i++) {
        printf("\tEnter %d. axis number : ", i);
        scanf("%d", &nr);
        drvs[i] = NULL;
        if (err = dsa_create_drive(&drvs[i])) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
        sprintf(url, DEFAULT_URL_FORMAT, nr);
        if (err = dsa_open_u(drvs[i], url)) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
        if (err = dsa_add_group_item(grp, drvs[i])) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
    }

    // Deactivate the pre-downloaded mapping
    if (err = dsa_stage_mapping_deactivate(grp)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

// Error management
_error : {
    ebool open;
    if (dsa_is_valid_drive_group(grp))
        dsa_destroy(&grp);
    for (i = 0; i < axis_number; i++) {
        dsa_is_open(drvs[i], &open);
        if (open)
            dsa_close(drvs[i]);
        if (dsa_is_valid_drive(drvs[i]))
            dsa_destroy(&drvs[i]);
    }
    return err;
}
}

/*
 * Check mapping state function.
 * The function ask first which are the concerned axes. The function then opens the devices
 * and create a group of devices.
 * The function then call the dsa_stage_mapping_get_activation function which check the mapping state of the drives
 */
int check_mapping()
{
    int              axis_number; // The number of axes which must be checked
    int              nr;          // A help variable containing the axis number
    DSA_DRIVE       *drvs[64];    // An array containing all devices participating to check
    DSA_DRIVE_GROUP *grp = NULL;  // A group containing all devices participating to check
    char             url[32];     // The url used by dsa_open_u function
    int              err;         // A variable containing the function call error
    int              i;           // A loop counter
    ebool            activation;  // A variable which will receive the state of the activation

    // Initialisation
    memset(drvs, 0, sizeof(drvs));

    // Ask the user to enter the number of axis which must be checked
    printf("\n");
    printf("Enter number of axes which must be checked : ");
    scanf("%d", &axis_number);

    // Create the group of device
    if (err = dsa_create_drive_group(&grp, axis_number)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    // Ask the drive number ,create and open the devices. Stored them in array and put them into device group
    for (i = 0; i < axis_number; i++) {
        printf("\tEnter %d. axis number : ", i);
        scanf("%d", &nr);
        drvs[i] = NULL;
        if (err = dsa_create_drive(&drvs[i])) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
        sprintf(url, DEFAULT_URL_FORMAT, nr);
        if (err = dsa_open_u(drvs[i], url)) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
        if (err = dsa_add_group_item(grp, drvs[i])) {
            DSA_EXT_DIAG(err, drvs[i]);
            goto _error;
        }
    }

    // Check the mapping state
    if (err = dsa_stage_mapping_get_activation(grp, &activation)) {
        DSA_EXT_DIAG(err, grp);
        goto _error;
    }

    printf("\n\tThe mapping is %s\n", activation ? "active" : "not active");

// Error management
_error : {
    ebool open;
    if (dsa_is_valid_drive_group(grp))
        dsa_destroy(&grp);
    for (i = 0; i < axis_number; i++) {
        dsa_is_open(drvs[i], &open);
        if (open)
            dsa_close(drvs[i]);
        if (dsa_is_valid_drive(drvs[i]))
            dsa_destroy(&drvs[i]);
    }
    return err;
}
}

/*
 *** Main entry point function.***
 */
int main(int argc, char *argv[])
{
    int  err;
    char choice;

    /* Display menu */
    for (;;) {
        do {
            fflush(stdin);
            printf("\n\n");
            printf("-----------------\n");
            printf("C mapping example\n");
            printf("-----------------\n");
            printf("\n\n\n");
            printf("Choose Action\n");
            printf("*************\n");
            printf("\tX: Exit\n");
            printf("\tA: Download mapping to drives\n");
            printf("\tB: Upload mapping from drives\n");
            printf("\tC: Activate mapping downloaded into the drives\n");
            printf("\tD: Deactivate mapping downloaded into the drives\n");
            printf("\tE: Check mapping activation state of the drives\n");

            printf("\n\t\tYour choice : ");
            fflush(stdout);
            scanf("%c", &choice);
        } while ((choice != 'A') && (choice != 'a') && (choice != 'B') && (choice != 'b') && (choice != 'C') && (choice != 'c') && (choice != 'D') &&
                 (choice != 'd') && (choice != 'E') && (choice != 'e') && (choice != 'X') && (choice != 'x'));
        printf("\n");

        switch (choice) {
        case 'a':
        case 'A':
            if (err = download_mapping()) {
                goto _error;
            }
            break;
        case 'b':
        case 'B':
            if (err = upload_mapping()) {
                goto _error;
            }
            break;
        case 'C':
        case 'c':
            if (err = activate_mapping()) {
                goto _error;
            }
            break;
        case 'D':
        case 'd':
            if (err = deactivate_mapping()) {
                goto _error;
            }
            break;
        case 'E':
        case 'e':
            if (err = check_mapping()) {
                goto _error;
            }
            break;
        case 'x':
        case 'X':
            return 0;
        }
    }

_error:
    if (err) {
        printf("Ended with error\n\n");
    }
    else
        printf("Ended with success\n\n");
    _getch();

    return err;
}
