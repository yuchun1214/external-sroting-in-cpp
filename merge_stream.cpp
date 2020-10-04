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

    while(true){
        // merge when task1.size() && task2.size() > 0
        
        if(task1.size() > 0 && task2.size() > 0){
            t1 = task1.at(0);
            t2 = task2.at(0);
            if(t1.numbers == nullptr && t2.numbers == nullptr){
                // wait for join
                return;
            }
            else if(t1.numbers == nullptr && t2.numbers != nullptr){
                //
                // numbers take all of numbers from t2.numbers;
                task2.erase(task2.begin()); // pop one
                numbers = new int[maxChunkSize];
                for(j = t2.current, k = 0; j < t2.size; ++j){
                    numbers[k] = t2.numbers[j]; 
                    ++k;
                    haventOutputed = true;
                    if(k >= maxChunkSize){
                        haventOutputed = false;
                        os.output(numbers, k); 
                        numbers = new int[maxChunkSize];
                        k = 0;
                    }
                }
            }else if(t1.numbers != nullptr && t2.numbers == nullptr){
                // numbers take alll of numbers from t1.numbers 
                task1.erase(task1.begin());// pop one
                numbers = new int[maxChunkSize];
                for(i = t1.current, k = 0; i < t1.size; ++i){
                    numbers[k] = t1.numbers[i];
                    ++k;
                    haventOutputed = true;
                    if(k >= maxChunkSize){
                        haventOutputed = false;
                        os.output(numbers, k);
                        numbers = new int[maxChunkSize];
                        k = 0; 
                    } 
                } 
            }else if(t1.numbers != nullptr && t2.numbers != nullptr){
                // task1.erase(task1.begin()); // pop one
                // task2.erase(task2.begin()); // pop one
                i = t1.current;
                j = t2.current;
                numbers = new int[maxChunkSize];
                k = 0;
                while(i < t1.size && j < t2.size){
                    if(t1.numbers[i] <= t2.numbers[j]){
                        numbers[k] = t1.numbers[i]; 
                        ++i;
                        ++k;
                        haventOutputed = true;
                    }else{
                        numbers[k] = t2.numbers[j];
                        ++j;
                        ++k; 
                        haventOutputed = true;
                    }

                    if(k >= maxChunkSize){
                        haventOutputed = false;
                        os.output(numbers, k);
                        numbers = new int[maxChunkSize];
                        k = 0;
                    }
                }
                
                // restore or pop
                if(i >= t1.size){
                    delete[] t1.numbers;
                    task1.erase(task1.begin());
                }else{
                    task1[0].current = i; 
                }

                if(j >= t2.size){
                    delete[] t2.numbers;
                    task2.erase(task2.begin());
                }else{
                    task2[0].current = j; 
                }
            }

            if(haventOutputed){
                os.output(numbers, k); 
            }

        }
	}
	// delete [] numbers;
}

