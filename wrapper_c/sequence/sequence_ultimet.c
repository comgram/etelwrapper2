#COMET special parameters
#ISO linear    units : m, m / s, m / s², V, A, s(used in case of interpolation)
#ISO rotary    units : t, t / s, t / s², V, A, s

#define axis_x 0
#define axis_y 1
#define axis_z 2

#define TRUE   1
#define FALSE  0

#define ON     1
#define OFF    0

// X0.axis_x: home status; homing is whether done or not
// X0.axis_y: home status; homing is whether done or not
// X0.axis_z: home status; homing is whether done or not
// *X0: home status; homing is whether done or not
// X1.axis_x: move status; axis is whether moving or not
// X1.axis_y: move status; axis is whether moving or not
// X1.axis_z: move status; axis is whether moving or not
// *X1: move status; axis is whether moving or not
// X2.axis_x: sequence status; sequence is whether executing or not
// X2.axis_y: sequence status; sequence is whether executing or not
// X2.axis_z: sequence status; sequence is whether executing or not
// *X2: sequence status; sequence is whether executing or not
// X3.axis_x: sequence number
// X3.axis_y: sequence number
// X3.axis_z: sequence number
// *X3: sequence number
// X10.axis_x: target position x
// X10.axis_y: target position y
// X10.axis_z: target position z
// X11.axis_x: target position x (temp)
// X11.axis_y: target position y (temp)
// X11.axis_z: target position z (temp)
// X12.axis_x: velocity x
// X12.axis_y: velocity y
// X12.axis_z: velocity z
// X13.axis_x: accerlation x
// X13.axis_y: accerlation y
// X13.axis_z: accerlation z
// X13.axis_x: deceleration x
// X13.axis_y: deceleration y
// X13.axis_z: deceleration z
// X14.axis_x: jerk time x (0 ~ 300 ms)
// X14.axis_y: jerk time y (0 ~ 300 ms)
// X14.axis_z: jerk time z (0 ~ 300 ms)
// *X20: interpolation group (0 ~ 1)
// *X21: interpolation, origin mode is whether absolute or relative
// *X22: interpolation, relative origin position on ipol coords x
// *X23: interpolation, relative origin position on ipol coords y
// *X24: interpolation, relative origin position on ipol coords z
// *X30: interpolation, user target position x
// *X31: interpolation, user target position y
// *X32: interpolation, user target position z
// *X33: interpolation, user target position x (temp)
// *X34: interpolation, user target position y (temp)
// *X35: interpolation, user target position z (temp)
// *X36: workpiece origin position on machine coordination x
// *X37: workpiece origin position on machine coordination y
// *X38: workpiece origin position on machine coordination z
// *X40: interpolation, tan velocity
// *X41: interpolation, tan accerlation
// *X42: interpolation, tan jerk time
// *X43: interpolation, tan deceleration
// *X44: interpolation PT time
// *X50: scale factor x
// *X51: scale factor y
// *X52: scale factor z
// *X53: tanslate coords x
// *X54: tanslate coords y
// *X55: tanslate coords z
// *X56: shearing coords
// *X57: shearing coords
// *X58: shearing coords
// *X59: rotate degree
// X100.axis_x: stable hold time last
// X100.axis_y: stable hold time last
// X100.axis_z: stable hold time last
// *X100: stable hold time last
// X101.axis_x: stable hold time middle
// X101.axis_y: stable hold time middle
// X101.axis_z: stable hold time middle
// *X101: stable hold time middle
// X102.axis_z: z safe, wherther or not
// *X102: z safe, wherther or not
// X103.axis_z: z safe position
// *X103: z safe position

// friendly function name
// 20 : move x abs
// 21 : move y abs
// 22 : move z abs

