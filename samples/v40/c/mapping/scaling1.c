/*
 * scaling1.c
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
 * This sample program show how to 
 *	-	download scaling file into a drive
 *	-	activate scaling of a drive
 *	-	deactivate scaling of a drive
 *	-	check scaling activity of a drive
 */
/*
 * To run this demo without modification, you must have:
 *  - an AccurET connected to your PC, either by USB, TCP/IP or through an UltimET
 * Just change the URL to fit the connection
 * There is 1 scaling file provided with the example:
 * - scaling.txt which is a correct scaling file downloadable into an AccurET number 0.
 */

/*** Initialisation steps ***/

/*** standard libraries ***/
#include <stdio.h>  /* the standard I/O library              */
#include <stdlib.h>	/* standard general purpose library      */
#include <ctype.h>  /* standard character conversion library */
#include <process.h>/* standard multithreading library       */
#include <math.h>   /* standard math interface; used for random numbers here*/
#include <conio.h>  /* standard library for getch()*/

/*** ETEL libraries ***/
#include <dmd40.h>
#include <dsa40.h>  /* EDI high level library version 3.0x.  */

/* The default url */
#define DEFAULT_URL_FORMAT "etb:ultimet:%d"								//Transnet url
//#define DEFAULT_URL_FORMAT "etb:USB:%%d"									//USB url
//#define DEFAULT_URL_FORMAT "etb:etn://172.22.10.101:1129:%%d"				//TCP-IP url


/*
 * Download function.
 * The function ask the scaling file name from which scaling datas must be read.
 * It then import the scaling file into memory by calling the dex_import. This is done
 * first to know which is the concerned axis. The function then opens the device.
 * The function ask in which pre-processing mode the datas must be downloaded.
 * The function then call the dsa_scale_mapping_download function which downloads the 
 * datas into the device.
 * The drive specified in scaling file must be present.
 * There is 1 scaling file provided with the example:
 * - scaling.txt which is a correct scaling file downloadable into an AccurET number 0.
 */
int download_scaling() 
{
	char filename[512];						//The file name entered by the user
	int nr;									//A help variable containing the axis number
	DSA_DRIVE *drv = NULL;					//The drive participating to download
	char url[32];							//The url used by dsa_open_u function
	int err;								//A variable containing the function call error
	int mode;								//Pre-processing mode

	//Ask the drive number ,create and open the device.
	printf("\nEnter axis number : ");
	scanf("%d", &nr);
	if (err = dsa_create_drive(&drv)) {
		DSA_EXT_DIAG(err, drv);
		goto _error;
	}
	sprintf(url, DEFAULT_URL_FORMAT, nr);
	if (err = dsa_open_u(drv, url)) {
		DSA_EXT_DIAG(err, drv);
		goto _error;
	}

	//Ask the user to enter the name of the file from which the data must be downloaded
	printf("\n");
	printf("Enter file name to download : ");
	scanf("%s", filename);

	//Ask the download pre-processing mode
	do {
		printf("\nEnter how the datas must be pre-processed :\n");
		printf("\t0 : No pre-processing\n");
		printf("\t1 : Add 0 at the ends of scaling area\n");
		printf("\t2 : Spline the scaling datas\n");
		printf("\t3 : Spline the scaling datas and add 0 at the ends of scaling area\n");
		printf("\tMode: ");
		scanf("%d", &mode);
	}
	while ((mode < 0) && (mode > 3));

	//Download the scaling data into the device
	if (err = dsa_scale_mapping_download(drv, filename, mode)) {
		DSA_EXT_DIAG(err, drv);
		goto _error;
	}
	
//Error management
_error:
	{
		ebool open;
		dsa_is_open(drv, &open);
		if (open)
			dsa_close(drv);
		if (dsa_is_valid_drive(drv))
			dsa_destroy(&drv);
		return err;
	}
}

/*
 * Activate function.
 * The function ask first which is the concerned axis. The function then opens the device
 * The function ask which kind of activation to do.
 * The function then call the dsa_scale_mapping_activate function which activate the pre-downloaded scaling
 * The drive specified in scaling file must be present.
 */
int activate_scaling() 
{
	int nr;									//A help variable containing the axis number
	DSA_DRIVE *drv = NULL;					//The drive participating to activation
	char url[32];							//The url used by dsa_open_u function
	int err;								//A variable containing the function call error
	char activation;						//Activation type of the scaling

	//Ask the drive number ,create and open the device.
	printf("\nEnter axis number : ");
	scanf("%d", &nr);
	if (err = dsa_create_drive(&drv)) {
		DSA_EXT_DIAG(err, drv);
		goto _error;
	}
	sprintf(url, DEFAULT_URL_FORMAT, nr);
	if (err = dsa_open_u(drv, url)) {
		DSA_EXT_DIAG(err, drv);
		goto _error;
	}

	//Ask the activation type
	do {
		printf("\nEnter type of activation :\n");
		printf("\tL : Linear\n");
		printf("\tC : Cyclic\n");
		printf("Activation : ");
		fflush(stdin);
		scanf("%c", &activation);
	}
	while ((activation != 'C') && (activation != 'c') &&
		   (activation != 'L') && (activation != 'l'));

	switch (activation) {
		case 'c':
		case 'C':
			//Activate the pre-downloaded scaling cyclicly
			if (err = dsa_scale_mapping_activate(drv, DSA_SCALE_MAPPING_CYCLIC_ACTIVATION)) {
				DSA_EXT_DIAG(err, drv);
				goto _error;
			}
			break;
		case 'l':
		case 'L':
			//Activate the pre-downloaded scaling linearly
			if (err = dsa_scale_mapping_activate(drv, DSA_SCALE_MAPPING_LINEAR_ACTIVATION)) {
				DSA_EXT_DIAG(err, drv);
				goto _error;
			}
			break;
	}

//Error management
_error:
	{
		ebool open;
		dsa_is_open(drv, &open);
		if (open)
			dsa_close(drv);
		if (dsa_is_valid_drive(drv))
			dsa_destroy(&drv);
		return err;
	}
}

