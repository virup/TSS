
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
	@echo "Scanner begin ... " 
	$(CC) -g -c Scanner.cc
	@echo "Scanner Done ... "

TSSParser.o: TSSParser.cc
	@echo "Parser begin ..."
	$(CC) -g -c TSSParser.cc -I$(ORACLE_HOME)/rdbms/public -I/home/aistdev/ranjan/includes/iBLOB_V2/vector
	@echo "Parser done ..."

Path.o: ../TSSParser/TSS/Path.cc
	@echo "Path begin ... "
	$(CC) -g -c ../TSSParser/TSS/Path.cc -I$(ORACLE_HOME)/rdbms/public   
	@echo "Path Done ... "

clean: 
	rm -f *.o
	rm -f *.out

