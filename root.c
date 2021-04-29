#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

// @return 0 - user is in group 'sudo', otherwise no
int checkSudo( const char* user ){

    //open file /etc/group and verify that it exists
    FILE *groups = fopen("/etc/group", "r");

    if(groups == NULL){
        printf("Failed to read group entry for user '%s'\n", user);
        return 1;
    }

    //get username length (useful later for parsing)
    int userlen = strlen(user);
    
    //read file line-by-line
    char buf[100];

    while(fgets(buf, 100, groups) != NULL){
        
        //look for 'sudo' entry [ example 'sudo:x:27:user1,user2' ]
        if(strncmp(buf, "sudo", 4) == 0){

            //nullify the last character (fgets ends in '\n', and it makes the string harder to parse)
            buf[strlen(buf) - 1] = 0;
            
            //shift to 'users' section of entry
            int n = 3, i = 4;
            while(n){
                if(buf[i] == ':'){ n--; }
                i++;
            }

            //loop through users to check for user to verify
            char *split, *loc = &buf[i];

            while(1){
                split = strchr(loc, ',');
                
                if(split == NULL){
                    
                    //last entry (so return the result of a check if user == last entry)
                    return strcmp(user, loc);

                } else {
                    
                    //general case
                    if(split - loc == userlen && strncmp(user, loc, userlen) == 0 ){
                        return 0;
                    }
                    loc = split + 1;

                }

            }

        }

    }
}

// @return 0 - password is correct, otherwise no
int checkLogin( const char* user, const char* password ){

    struct passwd* passwdEntry = getpwnam(user);

    if( !passwdEntry ){
        printf("User '%s' doesn't exist\n", user);
        return 1;
    }

    if ( strcmp(passwdEntry->pw_passwd, "x") != 0 ){

        return strcmp( passwdEntry->pw_passwd, crypt(password, passwdEntry->pw_passwd) );

    } else {

        struct spwd* shadowEntry = getspnam(user);

        if ( !shadowEntry ) {
            printf("Failed to read shadow entry for user '%s'\n", user);
            return 1;
        }

        return strcmp( shadowEntry->sp_pwdp, crypt(password, shadowEntry->sp_pwdp) );
    }
}

int main(int argc, char *argv[]){
    char *user;
    char pass[50];
    
    //check that there is at least 1 argument (+ the root command itself)
    if(argc > 1){

        //get user environment variable and make sure it is not NULL
        user = getenv("USER");

        if(user == NULL){
            puts("User environment variable not set!");
            exit(1);
        }

        //make sure user is either root or in group 'sudo'
        if(strcmp(user, "root") && checkSudo(user)){
            puts("User is not part of the group 'sudo'");
            exit(1);
        }

        //get password and nullify ending newline
        printf("Enter password for user '%s': ", user);
        fgets(pass, 50, stdin);
        if(strlen(pass) != 0){
            pass[strlen(pass) - 1] = 0;
        }

        //check that the login is valid, and if so run the command with arguments
        if(checkLogin(user, pass) == 0){
            execvp(argv[1], &argv[1]);
        } else {  
            puts("[Login Failed]");
        }

    } else {
        puts("Usage: root <command> <args>");
    }

    return 0;
}
