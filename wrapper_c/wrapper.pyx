# cython: language_level=3
# distutils: language = c
# distutils: sources = interface.c

import copy
import decimal
import time

import cython
import numpy as np
from datetime import date
import sys

limit = date(2022, 12, 31)
now = date.today()
if limit > now:
    pass
else:
    sys.exit()

# cimport cwrapper

# Axis NO.
X_AXIS_NO  =    1
Y_AXIS_NO  =    2
Z_AXIS_NO  =    4
D_AXIS_NO  =    8
ULTIMET_NO =    9

# ACCUET group NO.
ACCUET_XY =3
ACCUET_XYZ=   7
ACCUET_GRP0=    10

# Interpolation group NO.
IPOL_GRP0  =    20
IPOL_GRP1  =    21

AXIS = [1,2,4,8,9]
GRP = [3, 10, 11]
IGRP = [20, 21]

ctypedef void DSA_DEVICE_BASE
ctypedef void DSA_DEVICE
ctypedef void DSA_DEVICE_GROUP
ctypedef void DSA_DRIVE_BASE
ctypedef void DSA_DRIVE
ctypedef void DSA_DRIVE_GROUP
ctypedef void DSA_MASTER_BASE
ctypedef void DSA_MASTER
ctypedef void DSA_MASTER_GROUP
ctypedef void DSA_DSMAX_BASE
ctypedef void DSA_DSMAX
ctypedef void DSA_DSMAX_GROUP
ctypedef void DSA_IPOL_GROUP

