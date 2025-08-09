/*
MIT License

Copyright (c) 2025 Rouvik Maji

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef INCLUDED_HANDLELOCK_H
#define INCLUDED_HANDLELOCK_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "helper.h"

#include "ENV.h"

/**
 * @brief Driver code for handling the main soft lock mechanism
 * 
 * @return int The status code of lock, should never return until user gets to unlock and if unlocked returns 0
 */
int handleLock()
{
    char *HOME = getenv("HOME");
    if (!HOME)
    {
        perror("[FATAL ERROR] Missing HOME environment variable cannot detect stored password hash, aborting...");
        return 1;
    }

    struct termios old;
    bool echoSet = true; // to keep track if echoing has been disabled

    char passwdBuf[PASSWD_BUFFER_SIZE];

    echoSet = disableEcho(&old); // if false then warning, password will be visible :0

    int previousAttempts = 0;
    while (true)
    {
        printf("This shell has been soft locked by SLock " VERSION_STR " - " REPO_LINK "\n"
               "Please use the correct SLock password to unlock or contact owner if you are not him/her\n\n"
               "Previous attempts: %d\nEnter password: ",
               previousAttempts); // print last password attempts here and ask for new passwd
        fflush(stdout);           // flush is used to make sure the lines are printed UwU

        inputPasswd(passwdBuf, PASSWD_BUFFER_SIZE);

        // passwd check (hope no one does a LDPRELOAD hack to swap out strcmp (thats way out of scope for this program tho))
        if (!strcmp(passwdBuf, PASSWORD))
        {
            puts(ANSI_GREEN "\nWelcome back!" ANSI_WHITE);
            break;
        }
        else
        {
            fputs(CLRSCR ANSI_RED "[ERROR] Bad attempt, please try again\n" ANSI_WHITE, stderr);
            previousAttempts++;
        }
    }

    // reset terminal back to normal (if this fails we might be stuck in a broken state)
    enableEcho(&old, echoSet);

    return 0;
}

#endif // INCLUDED_HANDLELOCK_H