// FTP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <stdlib.h>
#include <windows.h>



#pragma comment(lib,"ws2_32.lib")

#define BUFSIZE 512

SOCKET pasv(SOCKET, char*);
void listCmd(SOCKET, SOCKET, char*);

void login(int rec, SOCKET conSock, char* buf){
	char* user = "USER ftp\r\n";
	char* pass = "PASS \r\n";
	char* opts = "OPTS UTF8 ON\r\n";
	
	char* pwd = "PWD\r\n";
	char* binary = "TYPE I\r\n";
	

	SOCKET dataSock;

	send(conSock, user, (strlen(user)), 0);
	printf("Sent USER\n");
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);


	send(conSock, pass, (strlen(pass)), 0);
	printf("Sent PASS\n");
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

	send(conSock, opts, (strlen(opts)), 0);
	printf("Sent OPT\n");
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

	

	send(conSock, pwd, (strlen(pwd)), 0);
	printf("Sent PWD\n");
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

//	rec = recv(dataSock, buf, 512 * 8, 0);
//	printf("%.*s\n", rec, buf);

	send(conSock, binary, (strlen(binary)), 0);
	printf("Sent TYPE I\n");
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

	dataSock = pasv(conSock, buf);

	listCmd(conSock, dataSock, buf);

	

}

void listCmd(SOCKET conSock, SOCKET dataSock, char* buf){
	char* list = "LIST\r\n";
	int rec;

	send(conSock, list, (strlen(list)), 0);
	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

	
	rec = recv(dataSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

	rec = recv(conSock, buf, 512 * 8, 0);
	printf("%.*s\n", rec, buf);

}

SOCKET pasv(SOCKET conSock, char* buf){
	struct sockaddr_in dataAddr;
	SOCKET dataSock;
	char* pasv = "PASV\r\n";
	char* recvBuf = (char*)malloc(sizeof(char)* 512);
	int rec;

	send(conSock, pasv, (strlen(pasv)), 0);
	printf("Sent PASV\n");
	rec = recv(conSock, recvBuf, 512 * 8, 0);
	while (rec == -1){
		rec = recv(conSock, recvBuf, 512 * 8, 0);
	}
	//rec = recv(conSock, recvBuf, 512 * 8, 0);
	printf("%.*s\n", rec, recvBuf);

	
	char* port = NULL;
	strtok(recvBuf, ",");
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
	char* buff = (char*)malloc(sizeof(char)* BUFSIZE);
	fgets(input, 256, stdin);
	strtok(input, "\n");
	rec = strlen(input);
	if (rec <= 254){
		input[rec] = '\r';
		input[rec + 1] = '\n';
		//input[rec + 2] = '\0';
	}
	bool li = strstr(input, "list");
	bool li2 = strstr(input, "LIST");
	if ( li != NULL ||  li2 != NULL){
		SOCKET dataSock;
		dataSock = pasv(conSock, buff);
		listCmd(conSock, dataSock, buff);
		closesocket(dataSock);
	}
	else if (strstr(input, "quit") != NULL){
		rec = send(conSock, input, rec + 2, 0);
		//printf("%.*s\n", rec, buff);
		rec = recv(conSock, buff, 512 * 8, 0);
		printf("%.*s\n", rec, buff);

		closesocket(conSock);
		Sleep(1000);
		exit(0);
	}
	else {
		rec = send(conSock, input, rec + 2, 0);
		//printf("%.*s\n", rec, buff);
		rec = recv(conSock, buff, 512 * 8, 0);
		printf("%.*s\n", rec, buff);
	}

	free(buff);
	
	
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
	int bytesRet = 0;
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
	//bytesRet = recv(conSock, buf, 512 * 8, 0);
	//printf("%.*s\n", bytesRet, buf);


	login(bytesRet, conSock, buf);

	

	//bytesRet = recv(conSock, buf, 512 * 8, 0);
	//printf("%.*s\n", bytesRet, buf);
	char* input = (char*)malloc(sizeof(char)* BUFSIZE);
	
	while (true){
		inputCmd(input, bytesRet, conSock, buf);
		memset(input, 0, sizeof(char)* BUFSIZE);
		//bytesRet = recv(dataSock, buf, 512 * 8, MSG_TRUNC);
		//perror("EWOULDBLOCK");
		//perror("EAGAIN");
		//printf("%.*s\n", bytesRet, buf);
	}
	
	
	return 0;
}

