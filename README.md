# Processes-Threads-and-IPC-in-C

This is a project for Operating Systems course in AGU. You can find project description below:

---

You must watch this short tutorial: Linux IPC
https://www.youtube.com/watch?v=vU2HDf5ZhO4

**Gains:** Learning process creation and process communication, thread creation and communication, semaphore creation and usage as well as multi-process / multi-threaded application programming.

Assume that you have several files representing bags containing balls with different colors. An input file might contain comma separated colors such as “yellow, yellow, green, red, blue, blue, orange, blue, yellow, red”. In this project, you are supposed to implement several programs which will count the colors of each ball in a bag given as input, and find and output the probability to take out a definite color from the bag in an output file. The color to be chosen will be given as user input. The programs will be named psearch. They need to take the following command line parameters: a string keyword (chosen color), number of input files (n), input filenames, and an output filename. Number of input files will be greater than or equal to 1. Each input file is a text file; containing a sequence of lines. All of these lines may contain one or more strings. The output file will contain the probability (2/7) along with the count of balls of different colors in the format below:
```
<probability>: <your result> <input filename>, <color name>: <color count>
```
The output must be sorted according to first <input filename> and then <color name>. An example execution of your program should be: psearch orange 3 input1.txt input2.txt input3.txt output.txt

## Task 1

a) Implement the program using ​multiple processes created by the parent ​main program. Call the program as psearch1a. Create children via the fork() system call. For n input files, n children must be created. Each child will process one input file and will write the result to a temporary output file. When all children end, the parent process will read the temporary output files and will write one final output file in the above format.
In psearch1a, f​iles will be used to pass information​from children to the parent process.
      
b) Repeat a, but this time name your program as psearch1b and use ​Shared memory with shared anonymous mapping mmap().​ Also you need to use unnamed semaphores for synchronization.

c) Repeat a, but this time name your program as psearch1c and use an ​anonymous (unnamed) pipeline pipe().​

## Task 2

a) In this task, you will have separate processes for different jobs. There will be ​a master and slave processes.​
Slave: Implement a program psearch2aslave which will take two arguments, first one is the search word, second one is the input file.
Master: Implement a program psearch2a which will take search word, input files and output file names from the user. For each input file, it should create a process psearch2slave and call it with proper arguments . (hint: exec() family of functions). So, the master will not search anything. It will make slaves do this. The master will only receive results from the slaves and saves the result into the output file.
To communicate with master, slaves should use ​named shared memory (shared file mapping mmap() with a file descriptor)

b) Repeat a), but the communication should be implemented using ​Posix shared memory shm_open() (without file mapping) + named semaphores (sem_open() ).​ Program names: psearch2bslave and psearch2b.

c)​Repeat a), but the communication should be implemented using ​Message passing (Message queues in Linux, mq_open() ).​Program names: psearch2cslave and psearch2c. d)​Repeat a), but the communication should be implemented using ​Named Pipes mkfifo().​ Program names: psearch2dslave and psearch2d.

## Task 3

Implement the program, called psearch3, ​using threads.​ Instead of creating children processes, create one thread per input file. Each thread will add the matching line information to a ​shared (global) linked list.​ When all threads end, the main thread will process the linked lists and will write the output file in the above format. Take care of any possible synchronization problems (use semaphores). You need to use POSIX Pthreads library.

### Coverage of the project
```
Process ID
fork(), exec(), exit(), wait(), abort(), pipe(), mkfifo(), shm_open()
IPC
Messagepassing(​MessagequeuesinLinux,mq_open())​
Direct - Indirect communication
Shared memory In Linux
Shared anonymous mapping (only between Parent - child) Also need unnamed semaphores
Shared file mapping + named semaphores sem_open()
Posix shared memory shm_open() (without file mapping) + named semaphores sem_open()
Client server systems
Socket (Sockets on Unix domain, AF_UNIX)
Remote Procedures ○ Pipes
amed mkfifo() and Anonymous pipe() 
Non-blocking and ​blocking calls
Buffers zero - bounded - unbounded
```
