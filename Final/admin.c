#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

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
//void inquiry();

int clnt_sock;

void main(int argc, char* argv[]) {

	int menu;
	struct info** infos = NULL;
	struct info req;

	initTCP(argc, argv);



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
			//			case 3:
			//				inquiry();
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

	printf("1. Current Request\n");
	printf("2. Admit request\n");
	printf("3. Answer to inquiry\n");

	scanf("%d", &select);

	return select;
}

void printRequest(struct info **infos)
{

	int i = 0;


	printf("Classname     date     purpose     studentName     studentNumber\n");
	while (infos[i]) {

		printf("%8d %8s %8s %8s %8s %8s", i + 1, infos[i]->CRName, infos[i]->date, infos[i]->purpose, infos[i]->studentName, infos[i]->studentNumber);

		i++;
	}

}

struct info getRequest(struct info** infos)
{
	int select;

	printf("select the ClassRoom : ");
	scanf("%d", &select);

	return *(infos[select - 1]);
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
		puts(info_ptr->CRName);

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
		puts(info_ptr->studentName);

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