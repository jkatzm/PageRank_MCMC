main:
	gcc -fopenmp -std=c++1z -o chop main.cpp graph.cpp transition_rule.cpp walk.cpp -I.

clean:
	rm -f chop