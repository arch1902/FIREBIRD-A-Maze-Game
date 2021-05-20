#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

static int sock;

static int connect_client(){
	sock = 0;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)  //  SOCK_STREAN = TCP
	{
		printf("\n Socket creation error \n");
		return -1;
	}
	serv_addr.sin_family = AF_INET; // Internet family for IPv4 only
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return false;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return false;
	}
	cout<<"Client Socket :" << sock<<endl;
	return sock;
}

static void send_from_client(string s, int n){
	const char * msg = s.c_str();
	send(n,msg,strlen(msg),0);
	
}

static std::string receive_in_client(int n){
	char buffer[8192] = {0};
	auto j = buffer[1];
	int valread = read( n , buffer, 8192);
	std::string out;
	int len = sizeof(buffer)/(8*sizeof(char));
	for (int i = 0;i<len;i++){
		if (buffer[i]== j){break;}
		out += buffer[i];
	}
	return out;
}

static void close_connection_client(int sock){
	shutdown(sock,2);
	close(sock);
	cout<<"Disconnected!"<<endl;
}
