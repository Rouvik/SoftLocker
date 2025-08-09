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

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "helper.h"
#include "handleLock.h"

int main(int argc, const char *argv[])
{
    if (argc > 1)
    {
        if (!strcmp(argv[1], "chpasswd"))
        {
            puts("Password changing, encryption coming soon!!!\nFor now you need to recompile the project to change passwords, edit " ANSI_GREEN "ENV.h" ANSI_WHITE " and recompile to reset new password\n");
        }
        else if (!strcmp(argv[1], "help"))
        {
            printf("SLock " VERSION_STR " - " REPO_LINK "\n"
                   "Syntax: %s\t\tSoft Locks the shell\n"
                   "Syntax: %s help\t\tBrings you here\n"
                   "Syntax: %s chpasswd\t\tUnimplemented\n\n",
                   argv[0], argv[0], argv[0]);
        }
        else
        {
            puts(ANSI_RED "[WARNING] Unknown subcommand" ANSI_WHITE "\n");
        }
    }

    return handleLock();
}