#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024

typedef struct Process{
    char pid;
    int size;
    int count;
    struct Process * next;
} Process;

void bestFitStrategy (Process * head);
int loadBestFit (Process ** head);

void worstFitStrategy (Process * head);
int loadWorstFit (Process ** head);

void nextFitStrategy(Process * head);
int loadNextFit (Process ** head);

void firstFitStrategy (Process * head);
int loadFirstFit (Process ** head);

Process * pop (Process ** head);
int numberProcesses();
int numberHoles();
int memUsage();

//global memory array and queue for process storage
Process * queue = NULL;
char memory[MAX];

/* ---- MAIN FUNCTION FOR RETREIVING USER INPUT & PARSING .TXT FILE ---- */

int main (int argc, char * argv[]){
    Process * head = NULL;
    char pid[400];

    if (argc != 3){ //must be 3 command line args for code to work (refer to readme.txt file for specs)
        printf("Please enter filename and allocation strategy in command line input.\n");
        exit(0);
    }

    FILE * file = fopen(argv[1], "r");
    
    if (file == NULL){
        printf("Error - file could not be opened. Try again.\n");
        return 0;
    }
    else{
        while (!feof(file)){
            while (fgets(pid, 400, file) != NULL){
                char * token = strtok(pid, " "); // tokenizing file by space chars
                char * length = strtok(NULL, " ");

                if (length != NULL){
                    int processSize = atoi(length);
                    
                    //initializing first process to the head of queue
                    Process * temp = malloc(sizeof(Process));
                    
                    int len = strlen(token) + 1;
                    char * p = malloc(sizeof(char) * len);
                    strcpy(p, token);
                    temp->pid = *p;
                    temp->size = processSize;
                    temp->count = 0; //since we are adding the head, the process counter is 0 at this point
                    temp->next = NULL;

                    /*
                     * Author username: Kara 
                     * Date: 03/31/2022
                     * Title of Resource: Move value to end of queue in C
                     * URL link: https://stackoverflow.com/questions/21589957/move-value-to-end-of-queue-in-c
                     * The code below is a modified version of the code found in the URL link above. It is used
                     * to insert the first process to the head/start of the queue and the following processes at
                     * the end of the queue.
                    */

                    Process * insert = head;
                    
                    if (head == NULL){ //if there is only 1 process in file, it will be assigned head
                        head = temp;
                        break;
                    }
                    else{ //if there are more processes in file, they will be added to the end of queue
                        while (insert->next != NULL){
                            insert = insert->next; //move to end of queue
                        }

                        insert->next = temp; //assign process to end of queue
                    }
                }
                else{
                    break;
                }
                
            }
        }
        fclose(file);
    }

    if (strcmp(argv[2], "best") != 0 && strcmp(argv[2], "worst") != 0 && strcmp(argv[2], "next") != 0 && strcmp(argv[2], "first") != 0 ){
        printf("Please enter correct allocation strategy type as second command line argument.\n");
        exit(0);
    }

    if (strcmp(argv[2], "best") == 0){
        printf("-------- BEST FIT: --------\n");
        bestFitStrategy(head);
    }
    
    if (strcmp(argv[2], "worst") == 0){
        printf("-------- WORST FIT: --------\n");
        worstFitStrategy(head);
    }
    
    if (strcmp(argv[2], "next") == 0){
        printf("-------- NEXT FIT: --------\n");
        nextFitStrategy(head);
    }
    
    if (strcmp(argv[2], "first") == 0){
        printf("-------- FIRST FIT: --------\n");
        firstFitStrategy(head);
    }

    return 0;
}

/* ---- LOOP PRINTING HELPER FUNCTIONS & POP FUNCTION ---- */

Process * pop (Process ** head){
    /*
     * Author username: Gene
     * Date: 03/31/2022
     * Title of Resource: How to pop from linked list?
     * URL Link: https://stackoverflow.com/questions/18456118/how-to-pop-from-linked-list 
     * This 'pop' function is a slightly modified version of the code found in the URL link above. The link
     * explains how to pop from a linked list, but since my queue is operates similarily to a linked list, it
     * is valid to use this popping method for my queue as well.
    */
    Process * remove = (*head);

    if (remove == NULL){
        return NULL;
    }
    else{
        *head = (*head)->next;

        remove->next = NULL;
    }

    return remove;
}