/*
 * Deactivate function.
 * The function ask first which is the concerned axis. The function then opens the device
 * The function then call the dsa_scale_mapping_deactivate function which deactivate the scaling
 */
int deactivate_scaling() 
{
	int nr;									//A help variable containing the axis number
	DSA_DRIVE *drv = NULL;					//The drive participating to deactivation
	char url[32];							//The url used by dsa_open_u function
	int err;								//A variable containing the function call error

	//Ask the drive number ,create and open the device.
	printf("\nEnter axis number : ");
	scanf("%d", &nr);
	if (err = dsa_create_drive(&drv)) {
		DSA_EXT_DIAG(err, drv);
		goto _error;
	}
	sprintf(url, DEFAULT_URL_FORMAT, nr);
	if (err = dsa_open_u(drv, url)) {
		DSA_EXT_DIAG(err, drv);
		goto _error;
	}

	//Deactivate the scaling
	if (err = dsa_scale_mapping_deactivate(drv)) {
		DSA_EXT_DIAG(err, drv);
		goto _error;
	}

//Error management
_error:
	{
		ebool open;
		dsa_is_open(drv, &open);
		if (open)
			dsa_close(drv);
		if (dsa_is_valid_drive(drv))
			dsa_destroy(&drv);
		return err;
	}
}

/*
 * Check scaling state function.
 * The function ask first which is the concerned axis. The function then opens the device
 * The function then call the dsa_scale_mapping_get_activation function which check the scale state of the drive
 */
int check_scaling() 
{
	int nr;									//A help variable containing the axis number
	DSA_DRIVE *drv = NULL;					//The drive participating to check
	char url[32];							//The url used by dsa_open_u function
	int err;								//A variable containing the function call error
	ebool activation;						//A variable which will receive the state of the activation


	//Ask the drive number ,create and open the device.
	printf("\nEnter axis number : ");
	scanf("%d", &nr);
	if (err = dsa_create_drive(&drv)) {
		DSA_EXT_DIAG(err, drv);
		goto _error;
	}
	sprintf(url, DEFAULT_URL_FORMAT, nr);
	if (err = dsa_open_u(drv, url)) {
		DSA_EXT_DIAG(err, drv);
		goto _error;
	}

	//Check the scaling state
	if (err = dsa_scale_mapping_get_activation(drv, &activation)) {
		DSA_EXT_DIAG(err, drv);
		goto _error;
	}

	printf("\n\tThe scaling is %s\n", activation ? "active" : "not active");


//Error management
_error:
	{
		ebool open;
		dsa_is_open(drv, &open);
		if (open)
			dsa_close(drv);
		if (dsa_is_valid_drive(drv))
			dsa_destroy(&drv);
		return err;
	}
}

/*
 *** Main entry point function.***
 */
int main(int argc, char *argv[])
{
	int err;
	char choice;

	/* Display menu */
	for (;;) {
		do {
			printf("\n\n");
			printf("-----------------\n");
			printf("C scaling example\n");
			printf("-----------------\n");
			printf("\n\n\n");
			printf("Choose Action\n");
			printf("*************\n");
			printf("\tX: Exit\n");
			printf("\tA: Download scaling to drive\n");
			printf("\tB: Activate scaling downloaded into the drive\n");
			printf("\tC: Deactivate scaling downloaded into the drive\n");
			printf("\tD: Check scaling activation state of the drive\n");

			printf("\n\t\tYour choice : ");
			fflush(stdout);
			fflush(stdin);
			scanf("%c", &choice);
		}
		while ((choice != 'A') && (choice != 'a') &&
			   (choice != 'B') && (choice != 'b') &&
			   (choice != 'C') && (choice != 'c') &&
			   (choice != 'D') && (choice != 'd') &&
			   (choice != 'X') && (choice != 'x'));
		printf("\n");

		switch(choice) {
			case 'a':
			case 'A': 
				if (err = download_scaling()) {
					goto _error;
				}
				break;
			case 'B':
			case 'b': 
				if (err = activate_scaling()) {
					goto _error;
				}
				break;
			case 'C':
			case 'c': 
				if (err = deactivate_scaling()) {
					goto _error;
				}
				break;
			case 'D':
			case 'd': 
				if (err = check_scaling()) {
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



