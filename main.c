#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>


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
 * should the shared memory array contain structs to also keep
 * the statistics as well as the integer
 * that the processes will read or write
 */

//command line arguments with order : childs, array size

int main(int argc, char* argv[])
{
    key_t key = ftok("shm", 65);
    int smhId = shmget(key, sizeof(int), 0666|IPC_CREAT);
    void *smhInt = shmat(smhId, NULL, 0); 
    memset(smhInt, 0, sizeof(int));

    pid_t pid;

    for (int i=0; i<atoi(argv[1]); i++) {    //creates childs bases on input

        if ( (pid = fork()) == -1 ) {
            printf("Error while using function fork() !");
            return -1;
        }

        if (pid == 0)                 //if it is a child break and dont fork again
            break;

    }
    
    *(int*)smhInt = *(int*)smhInt +1;
    printf("hello world : %d\n", *(int*)smhInt);
    
    shmdt(smhInt);
    exit(0);
    return 0;
}

