#ifndef TSS_CPP
#define TSS_CPP

#include "TSS.h"
#include "Path.h"
#include "TSSParser.h"
#include "constants.h"
#include "iBlobStore.h"
#include "iBlob.h"



TSS::TSS(const char *grammarString, bool isFile)
{
	this->tp = new TSSParser(grammarString, isFile);
    this->type = type;
}

TSS::TSS(string grammarString, bool isFile)
{
	this->tp = new TSSParser(grammarString.c_str(), isFile);
    this->type = type;
}



//TODO: IMPORTANT to implement it. Otherwise there are memory leaks
TSS::~TSS()
{}

Path TSS::createPath(void *storageHandle) const
{
    string str = "";
    Path *p = new Path(str, (this->tp), (iBlob *)storageHandle);
    return *p;
}


Path TSS::createPath(string strPath, void *storageHandle) const
{
   Path *p = new Path(strPath, (this->tp), (iBlob *)storageHandle);
   return *p;
}

bool remove(Path &path)
{
	return path.removeObj();
}

#endif
