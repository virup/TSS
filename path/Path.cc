#include "Path.h"

Path& Path::operator+(string strPath)
{
	//generate new path string
	string newStrPath = this->strPath;
    newStrPath.append(strPath);
    Path p(newStrPath, this->tp, this->iblob);
    return p;
}


Path& Path::operator=(const Path &path)
{
	/* ********* ADDED BY VIRU ************** */
	this->vPath.clear();
	for(vector<PathComponent> ::const_iterator it = path.vPath.begin();
            it != path.vPath.end();
            it++)
		this->vPath.push_back(*it);
    this->strPath = path.strPath;
    this->tp = path.tp;
    this->consistent = path.consistent;
    this->iblob = path.iblob;

	/* ************************************* */
	return *this;
}

bool Path::makeConsistent()
{
    this->PopulateLocators();
    return true;
}

void Path::UpdatePaths(string nStrPath)
{
/*
    // TODO: Check if this works. Also check memory leak
    Path p(nStrPath, this->tp, this->iblob);
    this = &p;
*/
}


bool Path::isBO()
{
	return tp->isBO(this);
}

bool Path::isSO()
{
	return tp->isSO(this);
}

bool Path::isList()
{
	return tp->isList(this);
}

bool Path::isRef()
{
	return tp->isRef(this);
}

Type Path::getBOType()
{
	return tp->getBOType(this);
}

vector<int> Path::getAccessCode()
{
    vector<int> accessCodeList;
    for(vector<PathComponent> ::iterator it = vPath.begin();
            it!= vPath.end();
            it++)
        accessCodeList.push_back(it->accessCode);

    return accessCodeList;
}

bool Path::isConsistent()
{
    return this->consistent;
}

void  Path::makeInconsistent()
{
    this->consistent = false;
}


//Default Constructor
Path::Path()
{
    this->consistent = false;
}


//Constructor that accepts an input path string
Path::Path(string strPath, TSSParser *tp, iBlob *iblob)
{
	/* *********** ADDED BY VIRU *********** */
	this->strPath = strPath;
	this->tp = tp;
	this->iblob = iblob;
    this->consistent = false;
    PopulateLocators();
	/* ************************************ */
}

// Update the locators in the vPath object
int Path::PopulateLocators()
{
	/* ************ ADDED BY VIRU ***************** */
	// Get the global locator
	Locator gLocator = iblob->locateGlobal();
	// The tempLocator will finally contain a locator to the object
	Locator tempLocator = gLocator;
	for(vector<PathComponent>::iterator it = vPath.begin(); it!= vPath.end(); it++)
	{
		tempLocator = iblob->locate(tempLocator, (*it).accessCode);
		(*it).loc = tempLocator;
    }
    consistent = true;
	/* ****************************************** */
    return 1;
}


int Path::readInt() //TODO: throws read error
{
    if(!this->isConsistent())
        this->makeConsistent();
    if(this->isBO() && this->getBOType() == Int)
    {
        Locator l = this->vPath[vPath.size()].loc;
        int intVal;
        if(iblob->readInt(intVal, l))
            return intVal;
        else
            return -1;
    }
    else
    {
        //TODO: Throw exception
        return -1; // this is not logically correct
    }
}

double Path::readDouble() //TODO: throws read error
{
    if(!this->isConsistent())
        this->makeConsistent();
    if(this->isBO() && this->getBOType() == Double)
    {
        Locator l = this->vPath[vPath.size()].loc;
        double doubleVal;
        if(iblob->readDouble(doubleVal, l))
            return doubleVal;
        else
            return -1;
    }
    else
    {
        //TODO: Throw exception
        return -1; // this is not logically correct
    }
}

uint Path::readIntArray(int *intBuf, uint bufsize)
{
    if(!this->isConsistent())
        this->makeConsistent();
    if(this->isBO() && this->getBOType() == IntAR)
    {
        Locator l = this->vPath[vPath.size()-1].loc;
        return iblob->readIntArray(intBuf, l, bufsize); 
    }
    else
    {
        //TODO: Throw exception
        return -1; // this is not logically correct
    }
}

