#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <thread>
#include <vector>
#include <ctime>

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

void sub_sub_merge(int * numbers1, int * numbers2,int size1, int size2, int round, int chunkNumber, int chunkSize){
    int * numbers = new int[size1 + size2];
    int i, j, k;
    i = j = k = 0;
    while(i < size1 && j < size2){
        if(numbers1[i] <= numbers2[j]){
            numbers[k] = numbers1[i];
            ++i;
        }else if(numbers1[i] > numbers2[j]){
            numbers[k] = numbers2[j];
            ++j;
        } 
        ++k;
    }

    while(i < size1){
        numbers[k] = numbers1[i];
        ++i;
        ++k;
    }

    while(j < size2){
        numbers[k] = numbers2[j];
        ++j;
        ++k;
    }

    
    output(numbers, round, chunkNumber, size2 + size1);

    delete[] numbers;

}


void sub_merge(string filename1, string filename2,int round, int chunkNumber, int chunkSize){
    fstream file1;
    file1.open(filename1, ios_base::in);
    
    fstream file2;
    file2.open(filename2, ios_base::in);
    
    int readValue1, readValue2; 
    int i = 0;
    int * numbers1, * numbers2;
    int size1, size2;
    size1 = 0;
    size2 = 0;
    numbers1 = new int[chunkSize];
    numbers2 = new int[chunkSize];

    while(file1 >> readValue1){
        numbers1[i] = readValue1;
        ++i;
    }
    size1 = i;
    i = 0;
    while(file2 >> readValue2){
        numbers2[i] = readValue2;
        ++i; 
    }
    size2 = i;

    sub_sub_merge(numbers1, numbers2,size1, size2, round, chunkNumber, chunkSize);
        
    
    file1.close();
    file2.close(); 
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
    // fstream file;
    // file.open("rand.txt", ios_base::in);
    // int readValue = 0;
    // int chunkSize = atoi(argv[1]);
    // int chunkNumber = 0;
    // int * numbers = new int[chunkSize];
    // int currentCount = 0;
    // bool haventSort = false;

    // vector<thread> threads;
    // 

    // clock_t time1 = clock();
    // cout<<"start"<<endl;
    // while(file >> readValue){
    //     haventSort = true;
    //     numbers[currentCount++] = readValue;
    //     if(currentCount == chunkSize){
    //         threads.push_back(thread(sort_and_write,numbers, chunkSize, chunkNumber));
    //         // sort_and_write(numbers, chunkSize, chunkNumber);  
    //         haventSort = false;
    //         currentCount = 0;
    //         ++chunkNumber;
    //         numbers = new int[chunkSize];
    //     }
    // }

    // if(haventSort){
    //     // sort_and_write(numbers, currentCount, chunkNumber);
    //     threads.push_back(thread(sort_and_write, numbers, currentCount, chunkNumber));
    //     chunkNumber++;
    // }

    // for(vector<thread>::iterator it = threads.begin(); it != threads.end(); it++){
    //     it->join();
    // }

    // clock_t time2 =  clock();
    // cout<<"finish"<<endl;


    // // cout<<"Used Seconds = "<<(double)(time2 - time1) / ( (double)CLOCKS_PER_SEC )<<endl;
    // cout<<"Used Seconds = "<<(double)(time2 - time1) / ( (double)CLOCKS_PER_SEC * (double)threads.size() )<<endl;
    // 
    // return 0;

    merge(2, 100000);
}
