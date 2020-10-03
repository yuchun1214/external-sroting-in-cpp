#ifndef MERGE_STREAM_H
#define MERGE_STREAM_H
#include <vector>
#include <thread>
#include "mergeTask.h"
#include "task.h"
#include "output_stream.h"

using namespace std;

class MergeConsumer{
public:
	vector<MergeTask> numbers_vector1;
	vector<MergeTask> numbers_vector2;

	static void merge(bool &, vector<MergeTask> &, vector<MergeTask> &,unsigned int maxChunkSize, OutputStream & os);

};

#endif
