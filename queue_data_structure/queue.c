

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


static const uint16_t MAX_PRI = 8; //for 8 priority queue
static const uint16_t MIN_PRI = 0; //priority can't be less than 0 
static const uint16_t capacity = 16; //capacity of the queue
uint16_t current_cap = 0;

typedef void(*task_p)(void); ///typedef 
#define left_node(p) (2*p+1)
#define right_node(p) (2*p+2)
#define root(c) ((c-1)/2)
//#define min(a,b) (a<b?a:b)

struct task {
	task_p task;
	uint16_t priority;
	uint16_t delay;
};
static const struct task dummy_f = { NULL, 1000, 1000 };

struct task_queue {
	uint16_t cur_prio; //
	uint16_t max_prio; //
	uint16_t ind; //
	struct task * tasks; //pointer to the tasks in the task queue 
};

int minimum(int a, int b)
{
	return (a < b ? a : b);
}

void push_to_main(struct task_queue* delayed_q, struct task_queue* main_q);
uint8_t order(struct task a, struct task b)
{
	if (a.delay < b.delay) return 1;
	return (a.delay == b.delay) && (a.priority < b.priority);
}

void swap(struct task *task1, struct task *task2) {
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

void enqueue(struct task_queue* q, task_p f, uint16_t priority)
{

	volatile struct task new_task = { f, priority, 0 };
	if (priority > MAX_PRI) {
		printf("Max priority reached");
		return;
	}
	if (priority < MIN_PRI) {
		printf(" priority violation");
		return;
	}
	if (q->ind > capacity) {
		printf(" queue if full ");
		return;
	}
	q->tasks[q->ind++] = new_task;
	int i = (q->ind - 1);
	while (i != 0 && order(q->tasks[i], q->tasks[root(i)])) {
		swap(&q->tasks[root(i)], &q->tasks[i]);
		i = root(i);
	}
}


// Max heap for pirority queue
void min_heap(struct task_queue* q, int i)
{
	int small = i;
	int left = left_node(i);
	int right = right_node(i);
	// get the minimum (left or right or this node)
	if (left <= q->cur_prio && order(q->tasks[left], q->tasks[small])) small = left;
	if (right <= q->cur_prio && order(q->tasks[right], q->tasks[small])) small = right;
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
	if (q->ind == 0) {
		printf("ERROR! - Queue is empty");
		return ret;
	}
	// Take task reference and replace its reference with the lasr task in the queue
	ret = q->tasks[0];
	q->tasks[0] = q->tasks[q->ind - 1];
	// Decrease queue size
	q->ind--;
	// Put the last task (now at the top of the queue) in its correct place
	min_heap(q, 0);
	// return the desired task
	return ret;
}


void delay(struct task_queue* q, uint16_t delay)
{
	for (uint8_t i = 0; i < q->cur_prio; i++)
		q->tasks[i].delay -= minimum(delay, q->tasks[i].delay);
}

// Pushes all ready tasks from the delayed queue to the main queue
// Guarded with 'cur_sz' --> no need for external guard
void push_to_main(struct task_queue* delayed_q, struct task_queue* main_q)
{
	while (delayed_q->ind > 0 && delayed_q->tasks[0].delay == 0 && main_q->ind < capacity)
	{
		volatile struct task new_task = dequeue(delayed_q);

		main_q->tasks[main_q->ind++] = new_task;
		int i = (main_q->ind - 1);
		while (i != 0 && order(main_q->tasks[i], main_q->tasks[root(i)])) {
			swap(&main_q->tasks[root(i)], &main_q->tasks[i]);
			i = root(i);
		}
	}


}

static void f1()
{
	uint8_t tst[] = "ONE\n";
	printf(tst);

}
static void f2()
{
	uint8_t tst[] = "TWO\n";
	printf(tst);

}


//testing//

static struct task_queue main_queue;

int main()
{
	queueINIT(&main_queue);
	enqueue(&main_queue, &f1, 1, 0);
	enqueue(&main_queue, &f2, 2, 0);
	dequeue(&main_queue);
	struct task new_task = (dequeue(&main_queue));
	dequeue(&main_queue);
	






	system("pause");
}
