#include <algorithm>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <cmath>
#include <thread>
#include <type_traits>
#include <vector>
#include <ctime>
#include "mergeTask.h"
#include "output_stream.h"
#include "task.h"
#include "merge_stream.h"

using namespace std;

struct chunkFile{
	FILE * file;
	vector<int> * readValues;
	unsigned int size;
	bool end;
	unsigned int current;
};

void readValue(chunkFile & cf, unsigned int chunkSize){
	cf.current = 0;
	unsigned int i =0;
	int readValue;
	bool hasRead = false;
	cf.readValues->clear();
	while(EOF != fscanf(cf.file, "%d", &readValue)){
		cf.readValues->push_back(readValue);
		++i;
		hasRead = true;
		if(i >= chunkSize){
			cf.size = i;
			return;
		}
	} 
	if(!hasRead){
		cf.size = i;
		cf.end = true;
	}else{
		cf.size = i;
	}
}



void merge(int numberOfChunks,int chunkSize){
	FILE * file = fopen("_output.txt", "w");
	char filename[50];
	// bool FILE_END[numberOfChunks];
	vector<bool> FILE_END;
	chunkFile * files = (chunkFile *)malloc(sizeof(chunkFile) * numberOfChunks);
	for(int i = 0; i < numberOfChunks; ++i){
		sprintf(filename, "chunk%d", i);
		files[i].file = fopen(filename, "r");
		files[i].readValues = new vector<int>;
		files[i].end = false;
		readValue(files[i], chunkSize);
		FILE_END.push_back(false);
	}

	
	bool CONTINUE_READ = true;
	int min;
	int min_position;
	bool exist;
	while(CONTINUE_READ){
		// search first value;
		exist = false;	
		for(int i = 0; i < numberOfChunks; ++i){
			if(!files[i].end){
				exist = true;
				min = files[i].readValues->at(files[i].current);
				break;
			}
		}

		if(!exist){
			break;
		}


		// find the min value and the position of min_value
		min_position = 0;
		for(int i = 0; i < numberOfChunks; ++i){
			if(!files[i].end && files[i].readValues->at( files[i].current ) <= min){
				min = files[i].readValues->at(files[i].current);
				min_position = i;
			}
		}

		files[min_position].current ++;
	
		// cout<<min<<endl;
		fprintf(file, "%d\n", min);

		// check if current >= size
		// yes -> read next chunk
		// no -> continue
		if(files[min_position].current >= files[min_position].size){
			readValue(files[min_position], chunkSize);
		}


		
	}
}

void sort_and_write(int * numbers, int size, int chunkNumber){
    sort(numbers, numbers + size);
    fstream file;
    file.open("chunk" + to_string(chunkNumber), ios_base::out);
    for(int i = 0; i < size; ++i){
        file<<numbers[i]<<endl; 
    } 
    file.close();
    delete[] numbers;
}


int main(int argc, const char * argv[]){
    time_t start, end;
    start = time(nullptr);
	FILE * file;
	file = fopen("rand.txt", "r");
    int readValue = 0;
    int chunkSize = atoi(argv[1]);
    int chunkNumber = 0;
    int * numbers = new int[chunkSize];
    int currentCount = 0;
    bool haventSort = false;

    vector<thread> threads;
    

    // clock_t time1 = clock();
    // cout<<"start"<<endl;

    while(EOF != fscanf(file, "%d\n", &readValue)){
        haventSort = true;
        numbers[currentCount++] = readValue;
        if(currentCount == chunkSize){
            threads.push_back(thread(sort_and_write,numbers, chunkSize, chunkNumber));
            haventSort = false;
            currentCount = 0;
            ++chunkNumber;
            numbers = new int[chunkSize];
        }
    }

    if(haventSort){
        threads.push_back(thread(sort_and_write, numbers, currentCount, chunkNumber));
        chunkNumber++;
    }

    for(vector<thread>::iterator it = threads.begin(); it != threads.end(); it++){
        it->join();
    }

    
    merge(chunkNumber, chunkSize);
    end = time(nullptr);
    double diff = difftime(end, start);
    printf("Time = %f\n", diff);
	return 0;
}
