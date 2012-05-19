
#ifndef TSS_CPP
#define TSS_CPP

#include "TSS.h"
#include "../path/Path.h"
#include "../TSSParser/TSSParser.h"
#include "constants.h"

TSS::TSS(const char *tssfile, void *mylob, void *svchp, void *errhp,  string type="default")
{
    this->tp = new TSSParser(tssfile, false);
    this->type = type;
    this->lobWrapper = new OCILobWrapper;
    this->lobWrapper->lob=(OCILobLocator *)mylob;
    this->lobWrapper->cntxt=(OCISvcCtx*)svchp;
    this->lobWrapper->errhp=(OCIError*)errhp;
    this->iblob = (iBlob *)(new mSlob(lobWrapper,true));
}

//May need to write more for this
TSS::TSS(const char* tssfile, bool isFile)
{
	this->tp = new TSSParser(tssfile, false);
}

TSS::TSS()
{}

TSS::~TSS()
{}

//IMPLEMENT
void TSS::setTypeSpec(const char* tssfile, bool isFile)
{
	this->type = tp->getGrammarType();
}

//IMPLEMENT
void TSS::setStoreHandle(void* b, bool ismem)
{}

Path TSS::createPath()
{
    string str;
    Path p(str,(this->tp), (this->iblob));
    //Path p(str);
    return p;
}


Path TSS::createPath(string strPath)
{
   Path p(strPath, (this->tp), (this->iblob));
   //Path p(strPath);
   return p;
}

//IMPLEMENT
Path TSS::set(Path &path, string name, bool isList)
{
	return path;
}

int TSS::readInt(Path &path)
{
    return path.readInt();
}

double TSS::readDouble(Path &path)
{
    return path.readDouble();
}

string TSS::readString(Path &path)
{
    return path.readString();
}

uint TSS::readIntArray(Path &path, int* intBuf, uint bufsize)
{
    return path.readIntArray(intBuf, bufsize);
}


int TSS::readDoubleArray(Path &path, double* doubleBuf, uint bufsize)
{
    return path.readDoubleArray(doubleBuf, bufsize);
}
int TSS::readBinary(Path &path, unsigned char* doubleBuf, uint bufsize)
{
    return path.readBinary(doubleBuf, bufsize);
}

Path TSS::setInt(Path &path, int value)
{
     path.setInt(value); // What is the 'name' for?
     return path;
}

Path TSS::setDouble(Path &path, double value)
{
	path.setDouble(value);
	return path;
}

Path TSS::setIntArray(Path &path, int array[], uint length)
{
	path.setIntArray(array, length);
	return path;
}

Path TSS::setDoubleArray(Path &path, double array[], uint length)
{
	path.setDoubleArray(array, length);
	return path;
}

Path TSS::setString(Path &path, string value)
{
	uint length = (unsigned int) value.size(); // this can be dangerous
	path.setBinary((unsigned char *) value.c_str(), length);
	return path;
}

Path TSS::setBinary(Path &path, unsigned char* binaryarray, uint length)
{
	path.setBinary(binaryarray, length);
	return path;
}

//IMPLEMENT
Path setRef(const Path &objp1, Path &objp2, string name2)
{


}

//IMPLEMENT
Path appendRef(const Path &objp1, Path &objp2)
{

}

//IMPLEMENT
Path appendInt(Path &path) // append a new structured element to $
{
	if(path.isBO() && path.isList())
	{

	}
	return path;
}

//IMPLEMENT
Path appendDouble(Path &path) // append a new structured element $
{
	if(path.isBO() && path.isList())
	{

	}

	return path;
}

//IMPLEMENT
Path appendIntArray(Path &path) // append a new structured elemen$
{
	if(path.isBO() && path.isList())
	{

	}

	return path;
}

//IMPLEMENT
Path appendDoubleArray(Path &path) // append a new structured ele$
{
	if(path.isBO() && path.isList())
	{

	}

}

//IMPLEMENT
Path appendString(Path &path) // append a new structured element $
{
	if(path.isBO() && path.isList())
	{
	}

	return path;
}

//IMPLEMENT
Path appendBinary(Path &path)
{
	if(path.isBO() && path.isList())
	{

	}
	
	return path;
}

bool remove(Path &path, uint idx)
{
	return path.removeObj(idx);
}

#endif