int numberProcesses(){
    int numProcesses = 0;
    Process * p = queue;

    if (queue != NULL){
        numProcesses = 1; // assuming minimum of 1 process in memory if queue is not empty
    }

    for (p = queue; p->next != NULL; p = p->next){
        numProcesses = numProcesses + 1;
    }

    return numProcesses;
}

int numberHoles(){
    bool hole = false;
    int i = 0, numHoles = 0;
    char empty = '\0';

    while (i < 1024){
        if (hole == false){
            if (memory[i] == empty){
                hole = true;
            }
        }
        else if (memory[i] != empty){
            hole = false;
            numHoles = numHoles + 1;
        }

        i++;
    }

    if (hole == true){
        numHoles = numHoles + 1;
    }

    return numHoles;
}

int memUsage(){
    int j = 0, memConsumed = 0;
    char empty = '\0';

    while (j < 1024){
        if (memory[j] != empty){
            memConsumed = memConsumed + 1;
        }
        j++;
    }

    return memConsumed;
}

/* ---- ALLOCATION STRATEGY FUNCTIONS & HELPERS ---- */

/*
 * Author: PrepInsta
 * Date: 03/31/2022
 * Title of Resource: Best Fit Algoritm Program in C
 * URL Link: https://prepinsta.com/operating-systems/page-replacement-algorithms/best-fit/best-fit-in-c/ 
 * The 'loadBestFit' and 'bestFitStrategy' functions were developed using some of the code
 * found in the link above. A lot of parts were changed due to the context of the assignment
 * and the way I store the processes inside global variables, but the general algorithm is pretty much the same.
*/

int loadBestFit (Process ** head){
    Process * root = (*head);
    int iter = 0;
    char empty = '\0';
    int memSize = 1024;

    if (!root){
        return -1; // all processes have been loaded, return and print total info
    }

    int oldest = 0, best = 2000, location = 0;
    bool isBest = false;

    //iterate through memory to find holes
    while (iter < memSize){
        bool isHole = false;

        int space = iter;
        int hole = 0;

        while ((space < memSize) && memory[space] == empty){
            hole = hole + 1; //hole found
            isHole = true; //flag is now true
            
            if (hole > 1 || hole < 1){
                space = space + 1;
            }
            else if (hole == 1){
                oldest = iter;
            }
        }

        if (isHole == true){
            if (((*head)->size <= hole) && (best > hole)){
                isBest = true; //best fit found, so setting flag to true

                location = oldest;
                best = hole;
            }
        }

        iter = space; 
        iter++;
    }

    if ((isBest == true) && (*head)->size <= best){
        int i = 0;

        Process * temp = pop(head); //remove process from queue
        Process * temp2;

        //adding best process to queue (aka memory)
        while (i < temp->size){
            memory[location] = temp->pid;
            location++;
            i++;
        }

        if (!queue){
            queue = temp;
            return 1;
        }

        if (!queue){
            temp2 = NULL;
        }
        else{
            Process * p = queue;
            
            while (p->next != NULL){
                p = p->next;
            }

            temp2 = p;
        }
        
        if (!temp2->next){
            temp2->next = temp;
        }

        return 1;
    }

    return 0;
}

