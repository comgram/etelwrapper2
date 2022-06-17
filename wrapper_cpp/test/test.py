import wrapper as wr

wr.open()
wr.homing()
wr.move_abs_m(1, 0.1)
wr.move_abs_m(2, 0.1)
wr.move_abs_m(3, 0.01)
wr.close()
