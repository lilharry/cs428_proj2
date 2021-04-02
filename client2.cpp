#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8081


using namespace std;

void chat(int sockfd){
    //hold messages sent to server and received from server    
    char buff[MAX];
    int n;

    //zero out buffer
	bzero(buff, sizeof(buff));
	
    //grab message from stdin
    n = 0;
	while ((buff[n++] = getchar()) != '\n');

    //write message to server socket	
    write(sockfd, buff, sizeof(buff));
    
    //zero out buffer
	bzero(buff, sizeof(buff));

    //read message from server and print
	read(sockfd, buff, sizeof(buff));
    cout << buff << endl;

}


int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    //create and verify socket initialization
    //AF_INET = IPv4
    //SOCK_STREAM = TCP
    //protocol = default
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        cerr << "Socket creation failed" << endl;
        exit(1);
    }
    else{
        //cout << "Socket creation successful" << endl;
    }

    //init to zero 
    bzero(&servaddr, sizeof(servaddr));
      
 
    // assign socket options (IPv4, localhost, and port)
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
  
    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        cerr << "Server connection failed" << endl;
		fprintf(stderr, "connect() failed: %s\n", strerror(errno));
        exit(1);
    }
    else
        cout << "Connected to server" << endl;
  
    // function for chat
    
    chat(sockfd);
  
    // close the socket
    close(sockfd);
}
