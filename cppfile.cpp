#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>

void *client(void *port)
{
    //local variables 
  int sockfd = 0;
  int bytesReceived = 0;
  unsigned char recvBuff[256];
  struct sockaddr_in serv_addr;
  char temp[256];
  int i = 1;

    //creating a TCP socket 
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0){
    printf("C: Could not create socket \n");
    return 0;
  }
  uint16_t *portnum=(uint16_t *)port;
    //obtaining server information
        serv_addr.sin_family = AF_INET; //IP family
        serv_addr.sin_port = htons(*portnum); // port
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //server IP

    //attempting a connection with server
        if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) {
          printf("C: Connection Failed \n");
          printf("%s\n", strerror(errno));
          return 0;
        }


    //read first message from server
        read(sockfd, temp, sizeof(temp));
        printf("%s", temp);

        //sending file name to server
          memset(recvBuff, '\0', sizeof(recvBuff)); //sanitizing buffer

          write(sockfd, "file.txt", sizeof(temp));
          FILE *fp;
          fp = fopen("file(client-copy).txt", "wb"); 
          if(NULL == fp){
            printf("C: Error opening file.\n");
            close(sockfd);
            return 0;
          }

          printf("client : Receiving file.\n"); //start receiving

    //receive file line by line
          while((bytesReceived = read(sockfd, recvBuff, sizeof(recvBuff))) > 0){
            for (i = 0; i < bytesReceived; i++)
                  recvBuff[i] ^= 12;      //encrypt file data for security
                fwrite(recvBuff, 1,bytesReceived,fp);
              }
              printf("client : file received. Going offline.\n");
          fflush(fp); //sanitize file handler
          fclose(fp); //close file handler
      close(sockfd);  //close connection
    }

    void *server (void *port){
      char *paths[100];
      int filesize[100];
      int pathcount=0;
      int listenfd = 0;
      int connfd = 0;
      int i, j =0;
      int bytesread;
      printf("Server : First run, starting thread.\n");
  //binding socket to a port
      uint16_t *portnum=(uint16_t *)port;
      struct sockaddr_in serv_addr;
      listenfd = socket(AF_INET, SOCK_STREAM, 0); //open a listen() on a certain port
      serv_addr.sin_family = AF_INET;     //IP addr family
      serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
      serv_addr.sin_port = htons(*portnum);  //port
      bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)); //bind port

  //main loop
      while(1){

       char temp[256];
       unsigned char buff[256];
       printf("Server : Waiting for new connection.\n");
       if(listen(listenfd, 10) == -1){
        printf("S: Failed to listen\n");
      }

    //accept connections from clients
      connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
      pathcount = 0;

    //send first message to client
      printf("Server : Connection established.\n");

      strcpy(temp, "Server : Welcome to C++ sever. Enter filename.\n");
      write(connfd, temp, sizeof(temp));

    //read file name
      read(connfd, temp, sizeof(temp));
      printf("client : filename = %s", temp);
      int fp = open(temp, O_RDWR ,  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

      memset(buff, 0, sizeof(buff));

      j = 0;
    //start reading file data and send it to client
      printf("Server : Sending file.\n");
      while(bytesread = read(fp,buff, sizeof(buff))){
        for(i = 0; i < bytesread; i++)
                    buff[i] ^= 12;    //encrypt file data for security
                  j = j + bytesread;
                  fflush(stdout);
                  write(connfd, buff, bytesread);
                }
                printf("Server : file sent, going to idle state.\n");
            close(connfd);  //close connection
          }
  //close socket
          close(listenfd);
          return 0;
        }

        int main ()
        {
          char temp[256];
          pthread_t t1;
          int serv = pthread_create(&t1, NULL,  server, (void *)55556);
          while(1){
            printf("Start client ? type JAVA for java server and C++ for c++ server\n");
            scanf("%s", temp);
            if (strcmp(temp, "JAVA")){
              pthread_t t2;
              int *port;
              *port = 55546; 
              int cl= pthread_create(&t2, NULL,  client, (void *)55555);
            }
            else{
              pthread_t t3;
               int *port;
              *port = 55546; 
              int cl= pthread_create(&t3, NULL,  client, (void *)55556);
            }
          }
        }
