#include "../include/struct.h"
#include "../include/functions.h"

double exponentialDistribution(double l){
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / l;
}

int readerFunction(int arraySize, entriePtr entries)
{
    clock_t start1, start2, end1, end2;

    int randomEntrie = rand() % arraySize;
    start1 = clock();
    sem_wait(&entries[randomEntrie].readerSem);
    end1 = clock();
    entries[randomEntrie].readerCounter ++;
    if (entries[randomEntrie].readerCounter == 1)
        sem_wait(&entries[randomEntrie].writerSem);
    sem_post(&entries[randomEntrie].readerSem);

    int time = exponentialDistribution(0.5);
    sleep(time);
    entries[randomEntrie].reads ++ ;                         //read procedure
    
    start2 = clock();
    sem_wait(&entries[randomEntrie].readerSem);
    end2 = clock();
    entries[randomEntrie].readerCounter --;
    if (entries[randomEntrie].readerCounter == 0)
        sem_post(&entries[randomEntrie].writerSem);
    sem_post(&entries[randomEntrie].readerSem);

    return (double)(end1 - start1 + end2 - start2);
}

int writerFunction(int arraySize, entriePtr entries)
{
    int randomEntrie = rand() % arraySize;
    clock_t start, end;

    start = clock();
    sem_wait(&entries[randomEntrie].writerSem);
    end = clock();
    
    int time = exponentialDistribution(0.5);
    sleep(time);
    entries[randomEntrie].content = rand()%arraySize;       //write procedure
    entries[randomEntrie].writes ++ ;
    
    sem_post(&entries[randomEntrie].writerSem);
    return (double)(end - start);
}

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
    for (int i=0; i<arraySize; i++) {
        sem_init(&entries[i].readerSem, 1, 1);               //2nd argument is >0 because
        sem_init(&entries[i].writerSem, 1, 1);               //fork creates processes not threads
        entries[i].content = rand() % arraySize;
        entries[i].readerCounter = 0;
        entries[i].reads = 0;
        entries[i].writes = 0;
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
    
    int reads =0, writes = 0;
    double totalTime = 0.0;
    clock_t start, end;

    srand(getpid());
    for (int i=0; i<peerActivationNum; i++) {
        if ((rand()%10) <= rwAnalogy) {
            totalTime += readerFunction(arraySize, entries);
            reads++;
        }
        else {
            totalTime += writerFunction(arraySize, entries);
            writes++;
        }
    }

    printf("Process with id : %d wrote %d times and read %d times and waited on average %12f time\n\n", getpid(), writes, reads, totalTime/(reads+writes));

    for (int i=0; i<childsNumber; i++)         //parent waits for childs to finish
        wait(NULL);

    if(pid != 0) {
        for (int n=0; n<arraySize; n++) {
            printf("For entrie number   : %d \n   number of reads  : %d\n   number of writes : %d\n \n", n,entries[n].reads , entries[n].writes);
            sem_destroy(&entries[n].readerSem);
            sem_destroy(&entries[n].writerSem);
        }
    }

    if (shmctl(shmId, IPC_RMID, NULL) == -1) {
        perror("shmctl error : ");
    }   
    shmdt(shmAddress);

    exit(0);
}