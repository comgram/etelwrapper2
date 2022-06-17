#COMET special parameters
#ISO linear    units : m, m / s, m / s², V, A, s(used in case of interpolation)
#ISO rotary    units : deg, deg / s, deg / s², V, A, s

///////////////////////////////////////////////////////////
// Customer/Project info : Kodis
// Projct Dimension/Sales Order No. : S2B213
// Revision history:
// R0 : 2021.11.18, HS LIM, First edition at Soonhan FAB
///////////////////////////////////////////////////////////

// X Axis Homing
void func10(void)
{
    X10 .1  = 0;  // user variable(Home done off)
    K40 .1  = 45; // master Axis homing methord
    K165 .1 = 0;  // compensation data on

    RST .1;
    K36 .1 = 0; // software position limit off
    WTT .1 = 0.05;

    INI .1; // master Axis phasing
    WTM .1;
    PWR .1 = 0;
    WTM .1;
    WTT .1 = 0.1;

    PWR .1 = 1; // master Axis Enable
    WTM .1;
    WTT .1 = 0.1;

    IND .1; // Index Homing start
    WTM .1;
    WTT .1 = 1.0;

    MVE .1 = 0.0; // Index position moving
    WTM .1;
    WTT .1 = 2.0;

    K36 .1  = 0x1; // software position limit on
    k165 .1 = 0x1; // compensation data on

    SPD .1 = 0.2;  // Set nominal velocity
    ACC .1 = 0.98; // Set Max Acceleration
    JRT .1 = 0.05; // Set Jeck time

    X10 .1 = 1; // user variable(Home Done ON)
}

void func50(void)
{
    int M;
    SPD .1 = 0.2;
    ACC .1 = 0.98;
    JRT .1 = 0.05;

    MVE .1 = 0.0;
    WTM .1;
    WTT .1 = 1.0;

    for (M = 0; M < 5000000; M++) // count
    {
        MVE .1 = 0.5;
        WTM .1;
        WTT .1 = 1.0;

        MVE .1 = 0.0;
        WTM .1;
        WTT .1 = 1.0;
    }
}

