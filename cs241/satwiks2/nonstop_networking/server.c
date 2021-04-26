/**
 * Nonstop Networking
 * CS 241 - Spring 2019
 */
#include "format.h"
#include "vector.h"
#include "common.h"
#include "dictionary.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_CLIENTS 20
#define MAX_EVENTS 21

#define FINISHED 1
#define ERROR 2
#define IN_PROGRESS 3
#define PROCESS 4
#define READ_HEAD 5
#define READ_HEAD_SECOND 6

typedef struct _clients_info {
    int sock_fd;
    int file_fd;
    int state;
    verb request;
    char *buffer;
    char *filename;
    ssize_t size;
    int offset;

    char *error;
} Clients_info;


void cleanup(int listen_sock);
void close_server();
void do_use_fd(int sock_fd);

ssize_t socket_read(int socket, char *buffer, size_t count);
ssize_t socket_write(int socket, char *buffer, size_t count);
ssize_t file_size(char *filename);

int write_error(Clients_info *info);
int write_response(Clients_info *info);
int process_request(Clients_info *info);

int process_put(Clients_info *info);
int process_get(Clients_info *info);
int process_list(Clients_info *info);
int process_delete(Clients_info *info);
int destroy_directory(char *path);

static int listen_sock = 0;
static int close_connection = 0;
static vector *files = NULL;
static dictionary *socket_dict = NULL;
static char template[] = "./XXXXXX";

void command_create(Clients_info *info, int sock_fd) {
    info->sock_fd = sock_fd;
    info->state = READ_HEAD;
    info->size = 0;
    info->offset = 0;
    info->error = NULL;
    info->buffer = NULL;
    info->filename = NULL;
}

void sigpipe() {
    return;
}

void cleanup(int listen_sock) {
    if (close(listen_sock) == -1) {
        if (errno == EBADF) {
            printf("invalid sock");
        }
    }

    if (shutdown(listen_sock, SHUT_RDWR) != 0) {
        perror("shutdown");
    }

    if (destroy_directory(".") == -1) {
        perror("directory");
        exit(1);
    }
}

int write_response(Clients_info *info) {
    char response[] = "OK\n";
    if (info->request == DELETE || info->request == PUT) {
        ssize_t bytes_written = socket_write(info->sock_fd, response, strlen(response));
        if (bytes_written == -1) {
            return -1;
        }
    } 
    if (info->request == GET) {
        ssize_t bytes_written = socket_write(info->sock_fd, response, strlen(response));
        if (bytes_written == -1) {
            return -1;
        }

        bytes_written = socket_write(info->sock_fd, (void*) &info->size, sizeof(ssize_t));
        if (bytes_written == -1) {
            return -1;
        }
    }

    if (info->state == FINISHED) {
        close(info->sock_fd);
    }

    return 0;
}

int write_error(Clients_info *info) {
    char response[1024];
    sprintf(response, "ERROR\n%s\n", info->error);
    ssize_t bytes_written = socket_write(info->sock_fd, response, strlen(response));
    if (bytes_written == -1) {
        return -1;
    }
    close(info->sock_fd);
    return 0;
}

void close_server() {
    close_connection = 1;
}

void do_use_fd(int sock_fd) {
    Clients_info *cmd = dictionary_get(socket_dict, &sock_fd);

    int status;
    if (cmd->state == READ_HEAD || cmd->state == READ_HEAD_SECOND) {
        status = process_request(cmd);
        if (status == -1) {
            cleanup(listen_sock);
            exit(1);
        }
    }
    if (cmd->request == PUT) {
        status = process_put(cmd);
        if (status == -1) {
            cleanup(listen_sock);
            exit(1);
        }
    } 
    else if (cmd->request == GET) {
        status = process_get(cmd);
        if (status == -1) {
            cleanup(listen_sock);
            exit(1);
        }
    } 
    else if (cmd->request == LIST) {
        status = process_list(cmd);
        if (status == -1) {
            cleanup(listen_sock);
            exit(1);
        }
    } 
    else if (cmd->request == DELETE) {
        status = process_delete(cmd);
        if (status == -1) {
            cleanup(listen_sock);
            exit(1);
        }
    }

    if (cmd->state == FINISHED && cmd->request != LIST && cmd->request != GET) {
        status = write_response(cmd);
        if (status == -1) {
            cleanup(listen_sock);
            exit(1);
        }
    } else if (cmd->state == ERROR) {
        status = write_error(cmd);
        if (status == -1) {
            cleanup(listen_sock);
            exit(1);
        }
    }
}


