#include "queue.h"

static struct task_queue ReadyQueue, DelayedQueue; 
static volatile uint16_t DelayCount = 0;
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
	//decrement the delays of the tasks in the delayed queue
	_delay(&DelayedQueue, DelayCount); 
	
	//pushing all ready tasks from the delayed queue to the main queue
	push_to_main(&DelayedQueue, &ReadyQueue); 

	//clearing the delay counter
	DelayCount = 0; 

	//popping tasks to be executed from the ready queue
	dispatched_task = dequeue(&ReadyQueue);
	//pointing to the currently running task and its associated fields 
	running_task_ptr = dispatched_task.task; 
	running_task_priority = dispatched_task.priority;
	running_task_delay = dispatched_task.delay;
}

//void ReRunMe function implementation 
void ReRunMe(int DelayCount)
{
	//adding to the ReadyQueue if the delay counter is cleared 
	if (DelayCount == 0)
		enqueue(&ReadyQueue, running_task_ptr, running_task_priority, DelayCount);
	
	//adding to the DelayedQueue if ReRunMe has some delay
	else 
		enqueue(&DelayedQueue, running_task_ptr, running_task_priority, DelayCount);
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
	ReRunMe(2);
}

//Task 3 function implementation 
void T3()
{
	printf("Task #3\n");
	ReRunMe(5); 
}

int main ()
{
	queueINIT(&ReadyQueue);

	QueTask(T1, 1); 
	QueTask(T2, 2);
	QueTask(T3, 3);

	Dispatch(); 

	system("pause"); 
	return 0; 
}