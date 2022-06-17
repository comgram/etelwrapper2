/*
 * sample_5.c
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
    C example for download firmware into a DRIVE or a ULTIMET

    ETEL SA, 2012
*/

#include <conio.h>
#include <dsa40.h>
#include <etb40.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/* functions headers */
void ETB_CALLBACK my_print_fct(int block_nb, int total_nb_blocks, int current_block_current_downloaded_size, int current_block, void *user);

/*
 * This function will be called by EDI package each time 4096 bytes have been downloaded
 * This allows the application to have a feedback of the download process
 * Be carefull : This function will block execution of download !
 *				 and  should not call any EDI functions
 * @param block_nb the current downloaded block number.\n
 * - If the block is -1, it tells the application that the analysis is in progress\n
 * - If the block is 0, it tells the application that the erasing of the parameters and/or sequence blocks is in progress
 * @param total_nb_blocks the total number of blocks which will be downloaded
 * @param current_block_current_downloaded_size the already downloaded size of the current block
 * @param current_block_size the size of the current block
 * @param user the user parameter
 */
void ETB_CALLBACK my_print_fct(int block_nb, int total_nb_blocks, int current_block_current_downloaded_size, int current_block_size, void *user)
{
    static int block_idx = 1;

    if (block_nb == -1)
        printf("\tMD5 Analysis\n");
    else if (block_nb == 0)
        printf("\tErasing sequence and/or register blocksMD5 Analysis\n");
    else {
        int percent = 0;
        if (current_block_size != 0)
            percent = current_block_current_downloaded_size * 100 / current_block_size;
        printf("\tDownload BLOCK %4d (%3d/%3d) (%3d %%)%c", block_nb, block_idx++, total_nb_blocks, percent, (percent == 100) ? '\n' : '\r');
    }
}

static void display_download_result(int nb_downloaded_blocks)
{
    switch (nb_downloaded_blocks) {
    case -1:
        printf("Nothing has been downloaded. Device up-to-date\n");
        break;
    case 0:
        printf("Nothing has been downloaded. Compatible device is not found\n");
        break;
    default:
        printf("%d blocks have been downloaded\n", nb_downloaded_blocks);
        break;
    }
}

/*
 * This example show how to download a firmware into a AccurET number 0 through a UltimET
 * To run this example,
 * - copy unzip program (provided with EDI package) into a directory specified in PATH
 * - copy AccurET_Modular_400_600-(32)-REV2-3.18B.far into current directory
 */
