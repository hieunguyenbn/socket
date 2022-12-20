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
    struct sockaddr_in6 server_addr, client_addr;
    int server_fd;
    int opt = 1;
    int i = 0;
    int port = PORT_NUM;
    ssize_t bytes_number;
    socklen_t length;
    char buffer[BUFFER_SIZE] = {0};
    char client_addr_str[INET6_ADDRSTRLEN] = {0};

    if (argc > 1)
        port = atoi(argv[1]);

    server_fd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (server_fd == -1)
    {
        perror("socket creation error!");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in6));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr = in6addr_any; // wildcard address;
    server_addr.sin6_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in6)) == -1)
    {
        perror("binding socket error !");
        exit(EXIT_FAILURE);
    }

    /* Receive messages, convert to uppercase, and return to client */
    for (;;)
    {
        length = sizeof(struct sockaddr_in6);
        bytes_number = recvfrom(server_fd, buffer, BUFFER_SIZE, 0,
                                (struct sockaddr *)&client_addr, &length);
        if (bytes_number == -1)
        {
            perror("recvfrom error !");
            exit(EXIT_FAILURE);
        }

        /* Display address of client that sent the message */
        if (inet_ntop(AF_INET6, &client_addr.sin6_addr, client_addr_str,
            INET6_ADDRSTRLEN) == NULL)
            printf("Couldn't convert client address to string\n");
        else
            printf("Server received %ld bytes from (%s, %u)\n",
                    (long)bytes_number, client_addr_str, ntohs(client_addr.sin6_port));

        for (i = 0; i < bytes_number; i++)
            buffer[i] = toupper((unsigned char)buffer[i]);

        if (sendto(server_fd, buffer, bytes_number, 0, (struct sockaddr *)&client_addr, length) !=
            bytes_number)
            perror("sendto error !");
            exit(EXIT_FAILURE);
    }
    return 0;
}
// https://stackoverflow.com/questions/7474066/how-to-listen-on-all-ipv6-addresses-using-c-sockets-api
// if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
// {
//     perror("setsockopt");
//     exit(EXIT_FAILURE);
// }