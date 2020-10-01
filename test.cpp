#include <iostream>
#include <locale>
#include <string>
#include <thread>
#include <vector>
#include "task.h"
#include "output_stream.h"


using namespace std;


int main(int argc, const char * argv[]){
		
	OutputStream os("test.txt");
	vector<Task> ts;
	Task testTask;
	testTask.numbers = new int[10000000];
	testTask.order = 0;
	testTask.size = 10000000;
	// thread t(OutputStream::test, ref(os.tasks));
	thread t(os.start, std::ref(os.tasks), std::ref(os.outputFile)); 
	for(int i = 0; i < 10000000; ++i){
		testTask.numbers[i] = i;
	}
	cout<<"push back"<<endl;
	os.tasks.push_back(testTask);
	
	cout<<"keep taking numbers"<<endl;
	testTask.numbers = new int[10000000];
	for(int i = 10000000; i > 0; --i)
		testTask.numbers[i] = i;

	testTask.order = 1;
	cout<<"push back part2"<<endl;
	os.tasks.push_back(testTask);
	testTask.numbers = nullptr;
	os.tasks.push_back(testTask);
	t.join();	
}
