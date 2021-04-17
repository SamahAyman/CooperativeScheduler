#include "queue.h"

static struct task_queue ReadyQueue, DelayedQueue; 
static volatile uint16_t TickFlag = 0;

struct task dispatched_task;
task_p running_task_ptr;
uint16_t running_task_priority;
uint16_t running_task_delay;

////to simulate systick handler (ticking every 50ms)
//void timer()
//{
//	int counter = 0;
//	TickFlag = 0;
//	for (int i = 1; i <= 50 ; i++)
//		counter++; 
//	
//	if (counter == 50)
//	{
//		TickFlag = 1; 
//	}
//}

//adds the tasks and their priorities to the ready queue
void QueTask(task_p task_ptr, int priority)
{
	enqueue(&ReadyQueue, task_ptr, priority, 0);
}

//dispatch function implementation 
void Dispatch()
{
	struct task dispatched_task;
	
//	//calling the timer function will ensure decrementing the delay queue only every tick
//	timer();

//	//decrement the delays of the tasks in the delayed queue
//	if (TickFlag)
//		_delay(&DelayedQueue);

	//pushing all ready tasks from the delayed queue to the main queue
	push_to_main(&DelayedQueue, &ReadyQueue); 

	//popping tasks to be executed from the ready queue
	if (ReadyQueue.ind > 0)
	{
		dispatched_task = dequeue(&ReadyQueue);

		//pointing to the currently running task and its associated fields 
		running_task_ptr = dispatched_task.task;
		running_task_priority = dispatched_task.priority;
		running_task_delay = dispatched_task.delay;
		dispatched_task.task();
	}
}

//void ReRunMe function implementation 
void ReRunMe(int sleepingTime)
{
	//adding to the ReadyQueue if the sleeping time elapsed
	if (sleepingTime == 0)
		enqueue(&ReadyQueue, running_task_ptr, running_task_priority, sleepingTime);
	
	//adding to the DelayedQueue if ReRunMe has some +ve sleeping time
	else
		enqueue(&DelayedQueue, running_task_ptr, running_task_priority, sleepingTime);

}

//Task 1 function implementation 
void T1()
{
	printf("Task #1\n");
}

//Task 2 function implementation
void T2()
{
	printf("Task #2\n");
	ReRunMe(5);
}

//Task 3 function implementation 
void T3()
{
	printf("Task #3\n");
	ReRunMe(3); 
}

/*
int main ()
{
	queueINIT(&ReadyQueue);
	queueINIT(&DelayedQueue);

	QueTask(&T1, 1); 
	QueTask(&T2, 2);
	QueTask(&T3, 3);
	
	while (1) 
	{
		Dispatch();
	}

	system("pause"); 
	return 0; 
}
*/
