#assumes the existence of the following environment variables and their values
#ORACLE_HOME
	# Boston: /opt/oracle/app/oracle/product/11.1.0/db_1 
    # Phoenix: /u01/app/oracle/product/11.2.0/dbhome_1
#RJNLIBS
	# Boston: /home/tachen/workspace/mylibs
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
#location of includes
	MODIPATH = /home/aistdev/MODImp/includes
	IBLOBPATH = /home/aistdev/ranjan/includes/
#locations maybe not in path to include
	INCLUDESTAL =  -I $(IBLOBPATH)/iBLOB_V2/vector -I $(MODIPATH)/realnum -I $(MODIPATH)/rgp -I $(MODIPATH)/stalTools -I $(MODIPATH)/spal2D -I $(IBLOBPATH)/iBLOB_V2/iBLOB -I $(ORACLE_INCLUDE)
# include OCILobWrapper
	INCLUDEOCIWRAPPER = -I ../OCILobWrapper
# use this command to erase files.
	RM = /bin/rm -f
#Linker flags go here.
	LDFLAGSSTAL = -L$(ORACLE_LIB1) -L$(ORACLE_LIB2) -L$(RJNLIBS)  -lirealnums -lirgps -limslobs -lispal2ds -listaltools -locci -lclntsh -llibstals

#location of Sources
	
	STAL_SRC_DIR = /home/aistdev/MODImp/includes/stal

#sources
	SRCSTAL = $(STAL_SRC_DIR)/interval.cpp $(STAL_SRC_DIR)/upoi.cpp $(STAL_SRC_DIR)/upoint.cpp $(STAL_SRC_DIR)/mpoint.cpp $(STAL_SRC_DIR)/undefinedObjectException.cpp $(STAL_SRC_DIR)stalException.cpp $(STAL_SRC_DIR)/mpointStructureException.cpp
	# SRCALGOS = mDir.cpp Algorithms_mpmp.cpp mPreds.cpp
	TSTSTAL = $(STAL_SRC_DIR)/teststal.cpp
	TSTALGOS = testalgos.cpp
	OBJSSTAL = interval.o upoint.o mpoint.o undefinedObjectException.o upoi.o mpointStructureException.o stalException.o
	# OBJSALGOS = mDir.o Algorithms_mpmp.o mPreds.o
#teststal
	OTSTALGOS = testalgos.o
	PROGALGOS = testalgos.out
	OTSTSTAL = teststal.o
	PROGSTAL = teststal.out
 
#STAL LIBS
	LIBSHARED = libistals.so
	
		
teststal:
	@echo "start testing stal..."
	$(CC) $(CFLAGS) $(INCLUDESTAL)  $(SRCSTAL)  $(TSTSTAL)
	$(LD) -o $(PROGSTAL) $(OBJSSTAL) $(OTSTSTAL) $(LDFLAGSSTAL)
	@echo "Done!"

testalgos:
	@echo "start testing algos..."
	$(CC) $(CFLAGS) $(INCLUDESTAL)  $(SRCSTAL) $(TSTALGOS) #$(#SRCALGOS) 
	$(LD) -o $(PROGALGOS) $(OBJSSTAL) $(OTSTALGOS) $(LDFLAGSSTAL) #$(#OBJSALGOS)
	@echo "Done!"



# first build the libistals.so by make libs, 
# then copy the libstal.so into ORACLELIB by make copy_db_libs
# -------------------------------
libs:
	@echo "*********** start compiling... **************"
	$(CC) $(CFLAGS) $(INCLUDESTAL) $(INCLUDEOCIWRAPPER) $(SRCSTAL) #$(#SRCALGOS)
	@echo "start linking..."
	$(LD) -shared -o $(LIBSHARED) $(OBJSSTAL) $(LDFLAGSSTAL) #$(#OBJSALGOS)
	cp $(LIBSHARED) $(RJNLIBS)
	@echo "*********** done compiling **************"
	
interval:
	@echo "*********** start compiling... **************"
	$(CC) $(CFLAGS) $(INCLUDESTAL) $(INCLUDEOCIWRAPPER) interval.cpp
	@echo "*********** done compiling **************"

upoi:
	@echo "*********** start compiling... **************"
	$(CC) $(CFLAGS) $(INCLUDESTAL) $(INCLUDEOCIWRAPPER) upoi.cpp
	@echo "*********** done compiling **************"

upoint:
	@echo "*********** start compiling... **************"
	$(CC) $(CFLAGS) $(INCLUDESTAL) $(INCLUDEOCIWRAPPER) upoint.cpp
	@echo "*********** done compiling **************"

mpoint:
	@echo "*********** start compiling... **************"
	$(CC) $(CFLAGS) $(INCLUDESTAL) $(INCLUDEOCIWRAPPER) mpoint.cpp
	@echo "*********** done compiling **************"


#clean libs
clean_libs:
	rm $(RJNLIBS)/$(LIBSHARED)

clean:	
	-rm *.o
	-rm *.out
	-rm *.so
	-rm *.a
	
