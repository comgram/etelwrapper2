#COMET special parameters
#ISO linear    units : m, m / s, m / s², V, A, s(used in case of interpolation)
#ISO rotary    units : deg, deg / s, deg / s², V, A, s

///////////////////////////////////////////////////////////
// Customer/Project info : Kodis
// Projct Dimension/Sales Order No. : S2B213
// Revision history:
// R0 : 2021.11.18, HS LIM, First edition at Soonhan FAB
///////////////////////////////////////////////////////////

// Y Axis Homing
void func10(void)
{
    X10 .0  = 0;  // user variable(Home done off)
    K40 .0  = 45; // master Axis homing methord
    K165 .0 = 0;  // compensation data on

    RST .0;
    K36 .0 = 0; // software position limit off
    WTT .0 = 0.05;

    INI .0; // master Axis phasing
    WTM .0;
    PWR .0 = 0;
    WTM .0;
    WTT .0 = 0.1;

    PWR .0 = 1; // master Axis Enable
    WTM .0;
    WTT .0 = 0.1;

    IND .0; // Index Homing start
    WTM .0;
    WTT .0 = 1.0;

    MVE .0 = 0.0; // Index position moving
    WTM .0;
    WTT .0 = 2.0;

    K36 .0  = 0x1; // software position limit on
    k165 .0 = 1;   // compensation data on

    SPD .0 = 0.2;  // Set nominal velocity
    ACC .0 = 0.98; // Set Max Acceleration
    JRT .0 = 0.05; // Set Jeck time

    X10 .0 = 1; // user variable(Home Done ON)
}

// ABS Move
// X20: target position
void func20(void)
{
    MVE .0 = X20;
    WTM .0;
}

// REL Move
// X21: target length
void func21(void)
{
    MVE .0 = M7 + X21;
    WTM .0;
}

void func50(void)
{
    int M;
    SPD .0 = 0.2;
    ACC .0 = 0.98;
    JRT .0 = 0.05;

    MVE .0 = 0.0;
    WTM .0;
    WTT .0 = 1.0;

    for (M = 0; M < 5000000; M++) // count
    {
        MVE .0 = 0.3;
        WTM .0;
        WTT .0 = 1.0;

        MVE .0 = 0.0;
        WTM .0;
        WTT .0 = 1.0;
    }
}

