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
This structure is used by the shared memory. It allows the producer and consumer to access and modify the buffer. Also declares semaphores, which will be used for mutual exclusion (only allowing one process to access the shared memory at a time).

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

    //Initialize semephores
    sem_init(&shmp->slotsFilled, 1, 0);
    sem_init(&shmp->slotsEmpty, 1, 2);
    sem_init(&shmp->mutexProducer, 1, 1);
    sem_init(&shmp->mutexConsumer, 1, 0);
    
slotsFilled will be initially set to 0, because the array is empty. For this reason, slotsEmpty is set to 2.
mutexProducer set to 1, as the producer needs to run before running the consumer.

## Semaphore Utilizaton

Before entering the producers critical section, the programs waits until the number of slots empty is greater than 0. If this is the case, it continues to the next line and waits until it is the producers turn.

        // slotsEmpty > 0, then continue
        sem_wait(&shmp->slotsEmpty);  

        // mutex > 0, Producers turn
        sem_wait(&shmp->mutexProducer); 
        
After exiting the critical section, the producer will increase the number of slots filled. Also, it will indicate that it is the consumers turn. The for loop below is explained in the producer section.

       // increase slots filled by number of items produced.
       for(int i = 0; i < itemsProduced; ++i){
            sem_post(&shmp->slotsFilled); 
        }

        // Consumers turn
        sem_post(&shmp->mutexConsumer);

The consumer code follows the same logic as the code above.


## The Producer
The producers job is to randomly produce 1 or 2 items into the shared buffer.
Uses rand() to determine how many items to produce.

    //Items the producer wants to produce
    int itemsToProduce = (std::rand() % 2) + 1;

### Producing Items
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

* ++count - increases # of times looped. Used by index to track the producers postion in the buffer.
* ++shmp->arrayCount - Increases number of items in buffer.
* ++itemsProduced - determines how many times the code needs to increase shmp->slotsFilled. This is utilized in the loop below:

        // increase slots filled by number of items produced.
        for(int i = 0; i < itemsProduced; ++i){
            sem_post(&shmp->slotsFilled); 
        }

## The consumer
The consumers job is to randomly consume 1 or 2 items from the shared buffer. 

    //# of items consumer wants to consume
    int itemsToConsume = (std::rand() % 2) + 1;

### Consuming Items
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
* ++count - increases # of times looped. Used by index variable to track the consumers postion in the buffer.
* --shmp->arrayCount - decreases number of items in buffer.
* ++itemsConsumed - determines how many times needed to increase shmp->slotsEmpty. This is utilized in the loop below:
  
        // increase slotsEmpty by number of items consumed
        for(int i = 0; i < itemsConsumed; ++i){
            sem_post(&shmp->slotsEmpty); 
        }





