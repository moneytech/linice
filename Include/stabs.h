/******************************************************************************
*                                                                             *
*   Module:     stabs.h                                                       *
*                                                                             *
*   Date:       06/10/01                                                      *
*                                                                             *
*   Copyright (c) 2001 Goran Devic                                            *
*                                                                             *
*   Author:     Goran Devic                                                   *
*                                                                             *
*   This source code and produced executable is copyrighted by Goran Devic.   *
*   This source, portions or complete, and its derivatives can not be given,  *
*   copied, or distributed by any means without explicit written permission   *
*   of the copyright owner. All other rights, including intellectual          *
*   property rights, are implicitly reserved. There is no guarantee of any    *
*   kind that this software would perform, and nobody is liable for the       *
*   consequences of running it. Use at your own risk.                         *
*                                                                             *
*******************************************************************************

    Module Description:

        Define wrappers for ELF stabs.

*******************************************************************************
*                                                                             *
*   Major changes:                                                            *
*                                                                             *
*   DATE     DESCRIPTION OF CHANGES                               AUTHOR      *
* --------   ---------------------------------------------------  ----------- *
* 06/10/01   Initial version                                      Goran Devic *
* --------   ---------------------------------------------------  ----------- *
*******************************************************************************
*   Important Defines                                                         *
******************************************************************************/
#ifndef _STABS_H_
#define _STABS_H_

/******************************************************************************
*                                                                             *
*   Global Defines, Variables and Macros                                      *
*                                                                             *
******************************************************************************/

#ifdef WIN32
#pragma pack(1)
#endif

//============================================================================
// 							STAB STRUCTURES
//============================================================================

typedef struct
{
    unsigned long n_strx;
    unsigned char n_type;
    unsigned char n_other;
    unsigned short n_desc;
    unsigned long n_value;
} StabEntry;


extern const char *SecType[];

// Statistics to be filled during the PASS 1
//===========================================

// Define a structure holding information for a given source file
typedef struct
{
    char *pPath;                        // Path to the source file
    char *pName;                        // File name of the source file
    DWORD nBINCL;                       // Number of include files for typedefs for a given source
    DWORD nTYPEDEF;                     // Number of TYPEDEFs in this source file
    DWORD nFUN;                         // Number of functions
    DWORD nGSYM;                        // Number of global symbols
    DWORD nSTSYM;                       // Number of file static symbols
} TSource;

extern DWORD nGSYM;                     // Total number of global symbols

#define MAX_SO      1024                // Max number of source file units that we handle
extern TSource SO[MAX_SO];              // Source file structure
extern DWORD nSO;                       // Number of basic source files


#endif // _STABS_H_