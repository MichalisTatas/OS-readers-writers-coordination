#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>


/*
 * i must create a shared memory array with size input from command line
 * create readers writers do thei thing
 * when someone reads an index increase its readerCounter
 * same goes for writers
 * lastly when readers and writers finish their job
 * the coordinator iterates the whole array
 * and prints data like readerCounter, writerCounter
 * time spent for each action etc
 */

/*
 * semaphore functions in one .h
 * 
 */

/*
 * should the shared memory array contain structs to also keep
 * the statistics as well as the integer
 * that the processes will read or write
 */

int main(int argc, char* argv[])
{
    key_t key = ftok("shm", 65);
    int smhId = shmget(key, 1024, 0666|IPC_CREAT);
    int *smhInt = (int*) shmat(smhId, (void*)0, 0);
    *smhInt = 0;

    pid_t pid;

    for (int i=0; i<atoi(argv[1]); i++) {    //creates childs bases on input

        if ( (pid = fork()) == -1 ) {
            printf("Error while using function fork() !");
            return -1;
        }

        if (pid == 0)                 //if it is a child break and dont fork again
            break;

    }
    
    *smhInt = *smhInt + 1;
    printf("hello world : %d\n", *smhInt);
    
    shmdt(smhInt);
    exit(0);
    return 0;
}