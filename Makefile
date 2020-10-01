main:
	g++ -g -o main main.cpp

merge:
	g++ -g -o merge merge.cpp

gen:
	g++ -o gen number_generator.cpp

verify:
	g++ -o verify verify.cpp


clean:
	rm main
	rm merge
	rm gen
	rm verify
