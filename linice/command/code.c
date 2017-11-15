/******************************************************************************
*                                                                             *
*   Module:     code.c                                                        *
*                                                                             *
*   Date:       11/16/00                                                      *
*                                                                             *
*   Copyright (c) 2000 - 2001 Goran Devic                                     *
*                                                                             *
*   Author:     Goran Devic                                                   *
*                                                                             *
*******************************************************************************

    Module Description:

        This module contains disassembly command

*******************************************************************************
*                                                                             *
*   Changes:                                                                  *
*                                                                             *
*   DATE     DESCRIPTION OF CHANGES                               AUTHOR      *
* --------   ---------------------------------------------------  ----------- *
* 11/16/00   Original                                             Goran Devic *
* 03/11/01   Second edition                                       Goran Devic *
* --------   ---------------------------------------------------  ----------- *
*******************************************************************************
*   Include Files                                                             *
******************************************************************************/

#include "module-header.h"              // Versatile module header file

#include "clib.h"                       // Include C library header file
#include "ice.h"                        // Include main debugger structures
#include "disassembler.h"               // Include disassembler

/******************************************************************************
*                                                                             *
*   Global Variables                                                          *
*                                                                             *
******************************************************************************/

/******************************************************************************
*                                                                             *
*   Local Defines, Variables and Macros                                       *
*                                                                             *
******************************************************************************/

#define CODE_BYTES         8

static char buf[MAX_STRING];
static char disasm[MAX_STRING];


/******************************************************************************
*                                                                             *
*   External Functions                                                        *
*                                                                             *
******************************************************************************/

/******************************************************************************
*                                                                             *
*   Functions                                                                 *
*                                                                             *
******************************************************************************/

static DWORD GetCodeLine(PTADDRDESC pAddr)
{
    TDISASM dis;
    int i, pos;

    pos = sprintf(buf, "%04X:%08X ", pAddr->sel, pAddr->offset);

    dis.dwFlags  = DIS_DATA32 | DIS_ADDRESS32;
    dis.wSel = pAddr->sel;
    dis.dwOffset = pAddr->offset;
    dis.szDisasm = disasm;

    // Disassemble and store into the line buffer
    Disassembler( &dis );

    // If CODE was ON, print the code bytes
    if( deb.fCodeOn )
    {
        for( i=0; i<dis.bInstrLen && i<CODE_BYTES; i++ )
        {
            pos += sprintf(buf+pos, "%02X ", dis.bCodes[i]);
        }

        // Append spaces
        while( i-- > 0 )
        {
            pos += sprintf(buf+pos, "   ");
        }
    }

    return( dis.bInstrLen );
}


static void PrintCodeLines(int lines)
{
    int nLen;
    TADDRDESC Addr;

    Addr = deb.codeAddr;                // Copy the current code address

    while( lines-- > 0 )
    {
        nLen = GetCodeLine(&Addr);
        dprinth(lines, "%s%s", buf, disasm);

        // Advance code offset for the next line
        Addr.offset += nLen;
    }
}

void CodeDraw()
{
    dprint("-Code---------------------------------------------------------------------------\n");

    PrintCodeLines(pWin->c.nLines - 1);
}


BOOL cmdUnassemble(char *args)
{
    PrintCodeLines(8);

    return( TRUE );
}
