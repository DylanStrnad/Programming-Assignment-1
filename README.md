# Programming-Assignment-1
**Author: Dylan Strnad**
This project simulates a producer and consumer using shared memory and semaphores.

##How it was made: 

**Programmming languaged used:** C++

Semaphores are used to implement mutual exlcusion, in which only one process can run at a time. The producer and consumer have their own files, and make use of semaphores to signal when it is time to run. Additionally, shared memory is used to share the contents between the files. This allows the producer and consumer to access data from the same memroy location. 

##Lessons Learned:

Implimenting the shared memory and semaphores was a difficult process. I have not worked with shared memory before, and this was a learning experience. Also, synchronizing the producer and consumer to not run at the same time was difficult. Learning how to set up the semaphores and use them to synchronize the procuder and consumer was not an easy task. 

## How To Run:
  ```sh
  npm install npm@latest -g
  ```
