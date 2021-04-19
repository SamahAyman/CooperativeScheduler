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

#### 1. Testing on visual studio ####
          
##### Test (1): Running tasks based on correct priorities #####

The output shows that the tasks were enqueued correctly into the ready queue based on the higher priority task, then the system is idle once all tasks have been dispatched and the ready queue is empty. 

T1: Priority 4

T2: Priority 2

T3: Priority 3

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Picture1.png)


##### Test (2): Testing Rerunme(0) case #####
Task 1 was given the highest priority, then the task reruns itself after 0 ticks “Rerunme(0)”, so it runs forever as it always has the highest priority in the queue. The other tasks were given less priorities so it is expected that they starve because Task  keeps rerunning itself with its highest priority.

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Picture2.png)



##### Test (3): Testing enqueuing and dequeuing tasks from the delayed queue according to their delays #####

Tasks were given the following priorities. Task 2 reruns itself after 5 ticks, while task 3 reruns itself after 3 ticks which means there will be 2 ticks where the system is idle before task 2 and task 3 keep repeating over and over.

T1: Priority 1 

T2: Priority 2   ReRunMe(5)

T3: Priority 3  ReRunMe(3)

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Picture3.png)




##### Test (4): Violating the priority limit case ##### 
T1: Priority 9 (violating max priority allowed) 

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Picture4.png)




##### Test (5): Testing scheduling of 4 tasks ##### 

Tasks were given the following priorities. T1 reruns itself after 2 ticks, T2, reruns itself after 8 ticks, T3 reruns itself after 6 ticks, and T4 runs only once with the lowest priority. After T4 runs once, T1,T2, and T3 keep alternating forever, and the system is only idle if neither of the sleeping time of the 3 tasks is elapsed yet. 
T1: Priority 1 => ReRunMe(2)
T2: Priority 2 => ReRunMe(8)
T3: Priority 3 => ReRunMe(6)
T4: Priority 4 

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Picture5.png)


##### Test (6): Scheduling 4 tasks with one running Rerunme(0) #####

Tasks were given the following priorities:

T1: Priority 1 => ReRunMe(2)

T2: Priority 2 => ReRunMe(8)

T3: Priority 3 => ReRunMe(6)

T4: Priority 4 => ReRunMe(0)

The first 3 tasks rerun themselves after delays 2, 8, and 6 respectively. And task 4 reruns itself after 0 ticks which means it doesn’t get inserted into the delayed queue rather it gets inserted into the ready queue directly. Despite task 4 reruns itself after 0 ticks and it is always ready, it doesn’t run forever as in Test (2) because other tasks are ready in the queue with higher priorities. 

#### 2. Testing on Keil uVision ####

Tasks were given the following priorities:
T1: Priority 1 
T2: Priority 2 => ReRunMe(5)
T3: Priority 3 => ReRunMe(3)
Task 2 reruns itself after 5 ticks while task 3 reruns itself after 3 ticks. Task 1 toggles the led with a delay of 300 ms, task 2 toggles the led with a delay of 100 ms, and task 3 toggles the led with a delay of 50 ms. So, as you can observe from the demo video: task 1 runs first, then task 2, then task 3, then both task 2 and task 3 keep running alternatively forever; the difference in the toggling frequency clarifies which task is currently running. 





## Applications ## 

### Ambient temperature monitor ###
The purpose of this application is to Read the ambient temperature using DS3231 RTC sensor (over I2C bus) every 30 sec, and then produce an alarm through an external LED flashing when the temperature exceeds a certain threshold that is given by the user through TeraTerm terminal emulator using an asynchronous serial link (UART2) connected via a USB-to-TTL module.

#### Tasks #### 
* *Initialization*: In this task we assign the registers to its corresponding values. 
* *Taking Threshold from user*: In this task we will display a message to the user to enter the threshold and then display this value on the TeraTerm screen. 
* *Taking Temperature reading*: In this task we utilized DS3231 module to measure the temperature. I2C communication was used to retrieve the sensor data and then UART communication was used to display the readings on TeraTerm. 
* *Alarm*: In this task we toggle a LED in case temperature readings exceeds the threshold. 

#### Hardware Circuitry #### 

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/temp.jpeg)

#### CubeMX pin Congiguration ####

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/C-temp.png)




### Parking sensor ###



## Demo Videos ## 

* [Altrasonic video](https://drive.google.com/file/d/1EFL_sFO069n72mjgJbUG4CE1OKInkyMb/view?usp=sharing) 
* [Temperature application video 1](https://drive.google.com/file/d/1lmi-P5ynD7VPU8V_cYlm9Q4O6gKKZzPc/view?usp=sharing)
* [Temperature application video 2](https://drive.google.com/file/d/1lk94JaLEqaJnZBGcgPB9RU9fmD0oPsug/view?usp=sharing)

## Datasheets ## 
 * [Temperature](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/HC-SR04%20User%20Guide.pdf)
 * [Ultrasonic](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/DS3231%20datasheet.pdf)
