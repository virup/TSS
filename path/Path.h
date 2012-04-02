#include <stdio.h>
#include <iostream>
//include all the iBLOB and Locator headers
#include "iBlob.h"
#include "OCILobWrapper.h"
//#include "TSSParser.h"
#include <cstring>
#include <vector>
#include <string>

using namespace std;

//enum that stores the 'Data Types' that Base Objects can take
enum DataType
{
	INT,
	INTAR,
	DOUBLEAR,
	DOUBLE,
	STRING,
	BYTE
};

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
	//Vector encapsulates pathstring + access code + iBlob
	vector <PathComponent> vpath;
	TSSParser *tp;

    // is the locators inside the vPath consistent?
    bool isConsistent;

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
	bool isList();
	DataType getBOType();
    vector<int> getAccessCode();
    bool isConsistent();
    void makeInconstent();

    // Add string component to a path object
	Path & operator+(string p);

	Path & operator=(const Path & path);
};
