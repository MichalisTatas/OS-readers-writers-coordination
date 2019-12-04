#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

typedef struct entrie {
    sem_t readerSem;
    sem_t writerSem;
    int readerCounter;
    int content;
    int reads;
    int writes;
    int averageTime;
} entrie;
typedef entrie* entriePtr;
