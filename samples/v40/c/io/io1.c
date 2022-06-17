/*
 * io1.c
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
 * This sample program show how to set and get io from UltimET.
 */

/*
 * To run this demo without modification, you must have:
 *  - an UltimET PCI board plugged into your PC or
 *  - an UltimET TCP/IP connected to the network
 */

#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <dsa40.h>

/*
 * Display a text and ask a user to enter an integer value
 */
int get_integer(char *text, int *value)
{
    char str[64];

    printf("\n\t%s :", text);
    fflush(stdout);
    scanf("%s", str);
    *value = atoi(str);
    return 0;
}

/*
 * Display a text and ask a user to enter an integer as hexadecimal value
 */
int get_dword(char *text, dword *value)
{
    char str[64];

    printf("\n\t%s :", text);
    fflush(stdout);
    scanf("%s", str);
    *value = strtol(str, NULL, 16);
    return 0;
}

/*
 * Display a text and ask a user to enter a ebool value
 */
int get_bool(char *text, ebool *value)
{
    char str[64];

    printf("\n\t%s :", text);
    fflush(stdout);
    scanf("%s", str);
    *value = atoi(str);
    return 0;
}

/*
 * Display a text and ask a user to enter a float value
 */
int get_float(char *text, float *value)
{
    char str[64];

    printf("\n\t%s :", text);
    fflush(stdout);
    scanf("%s", str);
    *value = (float)atof(str);
    return 0;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * setting an external digital output
 */
int set_external_digital_output(DSA_MASTER *ultimET)
{
    int err;
    int val = 0;

    if (err = get_integer("Set External Digital Output, Output number", &val)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_set_digital_output_s(ultimET, val, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nSet External Digital Output Number:%d successful\n", val);
        _getch();
    }
    else {
        printf("\nSet External Digital Output Number:%d error %d\n", val, err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * resetting an external digital output
 */
int reset_external_digital_output(DSA_MASTER *ultimET)
{
    int err;
    int val = 0;

    if (err = get_integer("Reset External Digital Output, Output number", &val)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_reset_digital_output_s(ultimET, val, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nReset External Digital Output Nunmber:%d successful\n", val);
        _getch();
    }
    else {
        printf("\nReset External Digital Output Number:%d error %d\n", val, err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * applying a mask to an external digital output
 */
int apply_mask_external_digital_output(DSA_MASTER *ultimET)
{
    int err;
    int val1 = 0;
    int val2 = 0;
    int val3 = 0;

    if (err = get_integer("Apply Mask External Digital Output, First output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_integer("Apply Mask External Digital Output, Number bits", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_dword("Apply Mask External Digital Output, Hex Value", &val3)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_apply_mask_digital_output_s(ultimET, val1, val2, val3, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nApply Mask External Digital Output FirstOutput:%d NbBits:%d Value:0x%xsuccessful\n", val1, val2, val3);
        _getch();
    }
    else {
        printf("\nApply Mask External Digital Output FirstOutput:%d NbBits:%d Value:0x%x error %d\n", val1, val2, val3, err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * setting a local digital output
 */
int set_local_digital_output(DSA_MASTER *ultimET)
{
    int err;
    int val = 0;

    if (err = get_integer("Set Local Digital Output, Output number", &val)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_localIO_set_digital_output_s(ultimET, val, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nSet Local Digital Output Number:%d successful\n", val);
        _getch();
    }
    else {
        printf("\nSet Local Digital Output Number:%d error %d\n", val, err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * resetting a local digital output
 */
int reset_local_digital_output(DSA_MASTER *ultimET)
{
    int err;
    int val = 0;

    if (err = get_integer("Reset Local Digital Output, Output number", &val)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_localIO_reset_digital_output_s(ultimET, val, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nReset Local Digital Output Number:%d successful\n", val);
        _getch();
    }
    else {
        printf("\nReset Local Digital Output Number:%d error %d\n", val, err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * applying a mask to a local digital output
 */
int apply_mask_local_digital_output(DSA_MASTER *ultimET)
{
    int err;
    int val1 = 0;
    int val2 = 0;
    int val3 = 0;

    if (err = get_integer("Apply Mask Local Digital Output, First output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_integer("Apply Mask Local Digital Output, Number bits", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_dword("Apply Mask Local Digital Output, Hex Value", &val3)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_localIO_apply_mask_digital_output_s(ultimET, val1, val2, val3, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nApply Mask Local Digital Output FirstOutput:%d NbBits:%d successful\n", val1, val2);
        _getch();
    }
    else {
        printf("\nApply Mask Local Digital Output FirstOutput:%d NbBits:%d error %d\n", val1, val2, err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting an external digital output
 */
int get_external_digital_output(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    ebool val2;
    dword get_val = 0;

    if (err = get_integer("Get External Digital Output, Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get External Digital Output, Fast Command", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_digital_output_s(ultimET, val1, &get_val, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tExternal Digital Output -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting an external digital output
 */
int get_external_digital_output_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1    = 0;
    ebool val2    = FALSE;
    dword get_val = 0;

    if (err = get_integer("Get External Digital Output State, Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get External Digital Output State, Fast Command", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_digital_output_state_s(ultimET, val1, &get_val, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tExternal Digital Output State -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting a masked external digital output
 */
int get_masked_external_digital_output(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    int   val2;
    ebool val3;
    dword get_val = 0;

    if (err = get_integer("Get Masked External Digital Output, First Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_integer("Get Masked External Digital Output, Number bits", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get Masked External Digital Output, Fast Command", &val3)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_masked_digital_output_s(ultimET, val1, val2, &get_val, val3, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tMasked External Digital Output -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting a masked external digital output
 */
int get_masked_external_digital_output_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    int   val2;
    ebool val3;
    dword get_val = 0;

    if (err = get_integer("Get Masked External Digital Output State, First Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_integer("Get Masked External Digital Output State, Number bits", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get Masked External Digital Output State, Fast Command", &val3)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_masked_digital_output_state_s(ultimET, val1, val2, &get_val, val3, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tMasked External Digital Output State -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting a masked external digital input
 */
int get_masked_external_digital_input_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    int   val2;
    ebool val3;
    dword get_val = 0;

    if (err = get_integer("Get Masked External Digital Input State, First Input number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_integer("Get Masked External Digital Input State, Number bits", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get Masked External Digital Input State, Fast Command", &val3)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_masked_digital_input_state_s(ultimET, val1, val2, &get_val, val3, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tMasked External Digital Input State -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting an external digital input
 */
int get_external_digital_input_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    ebool val2;
    dword get_val = 0;

    if (err = get_integer("Get External Digital Input State, Input number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get External Digital Input State, Fast Command", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_digital_input_state_s(ultimET, val1, &get_val, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tExternal Digital Input State -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting a local digital output
 */
int get_local_digital_output_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    ebool val2;
    dword get_val = 0;

    if (err = get_integer("Get Local Digital Output State, Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get Local Digital Output State, Fast Command", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_localIO_get_digital_output_s(ultimET, val1, &get_val, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tLocal Digital Output State -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting a local digital input
 */
int get_local_digital_input_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    ebool val2;
    dword get_val = 0;

    if (err = get_integer("Get Local Digital Input State, Input number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get Local Digital Input State, Fast Command", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_localIO_get_digital_input_state_s(ultimET, val1, &get_val, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tLocal Digital Input State -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting a masked local digital output
 */
int get_masked_local_digital_output_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    int   val2;
    ebool val3;
    dword get_val = 0;

    if (err = get_integer("Get Masked Local Digital Output State, First Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_integer("Get Masked Local Digital Output State, Number bits", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get Masked Local Digital Output State, Fast Command", &val3)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_localIO_get_masked_digital_output_s(ultimET, val1, val2, &get_val, val3, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tMasked Local Digital Output State -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting a masked local digital input
 */
int get_masked_local_digital_input_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    int   val2;
    ebool val3;
    dword get_val = 0;

    if (err = get_integer("Get Masked Local Digital Input State, First Input number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_integer("Get Masked Local Digital Input State, Number bits", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get Masked Local Digital Input State, Fast Command", &val3)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_localIO_get_masked_digital_input_state_s(ultimET, val1, val2, &get_val, val3, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tMasked Local Digital Input State -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * setting an external analog output using a raw output value
 */
int set_external_analog_output_raw_data(DSA_MASTER *ultimET)
{
    int err;
    int val1;
    int val2;

    if (err = get_integer("Set External Analog Output Raw Data, Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_integer("Set External Analog Output Raw Data, Hex Output Raw value", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_set_analog_output_raw_data_s(ultimET, val1, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nSet External Analog Output Raw Data Output number:%d Value:0x%x successful\n", val1, val2);
        _getch();
    }
    else {
        printf("\nSet External Analog Output Raw Data Output number:%d Value:0x%x error %d\n", val1, val2, err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * setting an external analog output using a converted output value
 */
int set_external_analog_output_converted_data(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    float val2;

    if (err = get_integer("Set External Analog Output Converted Data, Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_float("Set External Analog Output Converted Data, Output Converted value", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_set_analog_output_converted_data_s(ultimET, val1, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nSet External Analog Output Converted Data Output number:%d Value:%lf successful\n", val1, val2);
        _getch();
    }
    else {
        printf("\nSet External Analog Output ConvertedData Output number:%d Value:%lf error %d\n", val1, val2, err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting an external analog output as a raw value
 */
int get_external_analog_output_raw_data(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    ebool val2;
    dword get_val = 0;

    if (err = get_integer("Get External Analog Output Raw Data, Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get External Analog Output Raw Data, Fast Command", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_analog_output_raw_data_s(ultimET, val1, &get_val, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tExternal Analog Output Raw Data-> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting an external analog output as a raw value
 */
int get_external_analog_output_raw_data_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    ebool val2;
    dword get_val = 0;

    if (err = get_integer("Get External Analog Output Raw Data State, Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get External Analog Output Raw Data State, Fast Command", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_analog_output_raw_data_state_s(ultimET, val1, &get_val, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tExternal Analog Output Raw Data State-> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting an external analog output as a converted value
 */
int get_external_analog_output_converted_data(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    ebool val2;
    float get_val = 0;

    if (err = get_integer("Get External Analog Output Converted Data, Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get External Analog Output Converted Data, Fast Command", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_analog_output_converted_data_s(ultimET, val1, &get_val, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tExternal Analog Output Converted Data -> %lf\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting an external analog output as a converted value
 */
int get_external_analog_output_converted_data_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    ebool val2;
    float get_val = 0;

    if (err = get_integer("Get External Analog Output Converted Data State, Output number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get External Analog Output Converted Data State, Fast Command", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_analog_output_converted_data_state_s(ultimET, val1, &get_val, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tExternal Analog Output Converted Data State-> %lf\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting an external analog input as a raw value
 */
int get_external_analog_input_raw_data_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    ebool val2;
    dword get_val = 0;

    if (err = get_integer("Get External Analog Input Raw Data State, Input number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get External Analog Input Raw Data State, Fast Command", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_analog_input_raw_data_state_s(ultimET, val1, &get_val, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tExternal Analog Input Raw Data State -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting an external analog input as a converted value
 */
int get_external_analog_input_converted_data_state(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    ebool val2;
    float get_val = 0;

    if (err = get_integer("Get External Analog Input Converted Data State, Input number", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_bool("Get External Analog Input Converted Data State, Fast Command", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_analog_input_converted_data_state_s(ultimET, val1, &get_val, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tExternal Analog Input Converted Data State-> %lf\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * setting an external register of modbus
 */
int set_external_modbus_register(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    dword val2;

    if (err = get_integer("Set External Modbus Register, Register Address", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_dword("Set External Modbus Register, Hex value", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_set_modbus_register_s(ultimET, val1, val2, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nSet External Modbus Register Register Address:%d Value:0x%x successful\n", val1, val2);
        _getch();
    }
    else {
        printf("\nSet External Modbus Register Register Address:%d Value:0x%x error %d\n", val1, val2, err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * getting an external register of modbus
 */
int get_external_modbus_register(DSA_MASTER *ultimET)
{
    int   err;
    int   val1;
    int   val2;
    int   val3;
    dword get_val = 0;

    if (err = get_integer("Get External Modbus Register, Register Address", &val1)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_integer("Get External Modbus Register, Word count", &val2)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = get_integer("Get External Modbus Register, Word number", &val3)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_get_modbus_register_s(ultimET, val1, val2, val3, &get_val, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

    printf("\tGet External Modbus Register -> 0x%x\n", get_val);
    _getch();

_error:
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * resetting the client communication
 */
int external_reset_client_communication(DSA_MASTER *ultimET)
{
    int err;

    if (err = dsa_externalIO_reset_client_communication_s(ultimET, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nExternal Reset Client Communication successful\n");
        _getch();
    }
    else {
        printf("\nExternal Reset Client Communication error %d\n", err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * resetting the io cycle count
 */
int external_reset_io_cycle_count(DSA_MASTER *ultimET)
{
    int err;

    if (err = dsa_externalIO_reset_io_cycle_count_s(ultimET, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nExternal Reset IO Cycle count successful\n");
        _getch();
    }
    else {
        printf("\nExternal Reset IO Cycle count error %d\n", err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * resetting the maximal update time
 */
int external_reset_max_update_time(DSA_MASTER *ultimET)
{
    int err;

    if (err = dsa_externalIO_reset_max_update_time_s(ultimET, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nExternal Reset Max Update Time successful\n");
        _getch();
    }
    else {
        printf("\nExternal Reset Max Update Time error %d\n", err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * enable or disable cyclic update
 */
int external_enable_cyclic_update(DSA_MASTER *ultimET)
{
    int   err;
    ebool enable;

    if (err = get_bool("Enable cyclic Update", &enable)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_set_enable_cyclic_update_s(ultimET, enable, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nExternal Enable/Disable Cyclic Update successful\n");
        _getch();
    }
    else {
        printf("\nExternal Enable/Disable Cyclic Update Time error %d\n", err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * enable or disable watchdog
 */
int external_enable_watchdog(DSA_MASTER *ultimET)
{
    int   err;
    ebool enable;

    if (err = get_bool("Enable watchdog", &enable)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (enable) {
        if (err = dsa_externalIO_enable_watchdog_s(ultimET, 2000)) {
            DSA_EXT_DIAG(err, ultimET);
            goto _error;
        }
    }
    else {
        if (err = dsa_externalIO_disable_watchdog_s(ultimET, 2000)) {
            DSA_EXT_DIAG(err, ultimET);
            goto _error;
        }
    }

_error:
    if (!err) {
        printf("\nExternal Enable/Disable Watchdog successful\n");
        _getch();
    }
    else {
        printf("\nExternal Enable/Disable Watchdog error %d\n", err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * stop watchdog
 */
int external_stop_watchdog(DSA_MASTER *ultimET)
{
    int err;

    if (err = dsa_externalIO_stop_watchdog_s(ultimET, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nExternal Stop Watchdog successful\n");
        _getch();
    }
    else {
        printf("\nExternal Stop Watchdog error %d\n", err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the different parameter needed to call DSA function
 * enable or disable watchdog
 */
int external_set_watchdog_time(DSA_MASTER *ultimET)
{
    int err;
    int time;

    if (err = get_integer("Watchdog time [in 100 ms)", &time)) {
        err = -1;
        printf("Invalid value\n");
        goto _error;
    }

    if (err = dsa_externalIO_set_watchdog_time_s(ultimET, time, 2000)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    if (!err) {
        printf("\nSet Watchdog Time successful\n");
        _getch();
    }
    else {
        printf("\nSet Watchdog Time error %d\n", err);
        _getch();
    }
    return err;
}

/*
 * Ask the user to enter the communication bus he wants to use and open it
 */
int open_communication(DSA_MASTER *ultimET)
{
    int  err;
    char choice;
    char ip[64];
    char url[128];

    /* Display menu */
    do {
        fflush(stdin);
        printf("\n\n\n");
        printf("Choose Communication\n");
        printf("********************\n");
        printf("\tX: Exit\n");
        printf("\tA: UltimET PCI\n");
        printf("\tB: UltimET TCP-IP\n");

        printf("\n\t\tYour choice : ");
        fflush(stdout);
        scanf("%c", &choice);
    } while ((choice != 'A') && (choice != 'a') && (choice != 'B') && (choice != 'b') && (choice != 'X') && (choice != 'x'));
    printf("\n");

    switch (choice) {
    case 'a':
    case 'A':
        if (err = dsa_open_u(ultimET, "etb:ULTIMET:*")) {
            DSA_EXT_DIAG(err, NULL);
            goto _error;
        }
        break;
    case 'b':
    case 'B':
        printf("\tEnter IP Address : ");
        fflush(stdout);
        scanf("%s", ip);
        printf("\n");

        sprintf(url, "etb:etn://%s:1129:*", ip);
        if (err = dsa_open_u(ultimET, url)) {
            DSA_EXT_DIAG(err, NULL);
            goto _error;
        }
        break;
    case 'x':
    case 'X':
        return 0;
    }

_error:
    return err;
}

/*
 * Close the opened communication bus
 */
int close_communication(DSA_MASTER *ultimET)
{
    int err;

    if (err = dsa_close(ultimET)) {
        DSA_EXT_DIAG(err, ultimET);
        goto _error;
    }

_error:
    return err;
}

/*
 * Display a menu allowing the user to choose which external io he want to access.
 * Call the corresponding function
 */
int external_io(DSA_MASTER *ultimET)
{
    int  err;
    char choice;

    /* Display menu */
    for (;;) {
        do {
            fflush(stdin);
            printf("\n\n\n");
            printf("External IO\n");
            printf("***********\n");
            printf("\tX: Exit\n");
            printf("\tA: Set External Digital Output (SETEDOUT)\n");
            printf("\tB: Reset External Digital Output (RSTEDOUT)\n");
            printf("\tC: Get External Digital Output(GETEDOUT)\n");
            printf("\tD: Get External Digital Output State(GETEDOUTS)\n");
            printf("\tE: Apply Mask External Digital Output (SETMEDOUT)\n");
            printf("\tF: Get Masked External Digital Output (GETMEDOUT)\n");
            printf("\tG: Get Masked External Digital Output State (GETMEDOUTS)\n");
            printf("\tH: Set External Analog Output Raw (SETEAOUTR)\n");
            printf("\tI: Set External Analog Output Converted (SETEAOUTC)\n");
            printf("\tJ: Get External Analog Output Raw (GETEAOUTR)\n");
            printf("\tK: Get External Analog Output Raw State (GETEAOUTRS)\n");
            printf("\tL: Get External Analog Output Converted (GETEAOUTC)\n");
            printf("\tM: Get External Analog Output Converted State (GETEAOUTCS)\n");
            printf("\tN: Get External Analog Input Raw State (GETEAINRS)\n");
            printf("\tO: Get External Analog Input Converted State(GETEAINCS)\n");
            printf("\tP: Get External Digital Input State(GETEDINS)\n");
            printf("\tQ: Get Masked External Digital Input State(GETMEDINS)\n");
            printf("\tR: Set External Modbus Register (SETEREG)\n");
            printf("\tS: Get External Modbus Register (GETEREG)\n");
            printf("\tT: Reset Client Communication (RCT)\n");
            printf("\tU: Reset IO Cycle Count (RIC)\n");
            printf("\tV: Reset Max Update Time (EIOMAXURST)\n");
            printf("\tW: Enable/Disable Cyclic Update (EIOMAXURST)\n");
            printf("\t1: Enable/Disable watchdog (EIOWDEN)\n");
            printf("\t2: Stop watchdog (EIOWDSTP)\n");
            printf("\t3: Set watchdog time(EIOWDTIME)\n");

            printf("\n\t\tYour choice : ");
            fflush(stdout);
            scanf("%c", &choice);
            choice = toupper(choice);
        } while ((choice != 'A') && (choice != 'B') && (choice != 'C') && (choice != 'D') && (choice != 'E') && (choice != 'F') && (choice != 'G') &&
                 (choice != 'H') && (choice != 'I') && (choice != 'J') && (choice != 'K') && (choice != 'L') && (choice != 'M') && (choice != 'N') &&
                 (choice != 'O') && (choice != 'P') && (choice != 'Q') && (choice != 'R') && (choice != 'S') && (choice != 'T') && (choice != 'U') &&
                 (choice != 'V') && (choice != 'W') && (choice != '1') && (choice != '2') && (choice != '3') && (choice != 'X'));
        printf("\n");

        switch (choice) {
        case 'A':
            if (err = set_external_digital_output(ultimET))
                goto _error;
            break;
        case 'B':
            if (err = reset_external_digital_output(ultimET))
                goto _error;
            break;
        case 'C':
            if (err = get_external_digital_output(ultimET))
                goto _error;
            break;
        case 'D':
            if (err = get_external_digital_output_state(ultimET))
                goto _error;
            break;
        case 'E':
            if (err = apply_mask_external_digital_output(ultimET))
                goto _error;
            break;
        case 'F':
            if (err = get_masked_external_digital_output(ultimET))
                goto _error;
            break;
        case 'G':
            if (err = get_masked_external_digital_output_state(ultimET))
                goto _error;
            break;
        case 'H':
            if (err = set_external_analog_output_raw_data(ultimET))
                goto _error;
            break;
        case 'I':
            if (err = set_external_analog_output_converted_data(ultimET))
                goto _error;
            break;
        case 'J':
            if (err = get_external_analog_output_raw_data(ultimET))
                goto _error;
            break;
        case 'K':
            if (err = get_external_analog_output_raw_data_state(ultimET))
                goto _error;
            break;
        case 'L':
            if (err = get_external_analog_output_converted_data(ultimET))
                goto _error;
            break;
        case 'M':
            if (err = get_external_analog_output_converted_data_state(ultimET))
                goto _error;
            break;
        case 'N':
            if (err = get_external_analog_input_raw_data_state(ultimET))
                goto _error;
            break;
        case 'O':
            if (err = get_external_analog_input_converted_data_state(ultimET))
                goto _error;
            break;
        case 'P':
            if (err = get_external_digital_input_state(ultimET))
                goto _error;
            break;
        case 'Q':
            if (err = get_masked_external_digital_input_state(ultimET))
                goto _error;
            break;
        case 'R':
            if (err = set_external_modbus_register(ultimET))
                goto _error;
            break;
        case 'S':
            if (err = get_external_modbus_register(ultimET))
                goto _error;
            break;
        case 'T':
            if (err = external_reset_client_communication(ultimET))
                goto _error;
            break;
        case 'U':
            if (err = external_reset_io_cycle_count(ultimET))
                goto _error;
            break;
        case 'V':
            if (err = external_reset_max_update_time(ultimET))
                goto _error;
            break;
        case 'W':
            if (err = external_enable_cyclic_update(ultimET))
                goto _error;
            break;
        case '1':
            if (err = external_enable_watchdog(ultimET))
                goto _error;
            break;
        case '2':
            if (err = external_stop_watchdog(ultimET))
                goto _error;
            break;
        case '3':
            if (err = external_set_watchdog_time(ultimET))
                goto _error;
            break;
        case 'X':
            return 0;
        }
    }

_error:
    return err;
}

/*
 * Display a menu allowing the user to choose which local io he want to access.
 * Call the corresponding function
 */
int local_io(DSA_MASTER *ultimET)
{
    int  err;
    char choice;

    /* Display menu */
    for (;;) {
        do {
            fflush(stdin);
            printf("\n\n\n");
            printf("Local IO\n");
            printf("********\n");
            printf("\tX: Exit\n");
            printf("\tA: Set Local Digital Output (SETDOUT)\n");
            printf("\tB: Reset Local Digital Output (RSTDOUT)\n");
            printf("\tC: Get Local Digital Output (GETDOUT)\n");
            printf("\tD: Apply Mask Local Digital Output (MXDOUT)\n");
            printf("\tE: Get Masked Local Digital Output (GETMDOUT)\n");
            printf("\tF: Get Local Digital Input (GETDINS)\n");
            printf("\tG: Get Masked Local Digital Input (GETMXDINS)\n");

            printf("\n\t\tYour choice : ");
            fflush(stdout);
            scanf("%c", &choice);
            choice = toupper(choice);
        } while ((choice != 'A') && (choice != 'B') && (choice != 'C') && (choice != 'D') && (choice != 'E') && (choice != 'F') && (choice != 'G') &&
                 (choice != 'X'));
        printf("\n");

        switch (choice) {
        case 'A':
            if (err = set_local_digital_output(ultimET))
                goto _error;
            break;
        case 'B':
            if (err = reset_local_digital_output(ultimET))
                goto _error;
            break;
        case 'C':
            if (err = get_local_digital_output_state(ultimET))
                goto _error;
            break;
        case 'D':
            if (err = apply_mask_local_digital_output(ultimET))
                goto _error;
            break;
        case 'E':
            if (err = get_masked_local_digital_output_state(ultimET))
                goto _error;
            break;
        case 'F':
            if (err = get_local_digital_input_state(ultimET))
                goto _error;
            break;
        case 'G':
            if (err = get_masked_local_digital_input_state(ultimET))
                goto _error;
            break;
        case 'X':
            return 0;
        }
    }

_error:
    return err;
}

/*
 * Display a menu allowing the user to access to different functionnalities of the
 * example program.
 */
int main(char argc, int *argv[])
{
    int         err;
    char        choice;
    DSA_MASTER *ultimET = NULL;

    if (err = dsa_create_master(&ultimET)) {
        DSA_EXT_DIAG(err, NULL);
        goto _error;
    }

    /* Display menu */
    for (;;) {
        do {
            fflush(stdin);
            printf("\n\n\nChoose Action\n");
            printf("******************\n");
            printf("\tX: Exit\n");
            printf("\tA: Open communication\n");
            printf("\tB: Close communication\n");
            printf("\tC: Access External IO\n");
            printf("\tD: Access Local IO\n");

            printf("\n\t\tYour choice : ");
            fflush(stdout);
            scanf("%c", &choice);
            choice = toupper(choice);
        } while ((choice != 'A') && (choice != 'B') && (choice != 'C') && (choice != 'D') && (choice != 'X'));
        printf("\n");

        switch (choice) {
        case 'A':
            if (err = open_communication(ultimET))
                goto _error;
            break;
        case 'B':
            if (err = close_communication(ultimET))
                goto _error;
            break;
        case 'C':
            if (err = external_io(ultimET))
                goto _error;
            break;
        case 'D':
            if (err = local_io(ultimET))
                goto _error;
            break;
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
