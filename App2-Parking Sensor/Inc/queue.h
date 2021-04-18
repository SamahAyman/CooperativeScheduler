#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



static const uint16_t MAX_PRI = 8;    //for 8 priority queue
static const uint16_t MIN_PRI = 0;    //priority can't be less than 0 
static const uint16_t capacity = 16;  //capacity of the queue
uint16_t current_cap = 0;

typedef void(*task_p)(void); 
#define left_node(p) (2*p+1)
#define right_node(p) (2*p+2)
#define root(c) ((c-1)/2)

struct task 
{
	task_p task;
	uint16_t priority;
	uint16_t delay;
};
static const struct task dummy_f = {NULL, 1000, 1000};

struct task_queue 
{
	uint16_t cur_prio; 
	uint16_t max_prio; 
	uint16_t ind; 
	struct task * tasks; //pointer to the tasks in the task queue 
};

//utility function
int minimum(int a, int b)
{
	return (a < b ? a : b);
}
//utility function
void swap(struct task *task1, struct task *task2)
{
	struct task temp = *task1;
	*task1 = *task2;
	*task2 = temp;
}

void queueINIT(struct task_queue * q)
{
	q->cur_prio = 0;
	q->ind = 0;
	q->max_prio = MAX_PRI;
	/*dynamically allocate tasks into the queue */
	q->tasks = (struct task*)malloc(capacity * sizeof(struct task));
	/*initialize task queue with dummy tasks*/
	for (uint16_t i = 0; i < capacity; i++)
	{
		q->tasks[i] = dummy_f;
	}
}

uint8_t order(struct task a, struct task b)
{
	if (a.delay < b.delay) return 1;
	return (a.delay == b.delay) && (a.priority < b.priority);
}

void enqueue(struct task_queue* q, task_p f, uint16_t priority, uint16_t delay)
{
	volatile struct task new_task = {f, priority, delay};
	if (priority > MAX_PRI) 
	{
		printf("max priority reached\n");
		return;
	}
	if (priority < MIN_PRI) 
	{
		printf("priority violation\n");
		return;
	}
	if (q->ind > capacity) 
	{
		printf("queue if full\n");
		return;
	}
	q->tasks[q->ind++] = new_task;
	int i = (q->ind - 1);
	while (i != 0 && order(q->tasks[i], q->tasks[root(i)]) )
	{
		swap(&q->tasks[root(i)], &q->tasks[i]);
		i = root(i);
	}
}

// min heap for pirority queue
void min_heap(struct task_queue* q, int i)
{
	int small = i;
	int left = left_node(i);
	int right = right_node(i);
	// get the minimum (left or right or this node)
	if (left <= q->ind && order(q->tasks[left], q->tasks[small])) small = left;
	if (right <= q->ind && order(q->tasks[right], q->tasks[small])) small = right;
	// if this node is the minimum then left, right and this subtree are heapified
	if (small == i) return;
	// else put the small on top and heapify the affected tree
	swap(&q->tasks[small], &q->tasks[i]);
	min_heap(q, small);
}

// Dequeue function
struct task dequeue(struct task_queue* q)
{
	struct task ret = dummy_f;
	// Check for queue limits
	if (q->ind == 0) 
	{
		printf("ERROR! - Queue is empty\n");
		return ret;
	}
	// Take task reference and replace its reference with the last task in the queue
	ret = q->tasks[0];
	q->tasks[0] = q->tasks[q->ind - 1];
	// Decrease queue size
	q->ind--;
	// Put the last task (now at the top of the queue) in its correct place
	min_heap(q, 0);
	// return the desired task
	return ret;
}

//to decrement the delays of the tasks in the delay queue
void _delay(struct task_queue* q)
{
	for (uint8_t i = 0; i < q->ind; i++)
		if (q->tasks[i].delay > 0)
			q->tasks[i].delay--; 
}

//to push the ready tasks from the delay queue to the ready queue
void push_to_main(struct task_queue* delayed_q, struct task_queue* main_q)
{
	while (delayed_q->ind > 0 && delayed_q->tasks[0].delay == 0 && main_q->ind < capacity)
	{
		volatile struct task new_task = dequeue(delayed_q);
		main_q->tasks[main_q->ind++] = new_task;
		int i = (main_q->ind - 1);
		while (i != 0 && order(main_q->tasks[i], main_q->tasks[root(i)])) 
		{
			swap(&main_q->tasks[root(i)], &main_q->tasks[i]);
			i = root(i);
		}
	}
}


