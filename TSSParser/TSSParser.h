#ifndef TSSPARSER_H
#define	TSSPARSER_H


#include "../path/Path.h"
#include "../engine/constants.h"
#include <cstring>
#include <cstdlib>
#include <map>
#include <string>
#include <list>
#include <vector>

using namespace std;

class Node
{
    public:
    string name;                // Name of the Type(eg "REGION")
    string variableName;        // Name of the variable (eg "region")
    string objectType;
    Type type;                  // is populated with data type iif isBO
    int no_of_children;         // No. of chilren
    bool isBO;                  // Tracks if nodes is BO
    bool isSO;
    bool isRO;
    string pointingToType;      // only present if isRO == true
    Node * pointingToNode;      // points to the note which the RO is pointing
    // Only valid for RO objects
    bool isList;
    bool visited;               // Track if SO node has been visited
    int pos;                    // Position in the list of children
    map <string, Node *> children; //pointers to children
    Node * parent;
    Node * child;
    Node * next;

    public:

    Node() {
        name = "";
        objectType = "";
        no_of_children = 0;
        isBO = false;
        isRO = false;
        isSO = false;
        isList = false;
        visited = false;
        pos = -1;
        parent = NULL;
        child = NULL;
        next = NULL;
    }
    void setTypeName(string name){this->name.assign(name);}
    void setVariableName(string varName){this->variableName.assign(varName);}
    void setObjectType(string type){this->objectType.assign(type);}
    void setType(Type type){this->type = type;}
    void setBO(){this->isBO = true;}
    void setSO(){this->isSO = true;}
    void setRO(){this->isRO = true;}
    void setPos(int p){this->pos = p;}
    void setNext(Node *n){this->next = n;}
    void setList(bool b){this->isList = b;}
    void addChild(string s, Node *n){
        children.insert(pair<string, Node*>(s,n));
        n->parent = this;
    }
    void setPointingToType(string strtypename)
    {
        this->pointingToType.assign(strtypename);
    }
};


class TSSParser
{
    public:
        string dataTypeName; // name of the main data type
        bool isDataTypeNameFound; // true if dataTypeName has valid value
        Node *head;      // Actual data structure to store the grammar
        Node *current;
        string * grammar;
        int childCounter;

        // Depth first search of grammar to check connected components
        bool visit(Node * );
        Node * getParentNode();

        // check connected graph or not
        bool isConnected(map<string, Node *>);



        TSSParser(); //default constructor
        TSSParser(string grammar, bool isFile); // grammar is a file name if
        // isFile == true
        ~TSSParser();
        bool validateGrammar();   // Validate the given grammar

        //This function will break the objects from path string, and store corresponding
        //label and accesscode in pathVector. If the given string path is invalid, then
        //this function will return false
        bool storeAccessCode(string path, vector<PathComponent>&);

        //This function will simply get pathVector from Path p object and
        //extract the access code from it. It will then return an int array
        //that contains the access code.
        int* genAccessCode(Path *p);           // Generate access code

        //This following 4 functions will traverse the grammar tree
        //and check whether the last object specified in path is BO,
        // SO, List, or Ref.
        bool isBO(Path *p);
        bool isSO(Path *p);
        bool isList(Path *p);
        bool isRO(Path *p);
        string getPointingType(Path *p);
        string getType(Path *p);
        Node *gotoEnd(Path *p, bool&);

        //return the root's type , this holds the data type of the TSS that the grammar describes
        string getGrammarType();

        //If the type of a particular object in grammar is BO, then
        //following function will return the exact type of that BO.
        //Type could be Int, IntAR, Double, DoubleAR, String, or Byte
        Type getBOType(Path *p);
};
#endif	/* TSSPARSER_H */
