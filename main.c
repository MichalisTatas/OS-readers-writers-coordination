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

    pid_t pid;
    for (int i=0; i<atoi(argv[1]); i++) {    //creates childs bases on input

        if ( (pid = fork()) == -1) {
            printf("Error while using function fork() !");
            return -1;
        }

        if (pid == 0)                 //if it is a child break and dont fork again
            break;

    }
    printf("hello world \n");
    return 0;
}