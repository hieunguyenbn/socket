/* a datagram sockets IPvv6 domain
   Elon Nguyen */
#include <sys/types.h> /* Type definitions used by many programs */
#include <stdio.h>     /* Standard I/O functions */
#include <stdlib.h>    /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>    /* Prototypes for many system calls */
#include <errno.h>     /* Declares errno and defines error constants */
#include <string.h>    /* Commonly used string-handling functions */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

#define BUFFER_SIZE    25
#define PORT_NUM       50002

int main(int argc, char *argv[])
{
    struct sockaddr_in6 client_addr;
    int client_fd;
    int port = PORT_NUM;
    int j;
    size_t msgLen;
    ssize_t numBytes;
    char resp[BUFFER_SIZE];

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        printf("%s host-address msg...\n", argv[0]);

    /* Create a datagram socket; send to an address in the IPv6 domain */
    client_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (client_fd == -1)
    {
        perror("socket creation error!");
        exit(EXIT_FAILURE);
    }

    memset(&client_addr, 0, sizeof(struct sockaddr_in6));
    client_addr.sin6_family = AF_INET6;
    client_addr.sin6_port = htons(port);

    if (inet_pton(AF_INET6, argv[1], &client_addr.sin6_addr) <= 0)
        printf("inet_pton failed for address '%s'", argv[1]);

    /* Send messages to server; echo responses on stdout */

    for (j = 2; j < argc; j++)
    {
        msgLen = strlen(argv[j]);
        if (sendto(client_fd, argv[j], msgLen, 0, (struct sockaddr *)&client_addr,
                    sizeof(struct sockaddr_in6)) != msgLen)
            perror("sendto");

        numBytes = recvfrom(client_fd, resp, BUFFER_SIZE, 0, NULL, NULL);
        if (numBytes == -1)
            perror("recvfrom");

        printf("Response %d: %.*s\n", j - 1, (int)numBytes, resp);
    }

    exit(EXIT_SUCCESS);
}