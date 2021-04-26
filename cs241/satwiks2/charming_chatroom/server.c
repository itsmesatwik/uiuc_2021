/**
 * Charming Chatroom
 * CS 241 - Spring 2019
 */
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
 
#include "utils.h"
 
#define MAX_CLIENTS 8
 
void *process_client(void *p);
 
static volatile int serverSocket;
static volatile int endSession;
 
static volatile int clientsCount;
static volatile int clients[MAX_CLIENTS];
 
static pthread_t pid[MAX_CLIENTS];
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 
/**
 * Signal handler for SIGINT.
 * Used to set flag to end server.
 */
void close_server() {
    endSession = 1;
    // add any additional flags here you want.
    if(shutdown(serverSocket, SHUT_RDWR) != 0){
        perror(NULL);
    }
    close(serverSocket);
    serverSocket = 0;
    clientsCount = 0;
}
 
/**
 * Cleanup function called in main after `run_server` exits.
 * Server ending clean up (such as shutting down clients) should be handled
 * here.
 */
void cleanup() {
    if (shutdown(serverSocket, SHUT_RDWR) != 0) {
        perror("shutdown():");
    }
    close(serverSocket);
 
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != -1) {
            if (shutdown(clients[i], SHUT_RDWR) != 0) {
                perror("shutdown(): ");
            }
            close(clients[i]);
        }
    }
}
 
/**
 * Sets up a server connection.
 * Does not accept more than MAX_CLIENTS connections.  If more than MAX_CLIENTS
 * clients attempts to connects, simply shuts down
 * the new client and continues accepting.
 * Per client, a thread should be created and 'process_client' should handle
 * that client.
 * Makes use of 'endSession', 'clientsCount', 'client', and 'mutex'.
 *
 * port - port server will run on.
 *
 * If any networking call fails, the appropriate error is printed and the
 * function calls exit(1):
 *    - fprtinf to stderr for getaddrinfo
 *    - perror() for any other call
 */
void run_server(char *port) {
    /*QUESTION 1*/
    /*QUESTION 2*/
    /*QUESTION 3*/
    clientsCount = 0;  
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    serverSocket = fd;
    if(fd == -1){
        perror(NULL);
        exit(1);
    }
 
    /*QUESTION 8*/
    int option = 1;
    int status = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if(status == -1){
        perror(NULL);
        exit(1);
    }
 
    /*QUESTION 4*/
    struct addrinfo stat, * result;
    memset(&stat, 0, sizeof(stat));  
 
    /*QUESTION 5*/
    stat.ai_family = AF_INET;
    stat.ai_socktype = SOCK_STREAM;
 
    /*QUESTION 6*/
    int success = getaddrinfo(NULL, port, &stat, &result);
    if(success != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(success));
        exit(1);
    }
 
    /*QUESTION 9*/
    if(bind(fd, result->ai_addr, result->ai_addrlen) != 0){
        perror(NULL);
        exit(1);
    }
 
    /*QUESTION 10*/
    if(listen(fd, MAX_CLIENTS) != 0){
        perror(NULL);
        exit(1);
    }
   
    for(int i = 0; i < MAX_CLIENTS; i++){
        clients[i] = -1;
    }
 
    while(endSession == 0){
        fprintf(stdout, "Waiting for connection...\n");
        struct sockaddr addr;
        socklen_t addrlen = sizeof(struct sockaddr);
        memset(&addr, 0, addrlen);
        int client_fd = accept(fd, &addr, &addrlen);
        if(client_fd < 0){
            perror(NULL);
            exit(1);
        }
        printf("Connection made: client_fd=%d\n", client_fd);
        if(clientsCount >= MAX_CLIENTS){
            if(shutdown(client_fd, SHUT_RDWR) != 0){
                perror(NULL);
            }
            close(serverSocket);
            continue;
        }
        intptr_t i2 = clientsCount; int i = 0;
        for(i = 0; i < MAX_CLIENTS; i++){
            if(clients[i] == -1){
                break;
            }
        }
        i2 = i;
        clients[i] = client_fd;
        pthread_create(pid + clientsCount, NULL, process_client, (void *)i2);
        clientsCount++;

    }
 
}
 
/**
 * Broadcasts the message to all connected clients.
 *
 * message  - the message to send to all clients.
 * size     - length in bytes of message to send.
 */
void write_to_clients(const char *message, size_t size) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != -1) {
            ssize_t retval = write_message_size(size, clients[i]);
            if (retval > 0) {
                retval = write_all_to_socket(clients[i], message, size);
            }
            if (retval == -1) {
                perror("write(): ");
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}
 
/**
 * Handles the reading to and writing from clients.
 *
 * p  - (void*)intptr_t index where clients[(intptr_t)p] is the file descriptor
 * for this client.
 *
 * Return value not used.
 */
void *process_client(void *p) {
    pthread_detach(pthread_self());
    intptr_t clientId = (intptr_t)p;
    ssize_t retval = 1;
    char *buffer = NULL;
 
    while (retval > 0 && endSession == 0) {
        retval = get_message_size(clients[clientId]);
        if (retval > 0) {
            buffer = calloc(1, retval);
            retval = read_all_from_socket(clients[clientId], buffer, retval);
        }
        if (retval > 0)
            write_to_clients(buffer, retval);
 
        free(buffer);
        buffer = NULL;
    }
 
    printf("User %d left\n", (int)clientId);
    close(clients[clientId]);
 
    pthread_mutex_lock(&mutex);
    clients[clientId] = -1;
    clientsCount--;
    pthread_mutex_unlock(&mutex);
 
    return NULL;
}
 
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "%s <port>\n", argv[0]);
        return -1;
    }
 
    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_handler = close_server;
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        return 1;
    }
 
    run_server(argv[1]);
    cleanup();
    pthread_exit(NULL);
}