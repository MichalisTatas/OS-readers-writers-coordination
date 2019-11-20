#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 *i must create a shared memory array with size input from command line
 *create readers writers do thei thing
 *when someone reads an index increase its readerCounter
 *same goes for writers
 *lastly when readers and writers finish their job
 *the coordinator iterates the whole array
 *and prints data like readerCounter, writerCounter
 *time spent for each action etc
 */

int main(int argc, char* argv[])
{

    for (int i=0; i<atoi(argv[1]); i++) { 
        if (fork() == 0) { 
            printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid()); 
            exit(0); 
        } 
    }
    
    return 0;
}