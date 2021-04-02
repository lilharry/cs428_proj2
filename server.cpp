#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <iostream>
#include <string>
#include <unistd.h>

#define MAX 80
#define PORT 8081

using namespace std;

void chat(int connfd1, int connfd2){
    //hold messages for client 1, client 2, and the resulting message
    char buff1[MAX];
    char buff2[MAX];
    char buff3[MAX + MAX];
    int n;
	
    //create fd_set containing both client fds to pass to select() function
    fd_set readfds;
	int activity, nfds;
	FD_ZERO(&readfds);
	FD_SET(connfd1, &readfds);
	FD_SET(connfd2, &readfds);

    //set nfds to highest fd + 1 (argument for select() function)
	if (connfd1 > connfd2){
		nfds = connfd1 + 1;
	}else{
		nfds = connfd2 + 1;
	}
	
    //wait for client response
	activity = select(nfds, &readfds, NULL, NULL, NULL);
	if (activity < 0 && errno != EINTR){
		cerr << "select error" << endl;
		exit(1);
	}

    //zero out buffers
    bzero(buff1, MAX);
    bzero(buff2, MAX);
    bzero(buff3, MAX+MAX);
    
    //if client 1 responded first,
	if (FD_ISSET(connfd1, &readfds)){
        //read client 1 message
		read(connfd1, buff1, sizeof(buff1));
        
        //wait for client 2 message
        read(connfd2, buff2, sizeof(buff2));
        
        //print client messages on server side
        cout << "From client 1: " << buff1 << endl;
        cout << "From client 2: " << buff2 << endl;
        
        //write message to both clients
        strcpy(buff3, ("X: " + string(buff1) + " received before Y: " + string(buff2)).c_str());
        write(connfd1, buff3, sizeof(buff3));
        write(connfd2, buff3, sizeof(buff3));

    //if client 2 responds first
	}else{
		read(connfd2, buff2, sizeof(buff2));
        read(connfd1, buff1, sizeof(buff1));
        cout << "From client 2: " << buff2 << endl;
        cout << "From client 1: " << buff1 << endl;
        strcpy(buff3, ("Y: " + string(buff2) + " received before X: " + string(buff1)).c_str());
        write(connfd1, buff3, sizeof(buff3));
        write(connfd2, buff3, sizeof(buff3));

    }
}


int main(){
    int sockfd, connfd1, connfd2;
	socklen_t len1, len2;
    struct sockaddr_in servaddr, cli1, cli2;

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

	// Fill server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
	servaddr.sin_port = htons(PORT); // port number
         

    //bind socket to IP and verify socket bind
    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0){
        cerr << "Socket bind failed" << endl;
		fprintf(stderr, "bind() failed: %s\n", strerror(errno));
        exit(1);
    }
    else{
        //cout << "Socket bind successful" << endl;
    }
    
    //listen for up to 5 pending connections
    if (listen(sockfd, 5) != 0){
        cerr << "Listen failed" << endl;
        exit(1);
    }
    else{
        cout << "Server listening..." << endl;
    }

    //prepare length arguments for accept
    len1 = sizeof(cli1);
    len2 = sizeof(cli2);

    //accept data packet from client and verify success
    connfd1 = accept(sockfd, (struct sockaddr *) &cli1, &len1);
    if (connfd1 < 0){
        cerr << "Server accept failed" << endl;
        exit(1);
    }
    else{
        cout << "Server accepted client 1, waiting for client 2" << endl;
    }
    
    //accept data packet from client and verify success
    connfd2 = accept(sockfd, (struct sockaddr *) &cli2, &len2);
    if (connfd2 < 0){
        cerr << "Server accept failed" << endl;
        exit(1);
    }
    else{
        cout << "Server accepted client 2, now reading messages" << endl;
    }

    //start function to receive messages
	chat(connfd1, connfd2);
    
    //close socket
    close(sockfd);    


}

