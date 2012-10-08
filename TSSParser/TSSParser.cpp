#include "TSSParser.h"
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std;
typedef unsigned uint;


void visitTree(Node *head)
{
    if(head->parent())
        cerr<<" Parent = "<<head->parent()->name()<<"  ";
    cerr<<"Self =|"<<head->variableName()<<"|  "<<head->type()<<endl;
    if(head->children.size()!= 0)
    {
        map<string, Node *>::iterator it = head->children.begin();
        for(; it != head->children.end(); it++)
            visitTree(it->second);
    }
    cerr<<endl;
}

TSSParser::TSSParser() {
    childCounter = 0;
    head = NULL;
}

//get grammar from given .tss file
string getFromFile(string &grammarSource)
{
    ifstream grammarFile;
    grammarFile.open(grammarSource.c_str());
    string grammar;
    if(grammarFile.is_open())
    {
        string temp;
        while(getline(grammarFile, temp))
            grammar.append(temp);
    }
    else
    {
        cerr<<"Error in grammar file";
        exit(0);
    }
    return grammar;
}

TSSParser::TSSParser(string grammarSource, bool isFile)
{
    string grammar;
    if(isFile)
    {
        this->grammar = new string(getFromFile(grammarSource));
        int fileNameStart = grammarSource.rfind("/");
        if(fileNameStart == string::npos)
            fileNameStart = -1;
        this->dataTypeName.assign(grammarSource,fileNameStart+1, grammarSource.find(".")-fileNameStart-1);
        this->isDataTypeNameFound = true;
    }
    else
    {
        this->grammar = new string(grammarSource);
    }
    childCounter = 0;
    head = NULL;
    validateGrammar();
}

TSSParser::~TSSParser() {

}


// Returns the base object type from a string.
//  UNDEFINED if type not present
Type getType(string s)
{
    map<string, Type> baseTypeConversion;
    baseTypeConversion[string("int")] =  INT;
    baseTypeConversion[std::string("unsigned_int")] = UINT;
    baseTypeConversion[std::string("char")] = CHAR;
    baseTypeConversion[std::string("unsigned_char")] = UCHAR;
    baseTypeConversion[std::string("short")] = SHORT;
    baseTypeConversion[std::string("unsigned_short")] = USHORT;
    baseTypeConversion[std::string("long_int")] = LINT;
    baseTypeConversion[std::string("unsigned_long_int")] = ULINT;
    baseTypeConversion[std::string("double")] = DOUBLE;
    baseTypeConversion[std::string("long_double")] = LDOUBLE;
    baseTypeConversion[std::string("float")] = FLOAT;
    baseTypeConversion[std::string("wchar_t")] = WCHART;
    baseTypeConversion[std::string("bool")] = BOOL;

    if(baseTypeConversion.find(s) == baseTypeConversion.end())
        return UNDEFINED;
    else
        return baseTypeConversion.find(s)->second;
}


bool isList(string s)
{
    return (s.find("+") == s.length()-1)? true: false;
}

// check if TSS gramamr is connected 
bool TSSParser::visit(Node *n)
{
    if(n->visited)
        return true;
    else{
        n->visited = true;
        for(map<string, Node *>::iterator it = n->children.begin();
                it != n->children.end(); it++)
            visit(it->second);
    }
    return true;
}


bool checkIfBO(string type)
{
    return baseTypes.find(type)!=baseTypes.end();
}


bool TSSParser::isConnected(map<string, Node*> nodeMaps)
{
    Node *head = NULL;
    map<string, Node *>::iterator it;
    int parentMissing = 0;
    for(it = nodeMaps.begin(); it != nodeMaps.end(); it++)
        if(it->second->parent() == NULL && it->second->isBO() != true)
        {
            cout<<
            parentMissing++;
            head = it->second;
        }


    cout<<"parentMissing = "<<parentMissing<<endl;

    if(parentMissing != 1)
    {
        cerr<<"Error in creating tree. Please check hierarchy"<<endl;
        throw "Error in creating tree. Please check hierarchy";
    }

    // depth first search to check all components connected
    this->visit(head);
    for(it = nodeMaps.begin(); it != nodeMaps.end(); it++)
        if(!it->second->visited)
        {
            cerr<<"Hierarchy not well formed"<<endl;
            throw string("Hierarchy not well formed");
        }

    // special treatment for RO objects
    // children of RO objects == children of refered object.
    for(it = nodeMaps.begin(); it != nodeMaps.end(); it++)
    {
        if(it->second->isRO())
        {
            if(!checkIfBO(it->second->pointingToType()))
            {
                map<string, Node *>::iterator iit;
                for(iit = nodeMaps.begin(); iit != nodeMaps.end(); iit++)
                {
                    if(iit->second->name().compare(it->second->pointingToType())==0)
                    {
                        it->second->children = iit->second->children;
                        break;
                    }
                }
            }
        }
    }




    return true;
}


