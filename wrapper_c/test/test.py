import wrapper as wr
import time
import sys

x = 1
y = 2
z = 4
d = 8
u = 9

igrp0 =20
igrp1 =21

nstatus: dict = {
    "power_on": 0,
    "init_done": 1,
    "homing_done": 2,
    "present": 3,
    "moving": 4,
    "in_window": 5,
    "exec_seq": 6,
    "in_speed_window": 7,
    "fatal": 8,
    "trace_busy": 9,
    "ipol_grp0": 10,
    "ipol_grp1": 11,
    "exec_seq_thread": 12,
    "warning": 13,
    "int_error": 14,
    "warning": 15,
    "save_pos": 16,
    "breakpoint": 17,
    "user": 18,
}

def u2i(value:int):
    return value * 10

def main(no:int):

    if no == 0:
        wr.open_prepare()
        wr.open()
        wr.status(0)
        wr.homing(0)
        wr.get_position(0)

        wr.move_abs(1, 0.1)
        wr.wait_movement(1)
        wr.get_position(0)
        time.sleep(0.2)
        wr.get_position(0)

        wr.move_abs(2, 0.1)
        wr.wait_movement(2)
        wr.get_position(0)
        time.sleep(0.2)
        wr.get_position(0)

        wr.move_abs(4, 0.000001)
        wr.wait_movement(4)
        wr.get_position(0)
        time.sleep(0.4)
        wr.get_position(0)

        wr.set_velocity(3, 0.01)
        wr.move_xy_abs(0.3, 0.2)
        wr.wait_movement(3)
        wr.get_position(0)
        time.sleep(0.2)
        wr.get_position(0)

        wr.move_abs(1, 0.4)
        wr.wait_movement(3)
        wr.get_position(0)
        time.sleep(0.1)
        wr.get_position(0)

        for _ in range(30):
            wr.get_position(0)
            time.sleep(0.1)
        wr.close(0)
    elif no == 1:
        wr.open_prepare()
        wr.open()
        wr.homing(0)
        wr.cmd_mve(x, u2i(250000))
        wr.cmd_wtm(x)
        wr.cmd_mve(y, u2i(150000))
        wr.cmd_wtm(y)
        wr.cmd_wtt(y, 1000)
        for _ in range(10):
            print(wr.get_position(0))
            print(wr.get_position_increment(0))
        wr.close(0)
    elif no ==2:
        wr.open_prepare()
        wr.open()
        wr.homing(0)
        wr.cmd_mve(x, u2i(50000))
        wr.cmd_wtm(x)
        wr.cmd_mve(y, u2i(50000))
        wr.cmd_wtm(y)
        time.sleep(1)
        wr.set_reg_int32(9, 1, 523, 0, -7)
        wr.ipol_begin(igrp0)
        igrp_no = wr.ipol_get_igrp_no(igrp0)
        wr.cmd_iabsmode(igrp_no, 0)
        print("iabs mode set, hit")
        # _ = input()
        wr.cmd_imrot(igrp_no, 1000000 * 30)
        print("imort, hit")
        # _ = input()
        wr.cmd_iline(igrp_no, u2i(10000), u2i(10000), 0)
        print("iline, hit")
        # _ = input()
        wr.cmd_iwtm(igrp_no)
        for _ in range(5):
            print(wr.get_position_increment(0))
            print(wr.ipol_get_real_position(igrp_no))
            print(wr.ipol_get_user_position(igrp_no))
        wr.ipol_end(igrp0)
        wr.close()
    elif no ==3:
        wr.open_prepare()
        wr.open()
        wr.homing(0)
        wr.set_reg_int32(9, 1, 523, 0, -7)
        wr.ipol_begin(igrp0)
        igrp_no = wr.ipol_get_igrp_no(igrp0)
        wr.cmd_iabsmode(igrp_no, 1)
        print("iabs mode set, hit")
        # _ = input()
        wr.cmd_imtrans(igrp_no, u2i(250000), u2i(150000), 0)
        print("imtrans mode set, hit")
        # _ = input()
        wr.cmd_imrot(igrp_no, 1000000 * 30)
        print("imort, hit")
        # _ = input()
        wr.cmd_iline(igrp_no, u2i(10000), u2i(10000), 0)
        print("iline, hit")
        # _ = input()
        wr.cmd_iwtm(igrp_no)
        for _ in range(5):
            print(wr.get_position_increment(0))
            print(wr.ipol_get_real_position(igrp_no))
            print(wr.ipol_get_user_position(igrp_no))
        wr.ipol_end(igrp0)
        wr.close()
    elif no ==4:
        wr.open_prepare()
        wr.open()
        wr.cmd_spd(x, wr.u2i(1000))
        print("get:")
        print(wr.i2u(wr.cmd_get_spd(x)))
        wr.close()
    elif no ==5:
        wr.designated_open_prepare()
        wr.designated_open()
        status = wr.machine_status()
        wr.close()

        keys = list(nstatus.keys())
        print(keys)
        for k in keys:
            print(f"{k}: {status[nstatus[k]]}")

    elif no==6:
        import time
        wr.designated_open_prepare()
        wr.designated_open()
        # print(wr.set_digital_output_index(d, 1, 1))
        # wr.set_digital_output_on_index(u, 1)
        # print(wr.get_digital_output_status(u))
        # print(wr.get_digital_output_index(d, index))

        # wr.set_fast_digital_output_on_index(d, index)
        # wr.set_fast_digital_output_on_index(x, 2)
        # wr.set_fast_digital_output_on_index(x, 3)
        # wr.set_fast_digital_output_on_index(x, 4)
        # print(wr.get_fast_digital_output_index(d, index))
        # print(wr.get_fast_digital_output_index(x, 2))
        # print(wr.get_fast_digital_output_index(x, 3))
        # print(wr.get_fast_digital_output_index(x, 4))

        # print(wr.set_ultimet_digital_output_off_index(1))
        # wr.set_ultimet_digital_output_on_index(2)

        print(wr.set_ultimet_digital_output_on_index(1))
        # print(wr.get_ultimet_digital_output_index(2))
        
        for i in range(10):
            print(".", end='')
            time.sleep(1)
        print('\n')
        wr.close()

if __name__=='__main__':
    print(sys.argv)
    main(int(sys.argv[1]))
