#include "queue.h"

static struct task_queue ReadyQueue, DelayedQueue; 
static volatile uint16_t TickFlag = 0;
volatile int TickCount = 0;

struct task dispatched_task;
task_p running_task_ptr;
uint16_t running_task_priority;
uint16_t running_task_delay;

//adds the tasks and their priorities to the ready queue
void QueTask(task_p task_ptr, int priority)
{
	enqueue(&ReadyQueue, task_ptr, priority, 0);
}

//dispatch function implementation 
void Dispatch()
{
	struct task dispatched_task;

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

//to decrement the delays in the delayed queue each 50 ticks
void decrement()
{
	TickCount++; 
	if (TickCount == 50)
	{
		_delay(&DelayedQueue);
		TickCount = 0; 
	}
}

