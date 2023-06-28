#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>      /*  fputs   */
#include <stdlib.h>		/*	exit	*/

#define UNUSED(x) (void)(x)

#ifdef NDEBUG
#define DEBUG_ONLY(x) (x)
#else
#define DEBUG_ONLY(x)
#endif

/******************************************************************************/
/*  colors  */

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/******************************************************************************/

#define RETURN_IF_BAD(IS_GOOD, MSG, RETVAL) if (!(IS_GOOD)) \
                                            { \
                                                printf(ANSI_COLOR_RED);\
                                                fputs(MSG, stderr); \
                                                return(RETVAL); \
                                            }

#define RETURN_IF_BAD_CLEAN(IS_GOOD, MSG, RETVAL, CLEANUP) if (!(IS_GOOD)) \
                                                    { \
                                                        printf(ANSI_COLOR_RED);\
                                                        CLEANUP; \
                                                        fputs(MSG, stderr); \
                                                        return(RETVAL); \
                                                    }
#define RETURN_IF_BAD_NO_RETURN(IS_GOOD, MSG) if (!(IS_GOOD)) \
                                            { \
                                                printf(ANSI_COLOR_RED);\
                                                fputs(MSG, stderr); \
                                            }


enum {SUCCESS = 0 , FAIL = 1};

void ExitIfBad(int is_good_, const char *msg_, int exit_val_)
{
    if (is_good_)
    {
        return;
    }
    
    fputs(msg_, stderr);
    
    exit(exit_val_);
}

#endif  /*  __UTILS_H__  */
