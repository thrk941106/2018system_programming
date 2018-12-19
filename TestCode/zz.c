#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void * handle_clnt(void * arg);
void send_msg(char * msg, int len);
void error_handling(char * msg);

struct info {
	char *CRName;
	char *studentName;
	char *studentNumber;
	char *date;
	char *purpose;
};

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
char *CRNames[BUF_SIZE];
struct info infos[BUF_SIZE];
pthread_mutex_t mutx, mCR, mInfo;

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz, i;
	char *tmp;
	pthread_t t_id;
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	{
		memset(CRNames, 0, sizeof(CRNames));
		for (i = 0; i < 10; i++) {
			tmp = (char*)malloc(64);
			sprintf(tmp, "lecture %d", 300 + i);
			CRNames[i] = tmp;
		}
		memset(infos, 0, sizeof(infos));
	}

	pthread_mutex_init(&mutx, NULL);
	pthread_mutex_init(&mCR, NULL);
	pthread_mutex_init(&mInfo, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while (1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutx);

		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}

void * handle_clnt(void * arg)
{
	int clnt_sock = *((int*)arg);
	int str_len = 0, i, buff_len = 0;
	char msg[BUF_SIZE], buff[BUFSIZ];
	int size = 0;
	struct info *info_ptr;

	while (buff_len < 2) {
		str_len = read(clnt_sock, msg, 1);
		buff[buff_len] = msg[0];
		buff_len += str_len;
	}

	buff[buff_len] = 0;
	puts(buff);

	switch (buff[0])
	{
	case 'a':
		if (buff[1] == 'a') {
			i = 0;
			while (CRNames[i] != 0) {
				size = strlen(CRNames);
				write(clnt_sock, &size, 1);
				write(clnt_sock, CRNames[i], strlen(CRNames));
			}
			size = 0;
			send_msg(&size, 1);
		}
		else if (buff[1] == 'b') {
			i = 0;
			while (infos[i++].CRName != 0);

			info_ptr = (struct info*)malloc(sizeof(struct info));

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

			puts("name");
			puts(info_ptr->CRName);

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

			puts(info_ptr->CRName);
			puts(info_ptr->studentName);
		}

	case 'b':
		if (buff[1] == 'a') {

		}
		else if (buff[1] == 'b') {

		}
	}

	//send_msg(msg, str_len);

	pthread_mutex_lock(&mutx);
	for (i = 0; i<clnt_cnt; i++)   // remove disconnected client
	{
		if (clnt_sock == clnt_socks[i])
		{
			while (i < clnt_cnt - 1) {
				clnt_socks[i] = clnt_socks[i + 1];
				i++;
			}
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}
void send_msg(char * msg, int len)   // send to all
{
	int i;
	pthread_mutex_lock(&mutx);
	for (i = 0; i<clnt_cnt; i++)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}
void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}