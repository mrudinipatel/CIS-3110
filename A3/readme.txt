Name: Mrudini Patel
Student ID: 1141712
Date: April 4, 2022
Course: CIS*3110 - Assignment #3

Compilation & Execution commands:

Type the following commands inside Terminal or an equivalent software to compile and
execute the code. Please ensure you are inside of the working directory:

"make"
"./holes [input_file] [strategy_type]"

My program takes in two command line arguments: first is the file name and the second is the type 
of allocation strategy. It is imperative that the user enters these in this specific order, otherwise
the program will stop executing (a message will be printed to try again).

The 4 types of possible allocation strategies, the user can input into the program include:
"best", "worst", "next", and "first".

Known Limitations & Future Improvements:

The program is case sensitive. Allocation strategies entered in command line argument must be all lowercase.

As mentioned above, order of user input matters (it must be in the order specified in in the command
instructions above and the pdf).

The user must ensure that all processes in the input file are between 1 and 512 bytes (inclusive). My program does not check
if processes obey this range, and my program runs as intended, regardless of process size. 

I am not sure why exactly, but my output for the 'cumulative &mem' differs by 1% for .txt file example provided 
in the A3Hints.pdf slides. However, since the professor mentioned that a minor discrepancy for this value is okay 
to have, I did not look into it further. I assume it has to do with they way I rounded my percentage, but I am not too sure.
In the future, I would like to look into this matter further.

In my program, I am calling on void functions to perform the user-entered allocation strategy. Since at any given time 
my program performs a single allocation strategy before exiting the program, I decided to use global variables to store
the processes into memory. After heavily testing my code, I conclude that even though my code works as intended across a 
variety of .txt files and strategies, the global variable is consuming memory even if it is not being currently used. This 
seems to be a minor issue, but it may be worthwhile trying to pass in these variables into each function parameter, 
in the future.

Data Structures:

In my program I am implementing a queue data structure for the processes and a global array variable
for the memory. I have comments detailing this data structure within my holes.c file as well.

All references to resources are cited inside my holes.c code file.











