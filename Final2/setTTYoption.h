#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>


void set_SIGINT_handle();
void sig_inthandler(int);

void set_tty_cr_mode();
void set_tty_nonecho_mode();
void tty_mode(int);
