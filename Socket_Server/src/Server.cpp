/*
 * Server.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: Clayton.Bagnall
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
	char * response = "[Server] That is correct!";
	char * password = "Password\n";
     int sockfd, newsockfd, portnum;
     struct sockaddr_in serv_addr, cli_addr;
     socklen_t client_len = sizeof(cli_addr);
     char send_buffer[256];
     char recieve_buffer[256];
     int data, success;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);			//create socket

     if (sockfd < 0) {
        perror("ERROR opening socket");
     	exit(1);
     }

     memset((char *) &serv_addr, 0, sizeof(serv_addr)); //clear server address
     portnum = atoi(argv[1]); 							//get port number
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portnum);				//convert server byte address to network address

     success = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)); //bind socket to address
     if (success < 0){
         perror("ERROR on binding");
     	 exit(1);
     }
     printf("[Socket] listening...\n");
     listen(sockfd,1); 									//only allow one socket

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &client_len); //delay until accepting connection
     if (newsockfd < 0){
         perror("ERROR on accept");
     	 exit(1);
     }
     memset(send_buffer, 0, 256);						//clear send buffer first time only

     while (true) {

		 memset(recieve_buffer, 0, 256);
		 data = read(newsockfd,recieve_buffer,255); 	//blocking call
		 if (data < 0) {
			 perror("ERROR reading from socket");
			 exit(1);
		 }
		 //printf ("password is: %s, your guess was: %s", password, recieve_buffer);
		 int result = strcmp(recieve_buffer, password);
		 if (result == 0) {
			 strcpy(send_buffer, response);
			 data = write(newsockfd, send_buffer, sizeof(send_buffer));
			 if (data < 0) {
				 perror("ERROR writing to socket");
				 exit(1);
			 }
			 break; 										//terminate program
		 }
     }
     close(newsockfd);									//close socket connections
     close(sockfd);
     return 0;
}
