/**
 * Shell Lab
 * CS 241 - Spring 2019
 */
 
#include "format.h"
#include "shell.h"
#include "vector.h"
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "sstring.h"
#include <math.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>


typedef struct process {
    char *command;
    pid_t pid;
} process;

/*
	LOADS UP THE HISTORY VECTOR FROM FILE
*/
void load_history(char* path, vector* vec);
void save_history(char *path, vector *vec);
void run_command(char *buffer, vector* vec, vector* processes, vector *pid_commands, bool bg);
void run_history(char *buffer, vector *vec, vector *processes, vector *pid_commands);
void child_cleanup(int signal);
void ps_(vector *processes, vector* pid_commands);
void print_fd(pid_t pid);
void kill_pid(pid_t pid, char* command);
void stop_pid(pid_t pid, char* command);
void cont_pid(pid_t pid);
void kill_switch(vector *processes);
int get_index(pid_t pid, vector *vec);
char** tokenize(char* buffer, int *num);



static pid_t foreground = 0;

void catch_sigint(int signal) {
    if (foreground == 0)
        return;
	kill(foreground, signal);
}

int shell(int argc, char* argv[]) {
	signal(SIGINT, catch_sigint);
    signal(SIGCHLD, child_cleanup);
	if (argc-1 != 2 && argc-1 != 4 && argc-1 != 0) {
		print_usage();
	}
	char *history_path = NULL;
	char *command_path = NULL;
	size_t len = 100;
    ssize_t bytes;
	char *buffer = (char*)calloc(len,sizeof(char));

	// PARSE THE COMMAND LINE ARGS
	for(int i = 1; i < argc; i += 2) {
		if (strcmp(argv[i], "-h") == 0) {
            history_path = argv[i+1];
            if (argv[i+1][0] != '\\')
                history_path = get_full_path(history_path);
			continue;
		}
		if (strcmp(argv[i], "-f") == 0) {
			command_path = argv[i+1];
			continue;
		}
		print_usage();
		exit(1);
	}

    // VECTORS ****q*************************************************
	vector *commands = string_vector_create();
    vector *processes = int_vector_create();
    vector *pid_commands = string_vector_create();
    pid_t process = getpid();
    vector_push_back(processes, &process);
    vector_push_back(pid_commands, "./shell");

	if (history_path != NULL) {
		load_history(history_path, commands);
	}

	FILE *command_file = NULL;
	if (command_path != NULL) {
		command_file = fopen(command_path, "r");
        if (command_file == NULL) {
            print_script_file_error();
            exit(1);
        }
	}

	while(1) {
        bool bg = 0;
		if (command_file == NULL) {
		    print_prompt(get_current_dir_name(), getpid());
			bytes = getline(&buffer, &len, stdin);
		} else {
			bytes = getline(&buffer, &len, command_file);
		}

		if (bytes == -1)
            break;
        if (buffer[0] == '\n')
            continue;

        if(bytes >0 && buffer[bytes-1] == '\n')
            buffer[bytes-1] = 0;
        if (buffer[0] != '!' && buffer[0] != '#')
            vector_push_back(commands, buffer);
        if (buffer[strlen(buffer)-1] == '\n')
        	buffer[strlen(buffer)-1] = '\0';
        if(strcmp(buffer, "exit") == 0 )
            break;
       

        if (command_file != NULL) {
		    print_prompt(get_current_dir_name(), getpid());
			printf("%s\n",buffer);
        }

        if (buffer[0] == '#') {
        	size_t n = (size_t)atoi(buffer+1);
        	if (n < vector_size(commands)) {
        		puts(vector_get(commands, n));
        		free(buffer);
        		buffer = strdup((char*)vector_get(commands, n));
        		if(buffer[strlen(buffer) - 2] == '&' || buffer[strlen(buffer) - 1] == '&') {
        			bg = 1;
        		}
        		run_command(buffer, commands, processes, pid_commands,  bg);
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
            run_history(buffer, commands, processes, pid_commands);
            free(buffer);
        	buffer = calloc(100, sizeof(char));
        }

        else {
            if(buffer[strlen(buffer) - 1] == '&') {
        	    buffer[strlen(buffer)-1] = '\0';
        	    bg = 1;
            }
        	run_command(buffer, commands, processes, pid_commands, bg);
        	free(buffer);
        	buffer = calloc(100, sizeof(char));
        }

	}

    save_history(history_path, commands);
    vector_destroy(commands);
    kill_switch(processes);
    vector_destroy(processes);

	if (command_file) {
		fclose(command_file);
	}
    return 0;
}

void kill_switch(vector *processes) {
    for (size_t i = 0; i < vector_size(processes); i++) {
        pid_t pid = *((size_t*)vector_get(processes, i));
        kill(pid, SIGINT);
    }
}

int get_index(pid_t pid, vector *vec) {
    size_t i = 0;
    for (; i < vector_size(vec); i++) {
        if (pid == *((pid_t*)(vector_get(vec, i)))) {
            return (int) i;
        }
    }
    return -1;

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



void run_command(char *buffer, vector *vec, vector *processes, vector *pid_commands, bool bg) {
	sstring *sstr = cstr_to_sstring(buffer);
	char delim = '\0';
	for (int i = 0; (size_t)i < strlen(buffer) - 1; i++) {
		if(buffer[i]=='&'||buffer[i]=='|'||buffer[i]==';') {
			delim = buffer[i];
			break;
		}
	}

	vector *commands = sstring_split(sstr, delim);
	sstring_destroy(sstr);

	for (size_t i = 0; i < vector_size(commands); i++) {
		if (*(char*)vector_get(commands, i) == '\0')
			continue;

		char *command = strdup((char*)vector_get(commands, i));

		int argn;
		char** args = tokenize(command, &argn);
		bool builtin = 0;

		if (strcmp(args[0], "cd") == 0) {
			builtin = 1;
			int status = chdir(args[1]);
        	if (status != 0) {
            	print_no_directory(args[1]);
            }
		}

		if (strcmp(args[0], "ps") == 0) {
            ps_(processes, pid_commands);
			builtin = 1;
			continue;
		}

		if (strcmp(args[0], "pfd") == 0) {
            if (argn < 2) {
                print_invalid_command(command);
                continue;
            }
            pid_t pid = (pid_t)atoi(args[1]);
            print_fd(pid);
			builtin = 1;
			continue;
		}

		if (strcmp(args[0], "kill") == 0) {
            if (argn < 2) {
                print_invalid_command(command);
                continue;
            }
            pid_t pid = (pid_t)atoi(args[1]);
            int index = get_index(pid, processes);
            if (index == -1) {
                print_no_process_found(pid);
            }
            else {
                char* c = vector_get(pid_commands, (size_t)index);
                kill_pid(pid, c);
                vector_erase(pid_commands, index);
                vector_erase(processes, index);
                //free(c);
            }
			builtin = 1;
			continue;
		}

		if (strcmp(args[0], "stop") == 0) { 
            if (argn < 2) {
                print_invalid_command(command);
                continue;
            }
            pid_t pid = (pid_t)atoi(args[1]);
            int index = get_index(pid, processes);
            if (index == -1) {
                print_no_process_found(pid);
            }
            else {
                char* c = vector_get(pid_commands, (size_t)index);
                stop_pid(pid, c);
                //free(c);
            }
			builtin = 1;
			continue;
		}

		if (strcmp(args[0], "cont") == 0) { 
            if (argn < 2) {
                print_invalid_command(command);
                continue;
            }
            pid_t pid = (pid_t)atoi(args[1]);
            int index = get_index(pid, processes);
            if (index == -1) {
                print_no_process_found(pid);
            }
            else {
                //char* c = vector_get(pid_commands, (size_t)index);
                cont_pid(pid);
                //free(c);
            }
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
					waitpid(pid, &status, WUNTRACED);
					foreground = 0;

					if (WEXITSTATUS(status) == 1) {
						print_exec_failed(command);
					}

					if ((WEXITSTATUS(status) == 1 && delim == '&') 
						|| (WEXITSTATUS(status) != 1 && delim == '|')) {
						print_command_executed(pid);
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
                    vector_push_back(pid_commands, command);
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

void ps_(vector* processes, vector* pid_commands) {
    puts("1");
    print_process_info_header();
    size_t i = 0;
    for (; i < vector_size(processes); i++) {
        //printf("%lu\n", i);
        if (kill(*(pid_t*)vector_get(processes,i), 0) != 0)
            continue;
        size_t buflen = 1000;
        char *buffer = (char*)calloc(buflen, sizeof(char));
        sprintf(buffer, "/proc/%d/stat",*((pid_t*)vector_get(processes, i)));
        FILE *file = fopen(buffer, "r");
        getline(&buffer, &buflen, file);
        sstring *bufstr = cstr_to_sstring(buffer);
        vector *args = sstring_split(bufstr, ' ');

        process_info *process_ = (process_info*)malloc(sizeof(process_info));
         
        long long unsigned utime, stime;
        sscanf(vector_get(args, 0), "%d", &process_->pid);
        sscanf(vector_get(args, 2), "%c", &process_->state);
        sscanf(vector_get(args, 19), "%ld", &process_->nthreads);
        sscanf(vector_get(args, 22), "%lu", &process_->vsize);
        process_->vsize /= 1024;
        sscanf(vector_get(args, 13), "%llu", &utime);
        sscanf(vector_get(args, 14), "%llu", &stime);
        process_->command = vector_get(pid_commands, i);

        long long unsigned seconds = (utime+stime) / sysconf(_SC_CLK_TCK);
        time_t start_time = time(0) - seconds;

        size_t sec = seconds%60;
        size_t min = seconds/60;
        char* time_string = (char*) malloc((7)*sizeof(char));
        execution_time_to_string(time_string, 7, min, sec);
        struct tm *start = localtime(&start_time);
        char * start_time_string = (char*)calloc(15, sizeof(char));
        time_struct_to_string(start_time_string, 15, start);

        process_->start_str = start_time_string;

        process_->time_str  = time_string;

        print_process_info(process_);

        free(process_);
        vector_destroy(args); 
        sstring_destroy(bufstr);
        free(buffer);
        fclose(file);

    }
}

void print_fd(pid_t pid) {
    if(kill(pid, 0) != 0) {
        print_no_process_found(pid);
        return;
    }
    print_process_fd_info_header();
    char *dir = (char*)calloc(270, sizeof(char));
    struct dirent *fdinfo;
    sprintf(dir, "/proc/%d/fdinfo", pid);
    DIR *proc_dir = opendir(dir);
    if (proc_dir) {
        while((fdinfo = readdir(proc_dir)) != NULL) {
            if (!isdigit(fdinfo->d_name[0]))
                continue;
            char fd[PATH_MAX];
            sprintf(fd, "/proc/%d/fdinfo/%s", pid, fdinfo->d_name);
            FILE *fd_file = fopen(fd, "r");
            if (fd_file == NULL)
                continue;
            char yeet[100];
            while (fgets(yeet, 100, fd_file)) {
                if (strncmp("pos:",yeet,4) == 0) {
                    char real_path[PATH_MAX];
                    sprintf(fd, "/proc/%d/fd/%s", pid, fdinfo->d_name);
                    readlink(fd, real_path, PATH_MAX);
                    size_t pos = (size_t)atoi(yeet+4);
                    size_t fd_no = (size_t)atoi(fdinfo->d_name);
                    print_process_fd_info(fd_no,pos, real_path);

                }
            }
        }
        closedir(proc_dir);
    }

    return;


}

void kill_pid(pid_t pid, char* command) {
    int status = kill(pid, SIGTERM);
    if (status == 0)
        print_killed_process(pid, command);
    else
        print_no_process_found(pid);
    return;
}

void stop_pid(pid_t pid, char* command) {
    int status = kill(pid, SIGTSTP);
    if (status == 0)
        print_stopped_process(pid, command);
    else
        print_no_process_found(pid);
}

void cont_pid(pid_t pid) {
    int status = kill(pid, SIGCONT);
    if (status == 0)
        ;
    else
        print_no_process_found(pid);
}

void child_cleanup(int signal) {
    int status;
    while(waitpid(-1, &status, WNOHANG) > 0) {
    }
	return;
}

void run_history(char *buffer, vector *vec, vector *processes, vector *pid_commands) {
	if (strcmp(buffer+1, "history") == 0) {
		for (size_t i =0; i < vector_size(vec); i++) {
			print_history_line(i, (char*)vector_get(vec, i));
		}
        return;
	}
	int index = vector_size(vec) - 1;
    char* str = strdup(buffer+1);
    while(index >= 0) {
        if (strncmp((str), (char*)vector_get(vec,(size_t) index), strlen(str)) == 0) {
        	char* command = strdup((char*)vector_get(vec, (size_t) index));
            bool bg = 0;
            if(command[strlen(command) - 1] == '&')
                bg =1;
            run_command(command, vec, processes, pid_commands, bg);
            free(command);
            return;
        }
        index--;
    }
    print_no_history_match();
}




void save_history(char *path, vector *vec) {
    FILE *h = fopen(path, "w");
    if (h == NULL)
        return;
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