# header start
ctypedef long long __int64
cdef extern from "interface.h":
    int error_action()
    int wrap_close(int axis)
    int wrap_cmd_acc(int axis, int inc_acc)
    int wrap_cmd_ch_bit_reg32(int axis, int index, int mask, int value)
    int wrap_cmd_clx(int axis)
    int wrap_cmd_get_acc(int axis, __int64 *buf)
    int wrap_cmd_get_itacc(int igrp_no, int *buf)
    int wrap_cmd_get_itdec(int igrp_no, int *buf)
    int wrap_cmd_get_itjrt(int igrp_no, int *buf)
    int wrap_cmd_get_itspd(int igrp_no, int *buf)
    int wrap_cmd_get_jrt(int axis, int *buf)
    int wrap_cmd_get_spd(int axis, __int64 *buf)
    int wrap_cmd_iabscoords(int igrp_no, int inc_pos_x, int inc_pos_y, int inc_pos_z)
    int wrap_cmd_iabsmode(int igrp_no, int inc_value)
    int wrap_cmd_ibegin(int igrp_no)
    int wrap_cmd_iend(int igrp_no)
    int wrap_cmd_iline(int igrp_no, int inc_pos_x, int inc_pos_y, int inc_pos_z)
    int wrap_cmd_imres(int igrp_no)
    int wrap_cmd_imrot(int igrp_no, int micro_dgree)
    int wrap_cmd_imscale(int igrp_no, int inc_scale_x, int inc_scale_y, int inc_scale_z)
    int wrap_cmd_imshear(int igrp_no, int inc_shear_x, int inc_shear_y, int inc_shear_z)
    int wrap_cmd_imtrans(int igrp_no, int inc_trans_x, int inc_trans_y, int inc_trans_z)
    int wrap_cmd_ipol_move_xy_abs(int igrp_no, int inc_pos_x, int inc_pos_y)
    int wrap_cmd_ipol_move_xyz_abs(int igrp_no,
    int wrap_cmd_ipt(int igrp_no, int inc_pos_x, int inc_pos_y, int inc_pos_z, int atime)
    int wrap_cmd_ipvt(int igrp_no,
    int wrap_cmd_iset(int igrp_no)
    int wrap_cmd_itacc(int igrp_no, int inc_acc)
    int wrap_cmd_itdec(int igrp_no, int inc_dec)
    int wrap_cmd_itjrt(int igrp_no, int inc_jrt)
    int wrap_cmd_itspd(int igrp_no, int inc_spd)
    int wrap_cmd_iwtm(int igrp_no)
    int wrap_cmd_iwtt(int igrp_no, int s_atime)
    int wrap_cmd_jmp(int axis, int no, int thread)
    int wrap_cmd_jmp_a(int axis, int no, int thread)
    int wrap_cmd_jrt(int axis, int msec_jrt)
    int wrap_cmd_move_xy_abs(int inc_pos_x, int inc_pos_y)
    int wrap_cmd_move_xyz_abs(int inc_pos_x, int inc_pos_y, int inc_pos_z, int all_axis, int safe, int safe_pos_z)
    int wrap_cmd_move_z_abs(int inc_pos_z)
    int wrap_cmd_mve(int axis, int inc_pos)
    int wrap_cmd_mve_v(int axis, int inc_pos, int inc_velocity)
    int wrap_cmd_mve_v_a(int axis, int inc_pos, int inc_velocity, int inc_acceleration)
    int wrap_cmd_rmve(int axis, int inc_length)
    int wrap_cmd_set(int axis, int inc_pos)
    int wrap_cmd_spd(int axis, int inc_spd)
    int wrap_cmd_ujmp(int no, int thread)
    int wrap_cmd_ujmp_a(int no, int thread)
    int wrap_cmd_wtm(int axis)
    int wrap_cmd_wtt(int axis, int sec_atime)
    int wrap_convert_float32_from_iso(int axis, double value, int conv, float *buf)
    int wrap_convert_float32_to_iso(int axis, float value, int conv, double *buf)
    int wrap_convert_float64_from_iso(int axis, double value, int conv, double *buf)
    int wrap_convert_float64_to_iso(int axis, double value, int conv, double *buf)
    int wrap_convert_int32_from_iso(int axis, double value, int conv, int *buf)
    int wrap_convert_int32_to_iso(int axis, int value, int conv, double *buf)
    int wrap_convert_int64_from_iso(int axis, double value, int conv, __int64 *buf)
    int wrap_convert_int64_to_iso(int axis, __int64 value, int conv, double *buf)
    int wrap_designated_homing(int axis)
    int wrap_designated_open(const char *url, int port, int keep_alive)
    int wrap_designated_open_prepare()
    int wrap_destory_group()
    int wrap_destroy(int axis)
    int wrap_digital_input_status(int axis, int *buf)
    int wrap_digital_output_status(int axis, int *buf)
    int wrap_exec_sequence(int axis, int no)
    int wrap_get_actual_value(int axis, double *buf)
    int wrap_get_axis_number(int axis, int *buf)
    int wrap_get_demand_value(int axis, double *buf)
    int wrap_get_digital_input_index(int axis, int index, int *buf)
    int wrap_get_digital_output_index(int axis, int index, int *buf)
    int wrap_get_fast_digital_input_index(int axis, int index, int *buf)
    int wrap_get_fast_digital_input_status(int axis, int *buf)
    int wrap_get_fast_digital_output_index(int axis, int index, int *buf)
    int wrap_get_fast_digital_output_status(int axis, int *buf)
    int wrap_get_position(int axis, double *buf)
    int wrap_get_position_increment(int axis, int *buf)
    int wrap_get_reg_float32(int axis, int typ, int index, int depth, float *buf)
    int wrap_get_reg_float64(int axis, int typ, int index, int depth, double *buf)
    int wrap_get_reg_int32(int axis, int typ, int index, int depth, int *buf)
    int wrap_get_reg_int64(int axis, int typ, int index, int depth, __int64 *buf)
    int wrap_get_target_point(int axis, double *buf, int sidx)
    int wrap_get_ultimet_digital_input_index(int index, int *buf)
    int wrap_get_ultimet_digital_output_index(int index, int *buf)
    int wrap_halt_moving(int axis)
    int wrap_halt_sequence(int axis)
    int wrap_inc2iso(int axis, int value, double *buf)
    int wrap_ipol_abs_coords(int igrp, double pos_x, double pos_y, double pos_z)
    int wrap_ipol_abs_mode(int igrp, int mode)
    int wrap_ipol_begin(int igrp)
    int wrap_ipol_cmd_move_z_abs(int igrp_no, int inc_pos_z)
    int wrap_ipol_end(int igrp)
    int wrap_ipol_exec_sequence(int igrp, int no)
    int wrap_ipol_get_ipol_grp(int igrp, int *buf)
    int wrap_ipol_get_real_position(int igrp_no, int *buf)
    int wrap_ipol_get_user_position(int igrp_no, int *buf)
    int wrap_ipol_halt_moving(int igrp)
    int wrap_ipol_halt_sequence(int igrp)
    int wrap_ipol_line(int igrp, double pos_x, double pos_y, double pos_z)
    int wrap_ipol_move_xy_abs(int igrp, double pos_x, double pos_y)
    int wrap_ipol_move_xyz_abs(int igrp, double pos_x, double pos_y, double pos_z)
    int wrap_ipol_move_z_abs(int igrp, double pos_z)
    int wrap_ipol_prepare(int igrp)
    int wrap_ipol_pt(int igrp, double pos_x, double pos_y, double pos_z, double atime)
    int wrap_ipol_rotate(int igrp, double degree)
    int wrap_ipol_scale(int igrp, double scale_factor_x, double scale_factor_y, double scale_factor_z)
    int wrap_ipol_set_origin(int igrp, double pos_x, double pos_y, double pos_z)
    int wrap_ipol_set_tan_acceleration(int igrp, double acceleration)
    int wrap_ipol_set_tan_deceleration(int igrp, double deceleration)
    int wrap_ipol_set_tan_jerk_time(int igrp, double deceleration)
    int wrap_ipol_set_tan_speed(int igrp, double velocity, double acceleration, double jerk)
    int wrap_ipol_set_tan_velocity(int igrp, double velocity)
    int wrap_ipol_shear(int igrp)
    int wrap_ipol_translate(int igrp, double trans_x, double trans_y, double trans_z)
    int wrap_ipol_wait_movement(int igrp)
    int wrap_ipol_wait_sequence(int igrp)
    int wrap_is_ipol_in_progress(int igrp, int *buf)
    int wrap_is_open(int axis, unsigned char *buf)
    int wrap_iso2inc(int axis, double value, int *buf)
    int wrap_move_xy_abs(double pos_x, double pos_y, int wait)
    int wrap_move_xyz_abs(double pos_x, double pos_y, double pos_z, int wait)
    int wrap_move_z_abs(double pos_z, int wait)
    int wrap_power_off(int axis)
    int wrap_power_on(int axis)
    int wrap_reset(int axis)
    int wrap_set_acceleration(int axis, double acceleration)
    int wrap_set_brake_deceleration(int axis, double brake_deceleration)
    int wrap_set_digital_output_index(int axis, int index, int onoff)
    int wrap_set_digital_output_off_index(int axis, int index)
    int wrap_set_digital_output_on_index(int axis, int index)
    int wrap_set_fast_digital_output_index(int axis, int index, int onoff)
    int wrap_set_fast_digital_output_off_index(int axis, int index)
    int wrap_set_fast_digital_output_on_index(int axis, int index)
    int wrap_set_jerk_time(int axis, double jerk_time)
    int wrap_set_reg_float32(int axis, int typ, int index, int depth, float value)
    int wrap_set_reg_float64(int axis, int typ, int index, int depth, double value)
    int wrap_set_reg_int32(int axis, int typ, int index, int depth, int value)
    int wrap_set_reg_int64(int axis, int typ, int index, int depth, long value)
    int wrap_set_speed(int axis, double velocity, double acceleration, double jerk_time)
    int wrap_set_target_position(int axis, double pos, int sidx)
    int wrap_set_ultimet_digital_output_index(int index, int onoff)
    int wrap_set_ultimet_digital_output_off_index(int index)
    int wrap_set_ultimet_digital_output_on_index(int index)
    int wrap_set_user_position(int axis, double pos)
    int wrap_set_velocity(int axis, double velocity)
    int wrap_start_profiled_movement(int axis, double pos, double speed, double acc)
    int wrap_start_relative_profiled_movement(int axis, double signed_length)
    int wrap_status(int axis, int *buf)
    int wrap_step_motion_abs(int axis, double pos)
    int wrap_terminate()
    int wrap_test()
    int wrap_ultimet_digital_input_status(int *buf)
    int wrap_ultimet_digital_output_status(int *buf)
    int wrap_wait_movement(int axis)
    int wrap_wait_sequence(int axis)
# header end

# single binding

##################################################################
# general
##################################################################
def designated_open_prepare():
    wrap_designated_open_prepare()

def designated_open(url:str="172.30.1.80", port:int=1127, keep_alive:int=-1):
    # port: 1129, 1128, 1127
    # keep_alive: 3 times of keep_alive is connection timeout.
    # cdef const char* _url= bytes(url, 'utf-8')
    _url= bytes(url, 'utf-8')
    wrap_designated_open(_url, port, keep_alive)

def power_on(axis:int=0):
    wrap_power_on(axis)

def designated_homing(axis:int=0):
    wrap_designated_homing(axis)

def reset(axis:int=0):
    wrap_reset(axis)

def power_off(axis:int=0):
    wrap_power_off(axis)

def close(axis:int=0):
    wrap_close(axis)

def terminate():
    wrap_terminate()

##################################################################
# status and register
##################################################################
def axis_status(axis:int=0):
    cdef int buf[19]
    wrap_status(axis, buf)
    return buf

def status():
    cdef int x_status[19]
    cdef int y_status[19]
    cdef int z_status[19]
    cdef int d_status[19]
    cdef int u_status[19]
    wrap_status(X_AXIS_NO, x_status)
    wrap_status(Y_AXIS_NO, y_status)
    wrap_status(Z_AXIS_NO, z_status)
    wrap_status(D_AXIS_NO, d_status)
    wrap_status(ULTIMET_NO, u_status)
    return x_status, y_status, z_status, d_status, u_status

# typ : 0;X, 1:K, 2:M
def get_reg_float32(axis: int, typ:int, index:int, depth:int):
    cdef float buf
    wrap_get_reg_float32(axis, typ, index, depth, &buf)
    return buf

def get_reg_float64(axis: int, typ:int, index:int, depth:int):
    cdef double buf
    wrap_get_reg_float64(axis, typ, index, depth, &buf)
    return buf

def get_reg_int32(axis: int, typ:int, index:int, depth:int):
    cdef int buf
    wrap_get_reg_int32(axis, typ, index, depth, &buf)
    return buf

def get_reg_int64(axis: int, typ:int, index:int, depth:int):
    cdef __int64 buf
    wrap_get_reg_int64(axis, typ, index, depth, &buf)
    return buf

def set_reg_float32(axis: int, typ:int, index:int, depth:int, value: float):
    cdef float v = np.float32(value)
    wrap_set_reg_float32(axis, typ, index, depth, v)

def set_reg_float64(axis: int, typ:int, index:int, depth:int, value: float):
    cdef double v = value
    wrap_set_reg_float64(axis, typ, index, depth, v)

def set_reg_int32(axis: int, typ:int, index:int, depth:int, value: int):
    cdef int v = value
    wrap_set_reg_int32(axis, typ, index, depth, v)

def set_reg_int64(axis: int, typ:int, index:int, depth:int, value: int):
    cdef __int64 v = np.int64(value)
    wrap_set_reg_int64(axis, typ, index, depth, v)

def cmd_clx(axis:int=0):
    wrap_cmd_clx(axis)

##################################################################
# general move, based on high lvl function, meteric
##################################################################
def set_user_position(axis:int=0, pos:float=0):
    wrap_set_user_position(axis , pos)

def set_user_home():
    wrap_set_user_position(1, 0)
    wrap_set_user_position(2, 0)
    wrap_set_user_position(4, 0)

def get_position(axis:int=0):
    cdef double pos[3]
    pos[0] = -999999999
    pos[1] = -999999999
    pos[2] = -999999999
    wrap_get_position(axis, pos)
    # print(f"[py] position: {round(pos[0], 6)}, {round(pos[1],6)}, {round(pos[2],6)}")
    return pos

def set_velocity(axis: int=0, vel: float=0.01):
    wrap_set_velocity(axis, vel)

def set_acceleration(axis:int=0, acc:float=0.01):
    wrap_set_acceleration(axis, acc)

def set_brake_deceleration(axis:int=0, decl:float=0.01):
    wrap_set_brake_deceleration(axis,decl)

def set_speed(axis:int=0, vel:float=0.01, acc:float=0.01, decl:float=0.01):
    wrap_set_speed(axis, vel, acc, decl)

def set_target_position(axis:int, position:float, sidx:int=0):
    print(f"axis : {axis}, pos: {position} m")
    wrap_set_target_position(axis, position, sidx)

def wait_movement(axis:int):
    wrap_wait_movement(axis)

def halt_moving(int axis):
    wrap_halt_moving(axis)

##################################################################
# advanced move, based on high lvl function, meteric
##################################################################
def move_xyz_abs(pos_x: float, pos_y: float, pos_z:float, wait: int=0):
    wrap_move_xyz_abs(pos_x, pos_y, pos_z, wait)

def move_xy_abs(pos_x: float, pos_y: float, wait: int=0):
    wrap_move_xy_abs(pos_x, pos_y, wait)

def move_z_abs(pos_z: float, wait:int=0):
    wrap_move_z_abs(pos_z, wait)

##################################################################
# general move, based on command, increment
##################################################################
def get_position_increment(axis:int=0):
    cdef int buf[3]
    wrap_get_position_increment(axis, buf)
    return buf

def cmd_mve(axis:int, pos:int):
    wrap_cmd_mve(axis, pos)

def cmd_rmve(axis:int, length:int):
    wrap_cmd_rmve(axis, length)

def cmd_spd(axis:int, spd:int):
    wrap_cmd_spd(axis, spd)

def cmd_acc(axis:int, acc:int):
    wrap_cmd_acc(axis, acc)

def cmd_jrt(axis:int, jrt:int):
    wrap_cmd_jrt(axis, jrt)

def cmd_get_spd(axis:int):
    cdef __int64 buf = 0
    wrap_cmd_get_spd(axis, &buf)
    buf2 = int(buf) / (1024000 * 0.000001)
    return int(buf2)

def cmd_get_acc(axis:int):
    cdef __int64 buf = 0
    wrap_cmd_get_acc(axis, &buf)
    buf2 = int(buf) / (104858 * 0.000001)
    return int(buf2)

def cmd_get_jrt(axis:int):
    cdef int buf = 0
    wrap_cmd_get_jrt(axis, &buf)
    buf2 = int(int(buf) / 0.5)
    return int(buf2)

def cmd_set(axis:int, pos:int):
    wrap_cmd_set(axis, int)

def cmd_wtm(axis:int):
    wrap_cmd_wtm(axis)

def cmd_wtt(axis:int, atime:int):
    wrap_cmd_wtt(axis, int(atime / 0.4))

def cmd_jmp(axis: int, no:int, thread:int=0):
    wrap_cmd_jmp(axis, no, thread)

def cmd_jmp_a(axis: int, no:int, thread:int=0):
    wrap_cmd_jmp_a(axis, no, thread)

def cmd_ujmp(no:int, thread:int=0):
    wrap_cmd_ujmp(no, thread)

def cmd_ujmp_a(no:int, thread:int=0):
    wrap_cmd_ujmp_a(no, thread)
##################################################################
# advanced move, based on command, increment
##################################################################



##################################################################
# sequence, based on high lvl function, meteric
##################################################################
def exec_sequence(axis:int, no:int):
    wrap_exec_sequence(axis, no)

def wait_sequence(int axis):
    wrap_wait_sequence(axis)

def halt_sequence(int axis):
    wrap_halt_sequence(axis)

##################################################################
# sequence, based on command, increment
##################################################################



##################################################################
# interpolation, based on high lvl function, meteric
##################################################################
def ipol_prepare(igrp: int):
    wrap_ipol_prepare(igrp)

def ipol_in_progress(igrp):
    cdef int buf
    wrap_is_ipol_in_progress(igrp, &buf)
    return buf

def ipol_begin(igrp: int):
    wrap_ipol_begin(igrp)

def ipol_abs_mode(igrp:int , mode:int):
    wrap_ipol_abs_mode(igrp, mode)

def ipol_abs_coords(igrp:int, pos_x:float, pos_y:float, pos_z:float):
    wrap_ipol_abs_coords(igrp, pos_x, pos_y, pos_z)

def ipol_get_real_position(igrp:int):
    cdef int pos[3]
    wrap_ipol_get_real_position(igrp, pos)
    print(f"ipol real: {pos}")
    return pos

def ipol_get_user_position(igrp:int):
    cdef int pos[3]
    wrap_ipol_get_user_position(igrp, pos)
    print(f"ipol user: {pos}")
    return pos

def ipol_move_abs(igrp: int, x: float, y: float, z:float):
    wrap_ipol_move_xyz_abs(igrp, x, y, z)

def ipol_end(igrp: int):
    wrap_ipol_end(igrp)

def ipol_get_igrp_no(igrp:int):
    cdef int no
    wrap_ipol_get_ipol_grp(igrp, &no)
    return no

##################################################################
# interpolation, based on command, increment
##################################################################
def cmd_iabscoords(igrp_no:int, pos_x:int, pos_y:int, pos_z:int):
    wrap_cmd_iabscoords(igrp_no, pos_x, pos_y, pos_z)

def cmd_ibegin(igrp_no:int):
    wrap_cmd_ibegin(igrp_no)

def cmd_iend(igrp_no:int):
    wrap_cmd_iend(igrp_no)

def cmd_itspd(igrp_no: int, spd:int):
    wrap_cmd_itspd(igrp_no, spd)

def cmd_itacc(igrp_no: int, acc:int):
    wrap_cmd_itacc(igrp_no, acc)

def cmd_itdec(igrp_no: int, dec:int):
    wrap_cmd_itdec(igrp_no, dec)

def cmd_itjrt(igrp_no: int, jrt:int):
    wrap_cmd_itjrt(igrp_no, jrt)

def cmd_get_itspd(igrp_no:int):
    cdef int buf
    wrap_cmd_get_itspd(igrp_no, &buf)
    return buf

def cmd_get_itacc(igrp_no:int):
    cdef int buf
    wrap_cmd_get_itacc(igrp_no, &buf)
    return buf

def cmd_get_itdec(igrp_no:int):
    cdef int buf
    wrap_cmd_get_itdec(igrp_no, &buf)
    return buf

def cmd_get_itjrt(igrp_no:int):
    cdef int buf
    wrap_cmd_get_itjrt(igrp_no, &buf)
    return buf

def cmd_iabsmode(igrp_no:int, value:int):
    wrap_cmd_iabsmode(igrp_no, value)

def cmd_iline(igrp_no:int, pos_x:int, pos_y:int, pos_z:int):
    wrap_cmd_iline(igrp_no, pos_x, pos_y, pos_z)

def cmd_iwtm(igrp_no:int):
    wrap_cmd_iwtm(igrp_no)

def cmd_iwtt(igrp_no:int, atime:int):
    wrap_cmd_iwtt(igrp_no, atime)

def cmd_imrot(igrp_no:int, degree: int):
    wrap_cmd_imrot(igrp_no, degree)

def cmd_imscale(igrp_no:int, pos_x:int, pos_y:int, pos_z:int):
    wrap_cmd_imscale(igrp_no, pos_x, pos_y, pos_z)

def cmd_imshear(igrp_no:int, pos_x:int, pos_y:int, pos_z:int):
    wrap_cmd_imshear(igrp_no, pos_x, pos_y, pos_z)

def cmd_imtrans(igrp_no:int, pos_x:int, pos_y:int, pos_z:int):
    wrap_cmd_imtrans(igrp_no, pos_x, pos_y, pos_z)

def cmd_iset(igrp_no:int):
    wrap_cmd_iset(igrp_no)

def cmd_imres(igrp_no:int):
    wrap_cmd_imres(igrp_no)

##################################################################
# digitla input / output
##################################################################
def get_digital_input_status(axis: int):
    cdef int buf
    wrap_digital_input_status(axis, &buf)
    return buf

def get_digital_output_status(axis: int):
    cdef int buf
    wrap_digital_output_status(axis, &buf)
    return buf

def get_digital_input_index(axis: int, index: int):
    cdef int buf
    wrap_get_digital_input_index(axis, index, &buf)
    return buf

def get_digital_output_index(axis: int, index: int):
    cdef int buf
    wrap_get_digital_output_index(axis, index, &buf)
    return buf

def set_digital_output_index(axis: int, index:int, onoff: int):
    re = wrap_set_digital_output_index(axis, index, onoff)
    return re

def set_digital_output_on_index(axis: int, index: int):
    re = wrap_set_digital_output_on_index(axis, index)
    return re

def set_digital_output_off_index(axis: int, index: int):
    re = wrap_set_digital_output_off_index(axis, index)
    return re

def get_fast_digital_input_status(axis: int):
    cdef int buf
    wrap_get_fast_digital_input_status(axis, &buf)
    return buf

def get_fast_digital_output_status(axis: int):
    cdef int buf
    wrap_get_fast_digital_output_status(axis, &buf)
    return buf

def get_fast_digital_input_index(axis: int, index: int):
    cdef int buf
    wrap_get_fast_digital_input_index(axis, index, &buf)
    return buf

def get_fast_digital_output_index(axis: int, index: int):
    cdef int buf
    wrap_get_fast_digital_output_index(axis, index, &buf)
    return buf

def set_fast_digital_output_index(axis: int, index:int, onoff: int):
    re = wrap_set_fast_digital_output_index(axis, index, onoff)
    return re

def set_fast_digital_output_on_index(axis: int, index):
    re = wrap_set_fast_digital_output_on_index(axis, index)
    return re

def set_fast_digital_output_off_index(axis: int, index):
    re = wrap_set_fast_digital_output_off_index(axis, index)
    return re

def get_ultimet_digital_input_index(index: int):
    cdef int buf
    wrap_get_ultimet_digital_input_index(index, &buf)
    return buf

def get_ultimet_digital_output_index(index: int):
    cdef int buf
    wrap_get_ultimet_digital_output_index(index, &buf)
    return buf

def set_ultimet_digital_output_index(index: int, onoff: int):
    re = wrap_set_ultimet_digital_output_index(index, onoff)
    return re

def set_ultimet_digital_output_on_index(int index):
    re = wrap_set_ultimet_digital_output_on_index(index)
    return re

def set_ultimet_digital_output_off_index(int index):
    re = wrap_set_ultimet_digital_output_off_index(index)
    return re

##################################################################
# util
##################################################################
def convert_int32_from_iso(axis:int, value:float, conv:int):
    cdef int re = -999999999
    wrap_convert_int32_from_iso(axis, value, conv, &re)
    return re

def convert_int64_from_iso(axis:int, value:float, conv:int):
    cdef __int64 re = -9999999999
    wrap_convert_int64_from_iso(axis, value, conv, &re)
    return re

def convert_float32_from_iso(axis:int, value:float, conv:int):
    cdef float re = np.float32(-999999999.0)
    wrap_convert_float32_from_iso(axis, value, conv, &re)
    return re

def convert_float64_from_iso(axis:int, value:float, conv:int):
    cdef double re = -999999999.0
    wrap_convert_float64_from_iso(axis, value, conv, &re)
    return re

def convert_int32_to_iso(axis:int, value:int, conv:int):
    cdef double iso
    wrap_convert_int32_to_iso(axis, value, conv, &iso)
    return iso.value()

def convert_int64_to_iso(axis:int, value:int, conv:int):
    cdef double iso
    wrap_convert_int64_to_iso(axis, value, conv, &iso)
    return iso.value()

def convert_float32_to_iso(axis:int, value:float, conv:int):
    cdef double iso
    value = np.float32(value)
    wrap_convert_float32_to_iso(axis, value, conv, &iso)
    return iso.value()

def convert_float64_to_iso(axis:int, value:float, conv:int):
    cdef double iso
    wrap_convert_float64_to_iso(axis, value, conv, &iso)
    return iso.value()

def i2u(value:int):
    return int(value / 10)

def i2m(value:int):
    return float(value / 10000000)

def u2i(value:int):
    return int(value * 10)

def m2i(value:float):
    return int(value * 10000000)

def _iu(value: int):
    return int(value / 10)

def _im(value: int):
    return float(value / 10000000)

def _ui(value:int):
    return int(value * 10)

def _mi(value:float):
    return int(value * 10000000)

def _i(value:int , source='um'):
    if source == 'um':
        return int(value * 10)
    elif source == 'mm':
        return int(value * 10000)
    elif source == 'm':
        return int(value * 10000000)
    else:
        return value

def _u(value:int , source='inc'):
    if source == 'inc':
        return int(value / 10)
    elif source == 'mm':
        return float(value / 1000)
    elif source == 'm':
        return float(value * 1000000)
    else:
        return value

##################################################################
# test
##################################################################
def test():
    wrap_test()

def my_test():
    pass
