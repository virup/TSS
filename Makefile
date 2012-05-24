#Oracle envirement
	#ORACLELIB = $(ORACLE_HOME)/lib
	ORACLE_LIB1 = $(ORACLE_HOME)/lib/
	ORACLE_LIB2 = $(ORACLE_HOME)/rdbms/lib
	ORACLE_INCLUDE = $(ORACLE_HOME)/rdbms/public
	RANJAN_ROOT = /home/aistdev/ranjan/includes

# the linker is also "gcc". It might be something else with other compilers.
	CC = g++ -g -c
	LD = g++ -g

# Compiler flags go here.
	CFLAGS = -ggdb -fpic -Wall

#locations maybe not in path to include
	INCLUDEIBLOB = -I $(RANJAN_ROOT)/iBLOB_V2/iBLOB -I $(RANJAN_ROOT)/iBLOB_V2/vector -I $(RANJAN_ROOT)/OCILobWrapper

#include headers from the oracle_include
	INCLUDEORACLEHEADERS = -I $(ORACLE_INCLUDE)

# use this command to erase files.
	RM = /bin/rm -f

#Linker flags go here.
	LDFLAGSSTAL = -L$(ORACLE_LIB1) -L$(ORACLE_LIB2)  -L$(RJNLIBS) -limslobs -locci

#sources
	SRCTSS = path/Path.cc TSSParser/TSSParser.cc TSSParser/Scanner.cc engine/TSS.cpp
	OBJPATH = Path.o TSSParser.o TSS.o

#test related components
	TESTSRC = test.cpp
	TESTOBJ = test.o

#testpath
	TESTPATH = test.cpp
	OTESTPATH = test.o
	OUTFILE = test.out


test.out: $(OBJPATH) $(TESTOBJ)
	@echo "start LINKING and LOADING"
	$(LD) -o $(OUTFILE) $(TESTOBJ) $(ORACLELIB) $(OBJPATH) $(LDFLAGSSTAL) Scanner.o
	@echo "-----------------Done!-------------------"

TSSParser.o: TSSParser/TSSParser.cc TSSParser/Scanner.cc
	@echo "TSSParser begin compilation ..."
	$(CC) $(CFLAGS) $(INCLUDEIBLOB) $(INCLUDEORACLEHEADERS) TSSParser/TSSParser.cc TSSParser/Scanner.cc
	@echo "TSSParser done"

test.o: test.cpp
	@echo "Test file begin compilation ..."
	$(CC) $(CFLAGS) $(TESTSRC) $(INCLUDEIBLOB) $(INCLUDEORACLEHEADERS)
	@echo "Test File Done ..."

Path.o: path/Path.cc
	@echo "Path begin compilation ..."
	$(CC) $(CFLAGS) $(INCLUDEIBLOB)  $(INCLUDEORACLEHEADERS) path/Path.cc
	@echo "Path Done!"

TSS.o:engine/TSS.cpp
	@echo "TSS Engine begin compilation..."
	$(CC) $(CFLAGS) $(INCLUDEIBLOB) $(INCLUDEORACLEHEADERS) engine/TSS.cpp
	@echo "TSS Engine Done "


#clean libs
#clean_libs:
	#rm $(RJNLIBS)/$(LIBSHARED)


clean:
	-rm *.o
	-rm *.out
	-rm *.so
	-rm *.a
