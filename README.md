# CooperativeScheduler




```
   Embedded Systems Course Project 1
   
   Team names: 
   
   Israa Fahmy  & Samah Ayman 

```

# Project Description:
The objective of this short project is to develop a cooperative scheduler for embedded systems. This scheduler utilizes priority queues to enqueue and dequeue tasks and supports up to different 8 priority levels. The scheduler has a number of main functions to be utilized :  (QueTask), (ReRunMe), Dispatch(), Decrement(), and Systick Handler(). The discribtion of each function will be explained below. We can two queues. One reay queue and the other is delayed queue. Every tast can be inserted into the ready queue by other tasks, firing interrupt (ISRs) or by runnung itself through the ReRunMe() function. 



# Software Platforms #

* STM32CubeMX
* Keil μVision
* Visual Studio 
* TeraTerm 

# cooperative scheduler implementation #

## Different Queues## 

### Ready Queue ###
The queue is a struct that has a pointer to the tasks in the task queue, a pointer to its index and the used pirority levels. Each task appended in the task queue has a pointer to its priority, and delay. In ready queue delay is hard coded to be 0. We can enqueuing and qequeuing to and from the ready queue through using QueTask() and Dispatch() respectively.

### Delayed Queue ###
User use this queue to schedule a task through passing the delay neeed as a parameter in ReRunMe() function. The tasks are sorted in a descending order based on the delay value, which is decremented every 50 ms using SysTick Timer using DelayedQueueUpdate() function. Whenever the delay of the task at the head of the Delayed Queue is 0, it gets popped from the Delayed Queue and pushed to the Ready Queue. When delay of a task is 0, it get automatically pushed to the ready queue to be ready for execution. 



## main functions ##

###  QueTask() ###
This function takes 2 arguments: a pointer to the task and the priority of the task. Inside QueTask(), we call enqueue function which inserts the task into the ready queue with its priority and a default delay of 0 indicating that the task is ready to be dispatched once its turn in the ready queue comes. This function could be called inside the main function, other tasks, or the ISRs.

### Dispatch() ###
This function is used to dequeue tasks with the highest priority from the ready queue to run them. It checks if the queue is empty, it calls the IDLE state; if the ready queue is not empty, it dequeues from the queue. Dispatch function gets called once in the while(1) loop inside the main function, so every time unit (signaled by systick handler) dispatch function is called once.

### ReRunMe() ###
This is function is used to rerun certain tasks with specific time delays. It takes an integer delay argument: if the delay is 0, then the task is ready to be inserted into the ready queue; if the delay is greater than 0, then the task will be inserted into the delayed queue. 

### Decrement() ###
This function is called inside the systick handler to decrement the delays of the tasks inside the delayed queue by 1 each tick (each 50 ms). A counter is used inside it to count the number of ticks each time the systick handler throws an interrupt. 

### Systick Handler() ###
This function is called each time the systick throws an interrupt. The main purpose of this function is to track the time of execution of the program in time units. In addition, the ticks counter is incremented each time unit inside this function and the decrementing function is called to decrease the delays of the tasks in the delay queue by 1. Using the systick handler, timings of tasks scheduling and execution could be handled. 



# Test Cases #

## 1. Testing on visual studio ##
          
### Test (1): Running tasks based on correct priorities ###

The output shows that the tasks were enqueued correctly into the ready queue based on the higher priority task, then the system is idle once all tasks have been dispatched and the ready queue is empty. 

T1: Priority 4

T2: Priority 2

T3: Priority 3


![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Picture1.png)




### Test (2): Testing Rerunme(0) case ###

Task 1 was given the highest priority, then the task reruns itself after 0 ticks “Rerunme(0)”, so it runs forever as it always has the highest priority in the queue. The other tasks were given less priorities so it is expected that they starve because Task  keeps rerunning itself with its highest priority.

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Picture2.png)




### Test (3): Testing enqueuing and dequeuing tasks from the delayed queue according to their delays ###

Tasks were given the following priorities. Task 2 reruns itself after 5 ticks, while task 3 reruns itself after 3 ticks which means there will be 2 ticks where the system is idle before task 2 and task 3 keep repeating over and over.

T1: Priority 1 

T2: Priority 2 =>  ReRunMe(5)

T3: Priority 3 => ReRunMe(3)


![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Picture3.png)




### Test (4): Violating the priority limit case ###
T1: Priority 9 (violating max priority allowed) 

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Picture4.png)




### Test (5): Testing scheduling of 4 tasks ###

Tasks were given the following priorities. T1 reruns itself after 2 ticks, T2, reruns itself after 8 ticks, T3 reruns itself after 6 ticks, and T4 runs only once with the lowest priority. After T4 runs once, T1,T2, and T3 keep alternating forever, and the system is only idle if neither of the sleeping time of the 3 tasks is elapsed yet. 

