//dependencies
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

int main(void){

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

		//obtaining server information
    		serv_addr.sin_family = AF_INET; //IP family
    		serv_addr.sin_port = htons(54559); // port
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

        	printf("client : Receiving file.\n");	//start receiving
		
		//receive file line by line
        	while((bytesReceived = read(sockfd, recvBuff, sizeof(recvBuff))) > 0){
            		for (i = 0; i < bytesReceived; i++)
                	recvBuff[i] ^= 12;			//encrypt file data for security
            		fwrite(recvBuff, 1,bytesReceived,fp);
        	}
            printf("client : file received. Going offline.\n");
        	fflush(fp);	//sanitize file handler
        	fclose(fp);	//close file handler
			close(sockfd);	//close connection
    	return 0;
}
