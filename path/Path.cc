#include "Path.h"

Path& Path::operator+(string p)
{
	//generate new path string
	string newStrPath = this->strPath;
    newStrPath.append(p);
    Path p(this->newStrPath, this->tp, this->iblob);
    return p;
}


Path& Path::operator=(const Path &path)
{
	/* ********* ADDED BY VIRU ************** */
	this->vPath.clear();
	for(vector<PathComponent> ::iterator it = path.vPath.begin();
		it != path.vPath.end();
		it++)
		this->verbose.push_back(*it);
    this->strPath = path.strPath;
    this->tp = path.tp;
    this->isConsistent = path.isConsistent;
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
	int old_size = vPath.size();
	int i = 0;
	string tok;
	char *dup;
	dup = strdup(nStrPath.c_str());

	//Update the strPath
	(*this).strPath = nStrPath;

	//this will remove the old_path component in the new path
	while(i < old_size)
	{
		tok = strtok(dup,".");
		i++;
	}

	tok = strtok(dup,".");

	//Update the vPath
	while(tok.empty() != true)
	{
		PathComponent newComponent;
		newComponent.label = tok;
		vPath.push_back(newComponent);
	}
    */

    // TODO: Check if this works. Also check memory leak
    Path p(nStrPath, this->tp, this->iblob);
    this = &p;
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

DataType Path::getBoType()
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
    return this->isConsistent;
}

void  Path::makeInconsistent()
{
    this->isConsistent = false;
}


//Default Constructor
Path::Path()
{
    isConsistent = false;
}


//Constructor that accepts an input path string
Path::Path(string strPath, TSSParser *tp, iBlob *iblob)
{
	/* *********** ADDED BY VIRU *********** */
	this->strPath = strPath;
	this->tp = tp;
	this->iblob = iblob;
    this->isConsistent = false;
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
    isConsistent = true;
	/* ****************************************** */
    return 1;
}


int Path::readInt() //TODO: throws read error
{
    if(!this->isConsistent())
        this->makeConsistent;
    if(this->BO && this->getBoType == INT)
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
        this->makeConsistent;
    if(this->BO && this->getBoType == DOUBLE)
    {
        Locator l = this->vPath[vPath.size()].loc;
        double doubleVal;
        if(readDouble(iblob->doubleVal, l))
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

uint readIntArray(int *intBuf, uint bufsize)
{
    if(!this->isConsistent())
        this->makeConsistent;
    if(this->BO && this->getBoType == INTARRAY)
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

uint readDoubleArray(double *doubleBuf, uint bufsize)
{
    if(!this->isConsistent())
        this->makeConsistent;
    if(this->BO && this->getBoType == DOUBLEARRAY)
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

uint readBinary(unsigned char* charBuf, uint bufsize)
{
    if(!this->isConsistent())
        this->makeConsistent;
    if(this->BO && this->getBoType == BYTE)
    {
        Locator l = this->vPath[vPath.size()-1].loc;
        return iblob->readCharsArray(charsBuf, l, bufsize);
    }
    else
    {
        //TODO: Throw exception
        return -1; // this is not logically correct
    }
}

uint setint(int intVal)
{
    makeInconsistent();
    Locator l;

    l = iblob->locateGlobal();
    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate(l, 0);
    }

    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
        try{
            l = iblob->locate(l,it->accessCode, OBJECT_LEVEL);
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
    l = iblob->setInt(intVal, l, it->accessCode);
    it->loc = l;
    this->isConsistent =  true;
    return 1;
}

uint setDouble(double doubleVal)
{
    makeInconsistent();
    Locator l;

    l = iblob->locateGlobal();
    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate(l, 0);
    }

    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
        try{
            l = iblob->locate(l,it->accessCode, OBJECT_LEVEL);
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
    l = iblob->setDouble(doubleVal, l, it->accessCode);
    it->loc = l;
    this->isConsistent =  true;
    return 1;
}

uint setIntArray(int *intBuf, uint size)
{
    makeInconsistent();
    Locator l;

    l = iblob->locateGlobal();
    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate(l, 0);
    }

    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
        try{
            l = iblob->locate(l,it->accessCode, OBJECT_LEVEL);
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
    this->isConsistent =  true;
    return 1;
}
uint setDoubleArray(double *doubleBuf, uint size)
{
    makeInconsistent();
    Locator l;

    l = iblob->locateGlobal();
    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate(l, 0);
    }

    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
        try{
            l = iblob->locate(l,it->accessCode, OBJECT_LEVEL);
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
    l = iblob->insertIntDouble(doubleVal, size, l,it->accessCode);
    it->loc = l;
    this->isConsistent =  true;
    return 1;
}

uint setBinary(unsigned char *charBuf, uint size)
{
    makeInconsistent();
    Locator l;

    l = iblob->locateGlobal();
    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate(l, 0);
    }

    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
        try{
            l = iblob->locate(l,it->accessCode, OBJECT_LEVEL);
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
    this->isConsistent =  true;
    return 1;
}