void bestFitStrategy (Process * head){
    int processCounter = 0;
    float numProcesses = 0, numHoles = 0, memConsumed = 0, cumulative = 0;
    char empty = '\0';

    int val = loadBestFit(&head);

    for (val = val; val != -1; val = loadBestFit(&head)){
        processCounter++;

        while (val == 0){
            Process * temp;
            char empty = '\0';
            int iter = 0;

            if (!queue){
                temp = NULL;
            }
            else{
                Process * p = queue;
                queue = queue->next;
                p->next = NULL;
                temp = p;
            }
            
            do{
                continue; //no processing required
            }while (memory[iter++] != temp->pid); //swapping processese inside of memory
            
            iter = iter - 1;

            do{
                memory[iter] = empty; //ensuring the rest of memory is empty
                iter++;
            }while (memory[iter] == temp->pid);

            temp->count = temp->count + 1;

            if (temp->count < 3){ 
                //adding process to queue
                Process * insert = head;
                            
                if (head == NULL){
                    head = temp;
                    return;
                }

                while (insert->next != NULL){
                    insert = insert->next;
                }

                insert->next = temp;
            }
            else if (temp->count >= 3){
                temp = NULL;  //since process has been loaded (3 swaps complete), it can be set to NULL
            }

            val = loadBestFit(&head);
        }

        bool hole = false;
        int i = 0;

        while (i < 1024){
            /*
             * If there is space inside memory, there is a hole and there is
             * a hole present, otherwise there if is no space, there's no hole in memory.
            */
            if (hole == false){
                if (memory[i] == empty){
                    hole = true;
                }
            }
            else if (memory[i] != empty){
                hole = false;
                numHoles = numHoles + 1;
            }

            i++;
        }

        if (hole == true){
            numHoles = numHoles + 1;
        }

        int ret = numberProcesses();
        numProcesses = numProcesses + ret;

        float memorySize = (float)(1024);
        int ret2 = memUsage();
        memConsumed = (ret2/memorySize) * 100;

        cumulative = cumulative + ((ret2/memorySize) * 100);
        float total = cumulative/processCounter;

        printf("pid loaded, #processes = %d, holes = %d, %%memusage = %.0f, cumulative %%mem = %.0f\n", numberProcesses(), numberHoles(), memConsumed, total);
    }

    if (val == -1){
        float aveProcesses = numProcesses/processCounter;
        float aveHoles = numHoles/processCounter;
        float aveMemory = cumulative/processCounter;

        printf("Total loads = %d, average processes = %.1f, average #holes = %.1f, cumulative %%mem = %.0f\n", processCounter, aveProcesses, aveHoles, aveMemory);
    }
}

/*
 * Author: PrepInsta
 * Date: 04/01/2022
 * Title of Resource: Worst Fit Algorithm Program In C
 * URL Link: https://prepinsta.com/operating-systems/page-replacement-algorithms/worst-fit/worst-fit-in-c/ 
 * The 'loadWorstFit' and 'worstFitStrategy' functions were developed using some of the code
 * found in the link above. Despite the changes made to code from the link, the general algorithm is pretty much the same.
*/

int loadWorstFit (Process ** head){
    Process * root = (*head);
    int iter = 0;
    char empty = '\0';
    int memSize = 1024;

    if (!root){
        return -1; // all processes have been loaded, return and print total info
    }

    int oldest = 0, max = 0, location = 0; //load into the largest holes in memory

    while (iter < memSize){
        bool isHole = false;
        
        int space = iter;
        int hole = 0;

        while ((space < memSize) && memory[space] == empty){
            hole = hole + 1; //hole found
            isHole = true; //flag is now true
            
            if (hole > 1 || hole < 1){
                space = space + 1;
            }
            else if (hole == 1){
                oldest = iter;
            }
        }

        if (isHole == true){ //storing starting location & hole size of largest hole
            if (max < hole){    
                max = hole;
                location = oldest;
            }
        }

        iter++;
    }

    if ((*head)->size <= max){
        int i = 0;

        Process * temp = pop(head); //remove process from queue
        Process * temp2;

        //adding largest hole process to queue (aka memory)
        while (i < temp->size){
            memory[location] = temp->pid;
            location++;
            i++;
        }

        if (!queue){
            queue = temp;
            return 1;
        }

        if (!queue){
            temp2 = NULL;
        }
        else{
            Process * p = queue;
            
            while (p->next != NULL){
                p = p->next;
            }

            temp2 = p;
        }
        
        if (!temp2->next){
            temp2->next = temp;
        }

        return 1;
    }

    return 0;
}