// Temporary store for the statements' RHS
struct item{
    string variableName;
    string typeName;
    bool isList;
    bool isBO;
    bool isSO;
    bool isRO;
    bool isDefined; 
};
void printlist(vector<string> l)
{
    cout<<"================="<<endl;
    cout<<"Size = "<<l.size()<<endl;
    for(uint i = 0; i < l.size(); i++)
        cout<<l[i]<<endl;
    cout<<"================="<<endl;

}


void printmap(map<string, vector<item> > &tokenizedStatements)
{
    map<string, vector<item> >::iterator it = tokenizedStatements.begin();
    while(it!= tokenizedStatements.end())
    {
        cout<<it->first<<endl;
        for(uint i = 0; i < it->second.size(); i++)
            cout<<it->second[i].variableName<<":"<<it->second[i].typeName<<" ";
        cout<<endl;
        it++;
    }
}

void removeWhitespaces(string &str)
{
    uint start = 0;
    uint end = 0;
    while(str[end] == ' ' && end < str.length())
        end++;
    str.erase(start,start+end);

    int rstart = str.length()-1;
    int rend = str.length()-1;
    while(str[rend] == ' ' && rend >= 0)
        rend--;
    str.erase(rend+1, rstart);
}

vector<string> tokenize(string str, string sp)
{
    vector<string> elements;
    int start = 0;
    int seplen = sp.length();
    while(str.find(sp, start )!= string::npos)
    {
        int end = str.find(sp, start);
        string newterm = str.substr(start, end-start);
        removeWhitespaces(newterm);
        if(!newterm.empty())
            elements.push_back(newterm);
        start = end+seplen;
    }
    string lastterm = str.substr(start, str.length()-start);
    removeWhitespaces(lastterm);
    if(!lastterm.empty())
        elements.push_back(lastterm);
    return elements;
}


string clean(string t)
{
    removeWhitespaces(t);
    return t;
}




// populates the tokenizedStatements by the grammar
// returns false if some SO are not defined
bool populate(map<string, vector<item> > &tokenizedStatements, string grammar)
{
    //1. break grammar into individual statements
    vector<string> statements = tokenize(grammar, ";");

    //gramamr filetype extension
    string extension(".tss");


    //2. for each statement:
    for(uint i = 0; i < statements.size(); i++)
    {
        // Check if it an import statements
        // if true, then recursively open that file and recover grammarSource
        // and then populate the tokenizedStatements
        if(statements[i].find("import ", 0, 8)!=string::npos)
        {
            vector<string> importStatement = tokenize(statements[i], " ");
            if(importStatement.size() != 2)
                throw " 354 Error in statement " + statements[i];
            string undefType(clean(importStatement[1]));
            populate(tokenizedStatements,
                    getFromFile(undefType.append(extension)));

        }
        // // a. break into LHS and RHS
        vector<string> sides = tokenize(statements[i], "::=");
        if(sides.size()!=2)
            throw "363 Error in line " + statements[i];
        // // b. break the RHS into item object
        vector<string> RHS = tokenize(sides[1], " ");


        //Declare empty vector to store RHS
        vector<item> *vItems = new vector<item>;
        for(uint t = 0; t < RHS.size(); t++)
        {
            vector<string> names = tokenize(RHS[t], ":");
            if(names.size() != 2)
                throw "374 Error in line " + RHS[t];

            item *newItem = new item();
            newItem->variableName.assign(names[0]);
            newItem->isDefined = false;
            if(isList(names[1]))
            {
                newItem->isList = true;
                newItem->typeName.assign(names[1], 0, names[1].size() - 1);
            }
            else
            {
                newItem->isList = false;
                newItem->typeName.assign(names[1]);
            }
            newItem->isBO = checkIfBO(newItem->typeName);
            if(newItem->isBO)
            {
                newItem->isDefined = true;
                newItem->isSO = false;
            }
            else if(names[1][0]=='&')
            {
                newItem->isDefined = true;
                newItem->isRO = true;
            }
            else
            {
                newItem->isDefined = true;
                newItem->isSO = true;
            }
            vItems->push_back(*newItem);
        }
        //3. put in the map
        tokenizedStatements[sides[0]] = *vItems;
    }
    return true;
}

