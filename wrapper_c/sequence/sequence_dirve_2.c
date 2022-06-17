#COMET special parameters
#ISO linear    units : m, m / s, m / s², V, A, s(used in case of interpolation)
#ISO rotary    units : deg, deg / s, deg / s², V, A, s

///////////////////////////////////////////////////////////
// Customer/Project info : Kodis
// Projct Dimension/Sales Order No. : S2B213
// Revision history:
// R0 : 2021.11.18, HS LIM, First edition at Soonhan FAB
///////////////////////////////////////////////////////////

// Z Axis Homing
void func10(void)
{
    X10 .2  = 0;  // user variable(Home done off)
    K40 .2  = 45; // master Axis homing methord
    K165 .2 = 0;  // compensation data on

    RST .2;
    K36 .2 = 0; // software position limit off
    WTT .2 = 0.05;

    INI .2; // master Axis phasing
    WTM .2;
    PWR .2 = 0;
    WTM .2;
    WTT .2 = 0.1;

    PWR .2 = 1; // master Axis Enable
    WTM .2;
    WTT .2 = 0.1;

    IND .2; // Index Homing start
    WTM .2;
    WTT .2 = 1.0;

    MVE .2 = 0.0; // Index position moving
    WTM .2;
    WTT .2 = 1.0;

    K36 .2  = 0x1; // software position limit on
    k165 .2 = 1;   // compensation data on

    SPD .2 = 0.1;  // Set nominal velocity
    ACC .2 = 0.98; // Set Max Acceleration
    JRT .2 = 0.05; // Set Jeck time

    X10 .2 = 1; // user variable(Home Done ON)
}

void func20(void)
{
    int M;

    MVE .2 = 0.01;
    WTM .2;
    WTT .2 = 1.0;

    for (M = 0; M < 100; M++) // count
    {
        MVE .2 = 0.01;
        WTM .2;
        WTT .2 = 7.0;

        MVE .2 = 0.0101;
        WTM .2;
        WTT .2 = 7.0;
    }
}

void func50(void)
{
    int M;
    SPD .2 = 0.1;
    ACC .2 = 0.98;
    JRT .2 = 0.05;

    MVE .2 = 0.0;
    WTM .2;
    WTT .2 = 1.0;

    for (M = 0; M < 5000000; M++) // count
    {
        MVE .2 = 0.1;
        WTM .2;
        WTT .2 = 1.0;

        MVE .2 = 0.0;
        WTM .2;
        WTT .2 = 1.0;
    }
}

