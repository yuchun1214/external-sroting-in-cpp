#include <fstream>
#include <iostream>

using namespace std;


int main(int argc, const char * argv[]){
    FILE * file;
    file = fopen(argv[1], "w");
    int readValue;
    int lastValue;
    fscanf(file, "%d\n", &readValue);
    int currentLine = 1;
    while(EOF != fscanf(file, "%d\n", &readValue)){
        if(readValue < lastValue){
            cerr<<"Error at Line"<< currentLine <<endl;
            exit(-1);
        } 
        lastValue = readValue;
        ++ currentLine;
    
    }
    cout<<"No Problem Occur"<<endl;
    return 0;
}
