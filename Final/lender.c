#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
char **getCRNames();
void printCRName(char **name);
struct info getInfo();
void requestCR(struct info req);
//void inquiry();
void initTCP(int argc, char *argv[]);
void error_handling(char * msg);

int clnt_sock;

void main(int argc, char *argv[]) {
	int menu = 0;
	char **name;
	struct info req;

	initTCP(argc, argv);
	while (1) {
		menu = getMenu();
		switch (menu) {
		case 1:
			name = getCRNames();
			printCRName(name);
			break;
		case 2:
			req = getInfo(); //강의실 선택하고 신청내용입력?
			requestCR(req); //  requestCR > request로 수정
			break;
		case 3:
			//inquiry();
			break;
		}
	}
}

int getMenu() {
	int n = 0;
	while (1) {
		printf("1. 강의실 정보\n2. 강의실 대여 신청\n3. 문의하기\n");
		scanf("%d", &n);
		getchar();

		if (0 < n && n <= 3)
			return n;
		else
			printf("잘못입력했습니다.");
	}

}

void printCRName(char **name) {
	int i = 0;

	while (name[i]) {
		puts(name[i]);
		i++;
	}
}

struct info getInfo() {
	struct info ret;
	char input[BUFSIZ];
	char **name, *tmp;
	int i, num;

	name = getCRNames();
	while (name[i] != 0) {
		printf("%d %s\n", i, name[i]);
		i++;
	}

	//강의실 번호
	printf("Choice classroom : ");
	scanf("%d", &num);
	getchar();
	tmp = (char*)malloc(strlen(name[num]) + 1);
	strcpy(tmp, name[num]);
	ret.CRName = tmp;

	//학생 이름
	puts("Input the your name ");
	scanf("%s", input);
	getchar();
	tmp = (char*)malloc(strlen(input) + 1);
	strcpy(tmp, input);
	ret.studentName = tmp;

	//학생 번호
	puts("Input the your number ");
	scanf("%s", input);
	getchar();
	tmp = (char*)malloc(strlen(input) + 1);
	strcpy(tmp, input);
	ret.studentNumber = tmp;

	//날짜
	puts("Input date you want ");
	scanf("%[^\n]s", input);
	getchar();
	tmp = (char*)malloc(strlen(input) + 1);
	strcpy(tmp, input);
	ret.date = tmp;

	//용도
	puts("Input the your purpose ");
	scanf("%[^\n]s", input);
	getchar();
	tmp = (char*)malloc(strlen(input) + 1);
	strcpy(tmp, input);
	ret.purpose = tmp;

	return ret;
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

char **getCRNames() {
	int size, buff_len, i, str_len;
	char **CRNames, *tmp, req[] = "aa", buff[BUFSIZ], msg[BUFSIZ];

	CRNames = (char**)malloc(sizeof(char*) * 64);
	memset(CRNames, 0, sizeof(CRNames));
	write(clnt_sock, req, strlen(req));

	i = 0;
	while (1)
	{
		size = 0;
		read(clnt_sock, &size, 1);

		if (size == 0)
			break;

		buff_len = 0;
		while (buff_len < size) {
			str_len = read(clnt_sock, msg, 1);
			buff[buff_len] = msg[0];
			buff_len += str_len;
		}

		buff[buff_len] = 0;
		tmp = (char*)malloc(buff_len + 1);
		strcpy(tmp, buff);
		CRNames[i] = tmp;
		i++;
	}
	CRNames[i] = 0;

	return CRNames;
}

void requestCR(struct info req) {
	int len, i;
	char fl[] = "ab";

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