void worstFitStrategy (Process * head){
    int processCounter = 0;
    float numProcesses = 0, numHoles = 0, memConsumed = 0, cumulative = 0;
    char empty = '\0';

    int val = loadWorstFit(&head);

    for (val = val; val != -1; val = loadWorstFit(&head)){
        processCounter++;

        while (val == 0){
            Process * temp;
            char empty = '\0';
            int iter = 0;

            if (!queue){
                temp = NULL;
            }
            else{
                Process * p = queue;
                queue = queue->next;
                p->next = NULL;
                temp = p;
            }
            
            do{
                continue; //no processing required
            }while (memory[iter++] != temp->pid); //swapping
            
            iter = iter - 1;

            do{
                memory[iter] = empty; //ensuring the rest of memory is empty
                iter++;
            }while (memory[iter] == temp->pid);

            temp->count = temp->count + 1;

            if (temp->count < 3){ 
                //adding process to queue
                Process * insert = head;
                            
                if (head == NULL){
                    head = temp;
                    return;
                }

                while (insert->next != NULL){
                    insert = insert->next;
                }

                insert->next = temp;
            }
            else if (temp->count >= 3){
                temp = NULL;  //since process has been loaded (3 swaps complete), it can be set to NULL
            }
            
            val = loadWorstFit(&head);
        }

        bool hole = false;
        int i = 0;
        while (i < 1024){
            /*
             * If there is space inside memory, there is a hole and there is
             * a hole present, otherwise there if is no space, there's no hole in memory.
            */
            if (hole == false){
                if (memory[i] == empty){
                    hole = true;
                }
            }
            else if (memory[i] != empty){
                hole = false;
                numHoles = numHoles + 1;
            }

            i++;
        }

        if (hole == true){
            numHoles = numHoles + 1;
        }

        int ret = numberProcesses();
        numProcesses = numProcesses + ret;

        float memorySize = (float)(1024);
        int ret2 = memUsage();
        memConsumed = (ret2/memorySize) * 100;

        cumulative = cumulative + ((ret2/memorySize) * 100);
        float total = cumulative/processCounter;

        printf("pid loaded, #processes = %d, holes = %d, %%memusage = %.0f, cumulative %%mem = %.0f\n", numberProcesses(), numberHoles(), memConsumed, total);
    }

    if (val == -1){
        float aveProcesses = numProcesses/processCounter;
        float aveHoles = numHoles/processCounter;
        float aveMemory = cumulative/processCounter;

        printf("Total loads = %d, average processes = %.1f, average #holes = %.1f, cumulative %%mem = %.0f\n", processCounter, aveProcesses, aveHoles, aveMemory);
    }
}

/*
 * Author: Projjal Gop (username: projjal1)
 * Date: 04/02/2022
 * Title of Resource: Next Fit Memory Management scheme in C
 * URL Link: https://gist.github.com/projjal1/4ab0c0f389090fcd08c0e5bea975f7c1 
 * The 'loadNextFit' and 'nextFitStrategy' functions were developed with a similar algorithm as the link above.
 * Despite the changes made to the code in the link, the general algoithm is pretty much the same. Ex. I also
 * find the last process in 'queue', use the first index of the process to allocate a block.
*/

