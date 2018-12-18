#include<stdio.h.>

struct info {
	char *CRName;
	char *studentName;
	char *studentNumber;
	char *date;
	char *purpose;
};

struct info *getRequest();
void printRequest(struct info* infos);
struct info getRequest();
void adminRequest(struct info req);
void inquiry();

void main() {

	int menu = 0;

	memu = getMenu();
	switch (menu) {
	case 1:
		struct info *infos = getRequest();
		printRequest(infos);
		break;
	case 2:
		struct info req = getRequest();
		adminRequest(info);
	case 3:
		inquiry();
	}

}