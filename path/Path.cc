#include "Path.h"


Path& Path::operator+(string strPath)
{
    //generate new path string
    string newStrPath = this->strPath + ".";
    newStrPath.append(strPath);
    Path *p = new Path(newStrPath, this->tp, this->iblob);
    return *p;
}


Path& Path::operator=(const Path &path)
{
    this->vPath.clear();
    for(vector<PathComponent> ::const_iterator it = path.vPath.begin();
            it != path.vPath.end();
            it++)
        this->vPath.push_back(*it);
    this->strPath.assign(path.strPath);
    this->tp = path.tp;
    this->consistent = path.consistent;
    this->iblob = path.iblob;

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
Path::Path(string strpath, TSSParser *tp, iBlob *iblob)
{
    string temp(strpath);
    this->strPath.assign(temp);
    this->tp = tp;
    this->iblob = iblob;
    this->consistent = false;
    if(!this->tp->storeAccessCode(strpath, this->vPath))
        cout<<endl<<"FALSE";
    //PopulateLocators();
}

// Update the locators in the vPath object
int Path::PopulateLocators()
{
    try{
        // Get the global locator
        Locator gLocator = iblob->locateGlobal();
        // The tempLocator will finally contain a locator to the object
        Locator tempLocator = gLocator;
        for(vector<PathComponent>::iterator it = vPath.begin(); it!= vPath.end(); it++)
        {
            if(tempLocator.getElements() < it->accessCode)
            {
                consistent = false;
                return 0;
            }
            tempLocator = iblob->locate(tempLocator, (*it).accessCode);
            (*it).loc = tempLocator;
        }
        consistent = true;
        return 1;
    }
    catch(...)
    {
        cout<<"Cannot make the path consistent"<<endl;
        consistent = false;
        return 0;
    }
}

int Path::readInt() //TODO: throws read error
{
    if(!this->isConsistent())
        this->makeConsistent();
    if(this->isBO() && this->getBOType() == Int)
    {
        Locator l = this->vPath[vPath.size()-1].loc;
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


//TODO: reads only 25 chars
// && this->getBOType() == String)  <--------
string Path::readString()
{
    if(!this->isConsistent())
        this->makeConsistent();
    //read first 10 characters in the string, they represent the size of the string.
    unsigned char strLengthArray[10];
    if(this->isBO() && this->getBOType() == String)
    {
        cout<<"Reading base object"<<endl;
        Locator l = this->vPath[vPath.size()-1].loc;
        if(iblob->readChars(strLengthArray, l, 10))
        {
            int stringLength = atoi((const char*)strLengthArray);
            unsigned char returnString[stringLength + 10];
            iblob->readChars(returnString, l , stringLength+10);
            string str((const char*)returnString, stringLength + 10);
            return str;
        }
        else
        {
            cout<<"exception happens while reading string "<<endl;
            string str(" ");
            return str;
        }
    }
    else
    {
        cout<<"Not reading base object "<<endl;
        string str(" "); //Should actually throw an exception here
        return str;
    }

    //at this stage s

}

double Path::readDouble() //TODO: throws read error
{
    if(!this->isConsistent())
        this->makeConsistent();
    if(this->isBO() && this->getBOType() == Double)
    {
        Locator l = this->vPath[vPath.size()-1].loc;
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

int Path::readIntArray(int *intBuf, uint bufsize)
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

int Path::readDoubleArray(double *doubleBuf, uint bufsize)
{
    if(!this->isConsistent())
        this->makeConsistent();
    cout<<"Result : " <<isBO()<<"  "<<(getBOType() == DoubleAR)<<endl;
    if(this->isBO() && this->getBOType() == DoubleAR)
    {
        cout<<"readDoubleArray::inside"<<endl;
        Locator l = this->vPath[vPath.size()-1].loc;
        l.debug();
        cout<<"Locator index = "<<vPath.size()-1<<endl;
        return iblob->readDoubleArray(doubleBuf, l, bufsize);
    }
    else
    {
        //TODO: Throw exception
        return -1; // this is not logically correct
    }
}

int Path::readBinary(unsigned char* charBuf, uint bufsize)
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

int Path::count()
{
    if(!this->isConsistent())
    {
        this->makeConsistent();
    }
    if(this->isBO())
    {
        return -1;
    }
    else
    {
        Locator l = this->vPath[vPath.size() - 1].loc;
        return iblob->count(l);
    }
}

int Path::setInt(int intVal)
{
    Locator l;
    try{
        l = gotoBO();
    }
    catch(...)
    {
        return 0;
    }
    PathComponent *p = &vPath[vPath.size()-1];
    l = iblob->insertInt(intVal, l, p->accessCode);
    p->loc = l;
    this->consistent =  true;
    return 1;
}

int Path::setDouble(double doubleVal)
{
    Locator l;
    try{
        l = gotoBO();
    }
    catch(...)
    {
        return 0;
    }
    PathComponent *p = &vPath[vPath.size()-1];
    l = iblob->insertDouble(doubleVal, l, p->accessCode);
    p->loc = l;
    this->consistent =  true;
    return 1;
}

int Path::setIntArray(int *intBuf, uint size)
{
    Locator l;
    try{
        l = gotoBO();
    }
    catch(...)
    {
        return 0;
    }
    PathComponent *p = &vPath[vPath.size()-1];
    l = iblob->insertIntArray(intBuf, size, l,p->accessCode);
    p->loc = l;
    this->consistent =  true;
    return 1;
}




int Path::setDoubleArray(double *doubleBuf, uint size)
{
    Locator l;
    try{
        gotoBO();
    }
    catch(...)
    {
        return 0;
    }
    PathComponent *p = &vPath[vPath.size()-1];
    l = vPath[vPath.size()-2].loc;
    cout<<"Access code ="<<p->accessCode<<endl;
    p->loc = iblob->insertDoubleArray(doubleBuf, size, l, p->accessCode);
    this->consistent =  true;
    return 1;
}

int Path::setBinary(unsigned char *charBuf, uint size)
{

    Locator l;
    try{
        l = gotoBO();
    }
    catch(...)
    {
        return 0;
    }
    PathComponent *p = &vPath[vPath.size()-1];
    l = iblob->insert(charBuf, size, l, p->accessCode);
    p->loc = l;
    this->consistent =  true;
    return 1;
}

int Path::setRefTo(Path &path, int idx)
{
    Locator l2;
    try{
    l2 = this->gotoBO();//makes use of special access code for reference objects
    }catch(...)
    {
        cout<<endl<<"Goto error"<<endl;
    }
    const Locator l1;
    Locator tempLoc;
    path.makeInconsistent();
    //populate locator for the refernced object
    path.makeConsistent();
    vector<PathComponent>::iterator it;
    for(it = path.vPath.begin(); it != path.vPath.end() ; it++)
    {
        tempLoc = it->loc;
    }

    try{
    l2 = iblob->insert(tempLoc, l2, idx);
    }
    catch(...)
    {
        cout<<"Exception in path::setref"<<endl;
    }
}

Locator Path::gotoBO()
{
    makeInconsistent();
    Locator l;
    cout<<this->strPath<<endl;
    try{
    l = iblob->locateGlobal();
    }
    catch(...)
    {
        cout<<endl<<"ERROR locating Global"<<endl;
    }
    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate(0);
    }
    //int temp = -1;//delete
    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
        cout<<it->label<<".";
        try{
            if((l.getElements() == (it->accessCode)))
            {
                l = iblob->insert(l,it->accessCode, OBJECT_LEVEL);
                it->loc = l;
            }
            else
            {
                l = l.locate(it->accessCode);
            }

        }
        catch(...)
        {
            cerr<<"Path::gotoBO - Discontinuity detected"<<endl;
            //cerr << "fail idx = " << temp << endl;

            throw "discontinuity";
        }

        it++;
    }while(it!= vPath.end());
    it--;
    cout<<endl<<l.getElements()<<"    "<<it->accessCode<<endl;
    if(l.getElements() < it->accessCode)
        throw "Not Present";
    return l;
}

bool Path::removeObj(uint idx)
{
    //get locator
    Locator l;
    l = iblob->locateGlobal();

    if(l.getElements()==0)
    {
        l.insert(0,OBJECT_LEVEL);
        l = l.locate(0);
    }

    vector<PathComponent>::iterator it = vPath.begin();

    while(it != vPath.end())
    {
        try
        {
            l = l.locate(it->accessCode);
        }
        catch(...)
        {
            l = iblob->insert(l,it->accessCode, OBJECT_LEVEL);
            it->loc = l;
        }
        it++;
    }

    if(l.getElements() < it->accessCode)
        throw "Not Present";

    //call iBlob remove for the locator
    return iblob->remove(l, idx);
}


int Path::set(int value)
{
    return this->setInt(value);
}

int Path::set(double value)
{
    return this->setDouble(value);
}

int Path::set(int *value, int size)
{
    return this->setIntArray(value, size);
}


int Path::set(double *value, int size)
{
    return this->setDoubleArray(value, size);
}

int Path::set(unsigned char *value, int size)
{
    return this->setBinary(value, size);
}

int Path::set(Path &path, int idx)
{
    return this->setRefTo(path, idx);
}