// homing
void func0(void)
{
    // homing all axis sequencialy
    *X0 = FALSE;
    *X2 = TRUE;

    PWR.axis_z = ON;
    WTT.axis_z = 2;
    JMP.axis_z = 0;
    WTT.axis_z = 1;

    for (;;) {
        if (X0.axis_z == 1) {
            break;
        }
    }

    PWR.axis_y = ON;
    WTT.axis_y = 2;
    JMP.axis_y = 0;
    WTT.axis_y = 1;

    PWR.axis_x = ON;
    WTT.axis_x = 2;
    JMP.axis_x = 0;
    WTT.axis_x = 1;

    for (;;) {
        if (X0.axis_x == 1) {
            break;
        }
    }

    *X2 = FALSE;
    *X0 = TRUE;
}

// setup
void setup(void)
{
    X0.axis_x   = 0; // home status; homing is whether done or not
    X0.axis_y   = 0; // home status; homing is whether done or not
    X0.axis_z   = 0; // home status; homing is whether done or not
    *X0         = 0; // home status; homing is whether done or not
    X1.axis_x   = 0; // move status; axis is whether moving or not
    X1.axis_y   = 0; // move status; axis is whether moving or not
    X1.axis_z   = 0; // move status; axis is whether moving or not
    *X1         = 0; // move status; axis is whether moving or not
    X2.axis_x   = 0; // sequence status; sequence is whether executing or not
    X2.axis_y   = 0; // sequence status; sequence is whether executing or not
    X2.axis_z   = 0; // sequence status; sequence is whether executing or not
    *X2         = 0; // sequence status; sequence is whether executing or not
    X3.axis_x   = 0; // sequence number
    X3.axis_y   = 0; // sequence number
    X3.axis_z   = 0; // sequence number
    *X3         = 0; // sequence number
    X10.axis_x  = 0; // target position x
    X10.axis_y  = 0; // target position y
    X10.axis_z  = 0; // target position z
    X11.axis_x  = 0; // target position x (temp)
    X11.axis_y  = 0; // target position y (temp)
    X11.axis_z  = 0; // target position z (temp)
    X12.axis_x  = 0; // velocity x
    X12.axis_y  = 0; // velocity y
    X12.axis_z  = 0; // velocity z
    X13.axis_x  = 0; // accerlation x
    X13.axis_y  = 0; // accerlation y
    X13.axis_z  = 0; // accerlation z
    X13.axis_x  = 0; // deceleration x
    X13.axis_y  = 0; // deceleration y
    X13.axis_z  = 0; // deceleration z
    X14.axis_x  = 0; // jerk time x (0 ~ 300 ms)
    X14.axis_y  = 0; // jerk time y (0 ~ 300 ms)
    X14.axis_z  = 0; // jerk time z (0 ~ 300 ms)
    *X20        = 0; // interpolation group (0 ~ 1)
    *X21        = 0; // interpolation, origin mode is whether absolute or relative
    *X22        = 0; // interpolation, relative origin position on ipol coords x
    *X23        = 0; // interpolation, relative origin position on ipol coords y
    *X24        = 0; // interpolation, relative origin position on ipol coords z
    *X30        = 0; // interpolation, user target position x
    *X31        = 0; // interpolation, user target position y
    *X32        = 0; // interpolation, user target position z
    *X33        = 0; // interpolation, user target position x (temp)
    *X34        = 0; // interpolation, user target position y (temp)
    *X35        = 0; // interpolation, user target position z (temp)
    *X36        = 0; // workpiece origin position on machine coordination x
    *X37        = 0; // workpiece origin position on machine coordination y
    *X38        = 0; // workpiece origin position on machine coordination z
    *X40        = 0; // interpolation, tan velocity
    *X41        = 0; // interpolation, tan accerlation
    *X42        = 0; // interpolation, tan deceleration
    *X43        = 0; // interpolation, tan jerk time
    *X44        = 0; // interpolation PT time
    *X50        = 0; // scale factor x
    *X51        = 0; // scale factor y
    *X52        = 0; // scale factor z
    *X53        = 0; // tanslate coords x
    *X54        = 0; // tanslate coords y
    *X55        = 0; // tanslate coords z
    *X56        = 0; // shearing coords
    *X57        = 0; // shearing coords
    *X58        = 0; // shearing coords
    *X59        = 0; // rotate degree
    X100.axis_x = 0; // stable hold time last
    X100.axis_y = 0; // stable hold time last
    X100.axis_z = 0; // stable hold time last
    *X100       = 0; // stable hold time last
    X101.axis_x = 0; // stable hold time middle
    X101.axis_y = 0; // stable hold time middle
    X101.axis_z = 0; // stable hold time middle
    *X101       = 0; // stable hold time middle
    X102.axis_z = 0; // z safe, wherther or not
    *X102       = 0; // z safe, wherther or not
    X103.axis_z = 0; // z safe position
    *X103       = 0; // z safe position

    *X256        = 0;  // homing
    *X256.axis_x = 20; // move x abs
    *X256.axis_y = 21; // move y abs
    *X256.axis_z = 22; // move z abs
    *X257        = 23; // move xy abs
}

