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
    
    I'm not sure if it is safe or not, so use at your own risk.
    Only users in sudo group will be able to use command

Help
    
    If you see a security flaw please let me know.

Possible Bugs

    - if 'sudo' group in '/etc/group' is greater than 100 characters it may be possible to use an incomplete username to bypass the check
        - example:
            - if in '/etc/group' the sudo entry looks like 'sudo:x:27:user1,user2...safe_user' but the 100 character mark is on the 'saf'
            - so then the char buf[100] array will hold '...sa' (as the last character is nullified to handle newline)
            - and making a user names 'sa' will allow access to the program
            - this also means that if you try to use root as 'safe_user' it will say you are not in group sudo
    - if you have any advice on how to fix this, please let me know (but to be fair, if you have over 100 bytes of sudo entrys then...)
