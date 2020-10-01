#include "task.h"

bool task_comparator(Task t1, Task t2){
	return t1.order > t2.order;
}
