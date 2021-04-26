/**
 * Shell Lab
 * CS 241 - Spring 2019
 */
 
#include "format.h"
#include "shell.h"
#include "vector.h"
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "sstring.h"
#include <sys/types.h>

typedef struct process {
    char *command;
    pid_t pid;
} process;

/*
	LOADS UP THE HISTORY VECTOR FROM FILE
*/
void load_history(char* path, vector* vec);
void save_history(char *path, vector *vec);
void run_command(char *buffer, vector* vec, vector* processes, bool bg);
void run_history(char *buffer, vector *vec);
void child_cleanuq(vector *vec);
char** tokenize(char* buffer, int *num);



pid_t foreground;

void catch_sigint(int signal) {
	kill(foreground, signal);
}

int shell(int argc, char* argv[]) {
	signal(SIGINT, catch_sigint);
	if (arc-1 != 2 || argc-1 != 4 || argc != 1) {
		print_usage();
	}
	char *history_path = NULL;
	char *command_path = NULL;
	size_t len = 100;
	char *buffer = (char*)calloc(len,sizeof(char));

	// PARSE THE COMMAND LINE ARGS
	for(int i = 1; i < argc; i += 2) {
		if (strcmp(argv[i], "-h") == 0) {
			history_path = argv[i+1];
			continue;
		}
		if (strcmp(argv[i], "-f") == 0) {
			command_path = argv[i+1];
			continue;
		}
		print_usage();
		exit(1);
	}

	vector *commands = string_vector_create();
	vector *processes = int_vector_create();


	if (history_path != NULL) {
		load_history(history_path, commands);
	}

	FILE *command_file = NULL;
	if (command_path != NULL) {
		command_file = fopen(command_path, "r");
	}

	while(1) {
		print_prompt(get_current_dir_name(), getpid());
		if (command_file == NULL) {
			bytes = getline(&buffer, &len, stdin);
		} else {
			bytes = getline(&buffer, &len, command_file);
			puts(buffer);
		}

		if (bytes == -1)
            break;
        if (buffer[0] == '\n')
            continue;
        if(bytes >0 && buffer[bytes-1] == '\n')
            buffer[bytes-1] = 0;
        if(strcmp(buffer, "exit") == 0 )
            break;
        if(buffer[strlen(buffer) - 2] == '&' || buffer[strlen(buffer) - 1] == '&') {
        	bg = 1;
        }

        if (buffer[0] == '#') {
        	size_t n = (size_t)atoi(buffer+1);
        	if (n < vector_size(commands)) {
        		print_history_n(n, commands);
        		free(buffer);
        		buffer = strdup((char*)vector_get(commands, i));
        		if(buffer[strlen(buffer) - 2] == '&' || buffer[strlen(buffer) - 1] == '&') {
        			bg = 1;
        		}
        		run_command(buffer, commands, bg);
        		free(buffer);
        		buffer = calloc(100, sizeof(char));
        		continue;
        	}
        	else {
        		print_invalid_index();
        		continue;
        	}
        }

        else if (buffer[0] == '!') {
            run_history(buffer, commands);
            free(buffer);
        	buffer = calloc(100, sizeof(char));
        }

        else {
        	run_command(buffer, commands, processes);
        	free(buffer);
        	buffer = calloc(100, sizeof(char));
        }

	}


	if (command_file) {
		fclose(command_file);
	}
}




char** tokenize(char* buffer, int *num) {
    char* str = strdup(buffer);
    size_t i = 0;
    int num_arg = 1;
    for (; i < strlen(buffer); i++) {
        if (buffer[i] == ' ')
            num_arg++;
    }
    i = 0;
    char** tokens = (char**)calloc(num_arg, sizeof(char*));
    char *token = strtok(str, " ");
    while(token) {
        tokens[i++] = strdup(token);
        token = strtok(0, " ");
    }
    tokens[i] = 0;

    free(str);
    *num = (int)i;
    return tokens;
}



