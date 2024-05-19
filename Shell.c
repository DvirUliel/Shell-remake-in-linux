#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 100

int main() {
    int status = 0, pid, i;
    char cmd[MAX]; //collect the command from the terminal
    char* cmdArray[MAX]; //array of all commands
    char* splitCmd = NULL; //run with strtok
    char* path = getenv("PATH"); // get the path for the commands
    char* tempDir = NULL; //run with strtok
    char filePath[MAX] = "";

    while(1) {
        printf("$ ");
        fgets(cmd, MAX, stdin); //fgets return the input by the user + "\n"
        cmd[strlen(cmd) - 1] = '\0'; //replace the "\n"

        //cmd="leave" - stop the program
        if (strcmp(cmd, "leave") == 0) {
            break;
        }

        pid = fork(); //create a child process
        if (pid == -1) {
            perror("fork"); //prints a message of the error when occurred
            exit(EXIT_FAILURE); //value of a non zero integer
        } 
        else if (pid == 0) { // Child process
            splitCmd = strtok(cmd, " \t"); //split all the commands
            i = 0;
            
            // get array with all the commands
            while (splitCmd != NULL) {
                cmdArray[i++] = splitCmd;
                splitCmd = strtok(NULL, " \t");
            }
            cmdArray[i] = NULL; //the last char is null in the array of commands

            // Loop through directories to find command
            tempDir = strtok(path, ":"); //split the directories
            while (tempDir != NULL) {
                strcpy(filePath, tempDir);
                strcat(filePath, "/");
                strcat(filePath, cmdArray[0]); //get filePath + the command
                execv(filePath, cmdArray);
                tempDir = strtok(NULL, ":");
            }
            // If execv fails, print error message and exit child process
            exit(EXIT_FAILURE); //value of a non zero integer
        } 
        else { // Parent process
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
                printf("Child process exited with failure status\n");
            }
        }
    }
    return 0;
}

