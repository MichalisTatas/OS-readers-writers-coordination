#include <stdio.h>
#include <stdlib.h>
#include "../include/functions.h"

int main(int argc, char* argv[])
{
    int childs_number = atoi(argv[1]);
    int array_size = atoi(argv[2]);
    int RWanalogy = atoi(argv[3]);             //readers writers analogy
    int peersActivation = atoi(argv[4]);
    coordinator(childs_number, array_size, RWanalogy, peersActivation);
    return 0;
}
