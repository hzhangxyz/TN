coverage:
	g++ *.cpp --coverage -o a.out -g
	./a.out
	gcov *.cpp 1>/dev/null

clang:
	clang *.cpp