/* Code for destroying temp directory taken from stackoverflow 
*  https://stackoverflow.com/questions/2256945/removing-a-non-empty-directory-programmatically-in-c-or-c
*/
int destroy_directory(char *path) {
    DIR *dir = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (dir) {
        struct dirent *p;
        r = 0;

        while (!r && (p = readdir(dir))) {
            int r2 = -1;
            char *buffer;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
               continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buffer = malloc(len);

            if (buffer) {
               struct stat statbuf;
               snprintf(buffer, len, "%s/%s", path, p->d_name);
               if (!stat(buffer, &statbuf)) {
                   r2 = unlink(buffer);
               }

               free(buffer);
            }

            r = r2;
        }

        closedir(dir);
    }

    if (chdir("..") == -1) {
        perror("chdir");
        exit(1);
    }

    if (!r) {
        r = rmdir(template);
    }

    return r;
}


int parse_header(int socket, char* buffer) {
    int total_read = 0;
    // Read from socket character by character and stop at newline
    while (1) {
        // Reading 1 char at a time
        ssize_t bytes_read = socket_read(socket, buffer + total_read, 1);
        //Nothing to read
        if (bytes_read == 0) {
            break;
        }
        if (bytes_read == -1) {
            return bytes_read;
        }
        total_read += bytes_read;
        if (buffer[total_read - 1] == '\n') {
            break;
        }
    }

    buffer[total_read - 1] = '\0';
    return 0;
}

int process_request(Clients_info *info) {
    int sock_fd = info->sock_fd;
    if (info->state == READ_HEAD) {
        char *buffer = calloc(1024, sizeof(char));
        int status = parse_header(sock_fd, buffer);
        if (status == -1) {
            info->state = ERROR;
            info->error = (char*) err_bad_request;
            return -1;
        } 
        //puts(buffer);
        //strtok(buffer, " \n");
        char *type = NULL;
        type = strtok(buffer, " ");
        char *filename = NULL;
        filename = strtok(NULL, " ");
        //puts(filename);

        
        if (strncmp(type, "LIST", 4) == 0) {
            info->request = LIST;
        }
        else if (strncmp(type, "GET", 3) == 0) {
            info->request = GET;
        } 
        else if (strncmp(type, "PUT", 3) == 0) {
            info->request = PUT;
        } 
        else if (strncmp(type, "DELETE", 6) == 0) {
            info->request = DELETE;
        } 
        else {
            info->state = ERROR;
            info->error = (char*) err_bad_request;
            free(buffer);
            return 0;
        }
        if (filename != NULL && info->request == LIST) {
            info->state = ERROR;
            info->error = (char*) err_bad_request;
            free(buffer);
            return 0;
        }
        if (filename != NULL && (info->request == GET || info->request == PUT || info->request == DELETE)) {
            info->filename = strdup(filename);
        } 
        
        info->state = READ_HEAD_SECOND;
        free(buffer);
    }

    if (info->state == READ_HEAD_SECOND && info->request == PUT) {
        ssize_t size;
        ssize_t bytes_read = socket_read(sock_fd, (char*) &size, sizeof(size));
        if (bytes_read > 0) {
            info->size = size;
        } 
        if (bytes_read == -1) {
            info->state = ERROR;
            info->error = (char*) err_bad_request;
            return -1;
        }
    }

    info->state = PROCESS;
    return 0;
}

ssize_t socket_read(int socket, char *buffer, size_t count) {
    size_t total_read = 0;

    while (total_read < count) {
        ssize_t bytes_read = read(socket, buffer + total_read, count - total_read);
        if (bytes_read > 0) {
            total_read += bytes_read;
        } 
        if (bytes_read == -1) {
            if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                continue;
            }
            return -1;
        } 
        if (bytes_read== 0) {
            break;
        }
    }

    return total_read;
}

ssize_t socket_write(int socket, char *buffer, size_t count) {
    size_t total_written = 0;

    while (total_written < count) {
        ssize_t bytes_written = write(socket, buffer + total_written, count - total_written);
        if (bytes_written > 0) {
            total_written += bytes_written;
            continue;
        } 
        if (bytes_written == -1) {
            if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
                continue;
            }
            return -1;

        }
        if (bytes_written == 0) {
            break;
        }
    }

    return total_written;
}

