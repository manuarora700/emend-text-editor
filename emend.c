/*** INCLUDES ***/
#include<ctype.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<termios.h>

/*** DEFINES ***/
#define CTRL_KEY(K) ((K) & 0x1f) //Mapping with Q to perform Ctrl + Q Quit operation

/*** DATA ***/
struct editorConfig {
  struct termios orig_termios;
};
struct editorConfig E;

/*** TERMINAL ***/
void die(const char *s)
{
  write(STDOUT_FILENO, "\x1b[2J", 4); //Clearing the screen
  write(STDOUT_FILENO, "\x1b[H", 3); // Clearing the screen
  perror(s);
  exit(1);
}
void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    {
      die("tcsetattr");
    }
}

void enableRawMode()
{
    // struct termios, tcgetattr(), tcsetattr(), ECHO, and TCSAFLUSH all come from <termios.h>.
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
    {
      die("tcgetattr"); // die tcgetattr
    }
    atexit(disableRawMode);

    struct termios raw = E.orig_termios;
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

char editorReadKey()
{
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  return c;
}

/*** OUTPUT ***/

void editorDrawRows()
{
  int y;
  for (y = 0; y < 24; y++) {
    write(STDOUT_FILENO, "~\r\n", 3);
  }
}
void editorRefreshScreen()
{
  write(STDOUT_FILENO, "\x1b[2J", 4); // Escape sequence
  write(STDOUT_FILENO, "\x1b[H", 3); // Escape sequence

  editorDrawRows();
  write(STDOUT_FILENO, "\x1b[H", 3);
}

/*** INPUT ***/
void editorProcessKeypress()
{
  char c = editorReadKey();
  switch (c)
  {
    case CTRL_KEY('q'):
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
  }
}
/*** INIT ***/
int main()
{
    enableRawMode(); //Enables raw mode
    // read() and STDIN_FILENO part of <unistd.h>
    while (1)
    {
      editorRefreshScreen();
      editorProcessKeypress();
    }
  return 0;
}
