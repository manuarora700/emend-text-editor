EMEND: A TEXT EDITOR
Emend is a text editor which uses structures, Pointersm File systems and Interacts with the terminal at a very low level. Text editor is very useful for writing and editing codes and long articles. Typically, a text editor provides an "empty" display screen with a settled-line length and visible line numbers. So, user would then be able to fill the lines in with content, line by line. All of the text editor coding is done with converting the terminal into RAW mode from CANONICAL or COOKED mode.

Text Manager provides facilities, for example, spell checking, search and replace, macros, syntax highlighting, and alphabetic sorting.

This editor aims at writing long articles with hassle-free typing. The text editor is designed using C language. C99 is the C language standard that is used which lets us to declare the variables anywhere in the program, Unlike ANSI C standards.


-- Part of my semester minor project.
-- Group:
    -Manu Arora
    -Vansh Madan
    -Yash Saluja
    -Lakshit Kumar Singh

-- Total files included:
    --Makefile
    --emend.c
    --Readme.md
    --Executable emend
    --HTML document for Readme.
PS: EMEND means to edit a piece of text.

TIMELINE
___________________________________________________________________________________________________________________________________________

Date: 25/08/2017

TURNING OFF ECHOING INTO THE TERMINAL:
____________________________
-- Enabled raw mode in terminal to stop ECHOING text in the terminal. Echoing text in the terminal is helpful in Canonical mode but it is not recommended it Raw mode. ECHO causes each keypress to be printed to the terminal so you can see what you're typing. It does the same thing but doesn't print out anything. We can exit Raw mode by typing CTRL+C at the terminal and then typing reset followed by hitting ENTER. This resets the terminal and everything is set back to normal. All the raw mode attributes such as struct termios, tcgetattr(), tcsetattr(), ECHO and TCSAFLUSH all come from <termios.h> header file.

--Terminal attributes are set into termios using tcgetatt(). After modifying, THese can be applies to the terminal using tcsetattr(). TCSAFLUSH argument sets when to apply changes. Here in our case, it waits for all pending outputs to be written to terminal, and also discards any input that hasn't been read.

-- c_lflag field is called "local flags", other flag fields are input flag c_iflag, output flag c_oflag and control flag c_cflag. All of these has to be modified to enable raw mode. There are the flags which are used to enable raw mode into a terminal. ECHO is a bitflag. Bitwise NOT(~) is used on this value. Then bitwise AND is used which forces the fourth bit in the flag field to become zero and causes every other bit to retain its current value. As soon as you press 'q' the program will quit.

DISABLING RAW MODE AT THE EXIT OF THE FUNCTION:

-- To leave the terminal as we found it, we restore the terminal's original attributes when our program exits. This is done by saving the copy of termios struct in its original state.

--atexit() is there in stdlib.h file. It is used to register disableRawMode() function too be called automatically when the program exits() function from main() or using exit(). Leftover inputs are no longer fed into the input shell because TCSAFUSH option is no longer being passed in tcsetattr() when the program exits.

TURNING OFF CANONICAL MODE IN TERMINAL

--ICANON turns off canonical mode, Which allows us to read the text byte by byte instead of line by line. It is a local flag. Program will exit as soon as 'q' is entered.

DISPLAYING THE KEYPRESSES TO SHOW THE ASCII VALUES OF EDITOR

--To get an idea, let's print out each byte by byte character that we enter with their ASCII values.

--iscntrl() is from ctype.h and printf() from stdio.h. iscntrl() tests a character if it is a control character or not. cntrl characters are non-printable characters that we don't want to print. ASCII codes 32-126 are all printable/displayable. in printf() %d puts the ASCII code and %c emits out its character as it is.