int loadNextFit (Process ** head){
    Process * root = (*head);
    int iter = 0;
    char empty = '\0';
    int memSize = 1024;

    if (!root){
        return -1; // all processes have been loaded, return and print total info
    }

    int oldest = 0, first = 0;
    bool loop = false;

    Process * p;

    if (!queue){
        p = NULL;
    }
    else{
        Process * q = queue;
                    
        while (q->next != NULL){
            q = q->next;
        }

        p = q; //last process inside queue is stored inside temp 'p' to confirm queue is not empty

        if (p){ // if 'p' is true, it means queue isn't empty, and we can retrieve the first index of process
            int index = 0, i = 0;
            int proc_found = 0;

            while (i < memSize){
                if (memory[i] == p->pid){
                    index = i;
                }
                i++;
            }

            int value = index + 1;
            first = value;
        }
    }

    iter = first;

    while ((iter != first) || (loop == false)){
        if (memSize <= iter){
            loop = true;
            iter = 0;
        }

        bool isHole = false;
        
        int space = iter;
        int hole = 0;

        while ((space < memSize) && memory[space] == empty){
            hole = hole + 1; //hole found
            isHole = true; //flag is now true
            
            if (hole > 1 || hole < 1){
                space = space + 1;
            }
            else if (hole == 1){
                oldest = iter;
            }
        }

        if (isHole == true){
            if (hole >= (*head)->size){
                int i = 0;

                Process * temp = pop(head); //remove process from queue
                Process * temp2;

                //adding next fit process to queue (aka memory)
                while (i < temp->size){
                    memory[oldest] = temp->pid;
                    oldest++;
                    i++;
                }

                if (!queue){
                    queue = temp;
                    return 1;
                }

                if (!queue){
                    temp2 = NULL;
                }
                else{
                    Process * p = queue;
                    
                    while (p->next != NULL){
                        p = p->next;
                    }

                    temp2 = p;
                }
                
                if (!temp2->next){
                    temp2->next = temp;
                }

                return 1;
            }
        }

        iter = space;
        iter++;
    }

    return 0;
}

void nextFitStrategy (Process * head){
    int processCounter = 0;
    float numProcesses = 0, numHoles = 0, memConsumed = 0, cumulative = 0;
    char empty = '\0';

    int val = loadNextFit(&head);

    for (val = val; val != -1; val = loadNextFit(&head)){
        processCounter++;
        
        while (val == 0){
            Process * temp;
            char empty = '\0';
            int iter = 0;

            if (!queue){
                temp = NULL;
            }
            else{
                Process * p = queue;
                queue = queue->next;
                p->next = NULL;
                temp = p;
            }
            
            do{
                continue; //no processing required
            }while (memory[iter++] != temp->pid); //swapping processese inside of memory
            
            iter = iter - 1;

            do{
                memory[iter] = empty; //ensuring the rest of memory is empty
                iter++;
            }while (memory[iter] == temp->pid);

            temp->count = temp->count + 1;

            if (temp->count < 3){ 
                //adding process to queue
                Process * insert = head;
                            
                if (head == NULL){
                    head = temp;
                    return;
                }

                while (insert->next != NULL){
                    insert = insert->next;
                }

                insert->next = temp;
            }
            else if (temp->count >= 3){
                temp = NULL;  //since process has been loaded (3 swaps complete), it can be set to NULL
            }

            val = loadNextFit(&head);
        }

        bool hole = false;
        int i = 0;
        while (i < 1024){
            /*
             * If there is space inside memory, there is a hole and there is
             * a hole present, otherwise there if is no space, there's no hole in memory.
            */
            if (hole == false){
                if (memory[i] == empty){
                    hole = true;
                }
            }
            else if (memory[i] != empty){
                hole = false;
                numHoles = numHoles + 1;
            }

            i++;
        }

        if (hole == true){
            numHoles = numHoles + 1;
        }

        int ret = numberProcesses();
        numProcesses = numProcesses + ret;

        float memorySize = (float)(1024);
        int ret2 = memUsage();
        memConsumed = (ret2/memorySize) * 100;

        cumulative = cumulative + ((ret2/memorySize) * 100);
        float total = cumulative/processCounter;

        printf("pid loaded, #processes = %d, holes = %d, %%memusage = %.0f, cumulative %%mem = %.0f\n", numberProcesses(), numberHoles(), memConsumed, total);
    }

    if (val == -1){
        float aveProcesses = numProcesses/processCounter;
        float aveHoles = numHoles/processCounter;
        float aveMemory = cumulative/processCounter;

        printf("Total loads = %d, average processes = %.1f, average #holes = %.1f, cumulative %%mem = %.0f\n", processCounter, aveProcesses, aveHoles, aveMemory);
    }
}

/*
 * Author: PrepInsta
 * Date: 04/02/2022
 * Title of Resource: First fit in C
 * URL Link: https://prepinsta.com/operating-systems/page-replacement-algorithms/first-fit/first-fit-in-c/ 
 * The 'loadFirstFit' and 'firstFitStrategy' functions were developed using some of the code
 * found in the link above. Despite the changes made to code from the link, the general algorithm is pretty much the same.
*/

