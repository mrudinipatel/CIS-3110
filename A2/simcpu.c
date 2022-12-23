#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * Name: Mrudini Patel (1141712)
 * Please refer to readme.txt file for compilation and execution instructions.
*/

typedef struct{
    int pNum;
    int entry;
    int num;
    
    int bursts;
    int value;

    int startTime;
    int ioTime;
    int runTime;
    int * cpuTime;
    int * ioTimer;
} Data;

typedef struct{
	int importance;
	Data data;
} List;

void printOutput(float turnAroundTime, int counter, int runningTime, int curr);

void printOutput(float turnAroundTime, int counter, int runningTime, int curr){
    turnAroundTime = turnAroundTime / counter;

    int dec = runningTime * 100;

    int percentage = dec / curr;

    printf("Total Time required is %d time units\n", curr);
    printf("Average Turnaround Time is %.1f time units\n", turnAroundTime);
    printf("CPU Utilization is %d%%\n", percentage);
}

int main (int argc, char * argv[]){
    bool fcfs = true, roundRobin = false, details = false, verbose = false; //fcfs is set to true, because fcfs scheduling will occur in all other cases (except when -r flag is passed in)

    int quantum = 0, processes, numProcesses = 0, processSwitch, threadSwitch;
    int info, numThreads = 0, entry, bursts, scrap;
    int file = 0, counter = 0;
    int oldThread = -1, oldProcess = -1, difference = 0;
    int bufferTime = 0, curr = 0, runningTime = 0;

    float turnAroundTime = 0;
    List * list = malloc(sizeof(List) * 500);
    char * detail;
    char str[1000];

	for(int i = 1; i < argc; i++){ //seeing which flags are being ran
		if(strcmp(argv[i], "-d") == 0){
			details = true;
            detail = malloc(sizeof(char) * 1000000);
		}
		else if(strcmp(argv[i], "-v") == 0){
			verbose = true;
		}
        else if(strcmp(argv[i], "-r") == 0){
			fcfs = false;
			roundRobin = true;
			quantum = atoi(argv[i+1]);
            printf("Round Robin Scheduling (quantum = %d time units)\n", quantum);
		}
	}

    if (fcfs == true){
    	printf("FCFS Scheduling\n");
    }

    scanf("%d %d %d", &numProcesses, &threadSwitch, &processSwitch); //scan in number of processes, thread switch, and process switch input values

    for(int i = 0; i < numProcesses; i++){
        scanf("%d %d", &processes, &info); //scan in the processes and other info from file

        for(int j = 0; j < info; j++){
            Data data;
            scanf("%d %d %d", &numThreads, &entry, &bursts);
            data.ioTimer = malloc(sizeof(int) * bursts);
            data.cpuTime = malloc(sizeof(int) * bursts);

            //populating struct elements using scanned in information above
            data.pNum = processes;
            data.entry = entry;
            data.num = numThreads;

            data.bursts = bursts;
            data.value = 0;

            data.startTime = entry;
            data.runTime = 0;
            data.ioTime = 0;

            for(int l = 0; l < bursts; l++){
                scanf("%d %d", &scrap, &data.cpuTime[l]);

                if((bursts - 1) <= l){
                    data.ioTimer[l] = 0;
                    continue;
                }
                else{
                    scanf("%d", &data.ioTimer[l]);
                }
            }

            // adding item to list
            (list)[*&file].importance = entry;

            (list)[(*&file)++].data = data;

            int pos = *&file - 1;

            while (1){
                int location = pos - 1;
                int parent = location / 2;

                if((list)[parent].importance <= (list)[pos].importance){
                    break;
                }

                List buff = (list)[pos];

                (list)[pos] = (list)[parent];

                (list)[parent] = buff;

                pos = parent;

                if(pos == 0){
                    break;
                }
            }

            counter++; //incrementing counter that counts number of processes (used at the end)
        }
    }

    /*
     * Title: CPU Scheduling Simulator in C
     * Author: amiller3515
     * Date Accessed: March 8, 2022
     * URL Link: https://codereview.stackexchange.com/questions/251777/cpu-scheduling-simulator-in-c 
     * The code inside the while loop below is a modified version of the code found in the link above.
    */

    while (file != 0){
        //deleting item from list
        int max = 0, pos = 0;

        Data delete = (list)[0].data;

        (list)[0] = (list)[--(*&file)];

        while (1){
            int child = (2 * pos) + 1;
            int children = (2 * pos) + 2;

            if((*&file) <= child){
                break;
            }
            else{
                if(*&file <= children){
                    max = child;
                }
                else{
                    max = children;
                }

                if((list)[pos].importance <= (list)[max].importance){
                    break;
                }

                List buff = (list)[pos];

                (list)[pos] = (list)[max];

                (list)[max] = buff;

                pos = max;
            }
        }

        Data deleted = delete;

        if(verbose == true && (deleted.startTime == deleted.entry)){
            printf("At time %d: Thread %d of Process %d moves from new to ready\n", curr, deleted.num, deleted.pNum);
        }

        if (oldProcess != -1){
            if (oldThread != -1){
                if (deleted.pNum == oldProcess){
                    if (deleted.num != oldThread){
                        bufferTime = bufferTime + threadSwitch;
                        curr = curr + threadSwitch;
                    }
                }

                if (deleted.pNum != oldProcess){
                    if (deleted.num == oldThread){
                        curr = curr + processSwitch;
                        bufferTime = bufferTime + processSwitch;
                    }
                }
            }
        }

        if (curr < deleted.entry){
            difference = deleted.entry - curr;
            bufferTime = bufferTime + difference;
        	curr = curr + difference;
        }
        else{
        	deleted.entry = deleted.entry + (curr - deleted.entry);

            if(verbose == true){
                printf("At time %d: Thread %d of Process %d moves from ready to running\n", curr, deleted.num, deleted.pNum);
            }

            if (fcfs == true){ // fcfs scheduling implementation/calculation
                deleted.entry = deleted.entry + deleted.cpuTime[deleted.value];
                deleted.runTime = deleted.runTime + deleted.cpuTime[deleted.value];
                runningTime = runningTime + deleted.cpuTime[deleted.value];
                curr = curr + deleted.cpuTime[deleted.value];
            }

            if (roundRobin == true && (quantum < deleted.cpuTime[deleted.value])){ // round-robin scheduling implementation/calculation
                curr = curr + quantum;
                runningTime = runningTime + quantum;

                deleted.entry = deleted.entry + quantum;
                deleted.runTime = deleted.runTime + quantum;
                deleted.cpuTime[deleted.value] = deleted.cpuTime[deleted.value] - quantum;

                oldProcess = deleted.pNum;
                oldThread = deleted.num;

                //adding item to list
                (list)[*&file].importance = deleted.entry;

                (list)[(*&file)++].data = deleted;

                int pos = *&file - 1;

                while (1){
                    int location = pos - 1;
                    int parent = location / 2;

                    if((list)[parent].importance <= (list)[pos].importance){
                        break;
                    }

                    List buff = (list)[pos];

                    (list)[pos] = (list)[parent];

                    (list)[parent] = buff;

                    pos = parent;

                    if(pos == 0){
                        break;
                    }
                }

                continue;

                if(verbose == true){
                    printf("At time %d: Thread %d of Process %d moves from running to ready\n", deleted.entry, deleted.num, deleted.pNum);
                }
            }

            if (roundRobin == true){
                deleted.runTime = deleted.runTime + deleted.cpuTime[deleted.value];
                deleted.entry = deleted.entry + deleted.cpuTime[deleted.value];

                curr = curr + deleted.cpuTime[deleted.value];
                runningTime = runningTime + deleted.cpuTime[deleted.value];
            }

            if(deleted.value <= (deleted.bursts - 1)){ //if not the last burst
                if(verbose == true){
                    printf("At time %d: Thread %d of Process %d moves from running to blocked\n", deleted.entry, deleted.num, deleted.pNum);
                }

                deleted.entry = deleted.entry + deleted.ioTimer[deleted.value];

                if(verbose == true){
                    printf("At time %d: Thread %d of Process %d moves from blocked to ready\n", deleted.entry, deleted.num, deleted.pNum);
                }

                deleted.ioTime = deleted.ioTime + deleted.ioTimer[deleted.value];

                deleted.value++;
            }

            if(deleted.bursts == deleted.value){ //if the last burst
                turnAroundTime = turnAroundTime + (deleted.entry - deleted.startTime);

                if(verbose == true){
                    printf("At time %d: Thread %d of Process %d moves from running to terminated\n", deleted.entry, deleted.num, deleted.pNum);
                }

                if(details == true){
                    int taTime = deleted.entry - deleted.startTime;

                    sprintf(str, "Thread %d of Process %d:\n\tarrival time: %d\n\tservice time: %d units, I/O time: %d units, turnaround time: %d units, finish time: %d units\n", deleted.num, deleted.pNum, deleted.startTime, deleted.runTime, deleted.ioTime, taTime, deleted.entry);

                    int length = strlen(str);

                    strncat(detail, str, length); //since we need to print this after the -v flag outputs inside this loop, I am storing it inside a char * and printing it outside this loop
                }

                free(deleted.ioTimer);
                free(deleted.cpuTime);
                continue;
            }
        }

        oldProcess = deleted.pNum;
        oldThread = deleted.num;

        //adding item to list
        (list)[*&file].importance = deleted.entry;

        (list)[(*&file)++].data = deleted;

        int val = *&file - 1;

        while (1){
            int location = val - 1;
            int parent = location / 2;

            if((list)[parent].importance <= (list)[val].importance){
                break;
            }

            List buff = (list)[val];

            (list)[val] = (list)[parent];

            (list)[parent] = buff;

            val = parent;

            if(val == 0){
                break;
            }
        }
    }
    
    printOutput(turnAroundTime, counter, runningTime, curr); // printing output that applies for both fcfs and rr scheduling here

    //freeing allocated memory
    if(details == true){
        printf("%s\n", detail); //printing out the string here
        free(detail);
        free(list);
    }
    else{
        free(list);
    }

    return 0;
}







