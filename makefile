install: main.cpp
	g++ -std=c++20 -march=native -O3 main.cpp -o main

clean:
	rm -f main output.txt