// general move
void move_x(void)
{
    // move x abs
    *X1        = TRUE;
    X1.axis_x  = TRUE;
    MVE.axis_x = X10.axis_x;
    WTM.axis_x;
    WTT.axis_x = X100.axis_x;
    X1.axis_x  = FALSE;
    *X1        = FALSE;
}

void move_y(void)
{
    // move y abs
    *X1        = TRUE;
    X1.axis_y  = TRUE;
    MVE.axis_y = X10.axis_y;
    WTM.axis_y;
    WTT.axis_y = X100.axis_y;
    X1.axis_y  = FALSE;
    *X1        = FALSE;
}

void move_z(void)
{
    // move z abs
    *X1        = TRUE;
    X1.axis_z  = TRUE;
    MVE.axis_z = X10.axis_z;
    WTM.axis_z;
    WTT.axis_z = X100.axis_z;
    X1.axis_z  = FALSE;
    *X1        = FALSE;
}

void move_xy(void)
{
    // xy abs move
    *X1                 = TRUE;
    X1.(axis_x, axis_y) = TRUE;
    MVE.axis_x          = X10.axis_x;
    MVE.axis_y          = X10.axis_y;
    WTM.(axis_x, axis_y);
    WTT.(axis_x, axis_y) = *X100;
    X1.(axis_x, axis_y)  = FALSE;
    *X1                  = FALSE;
}

void move_xyz(void)
{
    // xyz abs move
    *X1                         = TRUE;
    X1.(axis_x, axis_y, axis_z) = TRUE;
    MVE.axis_z                  = X10.axis_z;
    MVE.axis_x                  = X10.axis_x;
    MVE.axis_y                  = X10.axis_y;
    WTM.(axis_x, axis_y, axis_z);
    X1.(axis_x, axis_y, axis_z) = FALSE;
    *X1                         = FALSE;
}

void move_zxy(void)
{
    // z-xy abs move
    *X1        = TRUE;
    X1.axis_z  = TRUE;
    MVE.axis_z = X3.axis_z;
    WTM.axis_z;
    X1.axis_z           = FALSE;
    X1.(axis_x, axis_y) = TRUE;
    MVE.axis_x          = X10.axis_x;
    MVE.axis_y          = X10.axis_y;
    WTM.(axis_x, axis_y);
    WTT.(axis_x, axis_y) = *X100;
    X1.(axis_x, axis_y)  = FALSE;
    *X1                  = FALSE;
}

void move_x_r(void)
{
    // move x relativly
    *X1        = TRUE;
    X1.axis_x  = TRUE;
    MVE.axis_x = M7.axis_x + X10.axis_x;
    WTM.axis_x;
    WTT.axis_x = X100.axis_x;
    X1.axis_x  = FALSE;
    *X1        = FALSE;
}

void move_y_r(void)
{
    // move y relativly
    *X1        = TRUE;
    X1.axis_y  = TRUE;
    MVE.axis_y = M7.axis_y + X10.axis_y;
    WTM.axis_y;
    WTT.axis_y = X100.axis_y;
    X1.axis_y  = FALSE;
    *X1        = FALSE;
}