void run_command(char *buffer, vector *vec, vector *processes, bool bg) {
	sstring *sstr = cstr_to_sstring(buffer);
	char delim = '\0';
	for (int i = 0; i < strlen(buffer) - 1; i++) {
		if(buffer[i]=='&'||buffer[i]=='|'||buffer[i]==';') {
			delim = buffer[i];
			break
		}
	}

	vector *commands = sstring_split(sstr, delim);
	sstring_destroy(sstr);

	for (size_t i = 0; i < vector_size(commands); i++) {
		if (vector_get(commands, i) == NULL)
			continue;


		char *command = strdup((char*)vector_get(commands));
		puts(command);
		vector_push_back(vec, command);
		int argn;
		char** args = tokenize(command, &argn);
		bool builtin = 0;

		if (strcmp(args[0], "cd") == 0) {
			builtin = 1;
			int status = chdir(arg[1]);
        	if (status != 0) {
            	print_no_directory(arg[1]);
            }
		}

		if (strcmp(args[0], "ps") == 0) {
			builtin = 1;
			continue;
		}

		if (strcmp(args[0], "pfd") == 0) {
			builtin = 1;
			continue;
		}

		if (strcmp(args[0], "kill") == 0) {
			builtin = 1;
			continue;
		}

		if (strcmp(args[0], "stop") == 0) {
			builtin = 1;
			continue;
		}

		if (strcmp(args[0], "cont") == 0) {
			builtin = 1;
			continue;
		}


		if (!builtin) {
			fflush(stdin);
			pid_t pid = fork();

			// FORK FAILED
			if (pid < 0) {
				print_fork_failed();
				continue;
			}

			// CHILD PROCESS
			if (pid == 0) {
				print_command_executed(getpid());
				execvp(args[0], args);
				exit(1);
			}

			// PARENT PROCESS
			if (pid > 0) {
				int status;

				if (!bg) {
					foreground = pid;
					waitpid(pid, &status, 0);
					foreground = 0;

					if (WEXITSTATUS(status) == 1) {
						print_exec_failed(command);
					}

					if ((WEXITSTATUS(status) == 1 && delim == '&') 
						|| (WEXITSTATUS(status) != 1 && delim == '|')) {
						break;
					}

					else {
						continue;
					}
				}

				else {
					if (setpgid(pid, pid) == -1) {
						print_setpgid_failed();
						exit(1);
					}
					vector_push_back(processes, &pid);
					break;
				}
			}
		}

		free(command);
		while(args[i] != NULL) {
			free(args[i++]);
		}
		free(args);
	}

	vector_destroy(commands);
	return;
}


void child_cleanup(vector *vec) {
	return;
}

void run_history(char *buffer, vector *vec) {
	int index = vector_size(vec) - 1;
    char* str = strdup(buffer+1);
    while(index >= 0) {
        if (strncmp((str), (char*)vector_get(vec,(size_t) index), strlen(str)) == 0) {
        	char* command = strdup((char*)vector_get(vec, (size_t) index));
            run_command(command, vec);
            free(command);
            return;
        }
        index--;
    }
    print_no_history_match();
}




void save_history(char *path, vector *vec) {
    FILE *h = fopen(path, "w");
    size_t i = 0;
    for (; i < vector_size(vec); i++) {
        fprintf(h, "%s\n", ((char*)vector_get(vec, i)));
    }
    fclose(h);
}



void load_history(char* path, vector* vec) {
	FILE *f = fopen(path, "r");
	if (f == NULL) {
		print_history_file_error();
		return;
	}
    size_t len = 100;
    ssize_t bytes;
    char* cmd = (char*)calloc(len, sizeof(char));
    while(1) {
        bytes = getline(&cmd, &len, f);
        if (bytes == -1)
            break;
        if(bytes >0 && cmd[bytes-1] == '\n')
            cmd[bytes-1] = 0;
        vector_push_back(vec, cmd);
    }
    free(cmd);
    fclose(f);
}