string getUndefined(map<string, vector<item> >& tokenizedStatements)
{
    map<string, vector<item> >::iterator it = tokenizedStatements.begin();
    while(it!=tokenizedStatements.end())
    {
        vector<item> ::iterator vit = it->second.begin();
        while(vit != it->second.end())
        {
            if(vit->isSO)
            {
                string checkTypeName = vit->typeName;
                if(tokenizedStatements.find(checkTypeName)!= tokenizedStatements.end())
                    return vit->typeName;
            }
            vit++;
        }
        it++;
    }
    return "";
}



Node* createNode (string parentType, item *component,Node *left,
        int count, map<string, Node *> &nodeMaps)
{
    Node *n = new Node;
    n->setTypeName(component->typeName);
    n->setVariableName(component->variableName);
    n->visited = false;
    if(component->isBO)
    {
        n->setBO();
        n->setType(getType(component->typeName));
    }
    if(component->isSO)
        n->setSO();
    if(component->isRO)
    {
        n->setRO();
        n->setPointingToType(component->typeName.substr(1,component->typeName.length()-1));
    }
    if(component->isList)
        n->setList(true);
    else
        n->setList(false);
    if(left != NULL)
        left->setNext(n);
    n->setPos(count);
    nodeMaps[component->variableName] = n;
    return n;
}

void createNodes(string parentType, vector<item> *vItem,
        map<string, Node *> &nodeMaps)
{
    vector<item> ::iterator it = vItem->begin();
    Node *temp = NULL;
    int count = 0;
    while(it != vItem->end())
    {
        temp = createNode(parentType, &(*it), temp, count++, nodeMaps);
        it++;
    }

}

void linkUp(vector<item> &children, Node *localHead,
        map<string, Node*> &nodeMaps)
{
    vector<item> ::iterator cit = children.begin();
    while(cit!= children.end())
    {
        Node *childNode = nodeMaps[cit->variableName];
        localHead->addChild(cit->variableName, childNode);
        cit++;
    }
}
void createTree(map<string, vector<item> > &tokenizedStatements,
        map<string, Node *> &nodeMaps)
{

    map<string, vector<item> > ::iterator iToken = tokenizedStatements.begin();
    while(iToken != tokenizedStatements.end())
    {
        vector<item> ::iterator vit = iToken->second.begin();
        while(vit!=iToken->second.end())
        {
            if(vit->isBO)
            {
                vit++;
                continue;
            }
            if(vit->isRO)
            {
                vit++;
                continue;
            }
            Node * localHead = nodeMaps[vit->variableName];

            string vName(vit->variableName);
            string tName(vit->typeName);
            vector<item> children = tokenizedStatements[tName];
            linkUp(children, localHead, nodeMaps);
            vit++;
        }
        iToken++;
    }
}

string findHead(map<string, vector<item> > &tokenizedStatements)
{
    return "";
}

bool TSSParser::validateGrammar()
{
    // vector<string> statements = tokenize(*this->grammar, ";");
    string grammarStr (*this->grammar);

    map<string, vector<item> > tokenizedStatements;

    populate(tokenizedStatements, grammarStr);


    if(!isDataTypeNameFound)
        /* TODO : This function is not implemented */
        this->dataTypeName = findHead(tokenizedStatements);

    // Actual grammar tree is stored here
    map<string, Node*> nodeMaps;

    map<string, vector<item> >::iterator it = tokenizedStatements.begin();

    // Create the nodes of the graph
    while(it!=tokenizedStatements.end())
    {
        createNodes(it->first, &it->second, nodeMaps);
        it++;
    }

    // Create the head nodes
    Node *headNode = new Node();
    headNode->setTypeName(dataTypeName);
    headNode->setVariableName(dataTypeName);
    headNode->setSO();
    headNode->setPos(0);
    headNode->setList(false);
    nodeMaps[dataTypeName] = headNode;


    // Create the actual tree from the nodes
    createTree(tokenizedStatements, nodeMaps);
    // Link up the head node (requires separate effort)
    vector<item> children = tokenizedStatements[dataTypeName];
    linkUp(children,headNode, nodeMaps);
    //Check that the tree is connected
    if(!isConnected(nodeMaps))
        throw "Error: Tree not connected";
    //head = checkTreeAndReturnHead(nodeMaps);
    this->head = headNode;
    //DONE!!
    return true;
}




void printChildren(map<string, Node *> maplist)
{
    map<string, Node *>:: iterator it;
    cout<<"Children are: "<<endl;
    for(it = maplist.begin(); it!= maplist.end(); it++)
    {
        cout<<it->first<<endl;

    }

}


