#ifndef STU_TSS_CPP
#define STU_TSS_CPP

#include "TSS.h"
#include "Path.h"
#include "TSSParser.h"
#include "constants.h"
#include "iBlobStore.h"
#include "iBlob.h"

using namespace stu;

TSS::TSS(const char *grammarString, bool isFile)
{
	this->tp_ = new TSSParser(grammarString, isFile);
}

TSS::TSS(string grammarString, bool isFile)
{
	this->tp_ = new TSSParser(grammarString.c_str(), isFile);
}



// Nothing to destruct
TSS::~TSS()
{}



/* The storageHandle is a pointer to an iBlob object */
Path TSS::createPath(void *storageHandle) const
{
    string str = "";
    Path *p = new Path(str, (this->tp), (iBlob *)storageHandle);
    return *p;
}


/* The storageHandle is a pointer to an iBlob object
 * strPath is a string representation of a Path object */
Path TSS::createPath(string strPath, void *storageHandle) const
{
   Path *p = new Path(strPath, (this->tp_), (iBlob *)storageHandle);
   return *p;
}

#endif