int download_drive_thru_ultimet()
{
    ETB *etb = NULL; /*ETB object */
    int  err = 0;    /*Will contain the error*/
    int  err2;
    int  nb_downloaded_blocks;

    printf("\nDownload Drive through transnET\n");
    /* If a dsa object has already been opened, use following code instead of creating
     * and opening etb bus :
     * if (err = dsa_get_etb_bus(dsa, &etb))
     *    return err;
     */

    /* Creation of ETB object */
    if (err = etb_create_bus(&etb)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    /* Opening of bus */
    if (err = etb_open(etb, "ULTIMET", ETB_FLAG_BOOT_RUN, 0, 20000)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    /* Change mode of ULTIMET to Bridge */
    if (err = etb_change_boot_mode(etb, ETB_BOOT_MODE_BRIDGE)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    /* Download firmware AccurET_Modular_400_600-(32)-REV2-3.18B.far into axis number 0
     * If AXIS does not exist or is not a AccurET, function won't do anything
     * Function my_print_fct will be called each time 4 kbytes are downloaded
     * If, instead of my_print_fct, NULL is passed, no function will be called
     */
    if (err = etb_etcom_download_firmware_ex(etb, 0x01, 0, "AccurET_Modular_400_600-(32)-REV2-3.18B.far", TRUE, &nb_downloaded_blocks, my_print_fct, NULL)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    display_download_result(nb_downloaded_blocks);

_end:
    /* Change boot mode of ULTIMET into RUN mode
     */
    if (err2 = etb_change_boot_mode(etb, ETB_BOOT_MODE_RUN))
        DSA_EXT_DIAG(err2, NULL);

    /* Do not close and destroy bus if you use a dsa object */
    etb_close(etb, 0, 10000);
    etb_destroy_bus(&etb);
    return err;
}

/*
 * This example show how to download a firmware into a ULTIMET PCI
 * To run this example,
 * - copy unzip program (provided with EDI package) into a directory specified in PATH
 * - copy UltimET_PCI-(48)-REV1-3.18B.far into current directory
 */
int download_ultimet()
{
    ETB *etb = NULL; /*ETB object */
    int  err, err2;  /*Will contain the error*/
    int  nb_downloaded_blocks;

    printf("\nDownload Ultimet PCI\n");
    /* If a dsa object has already been opened, use following code instead of creating
     * and opening etb bus :
     * if (err = dsa_get_etb_bus(dsa, &etb))
     *    return err;
     */

    /* Creation of ETB object */
    if (err = etb_create_bus(&etb)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    /* Opening of bus
     * Opening with these flags assumes that ULTIMET functionnates
     * If ULTIMET has its firmware corrupted, open it with ETB_FLAGS_BOOT_DIRECT
     * and don't change its boot mode after
     */
    if (err = etb_open(etb, "ULTIMET", ETB_FLAG_BOOT_RUN, 0, 20000)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    /* Change mode of ULTIMET to Direct
     * Not necessary if ULTIMET has been opened with ETB_BOOT_FLAG_DIRECT
     */
    if (err = etb_change_boot_mode(etb, ETB_BOOT_MODE_DIRECT)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    /* Download firmware UltimET_PCI-(48)-REV1-3.18B.far into ULTIMET
     * Function my_print_fct will be called each time 4 kbytes are downloaded
     * If, instead of my_print_fct, NULL is passed, no function will be called
     */
    if (err =
            etb_etcom_download_firmware_ex(etb, 0x8000000000000000LL, 0, "UltimET_PCI-(48)-REV1-3.18B.far", TRUE, &nb_downloaded_blocks, my_print_fct, NULL)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    display_download_result(nb_downloaded_blocks);

_end:
    /* Change boot mode of ULTIMET into RUN mode
     */
    if (err2 = etb_change_boot_mode(etb, ETB_BOOT_MODE_RUN)) {
        DSA_EXT_DIAG(err2, NULL);
    }

    /* Do not close and destroy bus if you use a dsa object */
    etb_close(etb, 0, 10000);
    etb_destroy_bus(&etb);
    return err;
}

/*
 * This example show how to download a firmware into a ULTIMET TCPIP
 * To run this example,
 * - copy unzip program (provided with EDI package) into a directory specified in PATH
 * - copy UltimET_TCPIP-(49)-REV1-3.18B.far into current directory
 */
int download_ultimet_tcpip()
{
    ETB *etb = NULL; /*ETB object */
    int  err, err2;  /*Will contain the error*/
    int  nb_downloaded_blocks;

    printf("\nDownload Ultimet TCPIP\n");
    /* If a dsa object has already been opened, use following code instead of creating
     * and opening etb bus :
     * if (err = dsa_get_etb_bus(dsa, &etb))
     *    return err;
     */

    /* Creation of ETB object */
    if (err = etb_create_bus(&etb)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    /* Opening of bus
     * Opening with these flags assumes that ULTIMET functionnates
     * If ULTIMET has its firmware corrupted, open it with ETB_FLAGS_BOOT_DIRECT
     * and don't change its boot mode after
     */
    if (err = etb_open(etb, "etn://10.44.19.103:1129,T=-1", ETB_FLAG_BOOT_RUN, 0, 20000)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }
    /* Change mode of ULTIMET to Direct
     * Not necessary if ULTIMET has been opened with ETB_BOOT_FLAG_DIRECT
     */
    if (err = etb_change_boot_mode(etb, ETB_BOOT_MODE_DIRECT)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    /* Download firmware UltimET_PCI-(48)-REV1-3.18B.far into ULTIMET
     * Function my_print_fct will be called each time 4 kbytes are downloaded
     * If, instead of my_print_fct, NULL is passed, no function will be called
     */
    if (err = etb_etcom_download_firmware_ex(etb,
                                             0x8000000000000000LL,
                                             0,
                                             "UltimET_TCPIP-(49)-REV1-3.18B.far",
                                             TRUE,
                                             &nb_downloaded_blocks,
                                             my_print_fct,
                                             NULL)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    display_download_result(nb_downloaded_blocks);

_end:
    /* Change boot mode of ULTIMET into RUN mode
     */
    if (err2 = etb_change_boot_mode(etb, ETB_BOOT_MODE_RUN)) {
        DSA_EXT_DIAG(err2, NULL);
    }

    /* Do not close and destroy bus if you use a dsa object */
    etb_close(etb, 0, 10000);
    etb_destroy_bus(&etb);
    return err;
}

/*
 * This example show how to download a firmware into a drive 0 connected through USB. If the
 * drive's firmware is corrupted, a download key must be plugged on the drive
 * To run this example,
 * - copy unzip program (provided with EDI package) into a directory specified in PATH
 * - copy AccurET_Modular_400_600-(32)-REV2-3.18B.far into current directory
 */
int download_usb_drive()
{
    ETB *etb = NULL; /*ETB object */
    int  err;        /*Will contain the error*/
    int  nb_downloaded_blocks;

    printf("\nDownload Drive through USB\n");
    /* Creation of ETB object */
    if (err = etb_create_bus(&etb)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }
    /*
      Opening of bus in BRIDGE mode allowing corrupted drive to be downloaded.
     */
    if (err = etb_open(etb, "USB", ETB_FLAG_BOOT_BRIDGE, 0, 20000)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    /* Download firmware AccurET_Modular_400_600-(32)-REV2-3.18B.far into axis number 0
     * Function my_print_fct will be called each time 4 kbytes are downloaded
     * If, instead of my_print_fct, NULL is passed, no function will be called
     */
    if (err = etb_etcom_download_firmware_ex(etb, 0x01, 0, "AccurET_Modular_400_600-(32)-REV2-3.18B.far", TRUE, &nb_downloaded_blocks, my_print_fct, NULL)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    /* Change boot mode of DRIVE into RUN mode
     */
    if (err = etb_change_boot_mode(etb, ETB_BOOT_MODE_RUN)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    display_download_result(nb_downloaded_blocks);

_end:
    etb_close(etb, 0, 10000);
    etb_destroy_bus(&etb);
    return err;
}

/*
 * This example show how to download a firmware into an AccurET 0 connected through TCPIP. If the
 * drive's firmware is corrupted, the download must be done using USB
 * To run this example,
 * - copy unzip program (provided with EDI package) into a directory specified in PATH
 * - copy AccurET_Modular_400_600-(32)-REV2-3.18B.far into current directory
 */
int download_tcpip_drive()
{
    ETB *etb = NULL; /*ETB object */
    int  err;        /*Will contain the error*/
    int  nb_downloaded_blocks;

    printf("\nDownload Drive through TCPIP\n");
    /* Creation of ETB object */
    if (err = etb_create_bus(&etb)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }
    /*
      Opening of bus in DIRECT mode
     */
    if (err = etb_open(etb, "etn://10.44.19.112:1129,T=-1", ETB_FLAG_BOOT_RUN, 0, 20000)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    if (err = etb_change_boot_mode(etb, ETB_BOOT_MODE_DIRECT)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }
    /* Download firmware AccurET_Modular_400_600-(32)-REV2-3.18B.far into axis number 0
     * Function my_print_fct will be called each time 4 kbytes are downloaded
     * If, instead of my_print_fct, NULL is passed, no function will be called
     */
    if (err = etb_etcom_download_firmware_ex(etb, 0x01, 0, "AccurET_Modular_400_600-(32)-REV2-3.18B.far", TRUE, &nb_downloaded_blocks, my_print_fct, NULL)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    /* Change boot mode of DRIVE into RUN mode
     */
    if (err = etb_change_boot_mode(etb, ETB_BOOT_MODE_RUN)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    display_download_result(nb_downloaded_blocks);

_end:
    etb_close(etb, 0, 10000);
    etb_destroy_bus(&etb);
    return err;
}

/*
 * This example show how to retreive information about a firmware
 * To run this example,
 * - copy unzip program (provided with EDI package) into a directory specified in PATH
 * - copy AccurET_Modular_400_600-(32)-REV2-3.18B.far into current directory
 */
int get_fw_info()
{
    int         err;  /*Will contain the error*/
    ETB_FW_INFO info; /*Will contain the returned firmware info */

    printf("Getting firmware info\n");

    /* get info about firmware AccurET_Modular_400_600-(32)-REV2-3.18B.far
     */
    if (err = etb_get_firmware_info("AccurET_Modular_400_600-(32)-REV2-3.18B.far", &info)) {
        DSA_EXT_DIAG(err, NULL);
        goto _end;
    }

    printf("Firmware info:\n");
    printf("\t title %s\n", info.title);
    printf("\t version %s\n", info.version);

_end:
    return err;
}

int main(void)
{
    int err;
#if 0
	/* Will get information about firmware */
	if (err = get_fw_info())
		goto _error;
	/* Will download firmware into a ultimet PCI*/
	if (err = download_ultimet())
		goto _error;
	/* Will download firmware into a ultimet TCPIP*/
	if (err = download_ultimet_tcpip())
		goto _error;
	/* Will download firmware into a drive thru ultimet */
	if (err = download_drive_thru_ultimet())
		goto _error;
#endif
    /* Will download firmware into a drive connected through TCPIP*/
    if (err = download_tcpip_drive())
        goto _error;
    /* Will download firmware into a drive connected through USB*/
    if (err = download_usb_drive())
        goto _error;
    printf("Ended successful\n");
    _getch();
    return;

_error:
    printf("Ended with error %d\n", err);
    _getch();
}
