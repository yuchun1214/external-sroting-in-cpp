#include <fstream>
#include <iostream>

using namespace std;


int main(int argc, const char * argv[]){
    fstream file;
    file.open(argv[1], ios_base::in);
    int readValue;
    int lastValue;
    file >> lastValue;
    int currentLine = 1;
    while(file>>readValue){
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
