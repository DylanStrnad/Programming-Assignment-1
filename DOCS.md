# Documenations

## Shared Memory Setup

    //creates shared memory object
    int fd = shm_open("/sharedObject", O_CREAT | O_RDWR, 0600);

    //Shared mem segment matches size of struct sharedMem
    ftruncate(fd, sizeof(sharedMem));

    //allows access of shared memory
    sharedMem *shmp;
    shmp = (sharedMem*)mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

## Shared Memory Data Structure

## The producer
The producers job is to randomly produce 1 or 2 items into the shared buffer.

### Producer semaphores

## The consumer
The consumers job is to randomly consume 1 or 2 items from the shared buffer. 

### Consumer semphores





