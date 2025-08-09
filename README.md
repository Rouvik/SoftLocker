# SLock
A simple command line soft locker to prevent others from messing around in my terminal

# Why use SLock ?
Have you experienced situations when you might have to work in a shared ssh server with terrible password security
(like every password same as account name)?

You might already know how dangerous it is and how easy it is to login
to someone else's account and then wreak havoc. However my institution doesnot care and doesnot allow us to change passwords
requiring me to setup such soft lockers to prevent easy shell access, it simply locks down the shell and asks for a hardcoded raw password hidden in
the executable to check.

It's simple, requires minimal setup in such restricted environments and is good enough to prevent snooping around or at least make it really hard to do so.

# Caveats
SLock is not secure from outside, even though most signals are blocked,
signals like SIGKILL and SIGSTOP cannot be caught so someone getting a shell else where and using `kill -9 ...` can provide access, however in my case I
use it to lock the shell directly on login so getting shell elsewhere is difficult.

# Stuff to do in future?
- Maybe store the password as a hash to prevent from disassembly and reading
- Maybe allow passwords to be changed and not hardcoded during compilation

# Author
Rouvik Maji - [majirouvik@gmail.com](mailto:majirouvik@gmail.com)