void move_z_r(void)
{
    // move z relativly
    *X1        = TRUE;
    X1.axis_z  = TRUE;
    MVE.axis_z = M7.axis_z + X10.axis_z;
    WTM.axis_z;
    WTT.axis_z = X100.axis_z;
    X1.axis_z  = FALSE;
    *X1        = FALSE;
}

void func190(void)
{
    // end sequence
    *END = X520;
}

void func200(void)
{
    // set workpiece home
    *X19 : depth_x = M7.axis_x;
    *X19 : depth_y = M7.axis_y;
    *X19 : depth_z = M7.axis_z;
}

////////////////////////////////////////////////////////////////////////////
// interpolation functions
////////////////////////////////////////////////////////////////////////////
void func300(void)
{
    // ipol begin
    if (*M519 != 0x01) {
        *K523 : 0 = -7; // increment (0.1 micro-meter)
        *ISET     = *X9, 0x2L, 0x1L, 0x4L, 0x0L;
        *IBEGIN   = *X9;
        *IABSMODE = *X9, 0;
    }
}

void func310(void)
{
    // ipol iabsmode
    if (*M519 == 0x01) {
        *IABSMODE = *X9, *X10;
    }
}

void func320(void)
{
    // ipol iabscoords
    if (*M519 == 0x01) {
        if (*X10 == 0) {
            // relative
            *X30           = *X19 : depth_x - *M641 : depth_x;
            *X31 : depth_y = *X19 : depth_y - *M641 : depth_y;
            *X32 : depth_z = 0;

            *IABSCOORDS = *X9, *X11 : depth_x, *X11 : depth_y, *X11 : depth_z, 0;
        }
        else {
            // absolute
            *X11 : depth_x = *X19 : depth_x;
            *X11 : depth_y = *X19 : depth_y;
            *X11 : depth_z = 0;

            *IABSCOORDS = *X9, *X11 : depth_x, *X11 : depth_y, *X11 : depth_z, 0;
        }
    }
}

void func330(void)
{
    // ipol translate
    if (*M519 == 0x01) {
        *IMTRANS = *X20, *X53, *X54, *X55, 0;
    }
}

void func340(void)
{
    // ipol scale
    if (*M519 == 0x01) {
        *IMSCALE = *X20, *X50, *X51, *X52, 0;
    }
}

void func350(void)
{
    // ipol rotate
    if (*M519 == 0x01) {
        *IMROT = *X20, 0, *X59;
    }
}

void func360(void)
{
    // ipol shearing
}

void func370(void)
{
    // ipol imres
    if (*M519 == 0x01) {
        *IMRES = *X20;
    }
}

void func380(void)
{
    // ipol move xyz abs
    if (*M519 == 0x01) {
        *X1                         = TRUE;
        X1.(axis_x, axis_y, axis_z) = TRUE;
        *ILINE                      = *X20, *X30, *X31, *X32, 0;
        *WTM                        = *X20;
        *WTT                        = *X100 : 1;
        X1.(axis_x, axis_y, axis_z) = FALSE;
        *X1                         = FALSE;
    }
}

void func390(void)
{
    // ipol move xy abs
    if (*M519 == 0x01) {
        *X1                 = TRUE;
        X1.(axis_x, axis_y) = TRUE;
        *X32                = *M644;
        *ILINE              = *X20, *X30, *X31, *X32, 0;
        *WTM                = *X20;
        *WTT                = *X100 : 1;
        X1.(axis_x, axis_y) = FALSE;
        *X1                 = FALSE;
    }
}

void func400(void)
{
    // ipol move z abs
    if (*M519 == 0x01) {
        *X1       = TRUE;
        X1.axis_z = TRUE;
        *X30      = *M640;
        *X31      = *M642;
        *ILINE    = *X20, *X30, *X31, *X32, 0;
        *WTM      = *X20;
        *WTT      = *X100 : 1;
        X1.axis_z = FALSE;
        *X1       = FALSE;
    }
}

