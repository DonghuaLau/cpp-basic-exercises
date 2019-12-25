
FLAG=-D SRC_MAIN -g
CPP11=-pthread --std=c++11

#$@:
#	g++ $@.cpp $(FLAG) -o $@.out




test-1:
	g++ $@.cpp $(FLAG) -o $@.out

test-class-1:
	g++ $@.cpp $(FLAG) -o $@.out

test-class-2:
	g++ $@.cpp $(FLAG) -o $@.out

test-type-1:
	g++ $@.cpp $(FLAG) -o $@.out

smart-pointer:
	g++ $@-test.cpp $(FLAG) -o $@.out

thread-pool:
	g++ $@-test.cpp $(FLAG) $(CPP11) -o $@.out

lockfree-queue:
	g++ $@-test.cpp $(FLAG) $(CPP11) -o $@.out

string:
	g++ $@.cpp $(FLAG) $(CPP11) -o $@.out

cpp11:
	g++ $@.cpp --std=c++11 -o $@

clean:
	rm *.out

