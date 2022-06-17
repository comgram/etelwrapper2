#include "interface.h"
#include "logger.h"
#include <math.h>
#include <stdio.h>
//#include <time.h>
#include <windows.h>

#define DEBUG            1

#define TEST_TIMEOUT     5000
#define LONGER_TIMEOUT   3000000

// command list
#define CMD_END          0
#define CMD_WTM          8
#define CMD_WTT          10
#define CMD_CLX          17
#define CMD_STP          18
#define CMD_SET          22
#define CMD_JMP          26
#define CMD_MVE          60
#define CMD_RMVE         62
#define CMD_NEW          80
#define CMD_RST          79
#define CMD_ERR          80
#define CMD_CH_BIT_REG32 90
#define CMD_HLO          119
#define CMD_HLT          120
#define CMD_HLB          121
#define CMD_PWR          124
#define CMD_CLRWAIT      180
#define CMD_ISET         552
#define CMD_IBEGIN       553
#define CMD_IEND         554
#define CMD_IABSMODE     555
#define CMD_IABSCOORDS   556
#define CMD_IBRAK        653
#define CMD_ISTP         656
#define CMD_GETDOUT      735
#define CMD_SETDOUT      736
#define CMD_GETDINS      738
#define CMD_ILINE        1025
#define CMD_IPVT         1028
#define CMD_IWTT         1029
#define CMD_ITSPD        1035
#define CMD_ITACC        1036
#define CMD_ITDEC        1037
#define CMD_ITJRT        1038
#define CMD_IPT          1045
#define CMD_IMTRANS      1054
#define CMD_IMSCALE      1055
#define CMD_IMROT        1056
#define CMD_IMSHEAR      1057
#define CMD_IMRES        1063

// ACCUET and ULTIMET Physical Drive NO.
#define X_DRIVE          0
#define Y_DRIVE          1
#define Z_DRIVE          2
#define D_DRIVE          3
#define UTLIMET_DRIVE    "*"

// Axis NO.
#define X_AXIS_NO        1 // 2^0
#define Y_AXIS_NO        2 // 2^1
#define Z_AXIS_NO        4 // 2^2
#define D_AXIS_NO        8 // 2^3
#define ULTIMET_NO       9

// ACCUET group NO.
#define GRP_XY           3
#define GRP_XZ           5
#define GRP_YZ           6
#define GRP_XYZ          7

// Interpolation group NO.
#define IPOL_GRP0        20
#define IPOL_GRP1        21

// designated sequence
#define SEQ_HOMING       0

// User regitster NO. of ETEL
#define HOM_FLAG         0 // register number of homing
#define MOV_FLAG         1 // register number of movement
#define SEQ_FLAG         2 // register number of executing

// X , int32: int, dword
// XL, int64: __int64, long long, qword
// XF, float32: float
// XD, float64: double

#define IPOL_HOME_GRP0   21 // machine coords

int POSITION_CONTROLLER_COUNT = 2;

// Assignement of drive an group
DSA_DRIVE *x = NULL;
DSA_DRIVE *y = NULL;
DSA_DRIVE *z = NULL;
DSA_DRIVE *d = NULL;

DSA_MASTER *ultimet = NULL;

DSA_DRIVE_GROUP *xy  = NULL;
DSA_DRIVE_GROUP *xz  = NULL;
DSA_DRIVE_GROUP *yz  = NULL;
DSA_DRIVE_GROUP *xyz = NULL;

DSA_IPOL_GROUP *igrp0 = NULL;
DSA_IPOL_GROUP *igrp1 = NULL;

// variable for error return
int err;

// active interpolation group
int active_interpolation_group_no = -1;
int default_unit                  = 1;
int workpiece_location[3]         = {0, 0, 0};
int homing_done                   = 0;
int coordinate_mode               = 0;
int open_prepared                 = 0;
int connected                     = 0;
int powered                       = 0;

DSA_DRIVE_BASE *Axis(int axis)
{
    switch (axis) {
    // Drive
    case X_AXIS_NO:
        return x;
    case Y_AXIS_NO:
        return y;
    case Z_AXIS_NO:
        return z;
    case D_AXIS_NO:
        return d;
    // Master
    case ULTIMET_NO:
        return ultimet;
    // Drive Group
    case GRP_XY:
        return xy;
    case GRP_XZ:
        return xz;
    case GRP_YZ:
        return yz;
    case GRP_XYZ:
        return xyz;
    // Interpolation Group
    case IPOL_GRP0:
        return igrp0;
    case IPOL_GRP1:
        return igrp1;
    // Default
    default:
        return ultimet;
    }
}

