main:
	/usr/tce/packages/gcc/gcc-8.1.0/bin/g++ -std=c++17 -o chop main.cpp graph.cpp transition_rule.cpp walk.cpp -I.

clean:
	rm -f chop *.core