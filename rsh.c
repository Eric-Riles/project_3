#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for(int i = 0; i < N; i++){
		if(strcmp(cmd, allowed[i])==0){
			return 1;
		}
	}
	return 0;
}

int main() {
    // TODO
    // Add variables as needed

    char line[256];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	//seperates comand and args
	char *tok = strtok(line, " ");
	char cmd[8];
	strcpy(cmd, tok);
	if(isAllowed(cmd)){
		pid_t pid;
		posix_spawnattr_t attr;
		posix_spawnattr_init(&attr);

		char *argv[20] = {NULL};
		//populate argv
		int argSize = 0;
		for(; tok != NULL; argSize++){
			argv[argSize] = strdup(tok);
			tok = strtok(NULL, " ");
		}
		int status;

		if(strcmp(cmd, "exit") == 0){
			return EXIT_SUCCESS;
		}
		if(strcmp(cmd, "cd") == 0 || strcmp(cmd, "help") == 0){
			if(strcmp(cmd, "cd") == 0){
				if(argSize > 2){
					printf("-rsh: cd: too many arguments\n");
				}else{
					chdir(argv[1]);
				}
			}else{
				printf("The allowed commands are:\n");
				printf("1: cp\n2: touch\n3: mkdir\n4: ls\n5: pwd\n6: cat\n7: grep\n8: chmod\n9: diff\n10: cd\n11: exit\n12: help\n");
			}
		}else{
			if(posix_spawnp(&pid, cmd, NULL, &attr, argv, environ) != 0){
				perror("spawn failed");
        		exit(EXIT_FAILURE);
			}

			if (waitpid(pid, &status, 0) == -1) {
				perror("waitpid failed");
				exit(EXIT_FAILURE);
    		}

			posix_spawnattr_destroy(&attr);
		}
		//frees all of argv
		for(argSize--; argSize >= 0; argSize--){
			free(argv[argSize]);
		}
	}else{printf("NOT ALLOWED!\n");}
	
	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c
    }
    return 0;
}
