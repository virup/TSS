
CC = g++ -O2 -Wno-deprecated

tag = -i

ifdef linux
tag = -n
endif

test.out: Scanner.o test.o
	$(CC) -o test.out Scanner.o test.o -lfl
		

test.o: test.cc
	$(CC) -g -c test.cc


Scanner.o: Scanner.cc
	$(CC) -g -c Scanner.cc
	

clean: 
	rm -f *.o
	rm -f *.out

