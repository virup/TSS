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
#include "Locator.h"
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

    // Helper function to iterate to a BO
    Locator gotoBO();


    // Write functions: Throws exception if the insertion is out of order
    int setInt(int intVal);
    int setDouble(double doubleVal);
    int setIntArray(int *intBuf, uint size);
    int setDoubleArray(double *doubleBuf, uint size);
    int setBinary(unsigned char *charBuf, uint size);
	int setRef(Path &path);


public:
	bool isBO();
	bool isSO();
   	bool isRO();
	bool isList();
    string pointingTo();
    string getType();
	Type getBOType();
    vector<int> getAccessCode();
    bool isConsistent();
    bool makeConsistent();
    void makeInconsistent();

    // Read functions
    int readInt();// const;
    double readDouble();// const;
    string readString();// const;
    int readIntArray(int *intBuf, uint bufsize);// const;
    int readDoubleArray(double *doubleBuf, uint bufsize);// const;
    int readBinary(unsigned char *buf, uint bufsize);// const;

    //Return the type which is being pointed to
    //only if the path is RO() == true;
    string pointingType();
	bool removeObj();


    //Append functions
    Path append(Path &p);
    Path append(int);
    Path append(double);
    Path append(int [], int size);
    Path append(double [], int size);
    Path append(unsigned char *, int size);

    // Add string component to a path object
	Path & operator+(string p);
	Path & operator=(const Path & path);

	//Vector encapsulates pathstring + access code + iBlob
	vector <PathComponent> vPath;

	//Count that counts the number of subobjects of the locator pointed to by a path
	int count();


    // Added code to use the same set function for all the base object writing
    int set(int);
    int set(double);
    int set(int *, int size);
    int set(double *, int size);
    int set(unsigned char *, int size);
    int set(Path &path);

    friend class TSS;
};
#endif
