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