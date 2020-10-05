main:main.cpp output_stream.o task.o merge_stream.o
	g++ -g -pthread --std=c++14 -c main.cpp
	g++ -pthread --std=c++14 -g -O3 -o main main.o output_stream.o task.o merge_stream.o

merge_stream.o: merge_stream.cpp
	g++ -g -pthread --std=c++14 -c merge_stream.cpp


output_stream.o: output_stream.cpp
	g++ -c output_stream.cpp

task.o: task.cpp
	g++ -c task.cpp

merge:
	g++ -g -o merge merge.cpp

gen: number_generator.cpp
	g++ -o gen number_generator.cpp

verify:
	g++ -o verify verify.cpp

testing:test.cpp  output_stream.o task.o
	g++ -pthread --std=c++14 -c test.cpp
	g++ -pthread --std=c++14 -o testing test.o output_stream.o task.o


clean:
	rm main merge gen verify chunk* *.o