int error_action(void)
{
    /* Destroy the group if not already done. */
//    if (dsa_is_valid_ipol_group(igrp0)) {
//        if (dsa_is_ipol_in_progress(igrp0))
//            dsa_ipol_end_s(igrp0, DSA_DEF_TIMEOUT);
//        dsa_destroy(&igrp0);
//    }
//
//    if (dsa_is_valid_ipol_group(igrp1)) {
//        if (dsa_is_ipol_in_progress(igrp1))
//            dsa_ipol_end_s(igrp1, DSA_DEF_TIMEOUT);
//        dsa_destroy(&igrp1);
//    }

    /* Is the drive pointer valid ? */
    if (dsa_is_valid_drive(x)) {
        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(x, &open);
        if (open) {
            /* Stop all movements. */
            dsa_quick_stop_s(x, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);
            /* When the motor has stopped, a power off is done. */
            dsa_wait_movement_s(x, 60000);
            dsa_power_off_s(x, 10000);
            /* Close the connection. */
            dsa_close(x);
        }
        /* And finally, release all resources to the OS. */
        dsa_destroy(&x);
    }

    /* Same operations for the second drive */
    if (dsa_is_valid_drive(y)) {
        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(y, &open);
        if (open) {
            /* Stop all movements. */
            dsa_quick_stop_s(y, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);
            /* When the motor has stopped, a power off is done. */
            dsa_wait_movement_s(y, 60000);
            dsa_power_off_s(y, 10000);
            /* Close the connection */
            dsa_close(y);
        }
        /* And finally, release all resources to the OS. */
        dsa_destroy(&y);
    }

    if (dsa_is_valid_drive(z)) {
        /* Is the drive open ? */
        ebool open = 0;
        dsa_is_open(z, &open);
        if (open) {
            /* Stop all movements. */
            dsa_quick_stop_s(z, DSA_QS_PROGRAMMED_DEC, DSA_QS_BYPASS | DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);
            /* When the motor has stopped, a power off is done. */
            dsa_wait_movement_s(z, 60000);
            dsa_power_off_s(z, 10000);
            /* Close the connection */
            dsa_close(z);
        }
        /* And finally, release all resources to the OS. */
        dsa_destroy(&z);
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

    return 0;
}

// Wrap Functions

int wrap_is_open(int axis, unsigned char *buf)
{
    unsigned char open;
    dsa_is_open(Axis(axis), &open);
    buf = (unsigned char *)open;

    return 1;
}

int wrap_designated_open_prepare(void)
{
    x = NULL;
    y = NULL;
    z = NULL;
    d = NULL;

    ultimet = NULL;

    xy  = NULL;
    xz  = NULL;
    yz  = NULL;
    xyz = NULL;

    igrp0 = NULL;
    igrp1 = NULL;

    // create dirve and master
    if (err = dsa_create_drive(&x)) {
        DSA_EXT_DIAG(err, x);
        return error_action();
    }
    if (err = dsa_create_drive(&y)) {
        DSA_EXT_DIAG(err, y);
        return error_action();
    }
    if (err = dsa_create_drive(&z)) {
        DSA_EXT_DIAG(err, z);
        return error_action();
    }
    if (err = dsa_create_drive(&d)) {
        DSA_EXT_DIAG(err, d);
        return error_action();
    }
    if (err = dsa_create_master(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    // create ACCUET group xy
    if (err = dsa_create_ipol_group(&xy, 2)) {
        DSA_EXT_DIAG(err, xy);
        return error_action();
    }
    if (err = dsa_set_group_item(xy, 0, x)) {
        DSA_EXT_DIAG(err, xy);
        return error_action();
    }
    if (err = dsa_set_group_item(xy, 1, y)) {
        DSA_EXT_DIAG(err, xy);
        return error_action();
    }

    // create ACCUET group xz
    if (err = dsa_create_ipol_group(&xz, 2)) {
        DSA_EXT_DIAG(err, xz);
        return error_action();
    }
    if (err = dsa_set_group_item(xz, 0, x)) {
        DSA_EXT_DIAG(err, xz);
        return error_action();
    }
    if (err = dsa_set_group_item(xz, 1, z)) {
        DSA_EXT_DIAG(err, xz);
        return error_action();
    }

    // create ACCUET group yz
    if (err = dsa_create_ipol_group(&yz, 2)) {
        DSA_EXT_DIAG(err, yz);
        return error_action();
    }
    if (err = dsa_set_group_item(yz, 0, y)) {
        DSA_EXT_DIAG(err, yz);
        return error_action();
    }
    if (err = dsa_set_group_item(yz, 1, z)) {
        DSA_EXT_DIAG(err, yz);
        return error_action();
    }

    // create ACCUET group xyz
    if (err = dsa_create_ipol_group(&xyz, 3)) {
        DSA_EXT_DIAG(err, xyz);
        return error_action();
    }
    if (err = dsa_set_group_item(xyz, 0, x)) {
        DSA_EXT_DIAG(err, xyz);
        return error_action();
    }
    if (err = dsa_set_group_item(xyz, 1, y)) {
        DSA_EXT_DIAG(err, xyz);
        return error_action();
    }
    if (err = dsa_set_group_item(xyz, 2, z)) {
        DSA_EXT_DIAG(err, xyz);
        return error_action();
    }

    // create interpolation group 0
    if (err = dsa_create_ipol_group(&igrp0, 3)) {
        DSA_EXT_DIAG(err, igrp0);
        return error_action();
    }
    if (err = dsa_set_group_item(igrp0, 0, x)) {
        DSA_EXT_DIAG(err, igrp0);
        return error_action();
    }
    if (err = dsa_set_group_item(igrp0, 1, y)) {
        DSA_EXT_DIAG(err, igrp0);
        return error_action();
    }
    if (err = dsa_set_group_item(igrp0, 2, z)) {
        DSA_EXT_DIAG(err, igrp0);
        return error_action();
    }
    if (err = dsa_set_master(igrp0, ultimet)) {
        DSA_EXT_DIAG(err, igrp0);
        return error_action();
    }

    // create interpolation group 1
    if (err = dsa_create_ipol_group(&igrp1, 3)) {
        DSA_EXT_DIAG(err, igrp1);
        return error_action();
    }
    if (err = dsa_set_group_item(igrp1, 0, x)) {
        DSA_EXT_DIAG(err, igrp1);
        return error_action();
    }
    if (err = dsa_set_group_item(igrp1, 1, y)) {
        DSA_EXT_DIAG(err, igrp1);
        return error_action();
    }
    if (err = dsa_set_group_item(igrp1, 2, z)) {
        DSA_EXT_DIAG(err, igrp1);
        return error_action();
    }
    if (err = dsa_set_master(igrp1, ultimet)) {
        DSA_EXT_DIAG(err, igrp1);
        return error_action();
    }

    if (DEBUG) {
        printf("Created drives and groups.\n");
    }

    return 1;
}

int wrap_designated_open(const char *url, int port, int keep_alive)
{
    // open device

    char buf[255];
    sprintf_s(buf, sizeof(buf), "etb:ETN://%s:%d,T=%d:%d", url, port, keep_alive, X_DRIVE);
    /**if (err = dsa_open_u(x, "etb:ETN://172.30.1.80:1129,T=-1:1")) {*/
    if (err = dsa_open_u(x, buf)) {
        DSA_EXT_DIAG(err, x);
        return error_action();
    }
    sprintf_s(buf, sizeof(buf), "etb:ETN://%s:%d,T=%d:%d", url, port, keep_alive, Y_DRIVE);
    if (err = dsa_open_u(y, buf)) {
        DSA_EXT_DIAG(err, y);
        return error_action();
    }
    sprintf_s(buf, sizeof(buf), "etb:ETN://%s:%d,T=%d:%d", url, port, keep_alive, Z_DRIVE);
    if (err = dsa_open_u(z, buf)) {
        DSA_EXT_DIAG(err, z);
        return error_action();
    }
    sprintf_s(buf, sizeof(buf), "etb:ETN://%s:%d,T=%d:%d", url, port, keep_alive, D_DRIVE);
    if (err = dsa_open_u(d, buf)) {
        DSA_EXT_DIAG(err, d);
        return error_action();
    }
    sprintf_s(buf, sizeof(buf), "etb:ETN://%s:%d,T=%d:*", url, port, keep_alive);
    if (err = dsa_open_u(ultimet, buf)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    if (DEBUG) {
        printf("Motion devices opened.\n");
    }

    return 1;
}

int wrap_status(int axis, int *buf)
{
    DSA_STATUS status = {sizeof(DSA_STATUS)};
    if (err = dsa_get_status_from_drive(Axis(axis), &status, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    buf[0]  = status.sw.sw1.power_on;
    buf[1]  = status.sw.sw1.init_done;
    buf[2]  = status.sw.sw1.homing_done;
    buf[3]  = status.sw.sw1.present;
    buf[4]  = status.sw.sw1.moving;
    buf[5]  = status.sw.sw1.in_window;
    buf[6]  = status.sw.sw1.exec_seq;
    buf[7]  = status.sw.sw1.in_speed_window;
    buf[8]  = status.sw.sw1.fatal;
    buf[9]  = status.sw.sw1.trace_busy;
    buf[10] = status.sw.sw1.ipol_grp0;
    buf[11] = status.sw.sw1.ipol_grp1;
    buf[12] = status.sw.sw1.exec_seq_thread;
    buf[13] = status.sw.sw1.warning;
    buf[14] = status.sw.sw2.int_error;
    buf[15] = status.sw.sw2.warning;
    buf[16] = status.sw.sw2.save_pos;
    buf[17] = status.sw.sw2.breakpoint;
    buf[18] = status.sw.sw2.user;

    return 1;
}

int wrap_power_on(int axis)
{
    if (err = dsa_power_on_s(Axis(axis), 10000)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_user_position(int axis, double pos)
{
    if (err = dsa_set_user_position_s(Axis(axis), pos, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_reset(int axis)
{
    if (axis == 0) {
        if (err = dsa_reset_error_ex_s(x, 0, 10000)) {
            DSA_EXT_DIAG(err, x);
            return error_action();
        }
        if (err = dsa_reset_error_ex_s(y, 0, 10000)) {
            DSA_EXT_DIAG(err, y);
            return error_action();
        }
        if (err = dsa_reset_error_ex_s(z, 0, 10000)) {
            DSA_EXT_DIAG(err, z);
            return error_action();
        }
        if (err = dsa_reset_error_ex_s(d, 0, 10000)) {
            DSA_EXT_DIAG(err, d);
            return error_action();
        }
        if (err = dsa_reset_error_ex_s(ultimet, 0, 10000)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }
    }
    else {
        if (err = dsa_reset_error_ex_s(Axis(axis), 0, 10000)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
    }

    if (DEBUG) {
        printf("Reset\n");
    }
    return 1;
}

int wrap_power_off(int axis)
{
    if (axis == 0) {
        if (err = dsa_power_off_s(x, 60000)) {
            DSA_EXT_DIAG(err, x);
            return error_action();
        }
        if (err = dsa_power_off_s(y, 60000)) {
            DSA_EXT_DIAG(err, y);
            return error_action();
        }
        if (err = dsa_power_off_s(z, 60000)) {
            DSA_EXT_DIAG(err, z);
            return error_action();
        }
    }
    else {

        if (err = dsa_power_off_s(Axis(axis), 60000)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
    }

    return 1;
}

int wrap_close(int axis)
{
    if (axis == 0) {
        if (err = dsa_close(x)) {
            DSA_EXT_DIAG(err, x);
            return error_action();
        }
        if (err = dsa_close(y)) {
            DSA_EXT_DIAG(err, y);
            return error_action();
        }
        if (err = dsa_close(z)) {
            DSA_EXT_DIAG(err, z);
            return error_action();
        }
        if (err = dsa_close(ultimet)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }
    }
    else {
        if (err = dsa_close(Axis(axis))) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
    }

    return 1;
}

int wrap_destroy(int axis)
{
    if (axis == 0) {
        if (err = dsa_destroy(&x)) {
            DSA_EXT_DIAG(err, x);
            return error_action();
        }
        if (err = dsa_destroy(&y)) {
            DSA_EXT_DIAG(err, y);
            return error_action();
        }
        if (err = dsa_destroy(&z)) {
            DSA_EXT_DIAG(err, z);
            return error_action();
        }
        if (err = dsa_destroy(&d)) {
            DSA_EXT_DIAG(err, d);
            return error_action();
        }
        if (err = dsa_destroy(&ultimet)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }
    }
    else {
        if (err = dsa_destroy(Axis(axis))) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
    }

    return 1;
}

int wrap_destory_group(void)
{
    if (err = dsa_destroy(&igrp0)) {
        DSA_EXT_DIAG(err, igrp0);
        return error_action();
    }
    if (err = dsa_destroy(&igrp1)) {
        DSA_EXT_DIAG(err, igrp1);
        return error_action();
    }
    if (err = dsa_destroy(&xy)) {
        DSA_EXT_DIAG(err, xy);
        return error_action();
    }
    if (err = dsa_destroy(&xz)) {
        DSA_EXT_DIAG(err, xz);
        return error_action();
    }
    if (err = dsa_destroy(&yz)) {
        DSA_EXT_DIAG(err, yz);
        return error_action();
    }
    if (err = dsa_destroy(&xyz)) {
        DSA_EXT_DIAG(err, xyz);
        return error_action();
    }

    return 1;
}

int wrap_terminate(void)
{
    // terminale all
    // power off all
    /**if (err = dsa_power_off_s(x, 60000)) {*/
    /**    DSA_EXT_DIAG(err, x);*/
    /**    return error_action();*/
    /**}*/
    /**if (err = dsa_power_off_s(y, 60000)) {*/
    /**    DSA_EXT_DIAG(err, y);*/
    /**    return error_action();*/
    /**}*/
    /**if (err = dsa_power_off_s(z, 60000)) {*/
    /**    DSA_EXT_DIAG(err, z);*/
    /**    return error_action();*/
    /**}*/

    // close all
    /**if (err = dsa_close(x)) {*/
    /**    DSA_EXT_DIAG(err, x);*/
    /**    return error_action();*/
    /**}*/
    /**if (err = dsa_close(y)) {*/
    /**    DSA_EXT_DIAG(err, y);*/
    /**    return error_action();*/
    /**}*/
    /**if (err = dsa_close(z)) {*/
    /**    DSA_EXT_DIAG(err, z);*/
    /**    return error_action();*/
    /**}*/
    /**if (err = dsa_close(ultimet)) {*/
    /**    DSA_EXT_DIAG(err, ultimet);*/
    /**    return error_action();*/
    /**}*/

    // destroy all
    if (err = dsa_destroy(&igrp0)) {
        DSA_EXT_DIAG(err, igrp0);
        return error_action();
    }
    if (err = dsa_destroy(&igrp1)) {
        DSA_EXT_DIAG(err, igrp1);
        return error_action();
    }
    if (err = dsa_destroy(&xy)) {
        DSA_EXT_DIAG(err, xy);
        return error_action();
    }
    if (err = dsa_destroy(&xz)) {
        DSA_EXT_DIAG(err, xz);
        return error_action();
    }
    if (err = dsa_destroy(&yz)) {
        DSA_EXT_DIAG(err, yz);
        return error_action();
    }
    if (err = dsa_destroy(&xyz)) {
        DSA_EXT_DIAG(err, xyz);
        return error_action();
    }
    if (err = dsa_destroy(&x)) {
        DSA_EXT_DIAG(err, x);
        return error_action();
    }
    if (err = dsa_destroy(&y)) {
        DSA_EXT_DIAG(err, y);
        return error_action();
    }
    if (err = dsa_destroy(&z)) {
        DSA_EXT_DIAG(err, z);
        return error_action();
    }
    /**if (err = dsa_destroy(&d)) {*/
    /**    DSA_EXT_DIAG(err, d);*/
    /**    return error_action();*/
    /**}*/
    if (err = dsa_destroy(&ultimet)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_designated_homing(int axis)
{
    if (axis == 0) {
        printf("homing %d\n", SEQ_HOMING);
        if (err = dsa_execute_sequence_s(ultimet, SEQ_HOMING, 10000)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }

        DSA_STATUS status = {sizeof(DSA_STATUS)};
        while (1) {
            if (err = dsa_get_status_from_drive(ultimet, &status, DSA_DEF_TIMEOUT)) {
                DSA_EXT_DIAG(err, ultimet);
                return error_action();
            }
            if (DEBUG) {
                printf("homing seq. executing => %d\n", status.sw.sw1.exec_seq);
            }
            if (status.sw.sw1.exec_seq == 0) {
                break;
            }
            else {
                Sleep(1000);
            }
        }
        Sleep(1000);
    }
    else {
        if (err = dsa_execute_sequence_s(Axis(axis), SEQ_HOMING, 10000)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
    }

    return 1;
}

int wrap_get_position(int axis, double *buf)
{
    if (axis == 0) {
        double pos_x = -999999999.0;
        if (err = dsa_get_position_actual_value_s(x, &pos_x, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        double pos_y = -999999999.0;
        if (err = dsa_get_position_actual_value_s(y, &pos_y, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        double pos_z = -999999999.0;
        if (err = dsa_get_position_actual_value_s(z, &pos_z, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        buf[0] = pos_x;
        buf[1] = pos_y;
        buf[2] = pos_z;
    }
    else {

        double pos = -999999999.0;
        if (err = dsa_get_position_actual_value_s(Axis(axis), &pos, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        buf[0] = pos;
    }
    if (DEBUG) {
        printf("[wrap] poistion: %f, %f, %f\n", buf[0], buf[1], buf[2]);
    }

    return 1;
}

int wrap_get_axis_number(int axis, int *buf)
{
    if (err = dsa_get_axis_number_s(Axis(axis), buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_get_actual_value(int axis, double *buf)
{
    if (err = dsa_get_position_actual_value_s(Axis(axis), buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_get_demand_value(int axis, double *buf)
{
    if (err = dsa_get_position_demand_value_s(Axis(axis), buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_velocity(int axis, double velocity)
{
    // SPD
    if (err = dsa_set_profile_velocity_s(Axis(axis), 0, velocity, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_acceleration(int axis, double acceleration)
{
    // ACC
    if (err = dsa_set_profile_acceleration_s(Axis(axis), 0, acceleration, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_jerk_time(int axis, double jerk_time)
{
    // JRK
    if (err = dsa_set_jerk_time_s(Axis(axis), 0, jerk_time, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_speed(int axis, double velocity, double acceleration, double jerk_time)
{
    if (err = dsa_set_profile_velocity_s(Axis(axis), 0, velocity, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    if (err = dsa_set_profile_acceleration_s(Axis(axis), 0, acceleration, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    if (err = dsa_set_jerk_time_s(Axis(axis), 0, jerk_time, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_brake_deceleration(int axis, double brake_deceleration)
{
    // When halt the movement
    if (err = dsa_set_brake_deceleration_s(Axis(axis), brake_deceleration, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_target_position(int axis, double pos, int sidx)
{
    // STP
    // parameter sidx
    // 0 : will immediately start a move on the specified axes (MVE)
    // 1..3 : will set depth 1..3 of parameter KL210 with the position
    printf("axis: %d, target position: %f\n", axis, pos);
    if (err = dsa_set_target_position_s(Axis(axis), sidx, pos, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_get_target_point(int axis, double *buf, int sidx)
{
    if (err = dsa_get_target_position_s(Axis(axis), sidx, buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_start_profiled_movement(int axis, double pos, double speed, double acc)
{
    // MVE
    if (err = dsa_start_profiled_movement_s(Axis(axis), pos, speed, acc, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    return 1;
}

int wrap_start_relative_profiled_movement(int axis, double signed_length)
{
    // RMVE
    if (err = dsa_start_relative_profiled_movement_s(Axis(axis), signed_length, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    return 1;
}

int wrap_step_motion_abs(int axis, double pos)
{
    if (err = dsa_step_motion_s(Axis(axis), pos, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    return 1;
}

int wrap_wait_movement(int axis)
{
    // WTM
    if (err = dsa_wait_movement_s(Axis(axis), 3000000)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_exec_sequence(int axis, int no)
{
    if (err = dsa_set_register_int32_s(Axis(axis), DMD_TYP_USER_INT32, SEQ_FLAG, 0, 1, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    if (err = dsa_execute_sequence_s(Axis(axis), no, 10000)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_wait_sequence(int axis)
{
    DSA_STATUS status = {sizeof(DSA_STATUS)};
    while (1) {
        if (err = dsa_get_status_from_drive(Axis(axis), &status, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        printf("Seq. executing => %d\n", status.sw.sw1.exec_seq);
        if (status.sw.sw1.exec_seq == 0) {
            break;
        }
        else {
            Sleep(1000);
        }
    }

    return 1;
}

int wrap_halt_moving(int axis)
{
    // TODO: need to check out mode and flag
    if (err = dsa_quick_stop_s(Axis(axis), DSA_QS_INFINITE_DEC, DSA_QS_BYPASS_AND_STOP_SEQUENCE, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_halt_sequence(int axis)
{
    // TODO:
    if (err = dsa_stop_sequence_s(Axis(axis), DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

//==============================================================================
// interpolation igrp
//==============================================================================
int wrap_ipol_prepare(int igrp)
{
    if (err = dsa_ipol_prepare_s(Axis(igrp), DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_set_origin(int igrp, double pos_x, double pos_y, double pos_z)
{
    if (err = dsa_set_register_float64_s(Axis(igrp), DMD_TYP_USER_FLOAT64, IPOL_HOME_GRP0, 0, pos_x, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }
    if (err = dsa_set_register_float64_s(Axis(igrp), DMD_TYP_USER_FLOAT64, IPOL_HOME_GRP0, 1, pos_y, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }
    if (err = dsa_set_register_float64_s(Axis(igrp), DMD_TYP_USER_FLOAT64, IPOL_HOME_GRP0, 2, pos_z, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_get_ipol_grp(int igrp, int *buf)
{
    if (err = dsa_ipol_get_ipol_grp(Axis(igrp), buf)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_is_ipol_in_progress(int igrp, int *buf)
{
    int no = dsa_is_ipol_in_progress(Axis(igrp));
    buf    = &no;

    return 1;
}

int wrap_ipol_begin(int igrp)
{
    if (dsa_is_ipol_in_progress(igrp0))
        dsa_ipol_end_s(igrp0, DSA_DEF_TIMEOUT);

    if (dsa_is_ipol_in_progress(igrp1))
        dsa_ipol_end_s(igrp1, DSA_DEF_TIMEOUT);

    if (err = dsa_ipol_prepare_s(Axis(igrp), DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    if (err = dsa_ipol_begin_s(Axis(igrp), DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_abs_mode(int igrp, int mode)
{
    if (mode != 0) {
        mode = 1;
    }
    if (err = dsa_ipol_set_abs_mode_s(Axis(igrp), mode, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_abs_coords(int igrp, double pos_x, double pos_y, double pos_z)
{
    DSA_VECTOR pos = {sizeof(DSA_VECTOR)};
    pos.val.dim.x  = pos_x;
    pos.val.dim.y  = pos_y;
    pos.val.dim.z  = pos_z;
    /**pos.val.dim.t  = pos_t;*/

    if (err = dsa_ipol_abs_coords_s(Axis(igrp), &pos, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_get_user_position(int igrp_no, int *buf)
{
    int pos_x = -999999999;
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       640 + 20 * igrp_no,
                                       0,
                                       &pos_x,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }
    int pos_y = -999999999;
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       642 + 20 * igrp_no,
                                       0,
                                       &pos_y,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }
    int pos_z = -999999999;
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       644 + 20 * igrp_no,
                                       0,
                                       &pos_z,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    buf[0] = pos_x;
    buf[1] = pos_y;
    buf[2] = pos_z;

    printf("[wrap] Now User Position: %d, %d, %d\n", buf[0], buf[1], buf[2]);

    return 1;
}

int wrap_ipol_get_real_position(int igrp_no, int *buf)
{
    int pos_x = -999999999;
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       641 + 20 * igrp_no,
                                       0,
                                       &pos_x,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }
    int pos_y = -999999999;
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       643 + 20 * igrp_no,
                                       0,
                                       &pos_y,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }
    int pos_z = -999999999;
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       645 + 20 * igrp_no,
                                       0,
                                       &pos_z,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        int pos_z = -999999999;
        return error_action();
    }

    buf[0] = pos_x;
    buf[1] = pos_y;
    buf[2] = pos_z;

    printf("[wrap] Now Real Position: %d, %d, %d\n", buf[0], buf[1], buf[2]);

    return 1;
}

int wrap_ipol_set_tan_velocity(int igrp, double velocity)
{
    if (err = dsa_ipol_tan_velocity_s(Axis(igrp), velocity, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_set_tan_acceleration(int igrp, double acceleration)
{
    if (err = dsa_ipol_tan_acceleration_s(Axis(igrp), acceleration, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_set_tan_jerk_time(int igrp, double deceleration)
{
    if (err = dsa_ipol_tan_jerk_time_s(Axis(igrp), deceleration, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_set_tan_speed(int igrp, double velocity, double acceleration, double jerk)
{
    if (err = dsa_ipol_tan_velocity_s(Axis(igrp), velocity, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }
    if (err = dsa_ipol_tan_acceleration_s(Axis(igrp), acceleration, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }
    if (err = dsa_ipol_tan_jerk_time_s(Axis(igrp), jerk, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_set_tan_deceleration(int igrp, double deceleration)
{
    if (err = dsa_ipol_tan_deceleration_s(Axis(igrp), deceleration, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_line(int igrp, double pos_x, double pos_y, double pos_z)
{
    DSA_VECTOR dest = {sizeof(DSA_VECTOR)};
    dest.val.dim.x  = pos_x;
    dest.val.dim.y  = pos_y;
    dest.val.dim.z  = pos_z;

    if (err = dsa_ipol_line_s(Axis(igrp), &dest, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_pt(int igrp, double pos_x, double pos_y, double pos_z, double atime)
{
    DSA_VECTOR dest = {sizeof(DSA_VECTOR)};
    dest.val.dim.x  = pos_x;
    dest.val.dim.y  = pos_y;
    dest.val.dim.z  = pos_z;
    if (err = dsa_ipol_pt_s(Axis(igrp), &dest, atime, DSA_DEF_TIMEOUT)) {
    }
}

int wrap_ipol_wait_movement(int igrp)
{
    if (err = dsa_ipol_wait_movement_s(Axis(igrp), DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_exec_sequence(int igrp, int no)
{
    if (err = dsa_set_register_int32_s(Axis(igrp), DMD_TYP_USER_INT32, SEQ_FLAG, 0, 1, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, igrp0);
        return error_action();
    }
    if (err = dsa_execute_sequence_s(igrp0, no, 10000)) {
        DSA_EXT_DIAG(err, igrp0);
        return error_action();
    }

    return 1;
}

int wrap_ipol_wait_sequence(int igrp)
{
    DSA_STATUS status = {sizeof(DSA_STATUS)};
    while (1) {
        if (err = dsa_get_status_from_drive(ultimet, &status, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }

        if (DEBUG) {
            printf("seq. executing => %d\n", status.sw.sw1.exec_seq);
        }
        if (status.sw.sw1.exec_seq == 0) {
            break;
        }
        else {
            Sleep(1000);
        }
    }

    return 1;
}

int wrap_ipol_halt_moving(int igrp)
{
    // TODO: need to check out mode and flag
    // dsa_ipol_quick_stop_s(Axis(axis), DSA_QS_INFINITE_DEC, DSA_QS_STOP_SEQUENCE, DSA_DEF_TIMEOUT);
    return 1;
}

int wrap_ipol_halt_sequence(int igrp)
{
    // TODO:
    dsa_stop_sequence_s(ultimet, DSA_DEF_TIMEOUT);
    return 1;
}

int wrap_ipol_scale(int igrp, double scale_factor_x, double scale_factor_y, double scale_factor_z)
{
    DSA_VECTOR scale    = {sizeof(DSA_VECTOR)};
    scale.val.dim.x     = scale_factor_x;
    scale.val.dim.y     = scale_factor_y;
    scale.val.dim.z     = scale_factor_z;
    scale.val.dim.theta = 1000000;

    if (err = dsa_ipol_scale_matrix_s(Axis(igrp), &scale, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_translate(int igrp, double trans_x, double trans_y, double trans_z)
{
    DSA_VECTOR trans    = {sizeof(DSA_VECTOR)};
    trans.val.dim.x     = trans_x;
    trans.val.dim.y     = trans_y;
    trans.val.dim.z     = trans_z;
    trans.val.dim.theta = 0;

    dsa_ipol_translate_matrix_s(Axis(igrp), &trans, DSA_DEF_TIMEOUT);
    return 1;
}

int wrap_ipol_rotate(int igrp, double degree)
{
    if (err = dsa_ipol_rotate_matrix_s(Axis(igrp), DSA_ROTATION_PLAN_XY, degree, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_shear(int igrp)
{
    return 1;
}

int wrap_ipol_end(int igrp)
{
    if (err = dsa_ipol_end_s(Axis(igrp), DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

//=================================================================================
// Wrap command function with increment
//=================================================================================
int wrap_get_position_increment(int axis, int *buf)
{
    if (axis == 0) {
        int pos_x = -999999999;
        if (err = dsa_get_register_int32_s(x, DMD_TYP_MONITOR_INT32, 7, 0, &pos_x, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        int pos_y = -999999999;
        if (err = dsa_get_register_int32_s(y, DMD_TYP_MONITOR_INT32, 7, 0, &pos_y, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        int pos_z = -999999999;
        if (err = dsa_get_register_int32_s(z, DMD_TYP_MONITOR_INT32, 7, 0, &pos_z, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        buf[0] = pos_x;
        buf[1] = pos_y;
        buf[2] = pos_z;
    }
    else {
        int pos = -999999999;
        if (err = dsa_get_register_int32_s(Axis(axis),
                                           DMD_TYP_MONITOR_INT32,
                                           7,
                                           0,
                                           &pos,
                                           DSA_GET_CURRENT,
                                           DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        buf[0] = pos;
    }
    if (DEBUG) {
        printf("[wrap] Now Position(inc): %d, %d, %d\n", buf[0], buf[1], buf[2]);
    }

    return 1;
}

int wrap_cmd_mve(int axis, int inc_pos)
{
    printf("MVE: %d, %d\n", axis, inc_pos);
    if (err = dsa_execute_command_d_s(Axis(axis), CMD_MVE, DMD_TYP_IMMEDIATE_INT32, inc_pos, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_cmd_spd(int axis, int inc_spd)
{
    __int64 value = inc_spd * (1024000 * 0.000001);
    if (err = dsa_set_register_int64_s(Axis(axis), DMD_TYP_PPK_INT64, 211, 0, value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    return 1;
}

int wrap_cmd_acc(int axis, int inc_acc)
{
    __int64 value = inc_acc * (104858 * 0.000001);
    if (err = dsa_set_register_int64_s(Axis(axis), DMD_TYP_PPK_INT64, 212, 0, value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    return 1;
}

int wrap_cmd_jrt(int axis, int msec_jrt)
{
    int value = (int)(msec_jrt * 0.5);
    if (err = dsa_set_register_int32_s(Axis(axis), DMD_TYP_PPK_INT32, 213, 0, value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    return 1;
}

int wrap_cmd_get_spd(int axis, __int64 *buf)
{
    __int64 value = 0;
    if (err = dsa_get_register_int64_s(Axis(axis), DMD_TYP_PPK_INT64, 211, 0, buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    /**value = value / (1024000 * 0.000001);*/
    /**printf("%lld\n", value);*/
    /**buf = value;*/
    return 1;
}

int wrap_cmd_get_acc(int axis, __int64 *buf)
{
    __int64 value = 0;
    if (err = dsa_get_register_int64_s(Axis(axis), DMD_TYP_PPK_INT64, 212, 0, buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    /**value = value / (104858 * 0.000001);*/
    /**buf = &value;*/
    return 1;
}

int wrap_cmd_get_jrt(int axis, int *buf)
{
    int value = 0;
    if (err = dsa_get_register_int32_s(Axis(axis), DMD_TYP_PPK_INT32, 213, 0, buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    /**buf = &value;*/
    return 1;
}

int wrap_cmd_mve_v(int axis, int inc_pos, int inc_velocity)
{
    if (err = dsa_execute_command_dd_s(Axis(axis),
                                       CMD_IABSMODE,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       inc_pos,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       inc_velocity,
                                       0,
                                       0,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_cmd_mve_v_a(int axis, int inc_pos, int inc_velocity, int inc_acceleration)
{
    DSA_COMMAND_PARAM param[3];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = inc_pos;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = inc_velocity;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = inc_acceleration;

    if (err = dsa_execute_command_x_s(Axis(axis), CMD_MVE, param, 3, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
}

int wrap_cmd_rmve(int axis, int inc_length)
{
    if (err =
            dsa_execute_command_d_s(Axis(axis), CMD_RMVE, DMD_TYP_IMMEDIATE_INT32, inc_length, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_cmd_wtm(int axis)
{
    /**if (err = dsa_execute_command_s(Axis(axis), CMD_WTM, 0, TRUE, DSA_DEF_TIMEOUT)) {*/
    /**    DSA_EXT_DIAG(err, Axis(axis));*/
    /**    return error_action();*/
    /**}*/

    if (err = dsa_execute_command_s(Axis(axis), CMD_WTM, 0, TRUE, 3000000)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }
    return 1;
}

int wrap_cmd_wtt(int axis, int sec_atime)
{
    if (err = dsa_execute_command_d_s(Axis(axis),
                                      CMD_WTT,
                                      DMD_TYP_IMMEDIATE_INT32,
                                      sec_atime,
                                      FALSE,
                                      FALSE,
                                      DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_cmd_set(int axis, int inc_pos)
{
    if (err = dsa_execute_command_d_s(Axis(axis),
                                      CMD_SET,
                                      DMD_TYP_IMMEDIATE_INT32,
                                      inc_pos,
                                      FALSE,
                                      FALSE,
                                      DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_cmd_iset(int igrp_no)
{
    DSA_COMMAND_PARAM param[5];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = 0;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = 2;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = 1;

    param[3].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[3].conv  = 0;
    param[3].val.i = 4;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = 0;

    if (err = dsa_execute_command_x_s(ultimet, CMD_ISET, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_itspd(int igrp_no, int inc_spd)
{
    // M649, M669
    if (err = dsa_execute_command_dd_s(ultimet,
                                       CMD_ITSPD,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       igrp_no,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       inc_spd,
                                       0,
                                       1,
                                       3000000)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_itacc(int igrp_no, int inc_acc)
{
    // M650, M670
    if (err = dsa_execute_command_dd_s(ultimet,
                                       CMD_ITACC,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       igrp_no,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       inc_acc,
                                       0,
                                       1,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_itdec(int igrp_no, int inc_dec)
{
    // M651, M671
    if (err = dsa_execute_command_dd_s(ultimet,
                                       CMD_ITDEC,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       igrp_no,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       inc_dec,
                                       0,
                                       1,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_itjrt(int igrp_no, int inc_jrt)
{
    // M652, M672
    if (err = dsa_execute_command_dd_s(ultimet,
                                       CMD_ITJRT,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       igrp_no,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       inc_jrt,
                                       0,
                                       1,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_get_itspd(int igrp_no, int *buf)
{
    // M649, M669
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       649 + igrp_no * 20,
                                       0,
                                       buf,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }
    return 1;
}

int wrap_cmd_get_itacc(int igrp_no, int *buf)
{
    // M650, M670
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       650 + igrp_no * 20,
                                       0,
                                       buf,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }
    return 1;
}

int wrap_cmd_get_itdec(int igrp_no, int *buf)
{
    // M651, M671
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       651 + igrp_no * 20,
                                       0,
                                       buf,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }
    return 1;
}

int wrap_cmd_get_itjrt(int igrp_no, int *buf)
{
    // M652, M672
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       652 + igrp_no * 20,
                                       0,
                                       buf,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }
    return 1;
}

int wrap_cmd_ibegin(int igrp_no)
{
    if (err = dsa_execute_command_d_s(ultimet, CMD_IBEGIN, DMD_TYP_IMMEDIATE_INT32, igrp_no, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_iabsmode(int igrp_no, int inc_value)
{
    /**if (igrp_no < 0 || igrp_no > 1)*/
    /**    return -1;*/

    if (err = dsa_execute_command_dd_s(ultimet,
                                       CMD_IABSMODE,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       igrp_no,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       inc_value,
                                       0,
                                       1,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_iend(int igrp_no)
{
    if (err = dsa_execute_command_d_s(ultimet, CMD_IEND, DMD_TYP_IMMEDIATE_INT32, igrp_no, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_iabscoords(int igrp_no, int inc_pos_x, int inc_pos_y, int inc_pos_z)
{
    DSA_COMMAND_PARAM param[5];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = igrp_no;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = inc_pos_x;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = inc_pos_y;

    param[3].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[3].conv  = 0;
    param[3].val.i = inc_pos_z;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = 0;

    if (err = dsa_execute_command_x_s(ultimet, CMD_IABSCOORDS, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_iline(int igrp_no, int inc_pos_x, int inc_pos_y, int inc_pos_z)
{
    DSA_COMMAND_PARAM param[5];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = igrp_no;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = inc_pos_x;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = inc_pos_y;

    param[3].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[3].conv  = 0;
    param[3].val.i = inc_pos_z;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = 0;

    if (err = dsa_execute_command_x_s(ultimet, CMD_ILINE, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_ipt(int igrp_no, int inc_pos_x, int inc_pos_y, int inc_pos_z, int atime)
{
    DSA_COMMAND_PARAM param[6];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = igrp_no;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = inc_pos_x;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = inc_pos_y;

    param[3].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[3].conv  = 0;
    param[3].val.i = inc_pos_z;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = 0;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = atime;

    if (err = dsa_execute_command_x_s(ultimet, CMD_IPT, param, 6, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_ipvt(int igrp_no,
                  int inc_pos_x,
                  int inc_pos_y,
                  int inc_pos_z,
                  int inc_vel_x,
                  int inc_vel_y,
                  int inc_vel_z,
                  int atime)
{
    DSA_COMMAND_PARAM param[10];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = igrp_no;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = inc_pos_x;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = inc_pos_y;

    param[3].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[3].conv  = 0;
    param[3].val.i = inc_pos_z;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = 0;

    param[5].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[5].conv  = 0;
    param[5].val.i = inc_vel_x;

    param[6].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[6].conv  = 0;
    param[6].val.i = inc_vel_y;

    param[7].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[7].conv  = 0;
    param[7].val.i = inc_vel_z;

    param[8].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[8].conv  = 0;
    param[8].val.i = 0;

    param[9].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[9].conv  = 0;
    param[9].val.i = atime;

    if (err = dsa_execute_command_x_s(ultimet, CMD_IPVT, param, 6, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_iwtm(int igrp_no)
{
    if (err = dsa_execute_command_d_s(ultimet, CMD_WTM, DMD_TYP_IMMEDIATE_INT32, igrp_no, 0, TRUE, 3000000)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_iwtt(int igrp_no, int s_atime)
{
    if (err = dsa_execute_command_dd_s(ultimet,
                                       CMD_IWTT,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       igrp_no,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       s_atime,
                                       0,
                                       TRUE,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_imtrans(int igrp_no, int inc_trans_x, int inc_trans_y, int inc_trans_z)
{
    DSA_COMMAND_PARAM param[5];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = igrp_no;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = inc_trans_x;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = inc_trans_y;

    param[3].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[3].conv  = 0;
    param[3].val.i = inc_trans_z;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = 0;

    if (err = dsa_execute_command_x_s(ultimet, CMD_IMTRANS, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_imscale(int igrp_no, int inc_scale_x, int inc_scale_y, int inc_scale_z)
{
    DSA_COMMAND_PARAM param[5];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = igrp_no;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = inc_scale_x;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = inc_scale_y;

    param[3].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[3].conv  = 0;
    param[3].val.i = inc_scale_z;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = 1000000;

    if (err = dsa_execute_command_x_s(ultimet, CMD_IMSCALE, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_imrot(int igrp_no, int micro_dgree)
{
    DSA_COMMAND_PARAM param[3];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = igrp_no;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = 0;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = micro_dgree;

    if (err = dsa_execute_command_x_s(ultimet, CMD_IMROT, param, 3, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }
    return 1;
}

int wrap_cmd_imshear(int igrp_no, int inc_shear_x, int inc_shear_y, int inc_shear_z)
{
    DSA_COMMAND_PARAM param[5];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = igrp_no;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = inc_shear_x;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = inc_shear_y;

    param[3].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[3].conv  = 0;
    param[3].val.i = inc_shear_z;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = 0;

    if (err = dsa_execute_command_x_s(ultimet, CMD_IMSHEAR, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}
int wrap_cmd_imres(int igrp_no)
{

    if (err = dsa_execute_command_d_s(ultimet, CMD_IMRES, DMD_TYP_IMMEDIATE_INT32, igrp_no, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_jmp(int axis, int no, int thread)
{
    if (err = dsa_execute_command_dd_s(Axis(axis),
                                       CMD_JMP,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       no,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       thread,
                                       0,
                                       1,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_cmd_jmp_a(int axis, int no, int thread)
{
    if (err = dsa_execute_command_dd_a(Axis(axis),
                                       CMD_JMP,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       no,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       thread,
                                       0,
                                       1,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_cmd_ujmp(int no, int thread)
{
    if (err = dsa_execute_command_dd_s(ultimet,
                                       CMD_JMP,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       no,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       thread,
                                       0,
                                       1,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_ujmp_a(int no, int thread)
{
    if (err = dsa_execute_command_dd_a(ultimet,
                                       CMD_JMP,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       no,
                                       DMD_TYP_IMMEDIATE_INT32,
                                       thread,
                                       0,
                                       1,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}
// register
int wrap_set_reg_int32(int axis, int typ, int index, int depth, int value)
{
    int dmd_type = 0;
    switch (typ) {
    case 0:
        dmd_type = DMD_TYP_USER_INT32;
        break;
    case 1:
        dmd_type = DMD_TYP_PPK_INT32;
        break;
    default:
        dmd_type = DMD_TYP_USER_INT32;
        break;
    }
    if (err = dsa_set_register_int32_s(Axis(axis), dmd_type, index, depth, value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_reg_int64(int axis, int typ, int index, int depth, long value)
{
    int dmd_type = 0;
    switch (typ) {
    case 0:
        dmd_type = DMD_TYP_USER_INT64;
        break;
    case 1:
        dmd_type = DMD_TYP_PPK_INT64;
        break;
    default:
        dmd_type = DMD_TYP_USER_INT64;
        break;
    }
    if (err = dsa_set_register_int64_s(Axis(axis), dmd_type, index, depth, value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_reg_float32(int axis, int typ, int index, int depth, float value)
{
    int dmd_type = 0;
    switch (typ) {
    case 0:
        dmd_type = DMD_TYP_USER_FLOAT32;
        break;
    case 1:
        dmd_type = DMD_TYP_PPK_FLOAT32;
        break;
    default:
        dmd_type = DMD_TYP_USER_FLOAT32;
        break;
    }
    if (err = dsa_set_register_float32_s(Axis(axis), dmd_type, index, depth, value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_reg_float64(int axis, int typ, int index, int depth, double value)
{
    int dmd_type = 0;
    switch (typ) {
    case 0:
        dmd_type = DMD_TYP_USER_FLOAT64;
        break;
    case 1:
        dmd_type = DMD_TYP_PPK_FLOAT64;
        break;
    default:
        dmd_type = DMD_TYP_USER_FLOAT64;
        break;
    }
    if (err = dsa_set_register_float64_s(Axis(axis), dmd_type, index, depth, value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_get_reg_int32(int axis, int typ, int index, int depth, int *buf)
{
    int dmd_type = 0;
    switch (typ) {
    case 0:
        dmd_type = DMD_TYP_USER_INT32;
        break;
    case 1:
        dmd_type = DMD_TYP_PPK_INT32;
        break;
    case 2:
        dmd_type = DMD_TYP_MONITOR_INT32;
        break;
    default:
        dmd_type = DMD_TYP_USER_INT32;
        break;
    }
    if (err = dsa_get_register_int32_s(Axis(axis), dmd_type, index, depth, buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_get_reg_int64(int axis, int typ, int index, int depth, __int64 *buf)
{
    int dmd_type = 0;
    switch (typ) {
    case 0:
        dmd_type = DMD_TYP_USER_INT64;
        break;
    case 1:
        dmd_type = DMD_TYP_PPK_INT64;
        break;
    case 2:
        dmd_type = DMD_TYP_MONITOR_INT64;
        break;
    default:
        dmd_type = DMD_TYP_USER_INT64;
        break;
    }
    __int64 value = 0;
    if (err = dsa_get_register_int64_s(Axis(axis), dmd_type, index, depth, buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_get_reg_float32(int axis, int typ, int index, int depth, float *buf)
{
    int dmd_type = 0;
    switch (typ) {
    case 0:
        dmd_type = DMD_TYP_USER_FLOAT32;
        break;
    case 1:
        dmd_type = DMD_TYP_PPK_FLOAT32;
        break;
    case 2:
        dmd_type = DMD_TYP_MONITOR_FLOAT32;
        break;
    default:
        dmd_type = DMD_TYP_USER_FLOAT32;
        break;
    }
    if (err = dsa_get_register_float32_s(Axis(axis), dmd_type, index, depth, buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_get_reg_float64(int axis, int typ, int index, int depth, double *buf)
{
    int dmd_type = 0;
    switch (typ) {
    case 0:
        dmd_type = DMD_TYP_USER_FLOAT64;
        break;
    case 1:
        dmd_type = DMD_TYP_PPK_FLOAT64;
        break;
    case 2:
        dmd_type = DMD_TYP_MONITOR_FLOAT64;
        break;
    default:
        dmd_type = DMD_TYP_USER_FLOAT64;
        break;
    }
    if (err = dsa_get_register_float64_s(Axis(axis), dmd_type, index, depth, buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_cmd_ch_bit_reg32(int axis, int index, int mask, int value)
{
    DSA_COMMAND_PARAM param[3];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = index;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = mask;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = value;

    if (err = dsa_execute_command_x_s(Axis(axis), CMD_CH_BIT_REG32, param, 3, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }
    return 1;
}

int wrap_cmd_clx(int axis)
{
    if (axis == 0) {
        if (err = dsa_execute_command_d_s(x, CMD_CLX, DMD_TYP_IMMEDIATE_INT32, 0, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        if (err = dsa_execute_command_d_s(y, CMD_CLX, DMD_TYP_IMMEDIATE_INT32, 0, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        if (err = dsa_execute_command_d_s(z, CMD_CLX, DMD_TYP_IMMEDIATE_INT32, 0, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
        if (err = dsa_execute_command_d_s(ultimet, CMD_CLX, DMD_TYP_IMMEDIATE_INT32, 0, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
    }
    else {
        if (err = dsa_execute_command_d_s(Axis(axis), CMD_CLX, DMD_TYP_IMMEDIATE_INT32, 0, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, Axis(axis));
            return error_action();
        }
    }
    return 1;
}

//=================================================================================
// Util
//=================================================================================
int wrap_convert_int32_from_iso(int axis, double value, int conv, int *buf)
{
    // conv: 10 ; 1.0 m -> 10000000 increment(100nm)
    if (err = dsa_convert_int32_from_iso(Axis(axis), buf, value, conv)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_convert_int32_to_iso(int axis, int value, int conv, double *buf)
{
    // conv: 10 ; 10000000 increment(100nm) -> 1.0 m
    if (err = dsa_convert_int32_to_iso(Axis(axis), buf, value, conv)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_convert_int64_from_iso(int axis, double value, int conv, __int64 *buf)
{
    // conv: 10 ; 1.0 m -> 10000000 increment(100nm)
    if (err = dsa_convert_int64_from_iso(Axis(axis), buf, value, conv)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_convert_int64_to_iso(int axis, __int64 value, int conv, double *buf)
{
    // conv: 10 ; 10000000 increment(100nm) -> 1.0 m
    if (err = dsa_convert_int64_to_iso(Axis(axis), buf, value, conv)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_convert_float32_from_iso(int axis, double value, int conv, float *buf)
{
    // conv: 10 ; 1.0 m -> 10000000 increment(100nm)
    if (err = dsa_convert_float32_from_iso(Axis(axis), buf, value, conv)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_convert_float32_to_iso(int axis, float value, int conv, double *buf)
{
    // conv: 10 ; 10000000 increment(100nm) -> 1.0 m
    /**temp = (float) value;*/
    if (err = dsa_convert_float32_to_iso(Axis(axis), buf, value, conv)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_convert_float64_from_iso(int axis, double value, int conv, double *buf)
{
    // conv: 10 ; 1.0 m -> 10000000 increment(100nm)
    if (err = dsa_convert_float64_from_iso(Axis(axis), buf, value, conv)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_convert_float64_to_iso(int axis, double value, int conv, double *buf)
{
    // conv: 10 ; 10000000 increment(100nm) -> 1.0 m
    if (err = dsa_convert_float64_to_iso(Axis(axis), buf, value, conv)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_inc2iso(int axis, int value, double *buf)
{
    if (err = dsa_convert_int32_to_iso(Axis(axis), buf, value, 10)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_iso2inc(int axis, double value, int *buf)
{
    // conv: 10 ; 1.0 m -> 10000000 increment(100nm)
    int inc = 0;
    if (err = dsa_convert_int32_from_iso(Axis(axis), buf, value, 10)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

//=================================================================================
// digital input / output
//=================================================================================

// Axis digital input / output
int wrap_digital_input_status(int axis, int *buf)
{
    if (err = dsa_get_digital_input_s(Axis(axis), buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_digital_output_status(int axis, int *buf)
{
    if (err = dsa_get_digital_output_s(Axis(axis), buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_get_digital_input_index(int axis, int index, int *buf)
{
    if (index < 0 || index > 4) {
        return -1;
    }

    int value;
    if (err = dsa_get_digital_input_s(Axis(axis), &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    value = (value >> (index - 1)) & 1U;
    buf   = &value;

    return 1;
}

int wrap_get_digital_output_index(int axis, int index, int *buf)
{
    if (index < 0 || index > 2) {
        return -1;
    }

    int value;
    if (err = dsa_get_digital_output_s(Axis(axis), &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    value = (value >> (index - 1)) & 1U;
    buf   = &value;

    return 1;
}

int wrap_set_digital_output_index(int axis, int index, int onoff)
{
    if (index < 0 || index > 2) {
        return -1;
    }

    int value;
    if (err = dsa_get_digital_output_s(Axis(axis), &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    if (onoff == 0) {
        value &= ~(1UL << (index - 1));
    }
    else {
        value |= 1UL << (index - 1);
    }
    if (err = dsa_set_digital_output_s(Axis(axis), value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_digital_output_on_index(int axis, int index)
{
    if (index < 0 || index > 2) {
        return -1;
    }

    int value;
    if (err = dsa_get_digital_output_s(Axis(axis), &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    value |= 1UL << (index - 1);

    if (err = dsa_set_digital_output_s(Axis(axis), value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_digital_output_off_index(int axis, int index)
{
    if (index < 0 || index > 2) {
        return -1;
    }

    int value;
    if (err = dsa_get_digital_output_s(Axis(axis), &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    value &= ~(1UL << (index - 1));

    if (err = dsa_set_digital_output_s(Axis(axis), value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

//  Controller digital input / output
int wrap_get_fast_digital_input_status(int axis, int *buf)
{
    if (err = dsa_get_fast_digital_input_s(Axis(axis), buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_get_fast_digital_output_status(int axis, int *buf)
{
    if (err = dsa_get_fast_digital_output_s(Axis(axis), buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_get_fast_digital_input_index(int axis, int index, int *buf)
{
    if (index < 0 || index > 4) {
        return -1;
    }

    int value;
    if (err = dsa_get_fast_digital_input_s(Axis(axis), &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    value = (value >> (index - 1)) & 1U;
    buf   = &value;

    return 1;
}

int wrap_get_fast_digital_output_index(int axis, int index, int *buf)
{
    if (index < 0 || index > 4) {
        return -1;
    }

    int value;
    if (err = dsa_get_fast_digital_output_s(Axis(axis), &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    value = (value >> (index - 1)) & 1U;
    buf   = &value;

    return 1;
}

int wrap_set_fast_digital_output_index(int axis, int index, int onoff)
{
    if (index < 0 || index > 4) {
        return -1;
    }

    int value;
    if (err = dsa_get_fast_digital_output_s(Axis(axis), &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    if (onoff == 0) {
        value &= ~(1UL << (index - 1));
    }
    else {
        value |= 1UL << (index - 1);
    }

    if (err = dsa_set_fast_digital_output_s(Axis(axis), value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_fast_digital_output_on_index(int axis, int index)
{
    if (index < 0 || index > 4) {
        return -1;
    }

    int value;
    if (err = dsa_get_fast_digital_output_s(Axis(axis), &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    value |= 1UL << (index - 1);

    if (err = dsa_set_fast_digital_output_s(Axis(axis), value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

int wrap_set_fast_digital_output_off_index(int axis, int index)
{
    if (index < 0 || index > 4) {
        return -1;
    }

    int value;
    if (err = dsa_get_fast_digital_output_s(Axis(axis), &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    value &= ~(1UL << (index - 1));

    if (err = dsa_set_fast_digital_output_s(Axis(axis), value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(axis));
        return error_action();
    }

    return 1;
}

// Ultimet (master) digital input / output
int wrap_ultimet_digital_input_status(int *buf)
{
    if (err = dsa_get_digital_input_s(ultimet, buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_ultimet_digital_output_status(int *buf)
{
    if (err = dsa_get_digital_output_s(ultimet, buf, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_get_ultimet_digital_input_index(int index, int *buf)
{
    if (index < 0 || index > 2) {
        return -1;
    }

    int value;
    if (err = dsa_get_digital_input_s(ultimet, &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    value = (value >> (index - 1)) & 1U;
    buf   = &value;

    return 1;
}

int wrap_get_ultimet_digital_output_index(int index, int *buf)
{
    if (index < 0 || index > 2) {
        return -1;
    }

    int value;
    if (err = dsa_get_digital_output_s(ultimet, &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    value = (value >> (index - 1)) & 1U;
    buf   = &value;

    return 1;
}

int wrap_set_ultimet_digital_output_index(int index, int onoff)
{
    if (index < 0 || index > 2) {
        return -1;
    }

    int value;
    if (err = dsa_get_digital_output_s(ultimet, &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    if (onoff == 0) {
        value &= ~(1UL << (index - 1));
    }
    else {
        value |= 1UL << (index - 1);
    }
    if (err = dsa_set_digital_output_s(ultimet, value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_set_ultimet_digital_output_on_index(int index)
{
    if (index < 0 || index > 2) {
        return -1;
    }

    int value;
    if (err = dsa_get_digital_output_s(ultimet, &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    value |= 1UL << (index - 1);

    if (err = dsa_set_digital_output_s(ultimet, value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_set_ultimet_digital_output_off_index(int index)
{
    if (index < 0 || index > 2) {
        return -1;
    }

    int value;
    if (err = dsa_get_digital_output_s(ultimet, &value, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    value &= ~(1UL << (index - 1));

    if (err = dsa_set_digital_output_s(ultimet, value, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }
    return 1;
}

// normal(iso) enhanced wrapping
int wrap_move_xyz_abs(double pos_x, double pos_y, double pos_z, int wait)
{
    if (err = dsa_set_target_position_s(x, 0, pos_x, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, x);
        return error_action();
    }
    if (err = dsa_set_target_position_s(y, 0, pos_y, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, y);
        return error_action();
    }
    if (err = dsa_set_target_position_s(z, 0, pos_z, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, z);
        return error_action();
    }

    if (wait != 0) {
        dsa_wait_movement_s(xyz, 3000000);
    }
    return 1;
}

int wrap_move_xy_abs(double pos_x, double pos_y, int wait)
{
    /**printf("axis: xy, target position: %f, %f\n", pos_x, pos_y);*/
    if (err = dsa_set_target_position_s(x, 0, pos_x, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, x);
        return error_action();
    }
    if (err = dsa_set_target_position_s(y, 0, pos_y, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, y);
        return error_action();
    }

    if (wait != 0) {
        dsa_wait_movement_s(xy, 3000000);
    }

    return 1;
}

int wrap_move_z_abs(double pos_z, int wait)
{
    if (err = dsa_set_target_position_s(z, 0, pos_z, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, z);
        return error_action();
    }

    if (wait != 0) {
        dsa_wait_movement_s(z, 3000000);
    }

    return 1;
}

// ipol(iso) enhanced wrapping
int wrap_ipol_move_xyz_abs(int igrp, double pos_x, double pos_y, double pos_z)
{
    DSA_VECTOR dest = {sizeof(DSA_VECTOR)};
    dest.val.dim.x  = pos_x;
    dest.val.dim.y  = pos_y;
    dest.val.dim.z  = pos_z;

    if (err = dsa_ipol_line_s(Axis(igrp), &dest, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    /**if (wait != 0) {*/
    /**    if (err = dsa_execute_command_s(xyz, CMD_WTM, 0, TRUE, 3000000)) {*/
    /**        DSA_EXT_DIAG(err, xyz);*/
    /**        return error_action();*/
    /**    }*/
    /**}*/

    return 1;
}

int wrap_ipol_move_xy_abs(int igrp, double pos_x, double pos_y)
{
    double inc_z = -999999999.0;

    if (err = dsa_get_position_actual_value_s(z, &inc_z, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, z);
        return error_action();
    }

    double pos_z = -999999999.0;

    if (err = dsa_convert_float64_to_iso(Axis(igrp), &pos_z, inc_z, 10)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    DSA_VECTOR dest = {sizeof(DSA_VECTOR)};
    dest.val.dim.x  = pos_x;
    dest.val.dim.y  = pos_y;
    dest.val.dim.z  = pos_z;

    if (err = dsa_ipol_line_s(Axis(igrp), &dest, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_ipol_move_z_abs(int igrp, double pos_z)
{
    double inc_x = -999999999.0;

    if (err = dsa_get_position_actual_value_s(x, &inc_x, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, z);
        return error_action();
    }

    double pos_x = -999999999.0;

    if (err = dsa_convert_float64_to_iso(Axis(igrp), &pos_x, inc_x, 10)) {
        DSA_EXT_DIAG(err, igrp0);
        return error_action();
    }

    double inc_y = -999999999.0;

    if (err = dsa_get_position_actual_value_s(y, &inc_y, DSA_GET_CURRENT, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, y);
        return error_action();
    }

    double pos_y = -999999999.0;

    if (err = dsa_convert_float64_to_iso(Axis(igrp), &pos_y, inc_y, 10)) {
        DSA_EXT_DIAG(err, igrp0);
        return error_action();
    }

    DSA_VECTOR dest = {sizeof(DSA_VECTOR)};
    dest.val.dim.x  = pos_x;
    dest.val.dim.y  = pos_y;
    dest.val.dim.z  = pos_z;

    if (err = dsa_ipol_line_s(Axis(igrp), &dest, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, Axis(igrp));
        return error_action();
    }

    return 1;
}

int wrap_cmd_move_xyz_abs(int inc_pos_x, int inc_pos_y, int inc_pos_z, int all_axis, int safe, int safe_pos_z)
{
    if (all_axis == 0) {
        if (safe != 0) {
            if (err = dsa_execute_command_d_s(z, CMD_MVE, DMD_TYP_IMMEDIATE_INT32, safe_pos_z, 0, 0, DSA_DEF_TIMEOUT)) {
                DSA_EXT_DIAG(err, z);
                return error_action();
            }

            if (err = dsa_execute_command_s(z, CMD_WTM, 0, TRUE, 3000000)) {
                DSA_EXT_DIAG(err, z);
                return error_action();
            }
        }

        if (err = dsa_execute_command_d_s(x, CMD_MVE, DMD_TYP_IMMEDIATE_INT32, inc_pos_x, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }

        if (err = dsa_execute_command_d_s(y, CMD_MVE, DMD_TYP_IMMEDIATE_INT32, inc_pos_y, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }

        if (err = dsa_execute_command_s(xy, CMD_WTM, 0, TRUE, 3000000)) {
            DSA_EXT_DIAG(err, xy);
            return error_action();
        }

        if (err = dsa_execute_command_d_s(z, CMD_MVE, DMD_TYP_IMMEDIATE_INT32, inc_pos_z, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, z);
            return error_action();
        }
    }
    else {
        if (err = dsa_execute_command_d_s(x, CMD_MVE, DMD_TYP_IMMEDIATE_INT32, inc_pos_x, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, x);
            return error_action();
        }
        if (err = dsa_execute_command_d_s(y, CMD_MVE, DMD_TYP_IMMEDIATE_INT32, inc_pos_y, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, y);
            return error_action();
        }
        if (err = dsa_execute_command_d_s(z, CMD_MVE, DMD_TYP_IMMEDIATE_INT32, inc_pos_z, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, z);
            return error_action();
        }
    }
    return 1;
}

int wrap_cmd_move_xy_abs(int inc_pos_x, int inc_pos_y)
{

    int now_inc_pos_x = -999999999;
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       7,
                                       0,
                                       &now_inc_pos_x,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    int now_inc_pos_y = -999999999;
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       7,
                                       0,
                                       &now_inc_pos_y,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    if (err = dsa_execute_command_d_s(x, CMD_MVE, DMD_TYP_IMMEDIATE_INT32, inc_pos_x, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    if (err = dsa_execute_command_d_s(y, CMD_MVE, DMD_TYP_IMMEDIATE_INT32, inc_pos_y, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_cmd_move_z_abs(int inc_pos_z)
{

    if (err = dsa_execute_command_d_s(z, CMD_MVE, DMD_TYP_IMMEDIATE_INT32, inc_pos_z, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

// ipol cmd(inc) enhanced wrapping
int wrap_cmd_ipol_move_xyz_abs(int igrp_no,
                               int inc_pos_x,
                               int inc_pos_y,
                               int inc_pos_z,
                               int all_axis,
                               int safe,
                               int safe_pos_z)
{
    DSA_COMMAND_PARAM param[5];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = igrp_no;

    param[1].typ  = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv = 0;

    param[2].typ  = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv = 0;

    param[3].typ  = DMD_TYP_IMMEDIATE_INT32;
    param[3].conv = 0;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = 0;

    if (all_axis != 0) {
        param[1].val.i = inc_pos_x;
        param[2].val.i = inc_pos_y;
        param[3].val.i = inc_pos_z;

        if (err = dsa_execute_command_x_s(ultimet, CMD_ILINE, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }
    }
    else {
        int now_inc_pos_x = -999999999;
        if (err = dsa_get_register_int32_s(ultimet,
                                           DMD_TYP_MONITOR_INT32,
                                           640 + 20 * igrp_no,
                                           0,
                                           &now_inc_pos_x,
                                           DSA_GET_CURRENT,
                                           DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }

        int now_inc_pos_y = -999999999;
        if (err = dsa_get_register_int32_s(ultimet,
                                           DMD_TYP_MONITOR_INT32,
                                           642 + 20 * igrp_no,
                                           0,
                                           &now_inc_pos_y,
                                           DSA_GET_CURRENT,
                                           DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }
        if (safe != 0) {
            param[1].val.i = now_inc_pos_x;
            param[2].val.i = now_inc_pos_y;
            param[3].val.i = safe_pos_z;

            if (err = dsa_execute_command_x_s(ultimet, CMD_ILINE, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
                DSA_EXT_DIAG(err, ultimet);
                return error_action();
            }

            if (err = dsa_execute_command_s(z, CMD_WTM, 0, TRUE, 3000000)) {
                DSA_EXT_DIAG(err, z);
                return error_action();
            }
        }

        int now_inc_pos_z = -999999999;
        if (err = dsa_get_register_int32_s(ultimet,
                                           DMD_TYP_MONITOR_INT32,
                                           644 + 20 * igrp_no,
                                           0,
                                           &now_inc_pos_z,
                                           DSA_GET_CURRENT,
                                           DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }
        param[1].val.i = inc_pos_x;
        param[2].val.i = inc_pos_y;
        param[3].val.i = now_inc_pos_z;

        if (err = dsa_execute_command_x_s(ultimet, CMD_ILINE, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }

        if (err = dsa_execute_command_s(xy, CMD_WTM, 0, TRUE, 3000000)) {
            DSA_EXT_DIAG(err, xy);
            return error_action();
        }

        param[1].val.i = inc_pos_x;
        param[2].val.i = inc_pos_y;
        param[3].val.i = inc_pos_z;

        if (err = dsa_execute_command_x_s(ultimet, CMD_ILINE, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
            DSA_EXT_DIAG(err, ultimet);
            return error_action();
        }
    }

    return 1;
}

int wrap_cmd_ipol_move_xy_abs(int igrp_no, int inc_pos_x, int inc_pos_y)
{
    int now_inc_pos_z = -999999999;
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       644 + 20 * igrp_no,
                                       0,
                                       &now_inc_pos_z,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    DSA_COMMAND_PARAM param[5];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = igrp_no;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = inc_pos_x;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = inc_pos_y;

    param[3].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[3].conv  = 0;
    param[3].val.i = now_inc_pos_z;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = 0;

    if (err = dsa_execute_command_x_s(ultimet, CMD_ILINE, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_ipol_cmd_move_z_abs(int igrp_no, int inc_pos_z)
{
    int now_inc_pos_x = -999999999;
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       640 + 20 * igrp_no,
                                       0,
                                       &now_inc_pos_x,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    int now_inc_pos_y = -999999999;
    if (err = dsa_get_register_int32_s(ultimet,
                                       DMD_TYP_MONITOR_INT32,
                                       642 + 20 * igrp_no,
                                       0,
                                       &now_inc_pos_y,
                                       DSA_GET_CURRENT,
                                       DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    DSA_COMMAND_PARAM param[5];

    param[0].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[0].conv  = 0;
    param[0].val.i = igrp_no;

    param[1].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[1].conv  = 0;
    param[1].val.i = now_inc_pos_x;

    param[2].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[2].conv  = 0;
    param[2].val.i = now_inc_pos_y;

    param[3].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[3].conv  = 0;
    param[3].val.i = inc_pos_z;

    param[4].typ   = DMD_TYP_IMMEDIATE_INT32;
    param[4].conv  = 0;
    param[4].val.i = 0;

    if (err = dsa_execute_command_x_s(ultimet, CMD_ILINE, param, 5, 0, 0, DSA_DEF_TIMEOUT)) {
        DSA_EXT_DIAG(err, ultimet);
        return error_action();
    }

    return 1;
}

int wrap_test(void)
{
    return 1;
}
