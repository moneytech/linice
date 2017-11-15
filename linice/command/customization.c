/******************************************************************************
*                                                                             *
*   Module:     customization.c                                               *
*                                                                             *
*   Date:       04/15/01                                                      *
*                                                                             *
*   Copyright (c) 2001 - 2001 Goran Devic                                     *
*                                                                             *
*   Author:     Goran Devic                                                   *
*                                                                             *
*******************************************************************************

    Module Description:

        Customization commands

*******************************************************************************
*                                                                             *
*   Changes:                                                                  *
*                                                                             *
*   DATE     DESCRIPTION OF CHANGES                               AUTHOR      *
* --------   ---------------------------------------------------  ----------- *
* 04/15/01   Original                                             Goran Devic *
* --------   ---------------------------------------------------  ----------- *
*******************************************************************************
*   Include Files                                                             *
******************************************************************************/

#include "module-header.h"              // Versatile module header file

#include "clib.h"                       // Include C library header file
#include "ice.h"                        // Include main debugger structures

#include "debug.h"                      // Include our dprintk()

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

// Structure used by SET command
typedef struct
{
    char *sVar;                         // Variable name
    int  sLen;                          // Length of the name
    BOOL *pVal;                         // Address of the value variable
} TSETVAR, *PTSETVAR;

static TSETVAR SetVar[] = {
{ "altscr",   6, &deb.fAltscr },
{ "code" ,    4, &deb.fCode },
{ "faults",   6, &deb.fFaults },
{ "i1here",   6, &deb.fI1Here },
{ "i3here",   6, &deb.fI3Here },
{ "lowercase",9, &deb.fLowercase },
{ "pause",    5, &deb.fPause },
{ "symbols",  7, &deb.fSymbols },
{ NULL, }
};

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

extern void RecalculateDrawWindows();

/******************************************************************************
*                                                                             *
*   BOOL cmdCode(char *args, int subClass)                                    *
*                                                                             *
*******************************************************************************
*
*   CODE [on | off]     - Set code view in disassembly output
*   CODE                - Display state of the code setting
*
******************************************************************************/
BOOL cmdCode(char *args, int subClass)
{
    switch( GetOnOff(args) )
    {
        case 1:         // On
            deb.fCode = TRUE;
            RecalculateDrawWindows();
        break;

        case 2:         // Off
            deb.fCode = FALSE;
            RecalculateDrawWindows();
        break;

        case 3:         // Display the state of the CODE view
            dprinth(1, "Code is %s\n", deb.fCode? "on":"off");
        break;
    }

    return( TRUE );
}


/******************************************************************************
*                                                                             *
*   BOOL cmdSet(char *args, int subClass)                                     *
*                                                                             *
*******************************************************************************
*
*   Multiple SET variable handling:
*       SET ALTSCR [ON | OFF]
*       SET CODE   [ON | OFF]
*       SET FAULTS [ON | OFF]
*       SET I1HERE [ON | OFF]
*       SET I3HERE [ON | OFF]
*       SET LOWERCASE [ON | OFF]
*       SET PAUSE  [ON | OFF]
*       SET SYMBOLS [ON | OFF]
*
******************************************************************************/
BOOL cmdSet(char *args, int subClass)
{
    int nLine = 1;
    PTSETVAR pVar;

    pVar = SetVar;

    if( *args==0 )
    {
        // Simple SET command without parameters - list all variables
        while( pVar->sVar )
        {
            if(dprinth(nLine++, "%s is %s\n", pVar->sVar, *pVar->pVal? "on":"off")==FALSE)
                break;
            pVar++;
        }
    }
    else
    {
        // Set <VARIABLE> [ON | OFF]
        // Find the variable name
        while( pVar->sVar )
        {
            if( strnicmp(args, pVar->sVar, pVar->sLen)==0 )
                break;

            pVar++;
        }

        // If we did not find a predefined variable...
        if( pVar->sVar==NULL )
        {
            dprinth(1, "Set variable not found\n");
        }
        else
        {
            // Advance the argument pointer pass the var name and into the
            // possible argument [ON | OFF]
            args += pVar->sLen;

            switch( GetOnOff(args) )
            {
                case 1:         // On
                    *pVar->pVal = TRUE;
                    RecalculateDrawWindows();
                break;

                case 2:         // Off
                    *pVar->pVal = FALSE;
                    RecalculateDrawWindows();
                break;

                case 3:         // Display the state of the variable
                    dprinth(1, "%s is %s\n", pVar->sVar, *pVar->pVal? "on":"off");
                break;
            }
        }
    }

    return( TRUE );
}


/******************************************************************************
*                                                                             *
*   BOOL cmdLines(char *args, int subClass)                                   *
*                                                                             *
*******************************************************************************
*
*   Display or change number of display lines
*
******************************************************************************/
BOOL cmdLines(char *args, int subClass)
{
    int nLines;

    if( *args==0 )
    {
        // No arguments - display number of lines
        dprinth(1, "Number of lines is %d\n", pOut->sizeY);
    }
    else
    {
        // Set new number of lines
//        nLines = Evaluate( args, &args );

        // Get the 2-digit decimal number
    }

    return( TRUE );
}

