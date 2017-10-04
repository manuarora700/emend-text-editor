// INCLUDES
#include<ctype.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>

// DEFINES

#define CTRL_KEY(K) ((K) & 0x1f) //Mapping with Q to perform Ctrl + Q Quit operation
// DATA
struct termios orig_termios;

// TERMINAL
void die(const char *s)
{
  perror(s);
  exit(1);
}
void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    {
      die("tcsetattr");
    }
}

void enableRawMode()
{
    // struct termios, tcgetattr(), tcsetattr(), ECHO, and TCSAFLUSH all come from <termios.h>.
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    {
      die("tcgetattr"); // die tcgetattr
    }
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8); //It is not a flag but a bitmask
    raw.c_lflag &= !(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME]= 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
      die("tcsetattr"); //Die tcsetattr()
    }
}

// INIT
int main()
{
    enableRawMode(); //Enables raw mode
    // read() and STDIN_FILENO part of <unistd.h>
    while (1)
    {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
        {
          die("read");
        }
        if(iscntrl(c))
        {
            printf("%d\r\n", c);
        }
        else
        {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == CTRL_KEY('q'))// previously if(c == 'q')
        {
            break;
        }
    }
  return 0;
}
