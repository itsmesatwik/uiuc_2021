/**
 * Nonstop Networking
 * CS 241 - Spring 2019
 */
#include "common.h"
#include "format.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>


char **parse_args(int argc, char **argv);
verb check_args(char **args);
void handle_response(int sock_fd, verb type, char **args);
void run_request(char **argv, verb request);
void send_request(int sock_fd, verb type, char **args);
int get_response_status(int sock_fd);
void get_response_body(int sock_fd, verb type, char **args);

ssize_t write_socket(int sock_fd, char* buffer, size_t len);
ssize_t read_socket(int sock_fd, char* buffer);

ssize_t read_all_from_socket(int socket, char *buffer, size_t count) {
    // Your Code Here
    ssize_t bytes_left_to_read = count;
    ssize_t bytes_already_read = 0;
    ssize_t read_ret = -1;
    while(bytes_left_to_read > 0 || (read_ret == -1 && errno == EINTR)){
        read_ret = read(socket,(void *) buffer + bytes_already_read, bytes_left_to_read);
        if(read_ret == 0){
            return 0;
        }
        else if(read_ret > 0) {
            bytes_already_read = bytes_already_read + read_ret;
            bytes_left_to_read = bytes_left_to_read - read_ret;
        }

    }
    return count;
}

int main(int argc, char **argv) {
    char **parsed = parse_args(argc, argv);
    verb request_type = check_args(parsed);
    //parsed[0] --> Host
    //parsed[1] --> Port
    //parsed[2] --> Request Type
    //parsed[3] --> Remote
    //parsed[4] --> Local
    run_request(parsed, request_type);

    free(parsed);

    return 0;
    // Good luck!
}


ssize_t write_socket(int sock_fd, char* buffer, size_t remaining) {
    ssize_t write_size  = 0;
    ssize_t total_size = 0;


    while (1) {
        write_size = write(sock_fd, (void*)buffer, remaining);
        if (write_size == 0) {
            break;
        } 
        if (write_size == -1 && errno == EINTR) {
            continue;
        } 
        if (write_size == -1 && errno != EINTR) {
            total_size = -1;
            break;
        }
        total_size += write_size;
        remaining -= write_size;

        if (remaining <= 0) {
            break;
        } 
        buffer = buffer + write_size;
    }
    return total_size;
}

void handle_response(int sock_fd, verb type, char **args) {
    int status = get_response_status(sock_fd);
    if (status == 0) {
        get_response_body(sock_fd, type, args);
    }
    else {

    }
}

void get_response_body(int sock_fd, verb type, char **args) {
    if (type == DELETE || type == PUT) {
        print_success();
        return;
    }

    size_t size;
    ssize_t read_bytes = read_all_from_socket(sock_fd, (char*)&size, sizeof(size_t));
    if (read_bytes == 0 || read_bytes == -1) {
        print_invalid_response();
        exit(1);
    }
    size_t read_size = 0;
    size_t remaining = size;
    size_t current_read = 0;

    ssize_t read_char = 0;
    
    // Read 4KB from socket everytime
    char buffer[4097];

    if (type == LIST) {
        while (read_size < size) {
            memset(buffer, 0, 4097);
            current_read = remaining > 4096 ? 4096 : remaining;
            read_char = read_all_from_socket(sock_fd, buffer, current_read);
            if (read_char == -1) {
                print_invalid_response();
                exit(1);
            }
                
            if (read_char == 0) {
                print_connection_closed();
                if (read_size < size) {
                    print_too_little_data();
                }
                if (read_size > size) {
                    print_received_too_much_data();
                }
                exit(1);
            }            
            remaining  -= read_char;
            read_size += read_char;
            printf("%s", buffer);
        }
        printf("\n");

        read_char = read_all_from_socket(sock_fd, buffer, 4096);
        if (read_char == 0) {
        } else {
            print_received_too_much_data();
        }

    } else if (type == GET) {
        FILE *localfile = fopen(args[4], "w");
        if (localfile == NULL) {
            perror(err_no_such_file);
            exit(1);
        }
        while (read_size < size) {
            memset(buffer, 0, 4097);
            current_read = remaining > 4096 ? 4096 : remaining;
            read_char = read_all_from_socket(sock_fd, buffer, current_read);
            if (read_char == -1) {
                print_invalid_response();
                exit(1);
            }
                
            if (read_char == 0) {
                print_connection_closed();
                if (read_size < size) {
                    print_too_little_data();
                }
                if (read_size > size) {
                    print_received_too_much_data();
                }
                exit(1);
            } 
            
            remaining  -= read_char;
            read_size += read_char;
            fwrite(buffer, 1, read_char, localfile);
        }

        // All required data should've been read already, check if the connection is closed
        read_char = read_all_from_socket(sock_fd, buffer, 4096);
        if (read_char == 0) {
            // Do nothing
        } else {
            print_received_too_much_data();
        }

        fclose(localfile);
    }
}

int get_response_status(int sock_fd) {
    char buffer[1024];
    memset(buffer, 0, 1024);
    read_socket(sock_fd, buffer);
    if (strcmp(buffer, "OK") == 0) {
        return 0;
    }
    else if (strcmp(buffer, "ERROR") == 0) {
        return 1;
    }
    else {
        print_invalid_response();
        exit(1);
    }
}

