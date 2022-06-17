#include "interface.h"

#define ETEL_OO_API
#include <conio.h>
#include <dex40.h>
#include <esd40.h>
#include <etb40.h>
#include <dsa40.h>
#include <lib40.h>
#include <tra40.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <windows.h>

DsaDrive drv;

void etel_wrapper(void)
{
}

void _open(const char *url)
{
    drv.open(url);
    drv.resetErrorEx(0, 2000);
}

void _create_drive(){
    drv.    
}
void _close(void)
{
    drv.close();
}

