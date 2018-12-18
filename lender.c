#include<stdio.h.>

struct info {
	char *CRName;
	char *studentName;
	char *studentNumber;
	char *date;
	char *purpose;
};

int getMenu();
char **getCRName();
void printCRName(char **name);
struct info getInfo();
void request(struct info req);
void inquiry();

void main() {
	int menu = 0;

	memu = getMenu();
	switch (menu) {
		case 1:
			char **name = getCRName();
			printCRName(name);
			break;
		case 2:
			struct info req = getInfo();
			requestCR(req);
		case 3:
			inquiry();
	}

}