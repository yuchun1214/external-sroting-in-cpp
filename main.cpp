#include <algorithm>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <cmath>
#include <thread>
#include <vector>
#include <ctime>
#include "mergeTask.h"
#include "output_stream.h"
#include "task.h"
#include "merge_stream.h"

using namespace std;

void output(int * numbers, int round, int chunkNumber, int chunkSize){
    fstream fileout;
    fileout.open("chunk" + to_string(round) + "-" + to_string(chunkNumber), fstream::app); 
    for(int i = 0; i < chunkSize; ++i){
        fileout <<numbers[i]<<endl; 
    } 
    delete[] numbers;
    fileout.close();
}



void sub_merge(string filename1, string filename2,int round, int chunkNumber, int chunkSize){
	// cout<<"sub_merge"<<endl;

    fstream file1;
    file1.open(filename1, ios_base::in);
    
    fstream file2;
    file2.open(filename2, ios_base::in);
    
    int readValue1, readValue2; 
    int i = 0, j = 0;
    //vector<int>  numbers1,  numbers2;
    int *numbers1, *numbers2;
    int size1, size2;
    size1 = 0;
    size2 = 0;
	bool gotTheValue1, gotTheValue2;
	bool haventSort1, haventSort2;
	bool threadOpen = true;

	OutputStream os("chunk" + to_string(round) + "-" + to_string(chunkNumber));
	cout<<filename1<<" + "<<filename2<<" >> "<<"chunk" + to_string(round) + "-" + to_string(chunkNumber)<<endl;
	MergeConsumer mergeConsumer;
	MergeTask mt;
	thread t(mergeConsumer.merge, ref(threadOpen), ref(mergeConsumer.numbers_vector1), ref(mergeConsumer.numbers_vector2), chunkSize, ref(os));


	while(true){
		size1 = size2 = i = j =0;
		gotTheValue2 = gotTheValue1 = false;
		numbers1 = new int[chunkSize];
		numbers2 = new int[chunkSize];

		while(file1 >> readValue1){
			numbers1[i] = readValue1;
			gotTheValue1 = true;
			haventSort1 = true;
			++i;
			if(i >= chunkSize){
				haventSort1 = false;
				mt.numbers = numbers1;
				mt.size = i;
				mt.current = 0;
				mergeConsumer.numbers_vector1.push_back(mt);
		    	numbers1 = new int[chunkSize];
                // numbers1.clear();
				break;
			}
		}
		size1 = i;

		while(file2 >> readValue2){
			numbers2[j] = readValue2;
			gotTheValue2 = true;
			haventSort2 = true;
			++j;
			if(j >= chunkSize){
				haventSort2 = false;
				mt.numbers = numbers2;
				mt.size = j;
				mt.current = 0;
				mergeConsumer.numbers_vector2.push_back(mt);
				numbers2 = new int[chunkSize];
                // numbers2.clear();
				break;
			}
		}
		size2 = j;

		if(haventSort1 && gotTheValue1){
			mt.numbers = numbers1;
			mt.size = size1;
            mt.current = 0;
			numbers1 = new int[chunkSize];
            //numbers1.clear();
			mergeConsumer.numbers_vector1.push_back(mt);	
		}

		if(haventSort2 && gotTheValue2){
			mt.numbers = numbers2;
			mt.size = size2;
            mt.current = 0;
			numbers2 = new int[chunkSize];	
            //numbers2.clear();
			mergeConsumer.numbers_vector2.push_back(mt);
		}

		// mergeConsumer.merge(threadOpen, mergeConsumer.numbers_vector1, mergeConsumer.numbers_vector2,chunkSize, os);


		if(!gotTheValue1 && !gotTheValue2)
			break;
	}

    mt.numbers = nullptr;
    mergeConsumer.numbers_vector1.push_back(mt);
    mergeConsumer.numbers_vector2.push_back(mt);
	// mergeConsumer.merge(threadOpen, mergeConsumer.numbers_vector1, mergeConsumer.numbers_vector2, chunkSize, os);
	threadOpen = false;
	t.join();
			
    file1.close();
    file2.close(); 
    remove(filename1.c_str());
    remove(filename2.c_str());
}

