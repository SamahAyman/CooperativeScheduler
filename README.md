# CooperativeScheduler
Embedded Systems Course Project 1

## Project Description:
The objective of this short project is to develop a cooperative scheduler for embedded systems. This scheduler is similar to the
function queue scheduler. 
Implementation comply with the following specifications/API:
1. All ready to run tasks (functions) are stored in the ready queue.
2. The ready queue is sorted based on the tasks priorities
3. A task is a just a function that returns no data and accepts no arguments.
4. To insert a task to the ready queue call QueTask() and pass two arguments(Pointer to the function (function name) that implements the task & Task’s priority)
5. Supports 8 priority levels
6. A task can be inserted into the ready queue by: Other tasks (QueTask),ISRs (QueTask), or Itself (ReRunMe).
7. Dispatch() API is used to remove the highest priority task from the queue and run it.
8. The system is initialized by a single call to the Init()function. This function creates and initializes all needed data
structures.
9. A task my enqueue itself by calling ReRunMe(0). If ReRunMe() is called with a non-zero argument, the task will be
inserted to the ready queue after a delay specified by the argument. For example, ReRunMe(5) inserts the task into
the ready queue after 5 ticks. To implement this functionality, another queue (delayed queue) is introduced to store
the delayed tasks. This queue is sorted based on the delay/sleeping time (descending order). Once the sleeping time
of the head of this queue expires, the task is removed from the delayed queue and inserted into the ready queue. The
sleeping time of the tasks in the delayed queue is updated (decremented by 1) every tick.
10. The tick is 50 msec. SysTick Timer is used to generate the tick interrupts.
11. Code developed using STM32CubeMX and Keil μVision to run on the Nucleo-32 board you have.


## cooperative scheduler implementation ##

### Queues ###


### main functions ###

### test cases ###

 Markup : * Testing n visual studio
              * Running tasks based on correct priorities

 Markup : * Testing n visual studio
              * Test (1)Running tasks based on correct priorities


The output shows that the tasks were enqueued correctly into the ready queue based on the higher priority task, then the system is idle once all tasks have been dispatched and the ready queue is empty. 
T1: Priority 4
T2: Priority 2
T3: Priority 3



## Applications ## 

### Ambient temperature monitor ###
The purpose of this application is to Read the ambient temperature using DS3231 RTC sensor (over I2C bus) every 30 sec, and then produce an alarm through an external LED flashing when the temperature exceeds a certain threshold that is given by the user through TeraTerm terminal emulator using an asynchronous serial link (UART2) connected via a USB-to-TTL module.



### Parking sensor ###
