#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>


void chgDir(char* path, char* currDir){
    if(chdir(path) == -1){
        printf("Error: cannot change directory\n");
        return;
        }
    if( (getcwd(currDir,1000)) == NULL) {
        perror("getcwd() error");
    }  
    setenv("pwd", currDir, 1);
}

void showpid(int idList[]){
    int i = 0;
    for (i = 0; i<5; i++){
        printf("%d\n", idList[i]);
    }
    return;
}

int main(){
    char* tokenArr;
    char* argumentList[10];
    int i;
    char* str = malloc(1000 * sizeof(char));
    char* command;
    pid_t pid;
    int execStatus;
    int idList[5] = {0};
    int a = 0;
    char currDir[1000];
    int status;
    for (i = 0; i <10; i++){
        argumentList[i] = (char*) malloc(sizeof(char) * 81);
        argumentList[i] = NULL;
    }
    if ( (getcwd(currDir,1000)) == NULL){
    }

    while(1){
	memset(str, 0, 1000);
        printf("\033[0;32m%s$ ", currDir); 
        printf("\033[0m"); 



        if (fgets(str, 1000, stdin) !=NULL){
            str[strcspn(str, "\n")] = 0; // Remove the newline character from the input

    // Check for the "exit" command first
    if (strncmp(str, "exit", 4) == 0){
        printf("Exiting shell\n");
        break; // Exit the loop and terminate the shell
    }
        }

        tokenArr = strtok(str, " ");
        i = 0;
        while(tokenArr != NULL){
            if (i ==0){
                command = strdup(tokenArr);
            }
            argumentList[i] = strdup(tokenArr);
            tokenArr = strtok(NULL, " ");
            i++;
        }

        if (strcmp(command, "cd") == 0){
            chgDir(argumentList[1], currDir);
        }
        else if(strcmp(command, "showpid") == 0){
            showpid(idList); 
        }

        else{
            if ((pid = fork()) == 0){
                execStatus = execvp(command, argumentList);

                if (execStatus == -1){
                    printf("Error: Command could not be executed\n");
                    exit(1);
                }
                kill(pid, SIGTERM);
            }
            else{
                //parent process (shell program itself)
                waitpid(pid, &status, 0);
                if (a == 5){
                    a = 0;
                }
                idList[a] = pid;
                a++;
            
            }
        }

        for (i = 0; argumentList[i] != NULL; i++) {
            free(argumentList[i]);
            argumentList[i] = NULL;
        }

    }
    free(str);
    return 0;
}
