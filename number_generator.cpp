#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, const char * argv[]){
   srand(time(NULL));
   fstream f;
   f.open("rand.txt", ios::out);
   double max = atof(argv[1]);
   for(unsigned int i = 0; i < max; ++i){
      f<<rand()<<endl;
      cout<<(double)i / max<<endl; 
   }
}
