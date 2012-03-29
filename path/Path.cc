#include "Path.h"

Path& Path::operator+(string p)
{
	//generate new path string
	string new_path_string;

	new_path_string = GenNewPathString(p);

/*
	//if the new_path_string conforms to the grammar  i.e. it is a valid path.
	if(TSSParserObj.Scan(new_path_string) == true)
	{
		//Update Path class.
		UpdatePaths(new_path_string);
	}
*/

}


Path& Path::operator=(const Path & path)
{
	/* ********* ADDED BY VIRU ************** */
	this->verbose_path.clear();
	for(vector<PathComponent> ::iterator it = path.verbose_path.begin();
		it != path.verbose_path.end();
		it++)
		this->verbose.push_back(*it);

	/* ************************************* */
	return *this;
}


void Path::UpdatePaths(string new_p)
{
	int old_size = verbose_path.size();
	int i = 0;
	string tok;
	char *dup;
	dup = strdup(new_p.c_str());

	//Update the path_string
	(*this).path_string = new_p;

	//this will remove the old_path component in the new path
	while(i < old_size)
	{
		tok = strtok(dup,".");
		i++;
	}

	tok = strtok(dup,".");

	//Update the verbose_path 
	while(tok.empty() != true)
	{
		PathComponent newComponent;
		newComponent.label = tok;
		verbose_path.push_back(newComponent);
	}
}

string Path::GenNewPathString(string p)
{
	string temp;

	vector<PathComponent>::iterator it;

	//build temp string to equal the old path
        for(it = verbose_path.begin() ; it < verbose_path.end() ; it++)
        {
        	temp += (*it).label;
        	temp += ".";
        }

	//concat input_path to temp (which holds the old path) 
        temp += p;

	return temp;
}


/* -- this comment block contains fuctions used to get Path type information

bool Path::isBO()
{
	return TSSParserObj.isBO(*this);
}

bool Path::isSO()
{
	return TSSParserObj.isSO(*this);
}

bool Path::isList()
{
	return TSSParserObj.isList(*this);
}

bool Path::isRef()
{
	return TSSParserObj.isRef(*this);
}

BO_DataType Path::getBoType()
{
	return TSSParserObj.getBOType(*this);
}
*/


//Default Constructor
Path::Path()
{

}


//Constructor that accepts an input path string
Path::Path(string strPath, TSSParser *tp, iBlob *iblob)
{
	/* *********** ADDED BY VIRU *********** */
	this->path_string = strPath;
	this->TSSParserObj = tp;
	this->iblob = iblob;
	// TODO: update the verbose path here 
	/* ************************************ */
}

//This is a critical function that populates the access_code and Locator fields of the verbose_path vector.
//It is this mapping of path components (label strings) to their corresponding iBloB Locators that provides semantic meaning to the path string with respect to the iBloB.
//Upon completion of this function a Path (i.e. verbose_path vector of thr Path class) will hold iBloB Locators corresponding to the path string.
Path::PopulateLocators()
{
	/* ************ ADDED BY VIRU ***************** */
	// Get the global locator
	Locator gLocator = iblob.locateGlobal();
	// The tempLocator will finally contain a locator to the object
	Locator tempLocator = gLocator;
	for(vector<PathComponent>::iterator it = verbose_path.begin(); it!= verbose_path.end(); it++)
	{
		tempLocator = iblob->locate(tempLocator, (*it).accessCode;
		(*it).loc = tempLocator;
	}
	/* ****************************************** */

	
}
