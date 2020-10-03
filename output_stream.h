#ifndef OUTPUT_STREAM_H
#define OUTPUT_STREAM_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "task.h"

using namespace std;

class OutputStream{
public:
	vector<Task> tasks;
	fstream outputFile;
public:
	OutputStream(string filename);
	void addTask(Task);
	void output(int *numbers,unsigned int size);
	static void test(vector<Task> &);
	static void start(vector<Task> & tasks, fstream & outputFile);
};


#endif
