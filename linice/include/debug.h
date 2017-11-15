/******************************************************************************
*                                                                             *
*   Module:     debug.h                                                       *
*                                                                             *
*   Date:       03/03/01                                                      *
*                                                                             *
*   Copyright (c) 2001 Goran Devic                                            *
*                                                                             *
*   Author:     Goran Devic                                                   *
*                                                                             *
*******************************************************************************

    Module Description:

        This header file defines debug functions

*******************************************************************************
*                                                                             *
*   Major changes:                                                            *
*                                                                             *
*   DATE     DESCRIPTION OF CHANGES                               AUTHOR      *
* --------   ---------------------------------------------------  ----------- *
* 03/03/01   Initial version                                      Goran Devic *
* --------   ---------------------------------------------------  ----------- *
*******************************************************************************
*   Important Defines                                                         *
******************************************************************************/
#ifndef _DEBUG_H_
#define _DEBUG_H_

/******************************************************************************
*                                                                             *
*   Global Defines, Variables and Macros                                      *
*                                                                             *
******************************************************************************/

#ifdef DBG // ----------------------------------------------------------------

extern int ice_debug_level;

#define INFO(args)                                          \
{                                                           \
    if(ice_debug_level > 0)                                 \
    {                                                       \
        printk("Info: %s,%d: ", __FILE__, __LINE__);        \
        printk##args;                                       \
    }                                                       \
}


#define ERROR(args)                                         \
{                                                           \
    printk("Error: %s,%d: ", __FILE__, __LINE__);           \
    printk##args;                                           \
}


#else // DBG -----------------------------------------------------------------

#define INFO(args)      NULL
#define ERROR(args)     NULL

#endif // DBG ----------------------------------------------------------------


#endif //  _DEBUG_H_