int checkAndReturnList(string &comp)
{
    int value = -1;
    if(comp.find("[") == string::npos)
        return value;

    uint start = comp.find("[");
    uint end =  comp.find("]");

    if(start+1 < end)
    {
        string number = comp.substr(start+1, end-start-1);
        comp.assign(comp.substr(0, start));
        value = atoi(number.c_str());
    }
    return value;
}



bool TSSParser::storeAccessCode(string strpath, vector<PathComponent>& pathVector) 
{
    bool returnVal = true;
    PathComponent pathComp;
    vector<string> strPathComp = tokenize(strpath, ".");
    //add the GLOBAL element

    Node *current = this->head;
    if(strPathComp[0].compare(current->name())!=0)
    {
        cerr<<"Wrong path root"<<endl;
        throw string("wrong path root");
    }

    pathComp.label = current->name();
    pathComp.accessCode = 0;
    pathVector.push_back(pathComp);

    for(uint i = 1; i < strPathComp.size(); i++)
    {
        string component = strPathComp[i];
        int arrayNo = checkAndReturnList(component);
        if(arrayNo != -1)
        {
            PathComponent listPathComp;
            listPathComp.label = "List";
            listPathComp.accessCode = arrayNo;

            // strip the substript
            pathComp.label = component;
            pathVector.push_back(pathComp);
            // put the listPathComp in the vector too
            pathVector.push_back(listPathComp);
            if (current->children.find(component) != current->children.end())
            {
                current = current->children[component];
                pathVector[i].accessCode = current->pos();
            }
            else
            {
                cerr << component<< " is not a valid path\n";
                returnVal = false;
                break;
            }
        }
        else
        {
            pathComp.label = component;
            pathVector.push_back(pathComp);
            if (current->children.find(component) != current->children.end())
            {
                current = current->children[component];
                pathVector[i].accessCode = current->pos();
            }
            else
            {
                cerr << component<< " is not a valid path\n";
                returnVal = false;
                break;
            }
        }
    }

    return returnVal;
}
int * TSSParser::genAccessCode(Path *p) {
    int * accessCodes = new int[p->vPath.size()];
    for (uint i = 0; i < p->vPath.size(); i++)
        accessCodes[i] = p->vPath.at(i).accessCode;
    return accessCodes;
}


Node *TSSParser::gotoEnd(Path *p, bool &lastItemIsListItem)
{
    lastItemIsListItem = false;
    Node *current = head;
    //p->vPath[0].label == "GLOBAL"
    for(uint i = 1; i < p->vPath.size(); i++)
    {
        lastItemIsListItem = false;
        if(p->vPath[i].label.compare("List") == 0)
        {
            lastItemIsListItem = true;
            continue;
        }
        if (current->children.find(p->vPath[i].label) != current->children.end())
        {
            current = current->children[p->vPath[i].label];
        }
        else
        {
            cerr << p->vPath[i].label << " is not a valid path\n";
            throw string("Error in path");
        }
    }
    return current;
}



string TSSParser::getPointingType(Path *p)
{
    bool lastItemIsListItem;
    Node *current = gotoEnd(p, lastItemIsListItem);
    return current->pointingToType();

}


string TSSParser::getType(Path *p)
{
    bool lastItemIsListItem;
    Node *current = gotoEnd(p, lastItemIsListItem);
    if(current->isRO())
        return string("&")+current->name();
    return current->name();

}


bool TSSParser::isBO(Path *p)
{
    bool lastItemIsListItem;
    Node *current = gotoEnd(p, lastItemIsListItem);
    if(current->isBO())
        return true;
    else
        return false;
}


bool TSSParser::isSO(Path *p)
{
    bool lastItemIsListItem;
    Node *current = gotoEnd(p, lastItemIsListItem);
    if(current->isSO())
        return true;
    else
        return false;
}

bool TSSParser::isList(Path *p)
{
    bool lastItemIsListItem;
    Node *current = gotoEnd(p, lastItemIsListItem);
    if(current->isList() && !lastItemIsListItem)
        return true;
    else
        return false;
}

bool TSSParser::isRO(Path *p)
{
    bool lastItemIsListItem;
    Node *current = gotoEnd(p, lastItemIsListItem);
    if(current->isRO())
        return true;
    else
        return false;
}

Type TSSParser::getBOType(Path *p)
{
    bool lastItemIsListItem;
    Node *current = gotoEnd(p, lastItemIsListItem);
    if (current->isBO())
        return current->type();
    else {
        cerr << "Object is not BO\n";
        return UNDEFINED;
    }

}



string TSSParser::getGrammarType()
{
    return head->name();
}
