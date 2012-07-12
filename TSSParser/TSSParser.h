/* 
 * File:   TSSParser.h
 * Author: Archesh
 *
 * Created on March 29, 2012, 3:28 PM
 */

#ifndef TSSPARSER_H
#define	TSSPARSER_H


#include "../path/Path.h"
#include "../engine/constants.h"
#include "Scanner.h"
#include <cstring>
#include <cstdlib>
#include <map>
#include <string>
#include <list>
#include <vector>

using namespace std;

class TSSParser
{
    public:
       struct Node
        {
            string name;                // Name of the node
            string objectType;
            Type type;                  // Data type iif isBO
            int no_of_children;         // No. of chilren
            bool isBO;                  // Tracks if nodes is BO
            bool isSO;
            bool isRO;
            string pointingToType;
            bool isList;
            bool visited;               // Track if SO node has been visited
            int pos;                    // Position in the list of children
            map <string, Node *> children; //pointers to children
            Node * parent;
            Node * child;
            Node * next;


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
            void setName(string name){this->name.assign(name);}
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

        Node *head;      // Actual data structure to store the grammar
        Node *current;
        string * grammar;
        int childCounter;

        // Depth first search of grammar to check connected components
        bool visit(Node * );
        //return head of tree
        Node *checkTreeAndReturnHead(map<string, Node*>);
        //this vector will store RO objects encountered during parsing
        vector<string> ROObjects; 
        //this vector will store RO pointers (*pointer) encountered during parsing
        vector<string> ROPointers;

        list<Node*> nodes;
        Node * getParentNode();

        //makes a copy of 'b' and stores it in 'a'
        //this function is need to store heads in vector
        void copy(Node *a, Node *b);

        //this function is used to link individual nodes formed in buildTree() function. It
        //is used by linkTree() function
        void linkNodes(Node *a ,Node *b);

        //this function makes individual tree of each line
        //provided in the grammar. Then, it stores the head
        //of each tree in the list.
        void buildTree(string &);
        void buildTree(vector<Node>);

        //this function links all the trees together in to 1 tree
        //using BFS algorithm
        void linkTrees();

        //this function prints the tree
        void print();

        //this function will check for each RO object there is a corresponding
        //RO Pointer i.e 1-1 relation. If it is the case, then the function will
        //return true
        bool matchRO();

        //visit all nodes and delete it to free up memory
        void cleanUp();

        // Tokenize a string
        vector<string> tokenize(string, string);


    public:

        TSSParser(); //default constructor
        TSSParser(string grammar, bool isFile); // grammar is a file name if
                                                // isFile == true
        ~TSSParser();
         bool validateGrammar();   // Validate the given grammar
         bool validateGrammar_1();   // Validate the given grammar

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
        bool isRef(Path *p);

	//return the root's type , this holds the data type of the TSS that the grammar describes
	string getGrammarType();

	//If the type of a particular object in grammar is BO, then
        //following function will return the exact type of that BO.
        //Type could be Int, IntAR, Double, DoubleAR, String, or Byte
        Type getBOType(Path *p);
};
#endif	/* TSSPARSER_H */
