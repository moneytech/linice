/******************************************************************************
*                                                                             *
*   Module:     initial.c                                                     *
*                                                                             *
*   Date:       10/26/00                                                      *
*                                                                             *
*   Author:     Goran Devic                                                   *
*                                                                             *
*******************************************************************************

    Module Description:

        This module contains initial load/unload code

*******************************************************************************
*                                                                             *
*   Changes:                                                                  *
*                                                                             *
*   DATE     DESCRIPTION OF CHANGES                               AUTHOR      *
* --------   ---------------------------------------------------  ----------- *
* 10/26/00   Original                                             Goran Devic *
* --------   ---------------------------------------------------  ----------- *
*******************************************************************************
*   Include Files                                                             *
******************************************************************************/

#include <linux/module.h>               // Include module header file

#include "clib.h"                       // Include C library header file

#include "intel.h"                      // Include Intel defines

#include "i386.h"                       // Include assembly code

#include "ice.h"                        // Include global structures

/******************************************************************************
*                                                                             *
*   Global Variables                                                          *
*                                                                             *
******************************************************************************/

int errno = 0;                          // Needed by the C library

TDeb deb;                               // Debugee state structure


/******************************************************************************
*                                                                             *
*   Local Defines, Variables and Macros                                       *
*                                                                             *
******************************************************************************/

/******************************************************************************
*                                                                             *
*   Functions                                                                 *
*                                                                             *
******************************************************************************/

void *malloc(size_t size)
{
    return( NULL );
}    


/******************************************************************************
*                                                                             *
*   int init_module(void)                                                     *
*                                                                             *
*******************************************************************************
*
*   This function is called when a module gets loaded.
*
*   Returns:
*       0 to load module
*       non-zero if a module should not be loaded
*
******************************************************************************/
int init_module(void)
{
    printk("<1>LinIce Copyright 2000 by Goran Devic\n");

#if DBG
    printk("<1>Debug build\n");
    ASSERT(sizeof(TSS_Struct) == 104);
    ASSERT(sizeof(TDescriptor) == 6);
#endif

    // Initialize deb data structure

    memset(&deb, 0, sizeof(deb));

    deb.fInt1Here = TRUE;
    deb.fInt3Here = TRUE;
    deb.fSetCode  = TRUE;

    // Set initial window visibility
    deb.wr.fVisible = TRUE;
    deb.wd.fVisible = TRUE;

    deb.wr.nLines = 3;
    deb.wd.nLines = 5;

    deb.nLines = 25;        // Set 25 line display

    deb.dumpMode = DD_BYTE; // Initially dump bytes
    deb.dumpSel = 0x18;     // Linux data selector :-)
    deb.dumpOffset = 0;     // Start at offset 0

    deb.codeMode = DC_ASM;
    deb.codeSel = 0x18;
    deb.codeOffset = 0xC0000000;

    // Initialize VGA text display subsystem

    VgaInit();

    // Store the current kernel IDT descriptor record so the 
    // hook functions can use it. We assume IDT does not change.
    GetIDT(&deb.idt);
    deb.pIdt = (TIDT_Gate *) GET_DESC_BASE(&deb.idt);

    // Save the IDT array while it is still pristine
    SaveIDT();

    // Hook the debugee IDT to break on selected interrupts/faults/traps
    HookDebuger();

    // Issue INT 3 to initially stop inside the debugger
    IssueInt3();

    return( 0 );
}    


/******************************************************************************
*                                                                             *
*   void cleanup_module(void)                                                 *
*                                                                             *
*******************************************************************************
*
*   This function is called when a module gets unloaded.
*
******************************************************************************/
void cleanup_module(void)
{
    // Unhook all the interrupts on exit.

    RestoreIDT();

    printk("<1>LinIce unloaded.\n");
}    

