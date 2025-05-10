/**
*COSC 3250 - Project 3
**This program has 4 syscalls that interact with the UART to get, unget, check, and put characters in and out of a buffer and register. While  working on this project, we had conversations with a classmate named Spencer and his partner, but did not share any lines of code between us. We utilized ChatGPT while deciphering the problem and for understanding the UART registers. This consisted of having it explain what lines of code given to us in kprintf. c did and explainig what each bit in the lsr was responsible for. It helped us identify which bit to use for kputc. 
*@author [Owen Mongoven and Miah Nelson]
*Instructor [Dr. Brylow]
*TA-BOT:MAILTO [owen.mongoven@marquette.edu miah.nelson@marquette.edu]
*/


#include <xinu.h>

#define UNGETMAX 10             /* Can un-get at most 10 characters. */
static unsigned char ungetArray[UNGETMAX];
unsigned int  bufp;

syscall kgetc()
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // TODO: First, check the unget buffer for a character.
    //       Otherwise, check UART line status register, and
    //       once there is data ready, get character c.
    char c;
    if(bufp>0){
            c = ungetArray[bufp-1];
            bufp--;
            return c;
         }

    while (!(regptr->lsr & 0x01));

//check array for char, get char when found, if not found, check uart
    c = regptr->rbr;
    return c;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // TODO: Check the unget buffer and the UART for characters.

    //Check unget buffer for character
    if(bufp>0){
            return TRUE;
        }

    //Read UART Data Ready bit
    if(regptr->lsr & 0x01){
            return TRUE;
    }

        return FALSE;
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
    // TODO: Check for room in unget buffer, put the character in or discard.
    if(bufp<10){
            ungetArray[bufp] = c;
            bufp++;
            return c;
    }
    return SYSERR;
}

syscall kputc(uchar c)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // TODO: Check UART line status register.
    //       Once the Transmitter FIFO is empty, send character c.
   while(!(regptr->lsr & 0x20));

   regptr->thr = c;


    // i
    // f  THRempty in lsr (bit5 true)
    //send c to thr
    //
    //kputc does not end until thr is empty and c is sent

    return c;
}

syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(long, long))kputc, 0);
    va_end(ap);
    return retval;
}
