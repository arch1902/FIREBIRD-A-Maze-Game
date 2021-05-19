#pragma once

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

static int new_socket;

static int start_server(){
	int server_fd, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	// Forcefully attaching socket to the port 8080
	opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (const struct sockaddr *)&address,(socklen_t)sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	cout<<"Connection Established"<<endl;
	cout<<"Server Socket :" << new_socket<<endl;
	return new_socket;
}

static void send_from_server(string s,int n){
	//cout<<"Server trying to send :"<<s<<endl;
	//cout<<"Server Socket :" << new_socket<<endl;
	const char * msg = s.c_str();
	send(n,msg,strlen(msg),0);
	//cout<<"Sent!"<<endl;
}

static std::string receive_in_server(int n){
	char buffer[1024] = {0};
	auto j = buffer[1];
	int valread = read(n, buffer, 1024);
	//printf("%s\n",buffer );
	std::string out;
	int len = sizeof(buffer)/(8*sizeof(char));
	for (int i = 0;i<len;i++){
		if (buffer[i]== j){break;}
		out += buffer[i];
	}
	return out;
}

static void close_connection_server(int sock){
	shutdown(sock,2);
	close(sock);
	cout<<"Disconnected!"<<endl;
}
