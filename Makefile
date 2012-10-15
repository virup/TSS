#Oracle envirement
	#ORACLELIB = $(ORACLE_HOME)/lib
	ORACLE_LIB1 = $(ORACLE_HOME)/lib/
	ORACLE_LIB2 = $(ORACLE_HOME)/rdbms/lib
	ORACLE_INCLUDE = $(ORACLE_HOME)/rdbms/public
	RANJAN_ROOT = /home/aistdev/ranjan/includes
	TSS_ROOT = /home/aistdev/TSS
	TSS_ALL = $(TSS_ROOT)/engine $(TSS_ROOT)/path $(TSS_ROOT)/TSSParser

# the linker is also "gcc". It might be something else with other compilers.
	CC = g++ -g -c
	LD = g++ -g

# Compiler flags go here.
	CFLAGS = -ggdb -fpic -Wall

#locations maybe not in path to include
	INCLUDEIBLOB = -I $(RANJAN_ROOT)/iBLOB_V3/iBLOB -I $(RANJAN_ROOT)/iBLOB_V3/vector -I $(RANJAN_ROOT)/OCILobWrapper
	INCLUDE_TSS = -I $(TSS_ROOT)/engine -I $(TSS_ROOT)/path -I$(TSS_ROOT)/TSSParser

#include headers from the oracle_include
	INCLUDEORACLEHEADERS = -I $(ORACLE_INCLUDE)

#include UDT
	INCLUDE_UDT = -I $(TSS_ROOT)/UDT

#include OBJLOC
	INCLUDE_OBJLOC = -I $(TSS_ROOT)/ObjLoc

# use this command to erase files.
	RM = /bin/rm -f

#Linker flags go here.
	LDFLAGSSTAL = -L$(ORACLE_LIB1) -L$(ORACLE_LIB2)  -L$(RJNLIBS) -liblob -locci

#sources
	TSSPARSER = TSSParser/TSSParser.cpp
	SRCTSS = path/Path.cpp $(TSSPARSER) engine/TSS.cpp UDT/UDT.cpp ObjLoc/ObjLocOracle.cpp
	OBJPATH = Path.o TSSParser.o TSS.o UDT.o ObjLoc.o

#test related components
	TESTSRC = test.cpp
	TESTOBJ = test.o
	TESTSEGMENTOBJ = testsegment.o
	TESTSEGMENTSRC = testsegment.cpp

#testpath
	TESTPATH = test.cpp
	OTESTPATH = test.o
	OUTFILE = test.out
	SEGMENTFILE = segment.out


#test.out: $(OBJPATH) $(TESTOBJ)
#	@echo "start LINKING and LOADING"
#	$(LD) -o $(OUTFILE) $(TESTOBJ) $(ORACLELIB) $(OBJPATH) $(LDFLAGSSTAL)
#	@echo "-----------------Done!-------------------"
#
#
#segment.out: $(OBJPATH) Segment.o $(TESTSEGMENTOBJ) $(TESTOBJ)
#	@echo "start LINKING and LOADING"
#	$(LD) -o $(SEGMENTFILE) $(TESTSEGMENTOBJ) Segment.o $(ORACLELIB) $(OBJPATH) $(LDFLAGSSTAL)
#	@echo "-----------------segment Done!-------------------"

stresssegment.out: $(OBJPATH) stressTestSegment.o Segment.o $(TESTSEGMENTOBJ) $(TESTOBJ)
	@echo "start LINKING and LOADING stress test"
	$(LD) -o stresssegment.out  stressTestSegment.o Segment.o $(ORACLELIB) $(OBJPATH) $(LDFLAGSSTAL) -I segment/
	@echo "-----------------Stress segment Done!-------------------"

TSSParser.o: $(TSSPARSER)
	@echo "TSSParser begin compilation ..."
	$(CC) $(CFLAGS) $(INCLUDEIBLOB) $(INCLUDEORACLEHEADERS) $(TSSPARSER)
	@echo "TSSParser done"

test.o: test.cpp
	@echo "Test file begin compilation ..."
	$(CC) $(CFLAGS) $(TESTSRC) $(INCLUDEIBLOB) $(INCLUDEORACLEHEADERS) $(INCLUDE_TSS) test.cpp
	@echo "Test File Done ..."

Path.o: path/Path.cpp path/Path.h
	@echo "Path begin compilation ..."
	$(CC) $(CFLAGS) $(INCLUDEIBLOB)  $(INCLUDEORACLEHEADERS) path/Path.cpp
	@echo "Path Done!"


TSS.o:engine/TSS.cpp
	@echo "TSS Engine begin compilation..."
	$(CC) $(CFLAGS) $(INCLUDEIBLOB) $(INCLUDEORACLEHEADERS)  $(INCLUDE_TSS) engine/TSS.cpp
	@echo "TSS Engine Done "

ObjLoc.o:ObjLoc/ObjLocOracle.cpp
	@echo "ObjLoc begin compilation..."
	$(CC) -o ObjLoc.o  $(CFLAGS) $(INCLUDEIBLOB) $(INCLUDEORACLEHEADERS) $(INCLUDE_OBJLOC) $(INCLUDE_UDT) $(ORACLELIB) ObjLoc/ObjLocOracle.cpp
	@echo "ObjLoc Done "

UDT.o:UDT/UDT.cpp
	@echo "UDT begin compilation..."
	$(CC) $(CFLAGS) $(INCLUDEIBLOB) $(INCLUDEORACLEHEADERS) $(INCLUDE_UDT) UDT/UDT.cpp
	@echo "UDT Done "

Segment.o:segment/Segment.cpp
	@echo "segment begin compilation..."
	$(CC) $(CFLAGS) $(INCLUDEIBLOB) $(INCLUDEORACLEHEADERS) $(INCLUDE_OBJLOC) $(INCLUDE_UDT) $(INCLUDE_TSS) segment/Segment.cpp
	@echo "segment Done "

testsegment.o: testsegment.cpp
	@echo "Test file begin compilation ..."
	$(CC) $(CFLAGS) $(testsegment) $(INCLUDEIBLOB) $(INCLUDEORACLEHEADERS) $(INCLUDE_OBJLOC) $(INCLUDE_UDT) $(INCLUDE_TSS) -I segment/ testsegment.cpp
	@echo "Test segment File Done ..."

stressTestSegment.o: stressTestSegment.cpp
	@echo "Test file begin compilation ..."
	$(CC) $(CFLAGS) stressTestSegment.cpp  $(INCLUDEIBLOB) $(INCLUDEORACLEHEADERS) $(INCLUDE_OBJLOC) $(INCLUDE_UDT) $(INCLUDE_TSS) -I segment/ testsegment.cpp
	@echo "Stress Test File Done ..."

clean:
	-rm *.o
	-rm *.out
