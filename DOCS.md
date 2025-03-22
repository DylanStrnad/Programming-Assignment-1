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
This structure is used by the shared memory. It allows the producer and consume to access and modify the buffer. Also declares semaphores, which will be used for mutual exclusion(only allowing one process to access the shared memory at a time).

    //make struct with semephore and table
    struct sharedMem{

    // array of 0s
    int buffer[SIZE] = { 0 }; 

    //track items in the array
    int arrayCount;

    //Used for mutual exclusion
    sem_t slotsFilled;
    sem_t slotsEmpty;
    sem_t mutexProducer;
    sem_t mutexConsumer; };

## Semaphore Setup
Initialized in the producer file.

    //Initialize emephores
    sem_init(&shmp->slotsFilled, 1, 0);
    sem_init(&shmp->slotsEmpty, 1, 2);
    sem_init(&shmp->mutexProducer, 1, 1);
    sem_init(&shmp->mutexConsumer, 1, 0);
    
slotsFilled will be initially set to 0, because the array is empty. For this reason, slotsEmpty is set to 2.
mutexProducer set to 1, as the producer needs to run before running the consumer.

## The producer
The producers job is to randomly produce 1 or 2 items into the shared buffer.
Uses rand() to determine how many items to produce.

    //Items the producer wants to produce
    int itemsToProduce = (std::rand() % 2) + 1;

### Producing items
Loops as many times as needed to produce the desired amount of items.

    for(int i = 0; i < itemsToProduce && shmp->arrayCount < SIZE; ++i){

         //tracks producer position in array
        index = count % 2;
            
        //place item in array
        shmp->buffer[index] = 1;

        ++count;
        ++shmp->arrayCount;
        ++itemsProduced;

        }

*++count - increases # of times looped. Used by index to track producer postion.
*++shmp->arrayCount - Increases number of items in buffer.
*++itemsProduced - determines how many times needed to increase shmp->slotsFilled.

    for(int i = 0; i < itemsProduced; ++i){
            sem_post(&shmp->slotsFilled); 
        }

## The consumer
The consumers job is to randomly consume 1 or 2 items from the shared buffer. 

    //# of items consumer wants to consume
    int itemsToConsume = (std::rand() % 2) + 1;

### consuming items
Loops as many times as needed to consume the desired amount of items

        //conusmer only consumes if array has item
        for(int i = 0; i < itemsToConsume && shmp->arrayCount > 0; ++i){

            //tracks position of consumer
            index = count % 2;

            int item = shmp->buffer[index];
            shmp->buffer[index] = 0;

            ++itemsConsumed;
            ++count;
            --shmp->arrayCount;
        }
*++count - increases # of times looped. Used by index to track producer postion.
*--shmp->arrayCount - decreases number of items in buffer.
++itemsConsumed - determines how many times needed to increase shmp->slotsEmpty





