#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

void sub_merge(string filename1, string filename2,int round, int chunkNumber){
    fstream file1;
    file1.open(filename1, ios_base::in);
    
    fstream file2;
    file2.open(filename2, ios_base::in);
    
    fstream fileout;
    fileout.open("chunk" + to_string(round) + "-" + to_string(chunkNumber), ios_base::out); 
    int readValue1, readValue2; 
    bool read1, read2;
    read1 = read2 = true;
    while(true){
        if(read1 && read2){
            if(!(file1 >> readValue1))
                break;
            if(!(file2 >> readValue2))
                break;
        }else if(read1){
            if(!(file1 >> readValue1))
                break;
        }else if(read2){
            if(!(file2 >> readValue2))
                break;
        }

        if(readValue2 > readValue1){
            fileout << readValue1<<endl;
            read1 = true;
            read2 = false;
        }else{
            fileout << readValue2<<endl;
            read2 = true;
            read1 = false;
        }
    }
    if(read1)
        fileout << readValue2<<endl;
    else if(read2)
        fileout << readValue1<<endl;

    while(file1 >> readValue1){
        fileout << readValue1 << endl;
    }

    while(file2 >> readValue2){
        fileout << readValue2 << endl;
    }
    
    fileout.close();
}

void merge(int numberOfChunks){
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
            sub_merge(fileHead + to_string(j),  fileHead + to_string(j + 1), i + 1, chunkNumber++); 
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
