#define INCL_WINWINDOWMGR
#define INCL_DOSPROCESS
#include <os2.h>

#include <malloc.h>

USHORT  _System PlaceDialogButtons
(
    HAB     hab,                /* Anchor block handle  */
    HWND    hwndDlg,            /* Dialog window handle */
    SHORT   cButtons,           /* Count of buttons     */
    PSHORT  aidButtons          /* Array of button ids  */
);


/*********************************************************************

    FUNCTION:

        PlaceDialogButtons

    PURPOSE:

        Place dialog buttons evenly across bottom of dialog box

    COPYRIGHT:

        Version 1.0, June 17, 1990

        Copyright 1990, David C. Briccetti
        All Rights Reserved

        May be used for any purpose but not sold

    DESCRIPTION:

        This function dynamically positions dialog pushbuttons across
        the bottom of a dialog box.  It is useful in cases where
        different pushbuttons appear in different circumstances.

        For example, you may have a dialog box like the following:

        +---------------------------------+
        |                                 |
        |               ...               |
        |                                 |
        |                                 |
        |  +----+  +--------+  +-------+  |
        |  | Ok |  | Cancel |  | Print |  |
        |  +----+  +--------+  +-------+  |
        |                                 |
        +---------------------------------+

        In some cases you may want the dialog box to appear without
        the Print pushbutton.  You can use the same dialog box template
        but hide the Print button in the WM_INITDLG message processing.
        Then the box will look like this:

        +---------------------------------+
        |                                 |
        |               ...               |
        |                                 |
        |                                 |
        |  +----+  +--------+             |
        |  | Ok |  | Cancel |             |
        |  +----+  +--------+             |
        |                                 |
        +---------------------------------+

        The remaining buttons are not evenly spaced, but you can call
        PlaceDialogButtons which moves them so that they appear as
        follows:

        +---------------------------------+
        |                                 |
        |               ...               |
        |                                 |
        |                                 |
        |      +----+      +--------+     |
        |      | Ok |      | Cancel |     |
        |      +----+      +--------+     |
        |                                 |
        +---------------------------------+

        Before calling the function you allocate an array containing
        the button identifiers.  The code to position the buttons
        looks like this:

            PSHORT          aidButtons;

            aidButtons    = malloc (2 * sizeof (SHORT));
            aidButtons[0] = DID_OK;
            aidButtons[1] = DID_CANCEL;
            PlaceDialogButtons (hab, hwndDlg, 2, aidButtons);
            free (aidButtons);

 *********************************************************************/

USHORT  PlaceDialogButtons
(
    HAB     hab,                /* Anchor block handle  */
    HWND    hwndDlg,            /* Dialog window handle */
    SHORT   cButtons,           /* Count of buttons     */
    PSHORT  aidButtons          /* Array of button ids  */
)
{
    SWP     swpDlg;             /* SWP structure for width of dlg box   */
    PSWP    aswpButtons;        /* Array of SWP structures for buttons  */
    SHORT   cxTotalButtonWidths;/* Total of button widths               */
    SHORT   cxFreeSpace;        /* Free space not used by buttons       */
    SHORT   cxButtonMargin;     /* Space between buttons                */
    SHORT   xCurrentButton;     /* Var for placement of each button     */
    SHORT   i;                  /* Loop variable                        */

    WinQueryWindowPos (hwndDlg, &swpDlg);   /* Get dlg box dimensions   */


    /*
     *  Allocate space for array of SWP structures for
     *  positioning buttons
     */

    aswpButtons = malloc (cButtons * sizeof (SWP));


    /*
     *  Get button position info, and sum button widths
     */

    for (i = 0, cxTotalButtonWidths = 0; i < cButtons; ++i )
    {
        WinQueryWindowPos (
            WinWindowFromID (hwndDlg, aidButtons[i]),
            &aswpButtons[i]);
        cxTotalButtonWidths += aswpButtons[i].cx;
    }


    /*
     *  Calculate free width and new button margins
     */

    cxFreeSpace    = swpDlg.cx - cxTotalButtonWidths;
    cxButtonMargin = cxFreeSpace / (cButtons + 1);


    /*
     *  Adjust SWP structures for moving buttons to new
     *  positions
     */

    for (i = 0, xCurrentButton = cxButtonMargin;
        i < cButtons; ++i )
    {
        aswpButtons[i].x   = xCurrentButton;
        aswpButtons[i].fl  = SWP_MOVE;
        xCurrentButton    += (aswpButtons[i].cx + cxButtonMargin);
    }


    /*
     *  Move buttons
     */

    WinSetMultWindowPos (hab, aswpButtons, cButtons);


    /*
     *  Clean up
     */

    free (aswpButtons);

    return 0;
}

/* Required EMX/GCC DLL Entry/Exit Point */

/* Prototypes for EMX C Runtime Init/Term */
int _CRT_init(void);
void _CRT_term(void);

unsigned long _System _DLL_InitTerm(unsigned long hModule, unsigned long ulFlag)
{
    switch (ulFlag)
    {
        case 0: /* DLL Initialization */
            if (_CRT_init() != 0) {
                return 0; /* Failed to initialize C runtime */
            }
            return 1;     /* Success */

        case 1: /* DLL Termination */
            _CRT_term();
            return 1;     /* Success */
    }
    return 0;             /* Fail on unknown flags */
}