--This program(temporarily) shows how various keypresses translate into bytes that we read. Take a look at Escape , Pageup, up, down, left, right etc keys. Arrow keys and pageup, pagedown, home and end are 3 or 4 bytes to the terminal. these are known as escape sequences.
--Backspace is 127 bytes. Delete is 4-byte escape sequence.
--Enter is 10 bytes. Which is a newline character alse called '\n'
--Ctrl-a is 1 byte, ctrl-b is 2 byte, ctrl-c is escape sequence which terminates the program.

--If you type ctrl+s you signal the terminal to stop sendint the output, to set everything back to normal...Hit ctrl+q to tell it to resume sending your output.

--If you press ctrl+z or ctrl+y, the programs seems to be frozen and that is because it goes in the background. To bring it back to the foreground hit 'fg' It may exit as a result of read() returning -1 t indicate error occured. It may happen on windows, but not on linux.

DISABLING CTRL+C AND CTRL+Z SIGNALS:

--Ctrlc sends sigint signal to the current process which causes it to terminate. Ctrl+z sends a sigtstp signal to the current process which causes it to suspend. ISIG turns them both off. ISIG comes from termios.h

DISBLING CTRL+S AND CTRL+Q

-- By default, both are software flow control. ctrl+s stops data from being transmitted to the terminal until you press ctrl+q. We turn off this feature.

--IXON comes from termios.h. I stands for input flag, and XON controls ctrl+s and ctrl+q. XOFF is to pause transmission and XON is to resume transmission.

DISABLING CTRL+V FEATURE:

-- To disable ctrl+v we use IEXTEN flag in raw.c_lflag. It comes from termios.h. It is another flag that starts with I but belongs in the c_lflag field.

FIXING Ctrl+M ISSUE:

-- If we run the program now, You'll see that ctrl+M is weird. We expect it to be read as 10 but it is being read as 13. To fix this we use ICRNL flag which comes from termios.h. I stands for input flag. CR stands for carriage return and NL stands for newline. Now ctrl+M will be read as 13(carriage return) and the same goes for the enter key.

TURNING OFF ALL OUTPUT PROCESSING:

--Terminal translates each newline we print into a carriage return followed by a newline. Carriage return moves the cursor back to the beginning of the current line, and the newline moves the cursor down a line, scrolling the screen if necessary. We turn this feature off all output processing features by turning off the OPOST flag.
--OPOST comes from termios.h > O is for output flag and POST stands for Post-Processing Of Output. Here occurs a problem. If we run the program at this point, the newline is there but it is not shifting to the left side of the terminal. To fix this let's add carruage return to our printf() statements.From now on, we’ll have to write out the full "\r\n" whenever we want to start a new line.

--We turn off some more miscellaneous flags.

A TIMEOUT FOR READ() FUNCTION:

--Read() currently waits indefinitely for input from the keyboard. We can set a timeout so that read() returns if it doesn't get any input for a certain amount of time.

--VMIN and VTIME come from termios.h. They play as an index to the c_cc field which stands for control characters. There is not much effect on BASH on windows. But plays a significant role in Ubuntu linux.

//Date: 19-9-2017
ERROR HANDLING IN THE TEXT EDITOR:

-enableRawMode() gets us into raw mode, Let us clean up the code by adding error handling. FIrst we add die() function that prints error messages.
-in die() perror() comes from stdio.h file. and exit() is from stdlib.h

- Reason for adding perror was that the global variable does not print descriptive text error. Perror prints descriptive error. It also prints string given to it before it prints error message.

-We the exit the program with exit(1) status which indcates failure. exit(0) indicates success.

-errno and EAGAIN come from <errno.h>.
tcsetattr(), tcgetattr(), and read() all return -1 on failure, and set the errno value to indicate the error.

An easy way to make tcgetattr() fail is to give your program a text file or a pipe as the standard input instead of your terminal. To give it a file as standard input, run ./emend <emend.c. To give it a pipe, run echo test | ./emend. Both should result in the same error from tcgetattr(), something like Inappropriate ioctl for device.

DATE: 26/9/2017
SECTIONS:

We split our code into sections and the handling of RAW mode is done till now.

