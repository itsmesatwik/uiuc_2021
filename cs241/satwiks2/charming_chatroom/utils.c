/**
 * Charming Chatroom
 * CS 241 - Spring 2019
 */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "utils.h"
static const size_t MESSAGE_SIZE_DIGITS = 4;
 
char *create_message(char *name, char *message) {
    int name_len = strlen(name);
    int msg_len = strlen(message);
    char *msg = calloc(1, msg_len + name_len + 4);
    sprintf(msg, "%s: %s", name, message);
    return msg;
}
 
ssize_t get_message_size(int socket) {
    int32_t size;
    ssize_t read_bytes =
        read_all_from_socket(socket, (char *)&size, MESSAGE_SIZE_DIGITS);
    if (read_bytes == 0 || read_bytes == -1)
        return read_bytes;
 
    return (ssize_t)ntohl(size);
}
 

ssize_t write_message_size(size_t size, int socket) {
    // Your code here
    int32_t host_to_network = htonl(size);
 
    ssize_t ret = write_all_to_socket(socket, (char *) & host_to_network, MESSAGE_SIZE_DIGITS);
    if(ret < 0){
        fprintf(stderr, "write failed\n");
    }  
    return ret;
}
 
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
 
ssize_t write_all_to_socket(int socket, const char *buffer, size_t count) {
    // Your Code Here
   
    ssize_t bytes_left_to_write = count;
    ssize_t bytes_already_write = 0;
    ssize_t write_ret = -1;
    while(bytes_left_to_write > 0 || (write_ret == -1 && errno == EINTR)){
        write_ret = write(socket, (void *)buffer + bytes_already_write, bytes_left_to_write);
        if(write_ret == 0){
            return 0;
        }
        else if(write_ret > 0) {
            bytes_already_write = bytes_already_write + write_ret;
            bytes_left_to_write = bytes_left_to_write - write_ret;
        }

    }
    return count;
}