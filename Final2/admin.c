#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "setTTYoption.h"

#define BUF_SIZE 100
#define NAME_SIZE 20
#define STARTCHAT "bc"

struct info {
	char *CRName;
	char *studentName;
	char *studentNumber;
	char *date;
	char *purpose;
};


int getMenu();
struct info **getRequests();
void printRequest(struct info** infos);
struct info getRequest(struct info** infos);
void adminRequest(struct info req);
void initTCP(int argc, char *argv[]);
void error_handling(char * msg);
void inquiry();
void * send_msg(void * arg);
void * recv_msg(void * arg);

int clnt_sock;
char name[] = "Admin";

void main(int argc, char* argv[]) {

	int menu;
	struct info** infos = NULL;
	struct info req;

	initTCP(argc, argv);

	tty_mode(0);
	set_SIGINT_handle();
	/*
	infos = (struct info**)calloc(5, sizeof(struct info*));

	for (int i = 0; i < 4; i++) {
	infos[i] = (struct info*)malloc(sizeof(struct info));
	infos[i]->CRName = "systemprogramming1  ";
	infos[i]->date = "13/13  ";
	infos[i]->purpose = "study  ";
	infos[i]->studentName = "fuckingman  ";
	infos[i]->studentNumber = "4  \n";
	}
	infos[5] = NULL;

	printf("%8d %8s %8s %8s %8s %8s", 2, infos[2]->CRName, infos[2]->date, infos[2]->purpose, infos[2]->studentName, infos[2]->studentNumber);
	*/

	while (1) {

		menu = getMenu();

		switch (menu) {
		case 1:
			infos = getRequests();
			printRequest(infos);
			break;
		case 2:
			req = getRequest(infos);
			adminRequest(req);
			break;
		case 3:
			inquiry();
		}
	}

}

void initTCP(int argc, char *argv[]) {
	struct sockaddr_in serv_addr;

	if (argc != 3) {
		printf("Usage : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}

	clnt_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");

}



int getMenu()
{
	int select;

	set_tty_cr_mode();
	set_tty_nonecho_mode();

	printf("1. 요청 내역\n");
	printf("2. 요청 승인\n");
	printf("3. 문의 응답\n");
	select = getchar();
	tty_mode(1);

	return select - '0';
}

void printRequest(struct info **infos)
{
	int i = 0;

	printf("순번     강의실		이용시간    목적	이름	학번\n");
	while (infos[i]) {

		printf("%4d	 %s	%s	    %s	%s	%s\n", i + 1, infos[i]->CRName, infos[i]->date, infos[i]->purpose, infos[i]->studentName, infos[i]->studentNumber);

		i++;
	}

}

struct info getRequest(struct info** infos)
{
	int select;
	tty_mode(0);
	set_tty_cr_mode();
	printf("승인할 요청을 선택하세요. : ");
	select = getchar();
	puts("\n");
	tty_mode(1);

	return *(infos[select - '1']);
}

struct info **getRequests() {
	struct info ** infos, *info_ptr;
	int size, buff_len, str_len, i;
	char msg[BUFSIZ], buff[BUFSIZ], req[] = "ba";

	infos = (struct info**)malloc(sizeof(struct info*) * 64);
	memset(infos, 0, sizeof(infos));

	write(clnt_sock, req, strlen(req));

	while (1) {
		size = 0;
		read(clnt_sock, &size, 1);

		if (size == 0)
			break;

		info_ptr = (struct info*)malloc(sizeof(struct info));

		//CRName
		size = 0;
		read(clnt_sock, &size, 1);
		info_ptr->CRName = (char*)malloc(size + 1);
		buff_len = 0;
		while (buff_len < size) {
			str_len = read(clnt_sock, msg, 1);
			buff[buff_len] = msg[0];
			buff_len += str_len;
		}

		buff[buff_len] = 0;
		strcpy(info_ptr->CRName, buff);

		//studentName
		size = 0;
		read(clnt_sock, &size, 1);
		info_ptr->studentName = (char*)malloc(size + 1);
		buff_len = 0;
		while (buff_len < size) {
			str_len = read(clnt_sock, msg, 1);
			buff[buff_len] = msg[0];
			buff_len += str_len;
		}

		buff[buff_len] = 0;
		strcpy(info_ptr->studentName, buff);

		//studentNumber
		size = 0;
		read(clnt_sock, &size, 1);
		info_ptr->studentNumber = (char*)malloc(size + 1);
		buff_len = 0;
		while (buff_len < size) {
			str_len = read(clnt_sock, msg, 1);
			buff[buff_len] = msg[0];
			buff_len += str_len;
		}

		buff[buff_len] = 0;
		strcpy(info_ptr->studentNumber, buff);

		//date
		size = 0;
		read(clnt_sock, &size, 1);
		info_ptr->date = (char*)malloc(size + 1);
		buff_len = 0;
		while (buff_len < size) {
			str_len = read(clnt_sock, msg, 1);
			buff[buff_len] = msg[0];
			buff_len += str_len;
		}

		buff[buff_len] = 0;
		strcpy(info_ptr->date, buff);

		//purpose
		size = 0;
		read(clnt_sock, &size, 1);
		info_ptr->purpose = (char*)malloc(size + 1);
		buff_len = 0;
		while (buff_len < size) {
			str_len = read(clnt_sock, msg, 1);
			buff[buff_len] = msg[0];
			buff_len += str_len;
		}

		buff[buff_len] = 0;
		strcpy(info_ptr->purpose, buff);

		i = 0;
		while (infos[i]) {
			i++;
		}

		infos[i] = info_ptr;
	}

	return infos;
}

void adminRequest(struct info req) {
	int len, i; char fl[] = "bb";

	write(clnt_sock, fl, strlen(fl));

	len = strlen(req.CRName);
	write(clnt_sock, &len, 1);
	write(clnt_sock, req.CRName, strlen(req.CRName));

	len = strlen(req.studentName);
	write(clnt_sock, &len, 1);
	write(clnt_sock, req.studentName, strlen(req.studentName));

	len = strlen(req.studentNumber);
	write(clnt_sock, &len, 1);
	write(clnt_sock, req.studentNumber, strlen(req.studentNumber));

	len = strlen(req.date);
	write(clnt_sock, &len, 1);
	write(clnt_sock, req.date, strlen(req.date));

	len = strlen(req.purpose);
	write(clnt_sock, &len, 1);
	write(clnt_sock, req.purpose, strlen(req.purpose));

}

void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void inquiry() {
	pthread_t snd_thread, rcv_thread;
	void * thread_return;
	char* fl = STARTCHAT;

	puts("채팅을 시작합니다.");

	write(clnt_sock, fl, strlen(fl));

	pthread_create(&snd_thread, NULL, send_msg, (void*)&clnt_sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&clnt_sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	close(clnt_sock);

}


void * send_msg(void * arg)   // send thread main
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE], msg[BUFSIZ];

	while (1)
	{
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			close(sock);
			return NULL;
		}
		sprintf(name_msg, "[%8s]  %s", name, msg);
		write(sock, name_msg, strlen(name_msg));
	}
	return NULL;
}

void * recv_msg(void * arg)   // read thread main
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	int str_len;
	while (1)
	{
		str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);
		if (str_len == -1)
			return (void*)-1;
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
	}
	return NULL;
}