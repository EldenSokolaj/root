/*
 * This code should be combined with root.c in order to update the parsing logic of reading the sudo group entry
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define true 1
#define false 0

int isSudo(const char* user){
    FILE *groups = fopen("groups", "r");
    
    if(!groups){
        puts("Failed to open file");
        exit(1);
    }
    
    char buf[101];
    buf[100] = 0;
    int userlen = strlen(user);
    
    while(fgets(buf, 100, groups)){
        if(!strncmp(buf, "sudo", 4)){
            
            int buflen = strlen(buf);
            
            char EOL = true;
            
            if(buf[buflen - 1] != 10){
                EOL = false;
            } else {
                buf[buflen - 1] = 0;
            }
            
            int n = 3, i = 4;
            while(n){
                if(buf[i] == ':'){
                    n--;
                }
                i++;
                if(i == buflen){
                    puts("Invalid sudo entry");
                    exit(1);
                }
            }
            
            char *list = &buf[i], *end;
            while(true){
                end = strchr(list, ',');
                if(end == NULL){
                    //last entry
                    if(EOL){
                        return strcmp(list, user);
                    } else {
                        int listlen = strlen(list);
                        strncpy(buf, list, listlen);
                        fgets(&buf[listlen], 100 - listlen, groups);
                        buflen = strlen(buf);
                        if(buf[buflen - 1] != 10){
                            EOL = false;
                        } else {
                            buf[buflen - 1] = 0;
                            EOL = true;
                        }
                        list = buf;
                    }
                } else {
                    //middle entry
                    if(end - list == userlen && !strncmp(list, user, userlen)){
                        return 0;
                    }
                    list = end + 1;
                }
            }
            
        }
    }
    
    puts("Failed to find a group named 'sudo'");
    return 1;
}

int main(){
    printf("%d\n", isSudo("it works!"));
    return 0;
}
