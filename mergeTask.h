#ifndef MERGE_TASK_H
#define MERGE_TASK_H

#include <vector>

typedef struct{
	// int * numbers;
	std::vector<int> numbers;
	int current;
	unsigned int size;
} MergeTask;

#endif
