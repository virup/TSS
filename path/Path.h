#ifndef PATH_H
#define	PATH_H


#include <stdio.h>
#include <iostream>
//include all the iBLOB and Locator headers
//#include "/home/aistdev/ranjan/includes/iBLOB_V2/iBLOB/iBlob.h"
//#include "/home/aistdev/ranjan/includes/iBLOB_V1/iBLOB/OCILobWrapper.h"
#include "../TSSParser/TSSParser.h"
#include "../engine/constants.h"
#include <cstring>
#include <vector>
#include <string>

using namespace std;

//TSS path class ... every path is stored as an instance of this class
class Path
{

private:
	//input path string
	string strPath;
	TSSParser *tp;

    // is the locators inside the vPath consistent?
    bool consistent;

	iBlob *iblob;

    // calculates the locators
	int PopulateLocators();

    // Create a new path by replacing the old path
	void UpdatePaths(string);

	//constructor
	Path ();
	Path (string strPath, TSSParser *tp, iBlob *iblob);


public:
	bool isBO();
	bool isSO();
   	bool isRef();
	bool isList();
	Type getBOType();
    	vector<int> getAccessCode();
    	bool isConsistent();
    	bool makeConsistent();
    	void makeInconsistent();

   	int readInt();// const;
    	double readDouble();// const;
    	string readString();// const;
    	int readIntArray(int *intBuf, uint bufsize);// const;
    	int readDoubleArray(double *doubleBuf, uint bufsize);// const;
    	int readBinary(unsigned char *buf, uint bufsize);// const;
	

    // Write functions: Throws exception if the insertion is out of order
    int setInt(int intVal);
    int setDouble(double doubleVal);
    int setIntArray(int *intBuf, uint size);
    int setDoubleArray(double *doubleBuf, uint size);
    int setBinary(unsigned char *charBuf, uint size);
	
	bool removeObj(uint idx);

    // Add string component to a path object
	Path & operator+(string p);
	Path & operator=(const Path & path);

	//Vector encapsulates pathstring + access code + iBlob
	vector <PathComponent> vPath;

    friend class TSS;
};
#endif