T1: Priority 1 => ReRunMe(2)

T2: Priority 2 => ReRunMe(8)

T3: Priority 3 => ReRunMe(6)

T4: Priority 4 


![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Picture5.png)




### Test (6): Scheduling 4 tasks with one running Rerunme(0) ###

Tasks were given the following priorities:

T1: Priority 1 => ReRunMe(2)

T2: Priority 2 => ReRunMe(8)

T3: Priority 3 => ReRunMe(6)

T4: Priority 4 => ReRunMe(0)

The first 3 tasks rerun themselves after delays 2, 8, and 6 respectively. And task 4 reruns itself after 0 ticks which means it doesn’t get inserted into the delayed queue rather it gets inserted into the ready queue directly. Despite task 4 reruns itself after 0 ticks and it is always ready, it doesn’t run forever as in Test (2) because other tasks are ready in the queue with higher priorities. 


![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Picture6.png)




## 2. Testing on Keil uVision ##

Tasks were given the following priorities:
T1: Priority 1 
T2: Priority 2 => ReRunMe(5)
T3: Priority 3 => ReRunMe(3)
Task 2 reruns itself after 5 ticks while task 3 reruns itself after 3 ticks. 
Task 1 toggles the led with a delay of 300 ms, task 2 toggles the led with a delay of 100 ms, and task 3 toggles the led with a delay of 50 ms. 
So, as you can observe from the demo video: task 1 runs first, then task 2, then task 3, then both task 2 and task 3 keep running alternatively forever; the difference in the toggling frequency clarifies which task is currently running. 





# Applications #

## Ambient temperature monitor ##
The purpose of this application is to Read the ambient temperature using DS3231 RTC sensor (over I2C bus) every 30 sec, and then produce an alarm through an external LED flashing when the temperature exceeds a certain threshold that is given by the user through TeraTerm terminal emulator using an asynchronous serial link (UART2) connected via a USB-to-TTL module.

### Tasks ###
* *Initialization*: In this task we assign the registers to its corresponding values. 
* *Taking Threshold from user*: In this task we will display a message to the user to enter the threshold and then display this value on the TeraTerm screen. 
* *Taking Temperature reading*: In this task we utilized DS3231 module to measure the temperature. I2C communication was used to retrieve the sensor data and then UART communication was used to display the readings on TeraTerm. 
* *Alarm*: In this task we toggle a LED in case temperature readings exceeds the threshold. 

### Hardware Circuitry ###

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/temp.jpeg)

### CubeMX pin Congiguration ###

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/C-temp.PNG )



## Parking sensor: ##
This application simulates the parking sensor functionality where the ultrasonic sensor is used to measure the distance between the car and the nearest obstacle. Based on the distance measurement, a buzzer sound is produced with beeps frequency relative to the distance to reflect how close the is from the parking car.

### Tasks ###
* *Read_Task()* :This task is used to get measurements from the ultrasonic and calculate the distance. Through using the HAL timer, it gets the elapsed time. And then having the speed of the produced ultrasonic waves, it gets the travelled distance of the wave in the given time duration. 

* *BUZZ_Task()*: This function is used to trigger the buzzer to produce a sound relative to the measured distance. This function is only activated if the measured distance is less than a certain threshold, which is 30 in our case.



### Hardware Circuitry ####

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/Altra.jpeg)

#### CubeMX pin Congiguration ####

![alt text](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/C-altra.jpeg )


### Parking sensor ###



## Demo Videos ## 

* [Scheduling Demo](https://drive.google.com/file/d/1EFL_sFO069n72mjgJbUG4CE1OKInkyMb/view?usp=sharing) 
* [Temperature application video 1](https://drive.google.com/file/d/1lmi-P5ynD7VPU8V_cYlm9Q4O6gKKZzPc/view?usp=sharing)
* [Temperature application video 2](https://drive.google.com/file/d/1lk94JaLEqaJnZBGcgPB9RU9fmD0oPsug/view?usp=sharing)
* [Parking sensor video 1](https://drive.google.com/file/d/1GntsNmDrNC5mCMAs-2iiV9q1Jk420FmO/view?usp=sharing)
* [Parking sensor video 2](https://drive.google.com/file/d/1VXHlPhfbKJitq2ebi97IFjt1ompTH--m/view?usp=sharing)

## Datasheets ## 
 * [Temperature](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/HC-SR04%20User%20Guide.pdf)
 * [Ultrasonic](https://github.com/SamahAyman/CooperativeScheduler/blob/main/Images/DS3231%20datasheet.pdf)
