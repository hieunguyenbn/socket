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

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr; //Cau truc chua dia chi server ma client can biet de ket noi toi

    char sendbuff[256];
    char recvbuff[256];

    //Client nhan tham so hostname va port tu dong lenh
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]); //Chuyen cong dich vu thanh so nguyen
   
   //Tao socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        perror("ERROR opening socket");
      
    
    memset(&serv_addr, '0', sizeof(serv_addr));

    //Thiet lap dia chi cua server de ket noi den
    serv_addr.sin_family = AF_INET;        //Mac dinh
    serv_addr.sin_port = htons(portno);    //Cong dich vu
   
    //Dia chi ip/domain may chu
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton perror occured\n");
        return 1;
    }

    //Goi ham connect de thuc hien mot ket noi den server
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    while (1)
    {
        //printf("Please enter the message: ");

        memset(sendbuff, 0, 256); //Khoi tao buffer
        fgets(sendbuff,255,stdin); //Chua thong diep doc tu ban phim
        sendbuff[strcspn(sendbuff, "\n")] = 0;
        //Gui du lieu den server bang cach ghi ra socket vua thiet lap
        // n = write(sockfd,sendbuff,strlen(sendbuff));
        // if (n < 0) 
        //     perror("ERROR writing to socket");
        send(sockfd, sendbuff, strlen(sendbuff), 0);
        //Nhan du lieu tu server
        memset(recvbuff, 0, 256);
        n = read(sockfd,recvbuff,255);
        if (n < 0) 
            perror("ERROR reading from socket");
        printf("%s\n",recvbuff);
    
        //close(sockfd); //Dong socket
    }
    close(sockfd); // Dong socket
    return 0;
}
