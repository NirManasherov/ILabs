/*******************************************************************************
 * Project:     Watchdog
 * Author:      HRD28
 * Version:     1.0 - 09/03/2023
 *		1.1 - 12/03/2023 add stddef.h, removed comments
*******************************************************************************/
#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include <stddef.h>

int MakeMeImmortal(int argc, char *argv[], size_t interval, size_t max_misses);


int DoNotResuscitate(void);

#endif  /*  __WATCHDOG_H__  */
