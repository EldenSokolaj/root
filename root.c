#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

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

    if(argc > 1){
        user = getenv("USER");
        if(user == NULL){
            puts("User environment variable not set!");
            exit(1);
        }

        printf("Enter password for user '%s': ", user);
        fgets(pass, 50, stdin);
        if(strlen(pass) != 0){
            pass[strlen(pass) - 1] = 0;
        }

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
