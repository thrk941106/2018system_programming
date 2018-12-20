#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include "setTTYoption.h"
/* set ttymode, -icanon, -echo
   reset ttymode   */

static struct termios original_mode;


void set_tty_cr_mode()
/*set tty no canonical, no echo*/
{
   
    struct termios ttystate;
  
    /* turn off canonical mode & echo mode */

    tcgetattr(0,&ttystate);
    ttystate.c_lflag &= ~ICANON; // no bufferig
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&ttystate); //apply
}

void set_tty_nonecho_mode()
{
    struct termios ttystate;

    tcgetattr(0,&ttystate);
    ttystate.c_lflag &= ~ECHO; // no echo
    tcsetattr(0,TCSANOW,&ttystate); //apply
}

void tty_mode(int how)
/* 0 :backup the original setting for reset */
/* 1 :reset tty , before changed*/
{
	if (how ==0) tcgetattr(0,&original_mode);
	else tcsetattr(0,TCSANOW,&original_mode);
}


