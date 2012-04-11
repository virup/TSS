#ifndef PATH_H
#define	PATH_H



#include <stdio.h>
#include <iostream>
//include all the iBLOB and Locator headers
#include "iBlob.h"
#include "OCILobWrapper.h"
#include "../TSSParser/TSSParser.h"
#include <cstring>
#include <vector>
#include <string>

using namespace std;

//struct used to hold path information :: building block of a PATH object
struct PathComponent
{
	string label;
	int accessCode;
	Locator loc;
};

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

    int readInt();
    double readDouble();
    string readString();
    uint readIntArray(int *intBuf, uint bufsize);
    uint readDoubleArray(double *doubleBuf, uint bufsize);
    uint readBinary(unsigned char *buf, uint bufsize);


    // Write functions: Throws exception if the insertion is out of order
    uint setInt(int intVal);
    uint setDouble(double doubleVal);
    uint setIntArray(int *intBuf, uint size);
    uint setDoubleArray(double *doubleBuf, uint size);
    uint setBinary(unsigned char *charBuf, uint size);


    // Add string component to a path object
	Path & operator+(string p);
	Path & operator=(const Path & path);

	//Vector encapsulates pathstring + access code + iBlob
	vector <PathComponent> vPath;
};
#endif
