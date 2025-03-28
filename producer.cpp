#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>  // for std::rand() and std::srand()
#include <ctime>    // for std::time()

// TABLESIZE = 2
const int SIZE {2};

//make struct with semephore and table
struct sharedMem{

    // array of 0s
    int buffer[SIZE] = { 0 }; 

    //track items in the array
    int arrayCount;

    sem_t slotsFilled;
    sem_t slotsEmpty;
    sem_t mutexProducer;
    sem_t mutexConsumer;

};

int main()
{
    //creates shared memory object
    int fd = shm_open("/sharedObject", O_CREAT | O_RDWR, 0600);

    //Shared mem segment matches size of struct sharedMem
    ftruncate(fd, sizeof(sharedMem));

    //allows access of shared memory
    sharedMem *shmp;
    shmp = (sharedMem*)mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //Initialize emephores
    sem_init(&shmp->slotsFilled, 1, 0);
    sem_init(&shmp->slotsEmpty, 1, 2);
    sem_init(&shmp->mutexProducer, 1, 1);
    sem_init(&shmp->mutexConsumer, 1, 0);

    //Count number of times producer will produce
    int count { 0 };
    
    //Will track position of producer
    int index { 0 };

    //tracks items in the buffer
    shmp->arrayCount = 0;

    std::srand(std::time(nullptr));

    while(true){

        sleep(1);
        
        // slotsEmpty > 0, then continue
        sem_wait(&shmp->slotsEmpty);  

        // mutex > 0, Producers turn
        sem_wait(&shmp->mutexProducer); 

        std::cout << "Producer Entered!\n";
        
        //Items the producer wants to produce
        int itemsToProduce = (std::rand() % 2) + 1;

        //keep track of how many items actually produced in the for loop
        int itemsProduced { 0 };

        //producer only produces if array has open spots
        for(int i = 0; i < itemsToProduce && shmp->arrayCount < SIZE; ++i){

            //tracks producer position in array
            index = count % 2;
            
            //place item in array
            shmp->buffer[index] = 1;

            ++count;
            ++shmp->arrayCount;
            ++itemsProduced;

        }

        std::cout << "Table after produced: | "  << shmp->buffer[0] << " | " << shmp->buffer[1] << " | " << '\n';

        std::cout << "Producer Exit!\n";

        

        // increase slots filled by number of items produced.
       for(int i = 0; i < itemsProduced; ++i){
            sem_post(&shmp->slotsFilled); 
        }

        // Consumers turn
        sem_post(&shmp->mutexConsumer); 
        
    }

    shm_unlink("/sharedObject");

    return 0;
}