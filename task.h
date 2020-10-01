#ifndef TASK_H
#define TASK_H

typedef struct{
	int *numbers;
	int size;
	int capacity;
	unsigned int order;

}Task;


bool task_comparator(Task t1, Task t2);

#endif