ssize_t file_size(char *filename) {
    struct stat st;
    if (stat(filename, &st) == -1) {
        perror("stat");
        exit(1);
    }
    return st.st_size;
}

int process_put(Clients_info *info) {
    // Checks if transfer has just begun 
    if (info->state == PROCESS) {
        int file_fd = open(info->filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        if (file_fd == -1) {
            return -1;
        }
        vector_push_back(files, info->filename);
        info->file_fd = file_fd;
        info->state = IN_PROGRESS;
        
    }

    while (info->state != FINISHED) {

        char *buffer = calloc(1024, sizeof(char));

        // Read the data to put into the buffer
        ssize_t bytes_read = socket_read(info->sock_fd, buffer, 1024);
        if (bytes_read == -1) {
            free(buffer);
            close(info->sock_fd);
            close(info->file_fd);
            return -1;
        }
        // Increment the number of bytes transferred into the buffer
        info->offset += bytes_read;
        if (bytes_read == 0 && (info->offset < info->size || info->offset > info->size)) {
            // end of file and data below/above expected value
            info->state = ERROR;
            info->error = (char*) err_bad_file_size;
            free(buffer);
            return 0;
        }

        // Write the buffered data into the file using file descriptor

        ssize_t bytes_written = socket_write(info->file_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            perror ("write");
            free(buffer);
            close(info->file_fd);
            close(info->sock_fd);
            return -1;
        }
        
        

        // Check if completed or not
        if (info->offset == info->size) {
            info->state = FINISHED;
            info->offset = 0;
        }

        free(buffer);
    }

    close(info->file_fd);
    return 0;
}

int file_exists(char *filename) {
    for (size_t i = 0; i < vector_size(files); i++) {
        if (strncmp(filename, vector_get(files, i), strlen(filename)) == 0) {
            return 1;
        }
    }
    return 0;
}

int process_get(Clients_info *info) {
    // Checks if transfer just begun and files need to be opened.
    if (info->state == PROCESS) {
        if (file_exists(info->filename) == 0) {
            info->state = ERROR;
            info->error = (char*) err_no_such_file;
            return 0;
        }

        info->file_fd = open(info->filename, O_RDONLY, S_IRUSR);
        if (info->file_fd == -1) {
            perror("open");
            return -1;
        }

        info->size = file_size(info->filename);
        int status = write_response(info);
        if (status == -1) {
            return -1;
        }
        info->state = IN_PROGRESS;
    }

    while (info->state != FINISHED) {
        char *buffer = calloc(1024, sizeof(char));
        // Fill the buffer from the local file
        ssize_t bytes_read = socket_read(info->file_fd, buffer, 1024);
        if (bytes_read == -1) {
            free(buffer);
            close(info->file_fd);
            close(info->sock_fd);
            return -1;
        }

        // Output filled data into the socket
        ssize_t bytes_written = socket_write(info->sock_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            perror ("write");
            free(buffer);
            close(info->file_fd);
            close(info->sock_fd);
            return -1;
        }

        // Increment the data transaction
        info->offset += bytes_written;
        if (bytes_written == 0 && (info->offset < info->size || info->offset > info->size)) {
            // end of file has been reached but data is not enough or too much data has been sent
            info->error = (char*) err_bad_file_size;
            info->state = ERROR;
            free(buffer);
            break;
        }

        // CHeck if transaction completed
        if (info->offset == info->size) {
            info->state = FINISHED;
            info->offset = 0;
        }

        free(buffer);
    }
    close(info->file_fd);
    close(info->sock_fd);
    return 0;
}

int process_list(Clients_info *info) {
    if (info->state == PROCESS) {
        ssize_t size = 0;
        if (vector_size(files) > 0) {
            for (unsigned i = 0; i < vector_size(files); i++) {
                size += strlen(vector_get(files, i));
            }

            size += (vector_size(files) - 1) * 1; // no \n for last

            // set up the response to write back to the client and realloc if size needed is more
            info->buffer = realloc(info->buffer, size);
            memset(info->buffer, 0, size);
            size_t i;
            for (i = 0; i < vector_size(files) - 1; i++) {
                strcat(info->buffer, vector_get(files, i));
                strcat(info->buffer, "\n");
            }
            strcat(info->buffer, vector_get(files, i));

            info->size = size;
        }

        char *response = "OK\n";
        // Write response
        int bytes_written = socket_write(info->sock_fd, response, strlen(response));
        if (bytes_written == -1) {
            return -1;
        }

        // Write size
        bytes_written = socket_write(info->sock_fd, (void*) &info->size, sizeof(ssize_t));
        if (bytes_written == -1) {
            return -1;
        }

        // Check if needed to write the output or if we're done
        if (vector_size(files) > 0) {
            info->state = IN_PROGRESS;
        } else {
            info->state = FINISHED;
        }
    }

    // Writing the output
    while (info->state != FINISHED) {
        int bytes_written = socket_write(info->sock_fd, info->buffer, info->size);
        if (bytes_written == -1) {
            return -1;
        }

        if (bytes_written >= info->size) {
            info->state = FINISHED;
        }
    }

    close(info->sock_fd);
    return 0;
}

int process_delete(Clients_info *info) {
    if (info->state == PROCESS) {
        char *filename = info->filename;
        int flag = 0;
        for (size_t i = 0; i < vector_size(files); i++) {
            if (strncmp(filename, vector_get(files, i), strlen(filename)) == 0) {
                if (remove(filename) == -1) {
                    return -1;
                }
                flag= 1;
                if (info->filename != NULL) {
                    free(info->filename);
                }
                if (info->buffer != NULL) {
                    free(info->buffer);
                }
                vector_erase(files, i);
                info->state = FINISHED;
            }
        }

        if (flag == 0) {
            info->error = (char*) err_no_such_file;
            info->state = ERROR;
            return 0;
        }
    }

    return 0;
}



void run_server(char *port) {
    listen_sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    struct addrinfo hints;
    struct addrinfo *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    int s = getaddrinfo(NULL, port, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
    }

    int optval;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    if (bind(listen_sock, result->ai_addr, result->ai_addrlen) != 0) {
        perror("bind");
        exit(1);
    }

    freeaddrinfo(result);


    if (listen(listen_sock, MAX_CLIENTS)) {
        perror("listen");
        exit(1);
    }

    struct epoll_event ev, events[MAX_EVENTS];
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("eppoll create");
        exit(1);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(1);
    }

    for(;;) {
        if (close_connection) {
            break;
        }
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds > 0) {
            for (int i = 0; i < nfds; i++) {
                if (events[i].data.fd == listen_sock) {
                    int client_sock = accept(listen_sock, NULL, NULL);
                    if (client_sock == -1) {
                        perror("accept");
                        exit(1);
                    }

                    int flags = fcntl(client_sock, F_GETFL, 0);
                    fcntl(client_sock, F_SETFL, flags | O_NONBLOCK);

                    Clients_info *info = calloc(1, sizeof(Clients_info));
                    command_create(info, client_sock);
                    dictionary_set(socket_dict, &client_sock, info);

                    struct epoll_event event;
                    memset(&event, 0, sizeof(ev));
                    event.events = EPOLLIN | EPOLLET | EPOLLOUT;
                    event.data.fd = client_sock;

                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sock, &event) == -1) {
                        perror("epoll_ctl: client_sock");
                        exit(1);
                    }
                } else {
                    do_use_fd(events[i].data.fd);
                }
            }
        }
    }

    cleanup(listen_sock);
    close(epoll_fd);
}

int main(int argc, char **argv) {
    // good luck!
    if (argc != 2) {
        print_server_usage();
        return 1;
    }

    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_handler = close_server;
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }

    signal(SIGPIPE, sigpipe);

    // create directory
    char *dir = mkdtemp(template);
    if (dir == NULL) {
        perror("mkdtemp");
        exit(1);
    }

    print_temp_directory(dir);
    if (chdir(template) == -1) {
        perror("chdir");
        exit(1);
    }

    files = shallow_vector_create();
    socket_dict = int_to_shallow_dictionary_create();

    run_server(argv[1]);

    vector_destroy(files);
    vector *keys = dictionary_keys(socket_dict);
    for (unsigned i = 0; i < vector_size(keys); i++) {
        int *key = vector_get(keys, i);
        Clients_info *cmd = dictionary_get(socket_dict, key);
        if (cmd) {
            free(cmd);
        }
        if (cmd->filename != NULL) {
            free(cmd->filename);
        }
        if (cmd->buffer != NULL) {
            free(cmd->buffer);
        }
        
    }
    vector_destroy(keys);
    dictionary_destroy(socket_dict);
    return 0;
}
