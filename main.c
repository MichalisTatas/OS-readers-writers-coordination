#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>

/*
 * use semaphores not monitors for this project
 * bcz with monitors ever if 1 reader reads nobody can read 
 */

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

typedef struct entrie { 
    int content;
    int reads;
    int writes;
    int averageTime;
} entrie;
typedef entrie* entriePtr;

int main(int argc, char* argv[])
{
    int childs_number = atoi(argv[1]);
    int array_size = atoi(argv[2]);

    pid_t pid;

    for (int i=0; i<childs_number; i++) {    //creates childs bases on input

        if ( (pid = fork()) == -1 ) {
            printf("Error while using function fork() !");
            return -1;
        }

        if (pid == 0)                 //if it is a child break and dont fork again
            break;
    }
    
    // key_t key = ftok("main", 65);             //produces unique key  //look at this befor : "smh"
    // if (key == -1) {
    //     perror("ftok : ");
    //     exit(1);
    // }
    
    int shmId = shmget(100, ((int)sizeof(entrie))*array_size, 0666|IPC_CREAT);  //gets the shared memory segment //if
    if (shmId == -1) {
        perror("shmget : ");
        exit(1);
    }

    void *shmAddress = shmat(shmId, NULL, 0);                //gets the address of the shares memory segment
    if (!shmAddress) {
        perror("shmat : ");
        exit(1);
    }

    entriePtr entries;
    entries = (entriePtr)shmAddress;
    srand(time(0));

    for (int m=0; m<array_size; m++) {
        entries[m].content = rand() % array_size;
        entries[m].reads = 0;
        entries[m].writes = 0;
        entries[m].averageTime = 0;
    }
    
    for (int n=0; n<array_size; n++) {
        printf("random number : %d \n", entries[n].content);
    }
    
    struct shmid_ds shm_desc;                       //delete shared memory
    if (shmctl(shmId, IPC_RMID, &shm_desc) == -1) {
        perror("main: shmctl: ");
    }   
    shmdt(shmAddress);                              //delete shared memory pointer
    exit(0);
    return 0;
}

