FLAGS=-Wall
TEST_FLAGS=$(FLAGS) -fprofile-arcs -ftest-coverage -fPIC
COV_FLAGS=-e 'catch.hpp|TestCases.cpp|BigInt.h'

test: BigInt.cpp Helper.cpp TestCases.o TestMain.o BigInt.h Helper.h
	g++ $(TEST_FLAGS) -o $@ BigInt.cpp Helper.cpp TestCases.o TestMain.o
	./test

coverage.html: test
	gcovr $(COV_FLAGS) --html . -o $@

coverage.txt: test
	gcovr $(COV_FLAGS) . -o $@

BigInt.a: BigInt.o Helper.o
	ar rcs $@ $^

BigInt.o: BigInt.cpp BigInt.h Helper.h
	g++ $(FLAGS) -c -o $@ $<

Helper.o: Helper.cpp Helper.h BigInt.h
	g++ $(FLAGS) -c -o $@ $<

TestCases.o: TestCases.cpp catch.hpp
	g++ $(TEST_FLAGS) -c -o $@ $<

TestMain.o: TestMain.cpp catch.hpp
	g++ $(TEST_FLAGS) -c -o $@ $<


clean:
	$(RM) test *.o 
	$(RM) *.gcda *.gcov *.gcno

cleanAll: clean
	$(RM) BigInt.a
	$(RM) coverage.txt coverage.html
