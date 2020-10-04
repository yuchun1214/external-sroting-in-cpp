#include "merge_stream.h"
#include "mergeTask.h"
#include "output_stream.h"
#include "task.h"

void MergeConsumer::merge(bool & open, vector<MergeTask> & task1, vector<MergeTask> & task2, unsigned int maxChunkSize, OutputStream &os){
	MergeTask t1, t2;
	unsigned int i, j, k, l;
	i = j = k = l =0;
	int * numbers = new int[maxChunkSize];
	bool haventOutputed = false;


	if(open){
		if(task1.size() == 0 && task2.size() > 0){
			k = 0;
			for(unsigned int i = 0; i < task2.size(); ++i){
				t2 = task2.at(i);
				for(unsigned int j = t2.current; j < t2.size; ++j){
					numbers[k] = t2.numbers[j];
					++k;
					haventOutputed = true;
					if(k >= maxChunkSize){
						os.output(numbers, k);
						k = 0;
						haventOutputed = false;
					}
				}
			}
			if(haventOutputed){
				os.output(numbers, k);
			}
			task2.clear();
			return;
		}
		
		if(task2.size() == 0 && task1.size() > 0){
			k = 0;
			for(unsigned int i = 0; i < task1.size(); ++i){
				t1 = task1.at(i);
				for(unsigned int j = t1.current; j < t1.size; ++j){
					numbers[k] = t1.numbers[j];
					++k;
					haventOutputed = true;
					if(k >= maxChunkSize){
						os.output(numbers, k);
						k = 0;
						haventOutputed = false;
					}
				}
			}
			if(haventOutputed){
				os.output(numbers, k);
			}
			task1.clear();
			return;
		}

		if(task2.size() == 0 && task1.size() == 0)
			return;

		t1 = task1.at(0);
		t2 = task2.at(0);
		i = t1.current;
		j = t2.current;
		k = 0;
		while(i < t1.size && j < t2.size){

			if(t1.numbers[i] < t2.numbers[j]){
				numbers[k] = t1.numbers[i];
				++i;
				++k;
			}else{
				numbers[k] = t2.numbers[j];
				++j;
				++k;
			}

			if(k >= maxChunkSize){
				l = 0;
				os.output(numbers, maxChunkSize);
				k = 0;
			}

			if(j >= t2.size && task2.size() > 1){
				task2.erase(task2.begin());
				t2 = task2[0];
				j = 0;
			}

			if(i >= t1.size && task1.size() > 1){
				task1.erase(task1.begin());
				t1 = task1[0];
				i = 0;
			}
		}

		// restore and pop
		if(i >= t1.size){
			task1.erase(task1.begin());
		}else{
			task1[0].current = i;
		}	

		if(j >= t2.size){
			task2.erase(task2.begin());
		}else{
			task2[0].current = j;
		}

		os.output(numbers, k);

		
	}	
	delete [] numbers;
}

