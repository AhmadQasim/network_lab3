
//dependencies
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>


//global variables
char *paths[100];
int filesize[100];
int pathcount=0;

int main(void){
	
	//local variables
	int listenfd = 0;
    int connfd = 0;
    int i, j =0;
    int bytesread;
    printf("Server : First run, starting thread.\n");
	//binding socket to a port
    struct sockaddr_in serv_addr;
    	listenfd = socket(AF_INET, SOCK_STREAM, 0); //open a listen() on a certain port
    	serv_addr.sin_family = AF_INET;			//IP addr family
    	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    	serv_addr.sin_port = htons(54559);	//port
    	bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));	//bind port

	//main loop
    	while(1){

           char temp[256];
           unsigned char buff[256];
           printf("Server : Waiting for new connection.\n");
           if(listen(listenfd, 10) == -1){
              printf("S: Failed to listen\n");
              return -1;
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
                		buff[i] ^= 12;		//encrypt file data for security
                 j = j + bytesread;
                 fflush(stdout);
                 write(connfd, buff, bytesread);
             }
             printf("Server : file sent, going to idle state.\n");
        		close(connfd);	//close connection
          }
	//close socket
          close(listenfd);
          return 0;
      }

