/******************************************************************************
*                                                                             *
*   Module:     module-header.h                                               *
*                                                                             *
*   Date:       03/03/01                                                      *
*                                                                             *
*   Copyright (c) 2001 Goran Devic                                            *
*                                                                             *
*   Author:     Goran Devic                                                   *
*                                                                             *
*******************************************************************************

    Module Description:

        This header file deals with differences in kernel versions.
        All differences should be abstracted here, mostly in the form of
        preprocessor macros.

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
#ifndef _VERSIONS_H_
#define _VERSIONS_H_

/******************************************************************************
*                                                                             *
*   Global Defines, Variables and Macros                                      *
*                                                                             *
******************************************************************************/

#include <autoconf.h>                   // Include our linux configuration
#include <version.h>                    // Include KERNEL_VERSION macro

//-----------------------------------------------------------------------------
// We had to have this kernel compiled with loadable modules

#ifndef CONFIG_MODULES
#error "This kernel is not compiled with CONFIG_MODULES option."
#endif

//-----------------------------------------------------------------------------
// In 2.2.3 /usr/include/linux/version.h includes a macro for this, 
//  but 2.0.35 doesn't - so add it here if necessary

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) ((a)*65536+(b)*256+(c))
#endif

//-----------------------------------------------------------------------------
// Define major kernel versions

#ifdef KERNEL_VERSION
#  if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 1, 0)
#    define KERNEL_2_1
#  endif
#  if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 2, 0)
#    define KERNEL_2_2
#  endif
#  if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 3, 1)
#    define KERNEL_2_3_1
#  endif
#endif


//-----------------------------------------------------------------------------
// Are we SMP?  Linux 2.2.x has this in the autoconf file, but prior versions 
// need it hand coded. 

#ifdef __SMP__
#undef __SMP__
#endif

#if defined(CONFIG_SMP) && !defined(SUPPORT_SMP)
#  error "SMP detected. Please uncomment SMP=1 in the Makefile."
#elif defined(KERNEL_2_1) && defined(SUPPORT_SMP)
#  define __SMP__ 1
#  define SMP_GLOBAL_VMLOCK
#endif

#if defined(CONFIG_MODVERSIONS) && defined(KERNEL_2_1)
#define MODVERSIONS
#include <linux/modversions.h>
#endif

#ifndef __KERNEL__
#define __KERNEL__
#endif


//-----------------------------------------------------------------------------
// Deal with the different function prototype for different kernel versions
//-----------------------------------------------------------------------------
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,0)

/* This function is called when a process closes the 
 * device file. It doesn't have a return value in 
 * version 2.0.x because it can't fail (you must ALWAYS
 * be able to close a device). In version 2.2.x it is 
 * allowed to fail - but we won't let it. 
 */

#define DEV_CLOSE_RET           int
#define DEV_CLOSE_RET_VAL       (0)

/* flush call is added in the kernel version 2.2.0 */

#define FLUSH_FOPS(function)    function,


#else // LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,0)

#define DEV_CLOSE_RET           void
#define DEV_CLOSE_RET_VAL

#define FLUSH_OPS(function)

#endif // LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,0)


#endif //  _VERSIONS_H_