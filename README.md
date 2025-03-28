# Programming-Assignment-1
**Author: Dylan Strnad**

This project simulates a producer and consumer using shared memory and semaphores.

## How it was made: 

**Programmming languaged used:** C++

Semaphores are used to implement mutual exlcusion, in which only one process can run at a time. The producer and consumer have their own files, and make use of semaphores to signal when it is time to run. Additionally, shared memory is used to share the contents between the files. This allows the producer and consumer to access data from the same memory location. 

## Lessons Learned:

Implimenting the shared memory and semaphores was a difficult process. This project showed me how to set up shared memory between 2 cpp files, and how to manipulate this shared memory. This shared memory had to be manipulated within the process code, but without semaphores, both processes would have unrestricted access to the shared memory, and the processes would not work as intended. To solve this issue, semaphores are implimented. This allows to lock a process out of its critical section if another process was currently inside its critical section. For this project, I learned how to implement these features.

## How To Run:
To compile code in linux environment, run these commands:

  ```sh
  $ g++ producer.cpp -pthread -lrt -o producer
  $ g++ consumer.cpp -pthread -lrt -o consumer
  $ ./producer & ./consumer &
  ```

## Examples

![image](https://github.com/user-attachments/assets/87bbbc6c-1233-4d6c-8468-fe0a88f43910)

This example shows when the producer and consumer enter and exit their critical sections, and the table after the producer and consumer make modifications to the table. A 1 indicates an item in the table. A 0 indicates an empty spot in the table. It also shows how these processes follow mutual exlcusion, in which only one will enter their critical section at a time. As shown, the producer must exit before the consumer can enter, and vice versa.
