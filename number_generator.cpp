#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>

using namespace std;

int main(int argc, const char * argv[]){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(-2147483648,  2147483647);
    FILE * file;
    file = fopen("input.txt", "w");
    double max = atof(argv[1]);
    for(unsigned int i = 0; i < max; ++i){
        fprintf(file, "%d\n", dist(gen));
        cout<<(double)i / max<<endl; 
    }
}
