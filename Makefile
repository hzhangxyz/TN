coverage:
	g++ *.cpp --coverage -o a.out
	./a.out
	gcov *.cpp
