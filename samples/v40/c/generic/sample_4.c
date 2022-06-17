/*
 * sample_4.c
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
    C example for upload / download of parameters in a ULTIMET.
    The functions defined in this example are also valid for AccurET drives

    ETEL SA, 30.05.02
*/

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <dmd40.h>
#include <dsa40.h>
#include <tra40.h>

// global variables
int line_count = 0;

// functions headers
int              download_parameters(DSA_DEVICE_BASE *dev, TRA_LINE_READER *reader, void *stream, int nb_typs, byte typs[]);
int TRA_CALLBACK file_line_reader(TRA *tra, char *buffer, int max, void *stream);
int              upload_parameters(DSA_DEVICE_BASE *dev, TRA_ETCOM_LINE_WRITER *writer, void *stream, int nb_typs, byte typs[]);
int TRA_CALLBACK file_line_writer(TRA *tra, const char *buffer, void *stream, ETB_ETCOM *rec);
int              wait_present(DSA_DEVICE_BASE *dev);
int              save_in_flash(DSA_DEVICE_BASE *dev);
int              reset(DSA_DEVICE_BASE *dev);

int main(int argc, char *argv[])
{

    FILE *in_file;

    // Definition of ULTIMET object
    DSA_MASTER *ultimet = NULL;
    DSA_DRIVE  *drv     = NULL;
    byte        typs[8];
    int         err;

    // Create one ultimet object
    if (err = dsa_create_master(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    if (err = dsa_create_drive(&drv)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    // Create connections.
    // Connections with axis x
    // Structure of the second parameter "etb:ULTIMET:n":
    //		etb		: ETEL protocol
    //		ULTIMET  : connexion port
    //		n		: axis number (0 to 62 for drives, * for ULTIMET)

    printf("Opening of the communication...\n");
    // Connections with the ultimet
    if (err = dsa_open_u(ultimet, "etb:ULTIMET:*")) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    if (err = dsa_open_u(drv, "etb:ULTIMET:0")) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    // upload the parameters from the Ultimet
    printf("Upload of the K(x),C(x) parameters...\n");
    typs[0] = DMD_TYP_PPK_INT32;
    typs[1] = DMD_TYP_PPK_INT64;
    typs[2] = DMD_TYP_PPK_FLOAT32;
    typs[3] = DMD_TYP_PPK_FLOAT64;
    typs[4] = DMD_TYP_COMMON_INT32;
    typs[5] = DMD_TYP_COMMON_INT64;
    typs[6] = DMD_TYP_COMMON_FLOAT32;
    typs[7] = DMD_TYP_COMMON_FLOAT64;

    // open file
    if ((in_file = fopen("trial.par", "w")) == NULL) {
        printf("ERROR at file %s, line %d file not found\n", __FILE__, __LINE__);
        goto _error;
    }
    // upload sequence
    line_count = 0;

    if (err = upload_parameters(drv, file_line_writer, in_file, 8, typs))
        goto _error;

    // close file
    fclose(in_file);

    // download the test paramters in the ultimet
    printf("Download of the K(x) and C(x) parameters...\n");
    // open file
    if ((in_file = fopen("trial.par", "r")) == NULL) {
        printf("ERROR at file %s, line %d file not found\n", __FILE__, __LINE__);
        goto _error;
    }

    // download parameters
    line_count = 0;
    if (err = download_parameters(drv, file_line_reader, in_file, 8, typs))
        goto _error;

    // close file
    fclose(in_file);

    // save sequence and parameters (save in the flash memory)
    printf("Save all...\n");
    save_in_flash(drv);
    printf("Reset...\n");
    if (err = dsa_reset(drv)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    // Close all connections.
    if (err = dsa_close(ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    if (err = dsa_close(drv)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    // We should then destroy the drive and ultimet objects
    if (err = dsa_destroy(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        goto _error;
    }

    if (err = dsa_destroy(&drv)) {
        DSA_EXT_DIAG(err, drv);
        goto _error;
    }

    printf("\nEnd of the program\n\n");
    _getch();
    return 0;

_error:
    if (dsa_is_valid_drive(drv)) {

        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(drv, &open);
        if (open) {

            /* Stop all movements. */
            dsa_quick_stop_s(drv, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);

            /* When the motor has stopped, a power off is done. */
            dsa_wait_movement_s(drv, 60000);
            dsa_power_off_s(drv, 10000);

            /* Close the connection */
            dsa_close(drv);
        }

        /* And finally, release all resources to the OS. */
        dsa_destroy(&drv);
    }

    /* Same operations for the ULTIMET */
    if (dsa_is_valid_master(ultimet)) {

        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(ultimet, &open);
        if (open) {

            /* Close the connection */
            dsa_close(ultimet);
        }

        /* And finally, release all resources to the OS. */
        dsa_destroy(&ultimet);
    }
    _getch();
    return err;
}

/*
 * The funtion that the traductor call to read a line.
 * @return zero if success, not zero if end of file
 */

int TRA_CALLBACK file_line_reader(TRA *tra, char *buffer, int max, void *stream)
{
    FILE *in_file = (FILE *)stream;
    if (!fgets(buffer, max, in_file))
        return -1;
    line_count++;
    return 0;
}

/*
 * download parameters into the drive
 * @param dev can be a device or a device group
 * @param reader a function call for read line
 * @return error code or zero on success
 */
int download_parameters(DSA_DEVICE_BASE *dev, TRA_LINE_READER *reader, void *stream, int nb_typs, byte typs[])
{
    eint64    mask = 0;
    ETB      *etb  = NULL;
    ETB_PORT *port = NULL;
    TRA      *tra  = NULL;
    int       err;

    /* check for drive or groups */
    if (dsa_is_valid_device_group(dev)) {
        /* the dev parameter is a group */
        DSA_DEVICE_BASE *dsa  = NULL;
        int              size = 0;
        int              i;
        /* extract group mask */
        if (err = dsa_get_group_item(dev, 0, &dsa)) {
            DSA_EXT_DIAG(err, dev);
            return err;
        }
        if (!dsa_is_valid_device(dsa)) {
            printf("ERROR at file %s, line %d %s\n", __FILE__, __LINE__, dsa_translate_edi_error(DSA_EBADPARAM));
            return DSA_EBADPARAM;
        }
        if (err = dsa_get_etb_bus(dsa, &etb)) {
            DSA_EXT_DIAG(err, dsa);
            return err;
        }
        if (err = dsa_get_group_size(dev, &size)) {
            DSA_EXT_DIAG(err, dev);
            return err;
        }
        for (i = 0; i < size; i++) {
            ETB *bus = NULL;
            int  axis;
            if (err = dsa_get_group_item(dev, i, &dsa)) {
                DSA_EXT_DIAG(err, dev);
                return err;
            }
            if (err = dsa_get_etb_bus(dsa, &bus)) {
                DSA_EXT_DIAG(err, dsa);
                return err;
            }
            if (bus != etb) {
                printf("ERROR at file %s, line %d %s\n", __FILE__, __LINE__, dsa_translate_edi_error(DSA_EBADPARAM));
                return DSA_EBADPARAM;
            }
            if (err = dsa_etcom_get_etb_axis(dsa, &axis)) {
                DSA_EXT_DIAG(err, dsa);
                return err;
            }
            mask |= 1LL << axis;
        }
    }
    else {
        /* the dev parameter is a device */
        int axis;
        if (err = dsa_get_etb_bus(dev, &etb)) {
            DSA_EXT_DIAG(err, dev);
            return err;
        }
        if (err = dsa_etcom_get_etb_axis(dev, &axis)) {
            DSA_EXT_DIAG(err, dev);
            return err;
        }
        mask = 1LL << axis;
    }

    /* create a etb port */
    if (err = etb_create_port(&port, etb)) {
        DSA_EXT_DIAG(err, NULL);
        return err;
    }

    /* create traductor */
    if (err = tra_create_register_traductor_e(&tra, port, TRA_FLAG_DISABLE_CHECKS)) {
        DSA_EXT_DIAG(err, NULL);
        return err;
    }

    /* download stream */
    if (err = tra_etcom_download_register_stream_e(tra, mask, nb_typs, typs, reader, stream)) {
        DSA_EXT_DIAG(err, NULL);
        return err;
    }

    /* destroy traductor */
    tra_destroy(&tra);

    /* destroy etb port */
    if (err = etb_destroy_port(&port)) {
        DSA_EXT_DIAG(err, NULL);
        return err;
    }

    /* success */
    return 0;
}

/*
 * The funtion that the traductor call to write a line.
 * @return zero if success, not zero if end of file
 */
int TRA_CALLBACK file_line_writer(TRA *tra, const char *buffer, void *stream, ETB_ETCOM *rec)
{
    FILE *in_file = (FILE *)stream;
    fputs(buffer, in_file);
    fputs("\n", in_file);
    line_count++;
    return 0;
}

/*
 * upload a set of parameters from the drive
 * @param dev can be a device or a device group
 * @param writer a function call for write line
 * @return error code or zero on success
 */
int upload_parameters(DSA_DEVICE_BASE *dev, TRA_ETCOM_LINE_WRITER *writer, void *stream, int nb_typs, byte typs[])
{
    eint64    mask = 0;
    ETB      *etb  = NULL;
    ETB_PORT *port = NULL;
    TRA      *tra  = NULL;
    int       err;

    /* check for drive or groups */
    if (dsa_is_valid_device_group(dev)) {
        /* the dev parameter is a group */
        DSA_DEVICE_BASE *dsa  = NULL;
        int              size = 0;
        int              i;
        /* extract group mask */
        if (err = dsa_get_group_item(dev, 0, &dsa)) {
            DSA_EXT_DIAG(err, dev);
            return err;
        }
        if (!dsa_is_valid_device(dsa)) {
            printf("ERROR at file %s, line %d %s\n", __FILE__, __LINE__, dsa_translate_edi_error(DSA_EBADPARAM));
            return DSA_EBADPARAM;
        }
        if (err = dsa_get_etb_bus(dsa, &etb)) {
            DSA_EXT_DIAG(err, dsa);
            return err;
        }
        if (err = dsa_get_group_size(dev, &size)) {
            DSA_EXT_DIAG(err, dev);
            return err;
        }
        for (i = 0; i < size; i++) {
            ETB *bus = NULL;
            int  axis;
            if (err = dsa_get_group_item(dev, i, &dsa)) {
                DSA_EXT_DIAG(err, dev);
                return err;
            }
            if (err = dsa_get_etb_bus(dsa, &bus)) {
                DSA_EXT_DIAG(err, dsa);
                return err;
            }
            if (bus != etb) {
                printf("ERROR at file %s, line %d %s\n", __FILE__, __LINE__, dsa_translate_edi_error(DSA_EBADPARAM));
                return DSA_EBADPARAM;
            }
            if (err = dsa_etcom_get_etb_axis(dsa, &axis)) {
                DSA_EXT_DIAG(err, dsa);
                return err;
            }
            mask |= 1LL << axis;
        }
    }
    else {
        /* the dev parameter is a device */
        int axis;
        if (err = dsa_get_etb_bus(dev, &etb)) {
            DSA_EXT_DIAG(err, dev);
            return err;
        }
        if (err = dsa_etcom_get_etb_axis(dev, &axis)) {
            DSA_EXT_DIAG(err, dev);
            return err;
        }
        mask = 1LL << axis;
    }

    /* create a etb port */
    if (err = etb_create_port(&port, etb)) {
        DSA_EXT_DIAG(err, NULL);
        return err;
    }

    /* create traductor */
    if (err = tra_create_register_traductor_e(&tra, port, TRA_FLAG_ALL_SUBINDEXES | TRA_FLAG_GROUP_SUBINDEXES)) {
        DSA_EXT_DIAG(err, NULL);
        return err;
    }

    /* download stream */
    if (err = tra_etcom_upload_register_stream_e(tra, mask, nb_typs, typs, 0, -1, -1, writer, stream)) {
        DSA_EXT_DIAG(err, NULL);
        return err;
    }

    /* destroy traductor */
    tra_destroy(&tra);

    /* destroy etb port */
    if (err = etb_destroy_port(&port)) {
        DSA_EXT_DIAG(err, NULL);
        return err;
    }

    /* success */
    return 0;
}

/*
 * wait if a device or a device group is not present
 * @param dev the device or the device group
 * @return error code or zero on success
 */
int wait_present(DSA_DEVICE_BASE *dev)
{
    int err;

    /* setup status masks */
    DSA_STATUS status_mask      = {sizeof(DSA_STATUS)};
    DSA_STATUS status_value     = {sizeof(DSA_STATUS)};
    status_mask.sw.sw1.present  = 1;
    status_value.sw.sw1.present = 1;

    /* check for device or group */
    if (dsa_is_valid_device_group(dev)) {
        /* dev is a group */
        int size;
        int i;
        /* for each device in the group */
        if (err = dsa_get_group_size(dev, &size)) {
            DSA_EXT_DIAG(err, dev);
            return err;
        }
        for (i = 0; i < size; i++) {
            DSA_DEVICE_BASE *dsa = NULL;
            if (err = dsa_get_group_item(dev, i, &dsa)) {
                DSA_EXT_DIAG(err, dev);
                return err;
            }
            if (!dsa_is_valid_device(dsa)) {
                printf("ERROR at file %s, line %d %s\n", __FILE__, __LINE__, dsa_translate_edi_error(DSA_EBADPARAM));
                return DSA_EBADPARAM;
            }
            /* wait that this device is present */
            if (err = dsa_wait_status_equal_s(dsa, &status_mask, &status_value, NULL, 20000)) {
                DSA_EXT_DIAG(err, dev);
                return err;
            }
        }
    }
    else if (dsa_is_valid_device(dev)) {
        /* dev is a device - wait on it */
        if (err = dsa_wait_status_equal_s(dev, &status_mask, &status_value, NULL, 20000)) {
            DSA_EXT_DIAG(err, dev);
            return err;
        }
    }
    else {
        /* dev is not valid */
        printf("ERROR at file %s, line %d %s\n", __FILE__, __LINE__, dsa_translate_edi_error(DSA_EBADPARAM));
        return DSA_EBADPARAM;
    }

    /* success */
    Sleep(1000);
    return 0;
}

/*
 * save parameters and/or sequence into the flash
 * @param dev the device or the device group
 * @param sequence TRUE if we want to save the sequence
 * @param parameters TRUE if we want to save parameters
 * @return error code or zero on success
 */
int save_in_flash(DSA_DEVICE_BASE *dev)
{
    int err;

    /* send the save command */
    if (err = dsa_save_parameters_s(dev, DSA_PARAM_SAVE_K_C_E_X_PARAMS, 10000)) {
        DSA_EXT_DIAG(err, dev);
        return err;
    }

    /* after a save, device is not ready - wait */
    if (err = wait_present(dev))
        return err;

    /* success */
    return 0;
}

/*
 * reset the device or the device group (software reset)
 * @param dev the device or the device group
 * @return error code or zero on success
 */
int reset(DSA_DEVICE_BASE *dev)
{
    int err;

    /* send reset command */
    if (err = dsa_execute_command_d_s(dev, DMD_CMD_RESET_DRIVE, DMD_TYP_IMMEDIATE, 255, 0, 0, 10000)) {
        DSA_EXT_DIAG(err, dev);
        return err;
    }

    /* wait that the device is ready */
    if (err = wait_present(dev))
        return err;

    /* success */
    return 0;
}
