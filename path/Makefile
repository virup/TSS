CC = g++ -O2 -Wno-deprecated

tag = -i

ifdef linux
tag = -n
endif


test.out: Path.o test.o 
	$(CC) -o test.out Path.o test.o


test.o: test.cc
	$(CC) -g -c test.cc


Path.o: Path.cc
	$(CC) -g -c Path.cc


clean:
	rm -f *.o
	rm -f *.out
