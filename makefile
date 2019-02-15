main:
	gcc -fopenmp -std=c++11 -o chop main.cpp graph.cpp transition_rule.cpp walk.cpp -I.

clean:
	rm -f chop