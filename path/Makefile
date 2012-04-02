#assumes the existence of the following environment variables and their values
#ORACLE_HOME
    # Phoenix: /u01/app/oracle/product/11.2.0/dbhome_1
#RJNLIBS
    # Phoenix: /home/aist/mylibs
#SPAL_SRC
	# Boston: /home/tachen/workspace/AIST/MovingImp/includes
    # Phoenix: 

#Oracle envirement
	ORACLELIB = $(ORACLE_HOME)/lib
	ORACLE_LIB1 = $(ORACLE_HOME)/lib/
	ORACLE_LIB2 = $(ORACLE_HOME)/rdbms/lib
	ORACLE_INCLUDE = $(ORACLE_HOME)/rdbms/public 
	
# the linker is also "gcc". It might be something else with other compilers.
	CC = g++ -c
	LD = g++
# Compiler flags go here.
	CFLAGS = -ggdb -fpic -Wall
#locations maybe not in path to include
	INCLUDEIBLOB = -I ~/ranjan/includes/iBLOB_V2/iBLOB -I ~/ranjan/includes/iBLOB_V2/vector -I ~/ranjan/includes/OCILobWrapper -I $(ORACLE_INCLUDE)
# include OCILobWrapper
	INCLUDEOCIWRAPPER = -I ../OCILobWrapper
# use this command to erase files.
	RM = /bin/rm -f
#Linker flags go here.
	LDFLAGSSTAL = -L$(ORACLE_LIB1) -L$(ORACLE_LIB2) -L$(RJNLIBS) -limslobs 
#sources
	SRCTSS = Path.cc
	OBJPATH = path.o
#testpath
	TESTPATH = test.cc
	OTESTPATH = test.o

	OUTFILE = test.out

teststal:
	@echo "start testing path..."
	$(CC) $(CFLAGS) $(INCLUDEIBLOB)  $(SRCTSS)
	$(LD) -o $(OUTFILE) $(TESTPATH) $(OBJPATH) $(LDFLAGSSTAL)
	@echo "Done!"

#clean libs
clean_libs:
	rm $(RJNLIBS)/$(LIBSHARED)

clean:	
	-rm *.o
	-rm *.out
	-rm *.so
	-rm *.a
	