uint Path::readDoubleArray(double *doubleBuf, uint bufsize)
{
    if(!this->isConsistent())
        this->makeConsistent();
    if(this->isBO() && this->getBOType() == DoubleAR)
    {
        Locator l = this->vPath[vPath.size()-1].loc;
        return iblob->readDoubleArray(doubleBuf, l, bufsize);
    }
    else
    {
        //TODO: Throw exception
        return -1; // this is not logically correct
    }
}

uint Path::readBinary(unsigned char* charBuf, uint bufsize)
{
    if(!this->isConsistent())
        this->makeConsistent();
    if(this->isBO() && this->getBOType() == Byte)
    {
        Locator l = this->vPath[vPath.size()-1].loc;
        return iblob->readChars(charBuf, l, bufsize);
    }
    else
    {
        //TODO: Throw exception
        return -1; // this is not logically correct
    }
}

uint Path::setInt(int intVal)
{
    this->makeInconsistent();
    Locator l;

    l = iblob->locateGlobal();
    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate(0);
    }

    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
        try{
            l = l.locate(it->accessCode);
        }
        catch(...)
        {
            l = iblob->insert(l,it->accessCode, OBJECT_LEVEL);
            it->loc = l;
        }
        it++;
    }while(it!= vPath.end());
    if(l.getElements() < it->accessCode)
        throw "Not Present";
    l = iblob->insertInt(intVal, l, it->accessCode);
    it->loc = l;
    this->consistent = true;
    return 1;
}

uint Path::setDouble(double doubleVal)
{
    makeInconsistent();
    Locator l;

    l = iblob->locateGlobal();
    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate(0);
    }

    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
        try{
            l = l.locate(it->accessCode);
        }
        catch(...)
        {
            l = iblob->insert(l,it->accessCode, OBJECT_LEVEL);
            it->loc = l;
        }
        it++;
    }while(it!= vPath.end());
    if(l.getElements() < it->accessCode)
        throw "Not Present";
    l = iblob->insertDouble(doubleVal, l, it->accessCode);
    it->loc = l;
    this->consistent =  true;
    return 1;
}

uint Path::setIntArray(int *intBuf, uint size)
{
    makeInconsistent();
    Locator l;

    l = iblob->locateGlobal();
    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate( 0);
    }

    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
        try{
            l = l.locate(it->accessCode);
        }
        catch(...)
        {
            l = iblob->insert(l,it->accessCode, OBJECT_LEVEL);
            it->loc = l;
        }
        it++;
    }while(it!= vPath.end());
    if(l.getElements() < it->accessCode)
        throw "Not Present";
    l = iblob->insertIntArray(intBuf, size, l,it->accessCode);
    it->loc = l;
    this->consistent =  true;
    return 1;
}
uint Path::setDoubleArray(double *doubleBuf, uint size)
{
    makeInconsistent();
    Locator l;

    l = iblob->locateGlobal();
    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate(0);
    }

    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
        try{
            l = l.locate(it->accessCode);
        }
        catch(...)
        {
            l = iblob->insert(l,it->accessCode, OBJECT_LEVEL);
            it->loc = l;
        }
        it++;
    }while(it!= vPath.end());
    if(l.getElements() < it->accessCode)
        throw "Not Present";
    l = iblob->insertDoubleArray(doubleBuf, size, l,it->accessCode);
    it->loc = l;
    this->consistent =  true;
    return 1;
}

uint Path::setBinary(unsigned char *charBuf, uint size)
{
    makeInconsistent();
    Locator l;

    l = iblob->locateGlobal();
    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate(0);
    }

    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
        try{
            l = l.locate(it->accessCode);
        }
        catch(...)
        {
            l = iblob->insert(l,it->accessCode, OBJECT_LEVEL);
            it->loc = l;
        }
        it++;
    }while(it!= vPath.end());
    if(l.getElements() < it->accessCode)
        throw "Not Present";
    l = iblob->insert(charBuf, size, l, it->accessCode);
    it->loc = l;
    this->consistent =  true;
    return 1;
}

