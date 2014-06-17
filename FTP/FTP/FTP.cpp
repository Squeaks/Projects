// FTP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <stdlib.h>

#pragma comment(lib,"ws2_32.lib")

SOCKET pasv(SOCKET, char*);

void login(int rec, SOCKET conSock, char* buf){
	char* user = "USER ftp\r\n";
	char* pass = "PASS \r\n";
	char* opts = "OPTS UTF8 ON\r\n";
	
	char* pwd = "PWD\r\n";
	char* binary = "TYPE I\r\n";

	send(conSock, user, (strlen(user)), 0);
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

	send(conSock, pass, (strlen(pass)), 0);
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

	send(conSock, opts, (strlen(opts)), 0);
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

	pasv(conSock, buf);

	send(conSock, pwd, (strlen(pwd)), 0);
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

//	rec = recv(dataSock, buf, 512 * 8, 0);
//	printf("%.*s\n", rec, buf);

	send(conSock, binary, (strlen(binary)), 0);
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);


}

SOCKET pasv(SOCKET conSock, char* buf){
	struct sockaddr_in dataAddr;
	SOCKET dataSock;
	char* pasv = "PASV\r\n";
	int rec;

	send(conSock, pasv, (strlen(pasv)), 0);
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

	
	char* port = NULL;
	strtok(buf, ",");
	strtok(port, ",");
	strtok(port, ",");
	strtok(port, ",");
	port = strtok(port, ",");
	int data = atoi(port) * 256 + atoi(strtok(NULL, ","));

	dataSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (dataSock == INVALID_SOCKET){
		wprintf(L"Socket failed with = %d\n", WSAGetLastError());
		return NULL;
	}



	//conName = gethostbyname(con);
	char IP[] = "192.168.1.107";


	dataAddr.sin_family = AF_INET;
	dataAddr.sin_addr.s_addr = inet_addr(IP);
	dataAddr.sin_port = htons(data);

	int err = connect(dataSock, (sockaddr*)&dataAddr, sizeof(dataAddr));
	listen(dataSock, 10);

	return dataSock;
}

void inputCmd(char* input, int rec, SOCKET conSock, char* buf){
	fgets(input, 256, stdin);
	strtok(input, "\n");
	rec = strlen(input);
	if (rec <= 254){
		input[rec] = '\r';
		input[rec + 1] = '\n';
		//input[rec + 2] = '\0';
	}
	if (strpbrk(input, "list") != NULL || strpbrk(input, "LIST") != NULL){
		SOCKET dataSock;
		dataSock = pasv(conSock, buf);
		rec = send(conSock, input, rec + 2, 0);
		rec = recv(dataSock, buf, 512 * 8, 0);
		rec = recv(dataSock, buf, 512 * 8, 0);
		printf("%.*s", rec, buf);
	}
	else {
		rec = send(conSock, input, rec + 2, 0);
	}
	//rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);
}


int main(int argc, char* argv)
{
	SOCKET conSock, dataSock;
	WSADATA wsadata;
	struct sockaddr_in conAddr, dataAddr;
	/*
	conAddr = (sockaddr_in*)malloc(sizeof(struct sockaddr_in));
	dataAddr = (sockaddr_in*)malloc(sizeof(struct sockaddr_in));
	*/
	char* conIP;
	hostent* conName;
	char* con = &argv[1];
	WSADATA wsaData = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	int bytesRet;
	conSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (conSock == INVALID_SOCKET){
		wprintf(L"Socket failed with = %d\n", WSAGetLastError());
		return -1;
	}
	
	
	
	//conName = gethostbyname(con);
	char IP[] = "192.168.1.107";
	addrinfo* temp;
	char* buf = (char*)malloc(sizeof(char)* 512);

	conAddr.sin_family = AF_INET;
	conAddr.sin_addr.s_addr = inet_addr(IP);
	conAddr.sin_port = htons(21);

	int err = connect(conSock, (sockaddr*)&conAddr, sizeof(conAddr));
	listen(conSock, 10);
	bytesRet = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", bytesRet, buf);


	login(bytesRet, conSock, buf);

	

	//bytesRet = recv(conSock, buf, 512 * 8, 0);
	//printf("%.*s\n", bytesRet, buf);
	char* input = (char*)malloc(sizeof(char)* 256);
	
	while (true){
		inputCmd(input, bytesRet, conSock, buf);
		//bytesRet = recv(dataSock, buf, 512 * 8, MSG_TRUNC);
		//perror("EWOULDBLOCK");
		//perror("EAGAIN");
		//printf("%.*s\n", bytesRet, buf);
	}
	
	
	return 0;
}

