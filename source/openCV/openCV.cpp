/*
 C ECHO client example using sockets
 */
#include<stdio.h> //printf
#include<string.h>    //strlen
#include <unistd.h>  // problem solved! it compiles!
#include <string.h>
#include <stdlib.h>     /* atof */

#ifdef WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <pthread.h>
#include <semaphore.h>
#endif

typedef struct {
	float x;
	float y;
	bool valid;
} st_keypoint;


st_keypoint kp[20];


void updatekp(int idx, int total, float fx, float fy) {

	if(idx<20)
	{
		kp[idx].x=fx;
		kp[idx].y=fy;
		kp[idx].valid=true;
	}

	for(int ii=total;ii<20;ii++)
		kp[ii].valid=false;


	for(int ii=0;ii<20;ii++)
	{
		if(kp[ii].valid)
			printf("=> %i %f %f\n", ii, kp[ii].x, kp[ii].y);
	}
}

char* parseLine(char* str) {
	int idx;
	int total;
	float fx, fy;

	char *token;
	/* get the first token */
	token = strtok(str, " ");

	/* walk through other tokens */
	if (token == NULL)
		return NULL;

	if (strcmp(token, "kp") != 0)
		return NULL;

	token = strtok(NULL, " ");
	if (token == NULL)
		return NULL;

	idx = atoi(token);

	token = strtok(NULL, " ");
	if (token == NULL)
		return NULL;

	total = atoi(token);

	token = strtok(NULL, " ");
	if (token == NULL)
		return NULL;

	fx = atof(token);

	token = strtok(NULL, " ");
	if (token == NULL)
		return NULL;

	fy = atof(token);

	updatekp(idx, total, fx, fy);

	token = strtok(NULL, "\n");
	return token;
}

void * maintcp(void * p) {
	int sock;
	struct sockaddr_in server;
	char server_reply[2000];
	char* tmp;
	//Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
		perror("connect failed. Error");
		return NULL;
	}

	puts("Connected\n");

	//keep communicating with server
	while (1) {

		//Receive a reply from the server
		server_reply[0] = 0;
		int len = recv(sock, server_reply, 2000, 0);
		if (len < 0) {
			puts("recv failed");
			break;
		}
		printf("%i\n", len);
		tmp = server_reply;

		while (tmp)
			tmp = parseLine(tmp);

	}

	close(sock);
	return NULL;
}

void runtcp() {
#ifndef WIN32
	pthread_t my_mouseThread;
	int ret = pthread_create(&my_mouseThread, NULL, &maintcp, NULL);
	if (ret != 0) {
		printf("Error: pthread_create() failed\n");
	}
#endif
}
