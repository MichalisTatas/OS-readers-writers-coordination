#include "../include/struct.h"
#include "../include/functions.h"

void coordinator(int childsNumber, int arraySize, int rwAnalogy, int peerActivationNum)
{
    key_t key = ftok("src/functions.c", 100);             //produces unique key
    if (key == -1) {
        perror("ftok error : ");
        exit(1);
    }
    
    int shmId = shmget(key, ((int)sizeof(entrie))*arraySize, 0666|IPC_CREAT);  //gets the shared memory segment
    if (shmId == -1) {
        perror("shmget error : ");
        exit(1);
    }

    void *shmAddress = shmat(shmId, NULL, 0);                //gets the address of the shares memory segment
    if (!shmAddress) {
        perror("shmat error : ");
        exit(1);
    }

    entriePtr entries;
    entries = (entriePtr)shmAddress;
    srand(time(0));
    for (int m=0; m<arraySize; m++) {
        sem_init(&entries[m].readerSem, 1, 1);               //2nd argument is >0 because
        sem_init(&entries[m].writerSem, 1, 1);               //fork creates processes not threads
        entries[m].content = rand() % arraySize;
        entries[m].readerCounter = 0;
        entries[m].reads = 0;
        entries[m].writes = 0;
        entries[m].averageTime = 0;
    }

    pid_t pid;
    for (int i=0; i<childsNumber; i++) {    //creates childs based on input

        if ((pid = fork()) == -1) {
            perror("fork error : ");
            exit(1);
        }

        if (pid == 0)                 //if it is a child break and dont fork again
            break;
    }
    
    srand(getpid());
    for (int i=0; i<peerActivationNum; i++) {
        if ((rand()%10) <= rwAnalogy) {
            //choose random entrie and try to read

            int randomEntrie = rand() % arraySize;
            sem_wait(&entries[randomEntrie].readerSem);
            entries[randomEntrie].readerCounter ++;
            if (entries[randomEntrie].readerCounter == 1)
                sem_wait(&entries[randomEntrie].writerSem);
            sem_post(&entries[randomEntrie].readerSem);
            sleep(1);
            // printf("Reading entrie num : %d with value : %d \n", randomEntrie, entries[randomEntrie].content);
            entries[randomEntrie].reads ++ ;

            sem_wait(&entries[randomEntrie].readerSem);
            entries[randomEntrie].readerCounter --;
            if (entries[randomEntrie].readerCounter == 0)
                sem_post(&entries[randomEntrie].writerSem);
            sem_post(&entries[randomEntrie].readerSem);
        }
        else {
            //choose random entrie and try to write

            int randomEntrie = rand() % arraySize;
            sem_wait(&entries[randomEntrie].writerSem);
            sleep(1);
            // printf("Writing on entrie number : %d with value : %d \n", randomEntrie, entries[randomEntrie].content);
            entries[randomEntrie].content = rand()%arraySize;
            entries[randomEntrie].writes ++ ;
            // printf("Changed its value to : %d\n", entries[randomEntrie].content);
            sem_post(&entries[randomEntrie].writerSem);
        }
    }

    if(pid != 0) {
        for (int n=0; n<arraySize; n++) {
            printf("random number : %d %d\n", entries[n].reads , entries[n].writes);
            sem_destroy(&entries[n].readerSem);
            sem_destroy(&entries[n].writerSem);
        }
    }
    
    // struct shmid_ds shm_desc;                       //delete shared memory
    if (shmctl(shmId, IPC_RMID, NULL) == -1) {
        perror("shmctl error : ");
    }   
    shmdt(shmAddress);                              //delete shared memory pointer
    exit(0);
}