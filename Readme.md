EMEND: A TEXT EDITOR

Emend is a text editor which uses structures, Pointersm File systems and Interacts with the terminal at a very low level. Text editor is very useful for writing and editing codes and long articles. Typically, a text editor provides an "empty" display screen with a settled-line length and visible line numbers. So, user would then be able to fill the lines in with content, line by line. All of the text editor coding is done with converting the terminal into RAW mode from CANONICAL or COOKED mode.

Text Manager provides facilities, for example, spell checking, search and replace, macros, syntax highlighting, and alphabetic sorting.

This editor aims at writing long articles with hassle-free typing. The text editor is designed using C language. C99 is the C language standard that is used which lets us to declare the variables anywhere in the program, Unlike ANSI C standards.


-- Part of my semester minor project.
-- Group:
    -Manu
    -Vansh
    -Yash
    -Lakshit

-- Total files:
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
