#include <cdk.h>
#include <ncurses.h>

// Compile
// $ gcc -w -std=c99 jeep-cdk_init.test.c -o initcdk -lncurses -lcdk && ./initcdk
int 
main() {

    CDKSCREEN   *cdks;
    CDKLABEL     *demo;
    char         *mesg[4];

    cdks = initCDKScreen (NULL);
    initCDKColor();

    /* Set the labels up.  */
    mesg[0] = "<C><#UL><#HL(26)><#UR>";
    mesg[1] = "<C><#VL></R>This text should be boxed.<!R><#VL>";
    mesg[2] = "<C><#LL><#HL(26)><#LR>";
    mesg[3] = "<C>While this is not.";

    /* Declare the labels.  */
    demo = newCDKLabel (cdks, CENTER, CENTER, mesg, 4, TRUE, TRUE);

    /* Is the label NULL???  */
    if (demo == (CDKLABEL *)NULL)
    {
       /* Clean up the memory.  */
       destroyCDKScreen (cdks);

       /* End curses...  */
       endCDK();

       /* Spit out a message.  */
       printf ("Oops. Can't seem to create the label. Is the window too small?\n");
       exit (1);
    }

    /* Draw the CDK screen.  */
    refreshCDKScreen (cdks);
    waitCDKLabel (demo, ' ');

    /* Clean up */
    destroyCDKLabel (demo);
    destroyCDKScreen (cdks);


    endCDK();
    exit(0);


    return 0;
}