NExt, we’ll do some more low-level terminal input/output handling, and use that to draw to the screen and allow the user to move the cursor around.

============================================================================================================================

Date: 4/10/2017
--Previously we used 'Ctrl' key combined with alphabetic keys seemed to map to bytes from 1-26. We can also use the same key combinations and perform different operations. We start by mappign Ctrl + Q to Quit the program instead of just pressing q key.

--Secondly, We make a function for low-level keypress reading and other functions for mapping keypresses to editor operations. We will also stop printing out keypresses now. We create a function for that.

-- We added two functions editorReadKey() and editorProcessKeypresses(). editorReadKey() waits for oen keypress and returns it later.,editorProcessKeypress() waits for a keypress, and then handles it. Later, it will map various Ctrl key combinations and other special keys to different editor functions, and insert any alphanumeric and other printable keys’ characters into the text that is being edited.

--Clearing the screen:
We’re going to render the editor’s user interface to the screen after each keypress. Let’s start by just clearing the screen.

write() and STDOUT_FILENO come from <unistd.h>.

We are writing an escape sequence to the terminal. Escape sequences always start with an escape character (27) followed by a [ character. Escape sequences instruct the terminal to do various text formatting tasks, such as coloring text, moving the cursor around, and clearing parts of the screen.
We are using the J command (Erase In Display) to clear the screen. Escape sequence commands take arguments, which come before the command. In this case the argument is 2, which says to clear the entire screen. <esc>[1J would clear the screen up to where the cursor is, and <esc>[0J would clear the screen from the cursor up to the end of the screen. Also, 0 is the default argument for J, so just <esc>[J by itself would also clear the screen from the cursor to the end.
For our text editor, we will be mostly using VT100 escape sequences, which are supported very widely by modern terminal emulators. See the VT100 User Guide for complete documentation of each escape sequence.
If we wanted to support the maximum number of terminals out there, we could use the ncurses library, which uses the terminfo database to figure out the capabilities of a terminal and what escape sequences to use for that particular terminal.

DATE: 2nd November 2017

REPOSITIONING THE CURSOR:
--<esc>[2J Command left the cursor at the bottom of the screen. Let's reposition it at the top left corner so that we're ready to draw the editor Interface from top to bottom.

--<esc>[H is only 3 bytes long and uses the command H to position the cursor.


-- We now clear the screen and reposition the cursor when our program exits. If an error occurs in the middle, We don't want a bunch of garbage left on the screen and we dont want the error to be printer wherever the cursor happens to be at that point.

-- We have two exit points we want to clear the screen at: die(), and when the user presses Ctrl-Q to quit.
We could use atexit() to clear the screen when our program exits, but then the error message printed by die() would get erased right after printing it.

TILDES:
-- Just like tildes in VIM text editor, We are going to use tildes in our text editor. we’ll draw a tilde at the beginning of any lines that come after the end of the file being edited. It is taken from the book PRACTICAL VIM.

Steve Oualline wrote the first book completely dedicated to Vim. It's written for Vim 5.7, but it is still a useful introduction to any modern version of Vim. The book is also available as a PDF

--For tildes, EDITORDRAWROWS() will habdle drawing of each row of the buffer of text being edited. For now it draws a tilde in each row, which means that row is not a part of the file and cant contain any text. Since we don't know the size of the terminal just yet, we dont know how many rows to draw. For now we just draw 24 rows.
After were done drawing, we do another <esc>[H Excape sequence to reposition the cursor back to the top left corner.

Our global variable containing our editor state is named E. We must then replace all ocurrences of orig_termios to E.orig_termios.

EASY WAY TO SIZE THE WINDOW:
On most systems, you should be able to get the size of the terminal by simply calling ioctl() with the TIOCGWINSZ request. (As far as I can tell, it stands for Terminal IOCtl (which itself stands for Input/Output Control) Get WINdow SiZe.)
--ioctl(), TIOCGWINSZ, and struct winsize come from <sys/ioctl.h>.
On success, ioctl() will place the number of columns wide and the number of rows high the terminal is into the given winsize struct. On failure, ioctl() returns -1. We also check to make sure the values it gave back weren’t 0, because apparently that’s a possible erroneous outcome. If ioctl() failed in either way, we have getWindowSize() report failure by returning -1. If it succeeded, we pass the values back by setting the int references that were passed to the function. (This is a common approach to having functions return multiple values in C. It also allows you to use the return value to indicate success or failure.)
Now let’s add screenrows and screencols to our global editor state, and call getWindowSize() to fill in those values.

--initEditor()’s job will be to initialize all the fields in the E struct.
This approach displays the correct number of tildes. THere's also a hard approach but let;s not get into that.

Append buffer
It’s not a good idea to make a whole bunch of small write()’s every time we refresh the screen. It would be better to do one big write(), to make sure the whole screen updates at once. Otherwise there could be small unpredictable pauses between write()’s, which would cause an annoying flicker effect.
We want to replace all our write() calls with code that appends the string to a buffer, and then write() this buffer out at the end. Unfortunately, C doesn’t have dynamic strings, so we’ll create our own dynamic string type that supports one operation: appending.
Let’s start by making a new /*** append buffer / section, and defining the abuf struct under it.
--An append buffer consists of a pointer to our buffer in memory, and a length. We define an ABUF_INIT constant which represents an empty buffer. This acts as a constructor for our abuf type.
Next, let’s define the abAppend() operation, as well as the abFree() destructor.

--realloc() and free() come from <stdlib.h>. memcpy() comes from <string.h>.
To append a string s to an abuf, the first thing we do is make sure we allocate enough memory to hold the new string. We ask realloc() to give us a block of memory that is the size of the current string plus the size of the string we are appending. realloc() will either extend the size of the block of memory we already have allocated, or it will take care of free()ing the current block of memory and allocating a new block of memory somewhere else that is big enough for our new string.
Then we use memcpy() to copy the string s after the end of the current data in the buffer, and we update the pointer and length of the abuf to the new values.
abFree() is a destructor that deallocates the dynamic memory used by an abuf.
Okay, our abuf type is ready to be put to use.


--In editorRefreshScreen(), we first initialize a new abuf called ab, by assigning ABUF_INIT to it. We then replace each occurrence of write(STDOUT_FILENO, ...) with abAppend(&ab, ...). We also pass ab into editorDrawRows(), so it too can use abAppend(). Lastly, we write() the buffer’s contents out to standard output, and free the memory used by the abuf.


--We use escape sequences to tell the terminal to hide and show the cursor. The h and l commands (Set Mode, Reset Mode) are used to turn on and turn off various terminal features or “modes”. The VT100 User Guide just linked to doesn’t document argument ?25 which we use above. It appears the cursor hiding/showing feature appeared in later VT models. So some terminals might not support hiding/showing the cursor, but if they don’t, then they will just ignore those escape sequences, which isn’t a big deal in this case.


--snprintf() comes from <stdio.h>.
We use the welcome buffer and snprintf() to interpolate our KILO_VERSION string into the welcome message. We also truncate the length of the string in case the terminal is too tiny to fit our welcome message.
Now let’s center it.

--By setting the first constant in the enum to 1000, the rest of the constants get incrementing values of 1001, 1002, 1003, and so on.
That concludes our arrow key handling code. At this point, it can be fun to try entering an escape sequence manually while the program runs. Try pressing the Escape key, the [ key, and Shift+C in sequence really fast, and you may see your keypresses being interpreted as the right arrow key being pressed. You have to be pretty fast to do it, so you may want to adjust the VTIME value in enableRawMode() temporarily, to make it easier. (It also helps to know that pressing Ctrl-[ is the same as pressing the Escape key, for the same reason that Ctrl-M is the same as pressing Enter: Ctrl clears the 6th and 7th bits of the character you type in combination with it.)
