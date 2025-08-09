#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// set according to your system and compile
#define PASSWORD "something"
#define UNAME "Rouvik Maji"

#define CLRSCR "\033[H\033[2J"
#define ANSI_RED "\e[0;31m"
#define ANSI_WHITE "\e[0;37m"
#define ANSI_GREEN "\e[0;32m"


static inline bool disableEcho(struct termios *old, struct termios *new)
{
    if (tcgetattr(STDIN_FILENO, old) == -1)
    {
        perror("[ERROR / WARNING] Failed to get termios structure, password echo is enabled");
        return false;
    }

    *new = *old;
    new->c_lflag &= ~ECHO;
    new->c_lflag &= ~ICANON;
    new->c_lflag &= ~ISIG;

    if (tcsetattr(STDIN_FILENO, TCSANOW, new) == -1)
    {
        perror("[ERROR / WARNING] Failed to set termios structure, password echo is enabled");
        return false;
    }

    return true;
}

int main()
{
    char passwdBuf[16];
    struct termios old, new;
    bool echoSet = true;

    echoSet = disableEcho(&old, &new);

    int previousAttempts = 0;
    while (true)
    {
        printf("Previous attempts: %d\nEnter password: ", previousAttempts);
        fflush(stdout);

        int i = 0;
        char ch = 0;
        while (read(STDIN_FILENO, &ch, 1) > 0 && ch != '\n' && i < sizeof(passwdBuf) - 1)
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

        char *newLine = strrchr(passwdBuf, '\n');
        if (newLine)
        {
            *newLine = 0;
        }

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

    // reset terminal back to normal
    if (echoSet)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
    }

    return 0;
}