void func410(void)
{
    // ipol speed
    if (*M519 == 0x01) {
        *ITSPD = 0, *X40;
        *ITACC = 0, *X41;
        *ITDEC = 0, *X42;
        *ITJRT = 0, *X43;
    }
}

void func420(void)
{
    // ipol PT move xyz abs
    if (*M519 == 0x01) {
        *X1                         = TRUE;
        X1.(axis_x, axis_y, axis_z) = TRUE;
        *IPT                        = *X20, *X30, *X31, *X32, 0, *X16;
        *WTM                        = *X20;
        *WTT                        = *X100 : 1;
        X1.(axis_x, axis_y, axis_z) = FALSE;
        *X1                         = FALSE;
    }
}

void func430(void)
{
    // ipol PT move z-xy abs
    if (*M519 == 0x01) {
        *X1 = TRUE;

        X1.(axis_x, axis_y, axis_z) = TRUE;
        // move z abs to safe position
        if (X24 : 1 == 1) {
            X1.axis_z      = TRUE;
            *X12 : depth_x = *M640;
            *X12 : depth_y = *M642;
            *X12 : depth_z = *X24 : 2;
            *IPT           = X8, *X12 : depth_x, *X12 : depth_y, *X12 : depth_z, 0, *X25;
            *WTM           = X9;
            *WTT           = X100 : 2;
            X1.axis_z      = FALSE;
        }

        // move xy abs to target position
        X1.(axis_x, axis_y) = TRUE;
        *X12 : depth_x      = X11 : depth_x;
        *X12 : depth_y      = X11 : depth_y;
        *IPT                = X8, *X12 : depth_x, *X12 : depth_y, *X12 : depth_z, 0, *X25;
        *WTM                = X9;
        *WTT                = X100 : 2;
        X1.(axis_x, axis_y) = FALSE;

        // move z abs to target position
        X1.axis_z      = TRUE;
        *X12 : depth_x = *M640;
        *X12 : depth_y = *M642;
        *X12 : depth_z = *X11 : depth_z;
        *IPT           = X8, *X12 : depth_x, *X12 : depth_y, *X12 : depth_z, 0, *X25;
        *WTM           = X9;
        *WTT           = X100 : 1;
        X1.axis_z      = FALSE;

        *X1 = FALSE;
    }
}

void func440(void)
{
    // ipol PT move xy abs
    if (*M519 == 0x01) {
        *X1                         = TRUE;
        X1.axis_z                   = TRUE;
        *X30                        = *M644;
        *IPT                        = *X20, *X11 : depth_x, *X11 : depth_y, *X11 : depth_z, 0, *X25;
        *WTM                        = X9;
        *WTT                        = X100 : 1;
        X1.(axis_x, axis_y, axis_z) = FALSE;
        *X1                         = FALSE;
    }
}

void func450(void)
{
    // ipol PT move z abs
    if (*M519 == 0x01) {
        *X1                         = TRUE;
        X1.(axis_x, axis_y, axis_z) = TRUE;
        *IPT                        = X8, *X11 : depth_x, *X11 : depth_y, *X11 : depth_z, 0, *X25;
        *WTM                        = X9;
        *WTT                        = X100 : 1;
        X1.(axis_x, axis_y, axis_z) = FALSE;
        *X1                         = FALSE;
    }
}

void func460(void)
{
    // ipol end
    if (*M519 == 0x01) {
        *IEND = *X20;
    }
}

void func800(void)
{
    // z calibration
}

void func810(void)
{
    // ipol move to index
}

void func820(void)
{
    // ipol move to index
    // shot
}

// interpolation default
void func1020(void)
{
    *K522 : 0 = 1;
    *K523 : 0 = -8;
    *K524 : 0 = -6;
    *K525 : 0 = -6;
    *K526 : 0 = -6;
}

// for test
void func1023(void)
{
    *X4 : axis_x = 1;
}
