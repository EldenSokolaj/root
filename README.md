# root
A less bloated sudo that is probably insecure and a horrible idea to ever use.

Will, of course, only work on linux.

Setup
    
    Compile, possibly with "gcc root.c -o root -lcrypt"
    Then make sure to set the setuid bit first
        - possibly with running "chown root root root" then "chmod +s root" as the user root)
    Then either set an alias in .bashrc of move the file to /bin/root

Usage:
    
    root $command (arguments)

Disclaimer
    
    I'm not sure if it is safe or not. So just don't use it.
    Also, unlike sudo there is no concept of "sudoers", so just any user can run commands as root...
    I am planning on fixing that soon, but for now, say good bye to any sense of security if you use this program

Help
    
    If you see a security flaw, or happen to know how to get the suid of the process, please let me know.
