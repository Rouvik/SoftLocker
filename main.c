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

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// set according to your system and compile
#define PASSWORD "something"
#define UNAME "Rouvik Maji"

#define PASSWD_BUFFER_SIZE 16
#define CLRSCR "\033[H\033[2J"
#define ANSI_RED "\e[0;31m"
#define ANSI_WHITE "\e[0;37m"
#define ANSI_GREEN "\e[0;32m"

/**
 * @brief Takes in a termios struct to store the old termios settings and then set the termios settings to hide passwd and disable signals
 *
 * @param old struct termios * used to store the old terminal settings
 * @return true If the password settings is set correctly
 * @return false If the password settings could not be set
 */
static inline bool disableEcho(struct termios *old)
{
    if (tcgetattr(STDIN_FILENO, old) == -1)
    {
        perror("[ERROR / WARNING] Failed to get termios structure, password echo is enabled");
        return false;
    }

    struct termios new = *old;
    new.c_lflag &= ~(ECHO | ICANON | ISIG); // disable terminal echoing (hide passwd, canonical to get raw IO and signals to make life easier for me to disable signals)

    if (tcsetattr(STDIN_FILENO, TCSANOW, &new) == -1)
    {
        perror("[ERROR / WARNING] Failed to set termios structure, password echo is enabled");
        return false;
    }

    return true;
}

int main()
{
    char passwdBuf[PASSWD_BUFFER_SIZE];
    struct termios old;
    bool echoSet = true; // to keep track if echoing has been disabled

    echoSet = disableEcho(&old); // if false then warning, password will be visible :0

    int previousAttempts = 0;
    while (true)
    {
        printf("Previous attempts: %d\nEnter password: ", previousAttempts); // print last password attempts here and ask for new passwd
        fflush(stdout);                                                      // flush is used to make sure the lines are printed UwU

        int i = 0;
        char ch = 0;
        while (read(STDIN_FILENO, &ch, 1) > 0 && ch != '\n' && i < sizeof(passwdBuf) - 1)   // read the passwd chars one by one and print * instead
        {
            if ((ch == 0x7f || ch == 0x08) && i > 0) // for backspace
            {
                i--;
                write(STDOUT_FILENO, "\b \b", 3);
                continue;
            }

            passwdBuf[i++] = ch;
            write(STDOUT_FILENO, "*", 1);
        }
        passwdBuf[i] = 0;

        // make sure no \n exists in final passwd
        char *newLine = strrchr(passwdBuf, '\n');
        if (newLine)
        {
            *newLine = 0;
        }

        // passwd check (hope no one does a LDPRELOAD hack to swap out strcmp (thats way out of scope for this program tho))
        if (!strcmp(passwdBuf, PASSWORD))
        {
            puts(ANSI_GREEN "\nWelcome back " UNAME "!" ANSI_WHITE);
            break;
        }
        else
        {
            fputs(CLRSCR ANSI_RED "[ERROR] Bad attempt, please try again\n" ANSI_WHITE, stderr);
            previousAttempts++;
        }
    }

    // reset terminal back to normal (if this fails we might be stuck in a broken state)
    if (echoSet)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
    }

    return 0;
}