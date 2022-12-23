Name: Mrudini Patel
Student ID: 1141712
Date: Mar 15, 2022
Course: CIS*3110 - Assignment #2

Compilation & Execution commands:

Type the following commands inside Terminal or an equivalent software to compile and
execute the code. Please ensure you are inside of the working directory:

make
./simcpu [-d] [-v] [-r] [quantum_number] < input_file

Please note that the flags listed after "./simcpu" above, are optional and can be run
according to user's choice.

Known Limitations & Future Improvements:

Although I tried my best to incorporate the priority queue concept discussed in the A2 details
slideshow, there are still many things that I have not incorporated:

A lot of my code is redundant and is squished inside a single main function, so in the future 
I would like to add functions to reduce redundancy and make my code more legible. For example,
everytime I add to the list/queue I am repeating the same few lines of code.

I have made many assumptions, first being that the input only passes in those three flags in the
order listed above. I did not error check the flags.

I also do not account for the thread switch overhead time, which is something I would like to fix
moving forwards. I also do not know if I have understood the verbose component well, and so my printfs
when the -v flag is passed in are all over the place.

All references to resources are cited inside my simcpu.c code file.



