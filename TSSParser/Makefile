
CC = g++ -O2 -Wno-deprecated

tag = -i

ifdef linux
tag = -n
endif

test.out: Scanner.o TSSParser.o Path.o test.o
	$(CC) -o test.out Scanner.o TSSParser.o Path.o test.o -lfl
		

test.o: test.cc
	$(CC) -g -c test.cc


Scanner.o: Scanner.cc
	$(CC) -g -c Scanner.cc

TSSParser.o: TSSParser.cc
	$(CC) -g -c TSSParser.cc   
	
Path.o: Path.cc
	$(CC) -g -c Path.cc    
	

clean: 
	rm -f *.o
	rm -f *.out

