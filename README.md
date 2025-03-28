# Programming-Assignment-1
**Author: Dylan Strnad**
This project simulates a producer and consumer using shared memory and semaphores.

## How it was made: 

**Programmming languaged used:** C++

Semaphores are used to implement mutual exlcusion, in which only one process can run at a time. The producer and consumer have their own files, and make use of semaphores to signal when it is time to run. Additionally, shared memory is used to share the contents between the files. This allows the producer and consumer to access data from the same memroy location. 

## Lessons Learned:

Implimenting the shared memory and semaphores was a difficult process. I have not worked with shared memory before, and this was a learning experience. Also, synchronizing the producer and consumer to not run at the same time was difficult. Learning how to set up the semaphores and use them to synchronize the procuder and consumer was not an easy task. 

## How To Run:
To compile code in linux environment, run these commands:

  ```sh
  $ g++ producer.cpp -pthread -lrt -o producer
  $ g++ consumer.cpp -pthread -lrt -o consumer
  $ ./producer & ./consumer &
  ```

## Examples

![image](https://github.com/user-attachments/assets/87bbbc6c-1233-4d6c-8468-fe0a88f43910)

This example shows when the producer and consumer enter and exit their critical sections, and the table after the producer and consumer make modifications to the table. This example shows how these processes follow mutual exlcusion, in whih only one will enter their critical section at a time. As shown in this example, the producer must exit before the consumer can enter, and vice versa.