int loadFirstFit (Process ** head){
    Process * root = (*head);
    int iter = 0;
    char empty = '\0';
    int memSize = 1024;

    if (!root){
        return -1; // all processes have been loaded, return and print total info
    }
    
    while (iter < memSize){
        bool isHole = false;
        
        int space = iter;
        int oldest, hole = 0;
        
        while ((space < memSize) && memory[space] == empty){
            hole = hole + 1; //hole found
            isHole = true; //flag is now true
            
            if (hole > 1 || hole < 1){
                space = space + 1;
            }
            else if (hole == 1){
                oldest = iter;
            }
        }

        if ((isHole == true) && ((*head)->size <= hole)){
            int i = 0;

            Process * temp = pop(head); //remove process from queue
            Process * temp2;

            //adding oldest process to queue (aka memory)
            while (i < temp->size){
                memory[oldest] = temp->pid;
                oldest++;
                i++;
            }

            if (!queue){
                queue = temp;
                return 1;
            }

            if (!queue){
                temp2 = NULL;
            }
            else{
                Process * p = queue;
                
                while (p->next != NULL){
                    p = p->next;
                }

                temp2 = p;
            }
            
            if (!temp2->next){
                temp2->next = temp;
            }

            return 1;
        } 
        
        iter++;
    }

    return 0;
}

void firstFitStrategy (Process * head){
    int processCounter = 0;
    float numProcesses = 0, numHoles = 0, memConsumed = 0, cumulative = 0;
    char empty = '\0';

    int val = loadFirstFit(&head); 

    for (val = val; val != -1; val = loadFirstFit(&head)){
        processCounter++;

        while (val == 0){
            Process * temp;
            char empty = '\0';
            int iter = 0;

            if (!queue){
                temp = NULL;
            }
            else{
                Process * p = queue;
                queue = queue->next;
                p->next = NULL;
                temp = p;
            }
            
            do{
                continue; //no processing required
            }while (memory[iter++] != temp->pid); //swapping
            
            iter = iter - 1;

            do{
                memory[iter] = empty; //ensuring the rest of memory is empty
                iter++;
            }while (memory[iter] == temp->pid);

            temp->count = temp->count + 1; 

            if (temp->count < 3){ 
                //adding process to queue
                Process * insert = head;
                            
                if (head == NULL){
                    head = temp;
                    return;
                }

                while (insert->next != NULL){
                    insert = insert->next;
                }

                insert->next = temp;
            }
            else if (temp->count >= 3){
                temp = NULL;  //since process has been loaded (3 swaps complete), it can be set to NULL
            }
            
            val = loadFirstFit(&head);
        }

        bool hole = false;
        int i = 0;
        while (i < 1024){
            /*
             * If there is space inside memory, there is a hole and there is
             * a hole present, otherwise there if is no space, there's no hole in memory.
            */
            if (hole == false){
                if (memory[i] == empty){
                    hole = true;
                }
            }
            else if (memory[i] != empty){
                hole = false;
                numHoles = numHoles + 1;
            }

            i++;
        }

        if (hole == true){
            numHoles = numHoles + 1;
        }

        int ret = numberProcesses();
        numProcesses = numProcesses + ret;

        float memorySize = (float)(1024);
        int ret2 = memUsage();
        memConsumed = (ret2/memorySize) * 100;

        cumulative = cumulative + ((ret2/memorySize) * 100);
        float total = cumulative/processCounter;

        printf("pid loaded, #processes = %d, holes = %d, %%memusage = %.0f, cumulative %%mem = %.0f\n", numberProcesses(), numberHoles(), memConsumed, total);

    }

    if (val == -1){
        float aveProcesses = numProcesses/processCounter;
        float aveHoles = numHoles/processCounter;
        float aveMemory = cumulative/processCounter;

        printf("Total loads = %d, average processes = %.1f, average #holes = %.1f, cumulative %%mem = %.0f\n", processCounter, aveProcesses, aveHoles, aveMemory);
    }
}




