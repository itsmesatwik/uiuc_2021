//Partners: Mihika Aggarwal (mihikaa2), Manas Biju (mbiju2), Karthik Mullapudi (ssm5), Satwik Singh (satwiks2), Kimi Hirano (khirano2)
/**
 * Resplendent RPCs
 * CS 241 - Spring 2019
 */
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "common.h"
#include "common.h"
#include "dns_query.h"
#include "dns_query_svc_impl.h"

#define CACHE_FILE "cache_files/rpc_server_cache"

char *contact_nameserver(query *argp, char *host, int port) {
    //use inet_pton to get the address
    struct sockaddr_in addr;
    int sockaddr_success = inet_pton(AF_INET, host, &addr.sin_addr);
    if (sockaddr_success <= 0) exit(1);

    //create socket (UDP, IPv4)
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) exit(1);
    bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr));

    //send packet
    char * packet = argp->host;
    size_t p_size = strlen(packet);
    ssize_t bytes_remaining = p_size;
    ssize_t sent_bytes = -1;
    while (bytes_remaining) {
        sent_bytes = sendto(sock_fd, (void*) packet, p_size, 0, (struct sockaddr*)&addr, sizeof(addr));

        if (sent_bytes == -1 && errno == EINTR) continue;
        else if (sent_bytes == -1) {exit(1); }//??

        packet += sent_bytes;
        bytes_remaining -= sent_bytes;
    }


    //receive IP address from server

    struct sockaddr src_addr;
    int src_addr_len = sizeof(src_addr);
    int MAX_IPADDR_SIZE = 15; //doesn't include the null byte
    char ipaddr[MAX_IPADDR_SIZE];

    ssize_t rec_bytes = -1;
    errno = EINTR;
    while (rec_bytes == -1 && errno == EINTR) {
         rec_bytes = recvfrom(sock_fd, ipaddr, MAX_IPADDR_SIZE, 0, (struct sockaddr*)&src_addr, (socklen_t*)&src_addr_len);
    }

    //check result
    char * result = strdup(ipaddr);
    if (strcmp(result, "-1.-1.-1.-1") == 0) return NULL;
    return result;
}

void create_response(query *argp, char *ipv4_address, response *res) {
    // Your code here
    // As before there are comments in the header file
    *res = *((response*)malloc(sizeof(response)));
    if (ipv4_address) {
        res->address = malloc(sizeof(host_address));
        res->address->host_ipv4_address = strdup(ipv4_address);
        res->address->host = strdup(argp->host);
        (*res).success = 1;
    } else {
        res->address = malloc(1); //should be malloced for mem management
        res->success = 0;
    }
}

// Stub code

response *dns_query_1_svc(query *argp, struct svc_req *rqstp) {
    printf("Resolving query...\n");
    // check its cache, 'rpc_server_cache'
    // if it's in cache, return with response object with the ip address
    char *ipv4_address = check_cache_for_address(argp->host, CACHE_FILE);
    if (ipv4_address == NULL) {
        // not in the cache. contact authoritative servers like a recursive dns
        // server
        printf("Domain not found in server's cache. Contacting authoritative "
               "servers...\n");
        char *host = getenv("NAMESERVER_HOST");
        int port = strtol(getenv("NAMESERVER_PORT"), NULL, 10);
        ipv4_address = contact_nameserver(argp, host, port);
    } else {
        // it is in the server's cache; no need to ask the authoritative
        // servers.
        printf("Domain found in server's cache!\n");
    }

    static response res;
    xdr_free(xdr_response, &res); // Frees old memory in the response struct
    create_response(argp, ipv4_address, &res);

    free(ipv4_address);

    return &res;
}