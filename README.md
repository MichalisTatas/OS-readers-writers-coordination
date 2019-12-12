# OS-readers-writers-coordination

A programm to synchronize readers and writers, which try to access a shared memory segment in c

Compile:
  make
  ./ex "number of processes" "number of entries in shared memory" "readers/writers analogy" "how many times to execute"
  
  Readers/writers analogy: 
    if the input is for example 6 it means that there will be 60% readers and 40% writers 