void merge(int numberOfChunks, int chunkSize){
    int levels = ceil(log2(numberOfChunks));
    int canParseChunks;
    int i, j;
    bool isOdd;
    string fileHead;
    int chunkNumber = 0;
    char newFileName[100];
    char oldFileName[100];
    cout<<"levels = "<<levels<<endl;
    for(i = 0; i < levels; ++i){
        fileHead = "chunk" + to_string(i) + "-";
        // test number of chunks is odd or even
        if(numberOfChunks % 2 == 1){
           canParseChunks = numberOfChunks - 1; 
           isOdd = true;
        }else{
            isOdd = false;
            canParseChunks = numberOfChunks;
        }



        chunkNumber = 0; 
        // merge even numbers of chuncks
        for(j = 0; j + 1 < canParseChunks; j += 2){
            sub_merge(fileHead + to_string(j),  fileHead + to_string(j + 1), i + 1, chunkNumber++, chunkSize); 
        }

        // pass the last chuncks
        if(isOdd) {
            fstream file;
            fstream outFile;

            sprintf(oldFileName, "chunk%d-%d", i, numberOfChunks - 1);
            sprintf(newFileName, "chunk%d-%d",  i+1, chunkNumber);
            file.open(oldFileName, ios_base::in);
            outFile.open(newFileName, ios_base::out);
            int value;
            if(file.good() && outFile.good()){
                while(file >> value)
                    outFile << value<<endl;
            }
            // rename(oldFileName , newFileName);
            remove(oldFileName);
            numberOfChunks = chunkNumber + 1;
        }
        else numberOfChunks = chunkNumber;
    } 
}

void sort_and_write(int * numbers, int size, int chunkNumber){
    sort(numbers, numbers + size);
    fstream file;
    file.open("chunk" + to_string(0) + "-" + to_string(chunkNumber), ios_base::out);
    for(int i = 0; i < size; ++i){
        file<<numbers[i]<<endl; 
    } 
    file.close();
    delete[] numbers;
}


int main(int argc, const char * argv[]){
	// sub_merge("chunk0-0", "chunk0-1", 1, 0, 10);
	// sub_merge("chunk0-2", "chunk0-3", 1, 1, 10);
	// sub_merge("chunk0-4", "chunk0-5", 1, 2, 10);
	// sub_merge("chunk0-6", "chunk0-7", 1, 3, 10);
	// sub_merge("chunk0-8", "chunk0-9", 1, 4, 10);
	// sub_merge("chunk1-0", "chunk1-1", 2, 0, 10);
	// sub_merge("chunk1-2", "chunk1-3", 2, 1, 10);
	// sub_merge("chunk4-2", "chunk4-3", 5, 1, 10);
    time_t start, end;
    start = time(nullptr);
    fstream file;
    file.open("input.txt", ios_base::in);
    int readValue = 0;
    int chunkSize = 10000000;
    int chunkNumber = 0;
    int * numbers = new int[chunkSize];
    int currentCount = 0;
    bool haventSort = false;

    vector<thread> threads;
    

    // clock_t time1 = clock();
    // cout<<"start"<<endl;

    while(file >> readValue){
        haventSort = true;
        numbers[currentCount++] = readValue;
        if(currentCount == chunkSize){
            threads.push_back(thread(sort_and_write,numbers, chunkSize, chunkNumber));
            // sort_and_write(numbers, chunkSize, chunkNumber);  
            haventSort = false;
            currentCount = 0;
            ++chunkNumber;
            numbers = new int[chunkSize];
        }
    }

    if(haventSort){
        // sort_and_write(numbers, currentCount, chunkNumber);
        threads.push_back(thread(sort_and_write, numbers, currentCount, chunkNumber));
        chunkNumber++;
    }

    for(vector<thread>::iterator it = threads.begin(); it != threads.end(); it++){
        it->join();
    }

    // clock_t time2 =  clock();
    // cout<<"finish"<<endl;


    // cout<<"Used Seconds = "<<(double)(time2 - time1) / ( (double)CLOCKS_PER_SEC )<<endl;
    // cout<<"Used Seconds = "<<(double)(time2 - time1) / ( (double)CLOCKS_PER_SEC * (double)threads.size() )<<endl;
    
    merge(chunkNumber, chunkSize);
    end = time(nullptr);
    double diff = difftime(end, start);
    printf("Time = %f\n", diff);
	return 0;
}