ssize_t read_socket(int sock_fd, char *buffer) {
    size_t count = 0;
    ssize_t read_count = 0;

    while (1) {
        read_count = read(sock_fd, (void*)buffer, 1);
        if (read_count == 0) {
            print_invalid_response();
            exit(1);
        } 
        else if (read_count == -1 && errno == EINTR) {
            continue;
        } 
        else if (read_count == -1 && errno != EINTR) {
            print_invalid_response();
            exit(1);
        }

        if (buffer[0] == '\n') {
            buffer[0] = '\0';
            break;
        }
        
        // increment the counter, and move the buffer point along
        count++;
        buffer++;

        // reach the max counter
        if (count >= 1024) {
            print_invalid_response();
            exit(1);
        }
    }

    return 0;
}

void run_request(char **parsed, verb request) {
    // CONNECT TO THE SERVER
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(parsed[0], parsed[1], &hints, &result);

    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }

    int sock_fd = socket(result->ai_family, result->ai_socktype, 0);
    status = connect(sock_fd, result->ai_addr, result->ai_addrlen);
    if (status == -1) {
        perror("connect");
        exit(1);
    }

    freeaddrinfo(result);

    send_request(sock_fd, request, parsed);

    if (shutdown(sock_fd, SHUT_WR) != 0) {
        perror("shutdown");
    }

    handle_response(sock_fd, request, parsed);

    close(sock_fd);
    
}

void send_request(int sock_fd, verb type, char **args) {
    char *header = NULL;
    if (type == LIST) {
        header = calloc(strlen(args[2])+ 2, 1);
        sprintf(header, "%s\n", args[2]);
    }
    else {
        header = calloc(strlen(args[2]) + 1 + strlen(args[3]) + 2, 1);
        sprintf(header, "%s %s\n", args[2], args[3]);
    }

    size_t len = strlen(header);
    ssize_t total_size = write_socket(sock_fd, header, len);

    free(header);

    if (total_size <= 0) {
        fprintf(stderr, "ERROR: write header failed\n");
        exit(1);
    }


    if (type == PUT) {
        FILE *localfile = fopen(args[4], "r");
        if (localfile == NULL) {
            perror(err_no_such_file);
            exit(1);
        }

        fseek(localfile, 0, SEEK_END);
        size_t filesize = ftell(localfile);
        fseek(localfile, 0, SEEK_SET);

        total_size = write_socket(sock_fd, (char*)&filesize, sizeof(size_t));

        if (total_size == 0 || total_size == -1) {
            print_invalid_response();
            exit(1);
        }

        size_t write_size  = 0;
        size_t writing_size = 0;
        size_t remaining = filesize;
        char buffer[4097];
        memset(buffer, 0, 4097);

        while (write_size < filesize) {
            if (remaining > 4096)
                writing_size = 4096;
            else
                writing_size = remaining;

            fread(buffer, 1, writing_size, localfile);
            total_size = write_socket(sock_fd, buffer, writing_size);

            if (total_size == -1) {
                    print_invalid_response();
                    exit(1);
                }
                
            if (total_size == 0) {
                print_connection_closed();
                if (write_size < writing_size) {
                    print_too_little_data();
                }
                exit(1);
            }

            remaining -= total_size;
            write_size += total_size;
        }

        fclose(localfile);

    }

}

/**
 * Given commandline argc and argv, parses argv.
 *
 * argc argc from main()
 * argv argv from main()
 *
 * Returns char* array in form of {host, port, method, remote, local, NULL}
 * where `method` is ALL CAPS
 */
char **parse_args(int argc, char **argv) {
    if (argc < 3) {
        return NULL;
    }

    char *host = strtok(argv[1], ":");
    char *port = strtok(NULL, ":");
    if (port == NULL) {
        return NULL;
    }

    char **args = calloc(1, 6 * sizeof(char *));
    args[0] = host;
    args[1] = port;
    args[2] = argv[2];
    char *temp = args[2];
    while (*temp) {
        *temp = toupper((unsigned char)*temp);
        temp++;
    }
    if (argc > 3) {
        args[3] = argv[3];
    }
    if (argc > 4) {
        args[4] = argv[4];
    }

    return args;
}

/**
 * Validates args to program.  If `args` are not valid, help information for the
 * program is printed.
 *
 * args     arguments to parse
 *
 * Returns a verb which corresponds to the request method
 */
verb check_args(char **args) {
    if (args == NULL) {
        print_client_usage();
        exit(1);
    }

    char *command = args[2];

    if (strcmp(command, "LIST") == 0) {
        return LIST;
    }

    if (strcmp(command, "GET") == 0) {
        if (args[3] != NULL && args[4] != NULL) {
            return GET;
        }
        print_client_help();
        exit(1);
    }

    if (strcmp(command, "DELETE") == 0) {
        if (args[3] != NULL) {
            return DELETE;
        }
        print_client_help();
        exit(1);
    }

    if (strcmp(command, "PUT") == 0) {
        if (args[3] == NULL || args[4] == NULL) {
            print_client_help();
            exit(1);
        }
        return PUT;
    }

    // Not a valid Method
    print_client_help();
    exit(1);
}
