/******************************************************************************
*                                                                             *
*   Module:     locals.c                                                      *
*                                                                             *
*   Date:       01/18/2002                                                    *
*                                                                             *
*   Copyright (c) 2000 - 2002 Goran Devic                                     *
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

        This module contans code to display local variables of a function
        scope.

*******************************************************************************
*                                                                             *
*   Changes:                                                                  *
*                                                                             *
*   DATE     DESCRIPTION OF CHANGES                               AUTHOR      *
* --------   ---------------------------------------------------  ----------- *
* 01/18/02   Original                                             Goran Devic *
* --------   ---------------------------------------------------  ----------- *
*******************************************************************************
*   Include Files                                                             *
******************************************************************************/

#include "module-header.h"              // Versatile module header file

#include "clib.h"                       // Include C library header file
#include "ice.h"                        // Include main debugger structures

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

#define MAX_LOCALS_QUEUE    32          // Max number of local variables to display

static TSYMFNSCOPE1 *LocalsQueue[MAX_LOCALS_QUEUE + 1];

static char buf[MAX_STRING];

/******************************************************************************
*                                                                             *
*   External Functions                                                        *
*                                                                             *
******************************************************************************/

extern BOOL SymEvalFnScope1(char *pBuf, TSYMFNSCOPE1 *Local);

/******************************************************************************
*                                                                             *
*   Functions                                                                 *
*                                                                             *
******************************************************************************/

/******************************************************************************
*                                                                             *
*   void LocalsDraw(BOOL fForce)                                              *
*                                                                             *
*******************************************************************************
*
*   Draws local variables
*
******************************************************************************/
void LocalsDraw(BOOL fForce)
{
    int maxLines = 9999;
    int nLine = 1;
    int i;

    if( pWin->l.fVisible==TRUE )
    {
        dprint("%c%c%c%c", DP_SAVEXY, DP_SETCURSORXY, 0+1, pWin->l.Top+1);
        PrintLine(" Locals");

        maxLines = pWin->l.nLines;
    }
    else
        if( fForce==FALSE )
            return;

    // Attempt to display locals only if we have function scope defined
    if( deb.pFnScope )
    {
        // This is without type information...
        // TODO: Type information for local variables

        i = 0;

        while( LocalsQueue[i]!=NULL && nLine<maxLines)
        {
            if( SymEvalFnScope1(buf, LocalsQueue[i])==TRUE )
            {
                if( dprinth(nLine++, "%s", buf)==FALSE )
                    break;
            }

            i++;
        }
    }


    if( pWin->l.fVisible==TRUE )
        dprint("%c", DP_RESTOREXY);
}

/******************************************************************************
*                                                                             *
*   BOOL cmdLocals(char *args, int subClass)                                  *
*                                                                             *
*******************************************************************************
*
*   List local variables from the current stack frame to the command window.
*
******************************************************************************/
BOOL cmdLocals(char *args, int subClass)
{
    LocalsDraw(TRUE);

    return( TRUE );
}


/******************************************************************************
*                                                                             *
*   BOOL FillLocalScope(TSYMFNSCOPE *pFnScope, DWORD dwOffset)                *
*                                                                             *
*******************************************************************************
*
*   Fills in the array of pointers to local variables at the given context
*   of pFnScope and the eip address.
*
*   Where:
*       pFnScope is the function scope to search
*       dwOffset is the offset address within that function scope
*   Returns:
*       TRUE array filled up with zero or more values
*       FALSE Invalid scope or offset
*
******************************************************************************/
BOOL FillLocalScope(TSYMFNSCOPE *pFnScope, DWORD dwOffset)
{
    DWORD dwFnOffset;
    int i, position, index, xNested;

    index = 0;                      // Index into the writing locals queue
    LocalsQueue[index] = NULL;      // Always terminate the queue
    
    // Make sure our code offset is within this function scope descriptor
    if( pFnScope && pFnScope->dwStartAddress<=dwOffset && pFnScope->dwEndAddress>=dwOffset )
    {
        // Scan forward for the right scope brackets depending on the code offset
        dwFnOffset = dwOffset - pFnScope->dwStartAddress;
        position = -1;                  // -1 means we did not find any locals or out of scope

        for(i=0; i<pFnScope->nTokens; i++ )
        {
            if( pFnScope->list[i].TokType==TOKTYPE_LBRAC || pFnScope->list[i].TokType==TOKTYPE_RBRAC )
            {
                if( dwFnOffset >= pFnScope->list[i].p1 )
                    position = i;
                else
                if( dwFnOffset < pFnScope->list[i].p1 )
                    break;
            }
        }

        i = position;               // We will use 'i' from now on...

        // Fill in the queue by traversing back the function scope list
        if( i >= 0 )
        {
            index = 0;              // Index into the locals queue
            xNested = 1;            // Make the current nesting level normal

            while( index<MAX_LOCALS_QUEUE && i>=0 )
            {
                if( pFnScope->list[i].TokType==TOKTYPE_RBRAC )
                    xNested++;      // Increase nesting level - hide scope variables of that level
                else
                if( pFnScope->list[i].TokType==TOKTYPE_LBRAC )
                {
                    if( xNested>0 ) // Decrease nesting level only if we have a extra scope to avoid
                        xNested--;
                }
                else
                {
                    if( xNested==0 )
                    {
                        // Store a local variable that is now visible
                        LocalsQueue[index] = &pFnScope->list[i];
                        index++;            // Increment the index into the locals queue
                    }
                }
                i--;                // Decrement the index within the function scope list
            }
        }
    }
    else
        return( FALSE );

    LocalsQueue[index] = NULL;      // Always terminate the queue

    return( TRUE );
}

