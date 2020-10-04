#include "output_stream.h"
#include "task.h"
#include <iostream>

OutputStream::OutputStream(string filename){
	outputFile.open(filename, ios_base::out);
	if(outputFile.bad()){
		cerr<<"Error in open output file"<<endl;
	}

}

void OutputStream::addTask(Task task){
	tasks.push_back(task);
}

void OutputStream::test(vector<Task> &){
	cout<<"testing"<<endl;
}

void OutputStream::output(int *numbers,unsigned int size){
	for	(unsigned int i = 0; i < size; ++i){
    	// cout<<numbers[i]<<endl;
		outputFile<<numbers[i]<<endl;
	}
	delete[] numbers;
}

void OutputStream::start(vector<Task> & tasks, fstream & outputFile){
	Task t;
	// while(true){
		if(tasks.size()){
			cout<<"start outputting"<<endl;
			sort(tasks.begin(), tasks.end(), task_comparator);
			t = tasks[0];
			if(t.numbers == nullptr){
				return;
				// break;
			}
			tasks.erase(tasks.begin());
			for(int i = 0; i < t.size; ++i){
				outputFile<<t.numbers[i]<<endl;	
			}
			delete[] t.numbers;
			cout<<"finish output"<<endl;
		}	

	// }		
}


