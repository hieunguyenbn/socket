/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

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
     int sockfd;  //socket server tạo ra để nghe kết nối 
     int newsockfd; //socket tạo ra khi server chấp nhận một kết nối từ client
     int portno; //So hieu cong dich vu
     
     char sendbuff[256]; //buffer to send data
     char recvbuff[256]; //buffer to read data
     
     struct sockaddr_in serv_addr, client_addr; //Cấu trúc chứa thông tin địa chỉ server, client
   
     int n, len, opt = 1;
     
        
    //So hieu cong dich vu (port number) truyen tu tham so dong lenh 
    if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     portno = atoi(argv[1]); //chuyen thanh so nguyen
             
     //Khoi tao gia tri cho cac vung nho
     memset(&serv_addr, '0', sizeof(serv_addr));
     memset(&client_addr, '0', sizeof(client_addr));
     memset(sendbuff, '0', 256);
     
     //Thiet lap dia chi server
     serv_addr.sin_family = AF_INET;       //default
     serv_addr.sin_addr.s_addr = INADDR_ANY;  //ip server
     serv_addr.sin_port = htons(portno);      //port number
     
     //Tao socket
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        perror("ERROR opening socket");
     if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
     {
        perror("setsockopt");
        exit(EXIT_FAILURE);
     }

     //bind socket
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              perror("ERROR on binding");
     
     //listening
     listen(sockfd,5);    //Toi da 5 backlog trong hang doi

     len = sizeof(client_addr); //Dung de lay thong tin dia chi client ket noi den

             
     //Server su dung mot vong lap de lien tuc nghe va phuc vu client ket noi den
     while(1){
      //printf("Server is listening at port %d\n", portno);
      newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t*)&len);
      if (newsockfd < 0) 
      {
             perror("ERROR on accept");
             continue;
         } 
         char s[255]; //Chua thong tin dia chi client ket noi den
         inet_ntop(client_addr.sin_family,(struct sockaddr *)&client_addr,s, sizeof s);
         printf("server: got connection from %s\n", s);
                
         memset(recvbuff, 0, 256);
      //read data from socket 
      n = read(newsockfd,recvbuff,255);    //block den khi co du lieu tu client gui toi
      if (n < 0) perror("ERROR reading from socket");
      
         printf("Message from client: %s\n",recvbuff);
     
      //write data via socket
      memset(sendbuff, 0, 256); //Khoi tao buffer
      fgets(sendbuff,255,stdin); //Chua thong diep doc tu ban phim
      sendbuff[strcspn(sendbuff, "\n")] = 0;
      //strcpy(sendbuff, "Server has got message\n");
    //   n = write(newsockfd,sendbuff,sizeof(sendbuff));
    //   if (n < 0) perror("ERROR writing to socket");
      send(newsockfd, sendbuff, strlen(sendbuff), 0);
      close(newsockfd);    //Dong ket noi cua client
      sleep(1);
     }
         
     close(sockfd);
     return 0; 
}
