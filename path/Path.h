#ifndef PATH_H
#define	PATH_H


#include <stdio.h>
#include <iostream>
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

    // What is the type a RO points to
    string pointingTo();

    //returns the type of any SO
    string getType();

    // returns the type of any BO
	Type getBOType();

    // Creates the access code for a path`
    vector<int> getAccessCode();

    // internal functions
    bool isConsistent();
    bool makeConsistent();
    void makeInconsistent();
	int setRef(Path &path);


public:
	bool isBO();
	bool isSO();
   	bool isRO();
	bool isList();

    //Return the type which is being pointed to
    //only if the path is RO() == true;
    string pointingType();
	bool removeObj();


    //Append functions
    Path append(Path &p);
    template<class T> int append(T *val, int size);

    // Add string component to a path object
	Path & operator+(string p);
	Path & operator=(const Path & path);

	//Vector encapsulates pathstring + access code + iBlob
	vector <PathComponent> vPath;

	//Count that counts the number of subobjects of the locator pointed to by a path
	int count();


    // Read functions. Throws exception if any error while reading
    template<class T> uint read(T &val);
    template<class T> uint read(T *&val, int &size);


    // Write functions: Throws exception if the insertion is out of order
    template<class T> int set(T &val);
    template<class T> int set(T *val, int size);

    // sets path to a reference object
    int set(Path &path);

    friend class TSS;

    void print()
    {
        cerr<<"...................."<<endl;
        cerr<<this->strPath<<endl;
        for(int i = 0; i < vPath.size(); i++)
        {
            cerr<<vPath[i].accessCode<<".";
        }
        cerr<<endl;
    }

};


////////////////////////////////////////////////////////////////////
////// Template functions has to be defined in the .h file /////////
////////////////////////////////////////////////////////////////////
template<class T>
int Path::set(T &val)
{
    if (this->isList())
        throw string("Error. Not a list");
    Locator l;
    try{
        l = gotoBO();
    }
    catch(...)
    {
        cerr<<"ERROR IN gotoBO"<<endl;
        return 0;
    }
    PathComponent *p = &vPath[vPath.size()-1];
    l = iblob->insertVal(val, l, p->accessCode);
    p->loc = l;
    this->consistent =  true;
    return 1;
}

template<class T>
int Path::set(T *val, int size)
{
    Locator l;
    if(!this->isList())
        throw string("Error. This is not a list");
    try{
        l = gotoBO();
    }
    catch(string s)
    {
        cerr<<s<<endl;
        return 0;
    }
    //vPath[vPath.size()-1].loc  = iblob->insert(l,
    //                                           vPath[vPath.size()-2].accessCode,
    //                                           OBJECT_LEVEL);
    vPath[vPath.size()-1].loc = l;
    for(int i = 0; i < size; i++)
    {
        iblob->insertVal(val[i], vPath[vPath.size()-1].loc, i);
    }

    this->consistent =  true;
    return 1;
}


template<class T>
uint Path::read(T &val)
{
    if(this->isList())
        throw string("Error. This is a list");
    cerr<<endl;
    if(!this->isConsistent())
        this->makeConsistent();
    if(this->isBO())
    {
        Locator l = this->vPath[vPath.size()-1].loc;
        if(iblob->readVal(val, l))
            return 1;
        else
            return -1;
    }
    else
    {
        throw string("Error in reading");
    }
}
template<class T>
uint Path::read(T *&val, int &size)
{
    if(!this->isConsistent())
        this->makeConsistent();
    if(this->isRO() || (this->isBO() && this->isList()))
    {
        Locator lTop = this->vPath[vPath.size()-1].loc;
        if(this->isRO())
            lTop = this->iblob->locate(lTop, vPath[vPath.size()-1].accessCode);
        size = lTop.getElements();
        val = new T[size];
        for(int i = 0; i < size; i++)
        {
            T tempval;
            Locator l = this->iblob->locate(lTop, i);
            if(iblob->readVal(tempval, l))
                val[i] = tempval;
            else
                return -1;
        }
    }
    else
    {
        throw string("Error reading array");
    }
    return 1;
}
template<class T>
int Path::append(T *val, int size)
{
    if(!this->isList())
        throw string("This is not a list.");
    if(!this->isBO())
        throw string("This is not BO");

    Locator l;
    try{
        l = gotoBO();
    }
    catch(string s)
    {
        cerr<<s<<endl;
        return 0;
    }
    int length = vPath[vPath.size() -1].loc.getElements();

    for(int i = 0; i < size; i++)
    {
        iblob->insertVal(val[i], vPath[vPath.size()-1].loc,length+i-1);
    }
    this->consistent =  true;
    return 1;

}
#endif
