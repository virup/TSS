#include "Path.h"

using namespace stu;

// The strPath should contain the rest of the path (in string representation)
Path& Path::operator+(string strPath)
{
    string newStrPath;
    //generate new path string
    if(strPath[0] == '[' && strPath[strPath.length()-1] == ']')
        newStrPath = this->strPath;
    else
        newStrPath = this->strPath + ".";


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



bool Path::isRO()
{
    return tp->isRO(this);
}



Type Path::getBOType()
{
    return tp->getBOType(this);
}



string Path::pointingType()
{
    if(this->isRO())
        return this->tp->getPointingType(this);
    else
        throw string("Not a RO path");
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
        cerr<<endl<<"FALSE";
}

//  Traverse the path in the iBlob and reach the end of the path
int Path::PopulateLocators()
{
    try{
        // Get the global locator
        Locator tempLocator = iblob->locateGlobal();
        // The tempLocator will finally contain a locator to the object
        for(vector<PathComponent>::iterator it = vPath.begin(); it!= vPath.end(); it++)
        {
            if(tempLocator.getElements() < (uint)it->accessCode)
            {
                consistent = false;
                throw 0;
            }
            tempLocator = iblob->locate(tempLocator, (*it).accessCode);
            (*it).loc = tempLocator;
        }
        consistent = true;
        return 1;
    }
    catch(...)
    {
        consistent = false;
        cerr<<"Cannot make the path consistent"<<endl;
        throw string("Cannot make the path consistent");
        return 0;
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
    else if(this->isList())
    {
        Locator l = this->vPath[vPath.size() - 1].loc;
        return iblob->count(l);
    }
    else
    {
        cerr<<"!! Not a list!";
    }
}

string Path::getType()
{
    return this->tp->getType(this);
}



int Path::set(Path &path)
{
    Locator l2;

    if(this->isRO())
    {
        string pointingType = this->pointingType();
        if(pointingType.compare(path.getType()) != 0)
        {
            throw string("RO object type mismatch");
        }
    }
    else
        throw string("Not a RO object");

    try{
        l2 = this->gotoBO(); //makes use of special access code for reference objects
    }catch(...)
    {
        cerr<<endl<<"Goto error"<<endl;
    }
    const Locator l1;
    Locator tempLoc;

    path.makeInconsistent();
    path.makeConsistent();

    vector<PathComponent>::iterator it;
    for(it = path.vPath.begin(); it != path.vPath.end() ; it++)
        tempLoc = it->loc;

    try{
        l2 = iblob->insert(tempLoc, l2, vPath[vPath.size()-1].accessCode);
    }
    catch(...)
    {
        cerr<<"Exception in path::setref"<<endl;
    }
    vPath[vPath.size() -1 ].loc = l2;
    return 1 ;
}



Locator Path::gotoBO()
{
    makeInconsistent();
    Locator l;

    try{
        l = iblob->locateGlobal();
    }
    catch(...)
    {
        cerr<<endl<<"ERROR locating Global"<<endl;
    }

    vector<PathComponent>::iterator it = vPath.begin();
    do
    {
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
            throw string("discontinuity");
        }
        it++;
    }while(it!= vPath.end()-1);

    if(l.getElements() < (uint)it->accessCode)
        throw string("Not Present");
    return l;
}



bool Path::removeObj()
{
    //get locator
    Locator l = gotoBO();
    int index = vPath[vPath.size() -1].accessCode;
    //call iBlob remove for the locator
    return iblob->remove(l, index);
}

