#include <stdio.h>
#include <iostream>
//include all the iBLOB and Locator headers
#include "iBlob.h"
#include "OCILobWrapper.h"
#include "TSSParser.h"
#include <cstring>
#include <vector>
#include <string>

using namespace std;
//using namespace TSSPath;

//enum that stores the 'Data Types' that Base Objects can take
enum BO_DataType
{
	Int,
	IntAR,
	DoubleAR,
	Double,
	String,
	Byte
};

//struct used to hold path information :: building block of a PATH object
struct PathComponent
{
	string label;
	int access_code;
	Locator loc;
}; 

//TSS path class ... every path is stored as an instance of this class
class Path
{

private:
	//input path string
	string path_string;
	//Vector encapsulates pathstring + access code + iBlob 
	// locators -> gives total representation of the path
	vector <PathComponent> verbose_path;
	//TSS parser object
	TSSParser *TSSParserObj;

	// The blob pointer
	iBlob *iblob;
	
	//This will populate the locator fields for the components in the verbose_path vector (if they are empty when this function is called). This is done explicitly via this function and not in the constructor to defer the assignment of locators to when the Path object is actually used.
	void PopulateLocators();
	//Will concatenate the string 'p' to the current path_string that is encapsulated within the 'verbose_path' vector  and returns the new string.
	string GenNewPathString(string p);
	//Given an input path string it will blindly modify the exisitng path_string and verbose_path :: IT PERFROMS NO CHECK OF ITS OWN :: potentially dangerous, use carefully.
	void UpdatePaths(string new_p);

	//Default constructor
	Path ();
	//Constructor that takes an path string as input
	Path (string strPath, TSSParser *tp, iBlob *iblob);


public:
	//Will return true if path points to a Base Object , else return false
	bool isBO();
	//Will return true if path points to a Structured Object , else return false
	bool isSO();
	//Will return true if path points to a List , else return false
	bool isList();
	//If path points to a BO , then this function will return the DataType that the base object holds. the return value will be a member of the BO_DataType enum.
	//else the function will throw a bad read exception.
	BO_DataType getBOType();

	//Overloaded Operators, to make life a little easier for the TSS implementor (TSI)
	//Advantages:
	//1) TSI can deal purely with strings
	//2) Can be considered as a form of explicit caching
	//3) Will make iterations over Objects in the ADT heirarchy easy.
	
	//Allow users to concatenate strings to path objects -> the overloaded operator will take care of creating new verbose path (provided the new path is valid - this check is also performed)
	Path & operator+(string p);
	//Overloaded Assingment Operator to deal with Path Objects. (assigns a Path object to another Path Object.
	Path & operator=(const Path & path);
};
