#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "setTTYoption.h"

void set_SIGINT_handle()
{
    void sig_inthandler(int);
	tty_mode(0);
    signal(SIGINT, sig_inthandler);
}

void sig_inthandler(int s)
{
	int i=1;
	set_tty_cr_mode();
	set_tty_nonecho_mode();
    printf("\n종료하시겠습니까? (y/n)");
    
  
     // 시스템 입력 버퍼 처리
while(i){
    switch (getchar())
    {
	case 'y' :       
	case 'Y' :    tty_mode(1);  printf("\n이용해주셔서 감사합니다.\n"); exit(EXIT_SUCCESS);
        case 'n' :    
	case 'N' :	i=0; puts("\n메뉴화면으로 돌아가기 : enter"); break;
	case EOF : exit(1);
    }    
}           
	        
}


