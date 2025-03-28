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
#include <cstdlib>
#include <ctime> 

// TABLESIZE = 2
const int SIZE {2};

//make struct with semephore and table
struct sharedMem{

    // array of 0s
    int buffer[SIZE] = {}; 

    //track items in the array
    int arrayCount = 0;

    sem_t slotsFilled;
    sem_t slotsEmpty;
    sem_t mutexProducer;
    sem_t mutexConsumer;

};

int main(){

    int fd = shm_open("/sharedObject", O_CREAT | O_RDWR, 0600);
    sharedMem *shmp;
    shmp = (sharedMem*)mmap(0, sizeof(sharedMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    int count { 0 };

    int index { 0 };

    std::srand(std::time(nullptr));

    while(true){
        sleep(1);
        
        //wait(until table is full)
        sem_wait(&shmp->slotsFilled); // slotsFilled > 0, then continue to next line
        sem_wait(&shmp->mutexConsumer); //mutex > 0, consumers turn

        std::cout << "Consumer Entered!\n";
        std::cout << "items in array:" << shmp->arrayCount << "\n";
        
        //Items consumer wants to consume
        int itemsToConsume = (std::rand() % 2) + 1;

        //Keep track of items actually consumed in the for loop
        int itemsConsumed { 0 };

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

        std::cout << "Table after consumed: | "  << shmp->buffer[0] << " | " << shmp->buffer[1] << " | " << '\n';


        std::cout << "Consumer Exit!\n";

        

        // increase slotsEmpty by number of items consumed
        for(int i = 0; i < itemsConsumed; ++i){
            sem_post(&shmp->slotsEmpty); 
        }

        sem_post(&shmp->mutexProducer); //Producers turn
    }

    return 0;
}
