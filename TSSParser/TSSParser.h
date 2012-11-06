#ifndef STU_TSSPARSER_H
#define	STU_TSSPARSER_H


#include "../path/Path.h"
#include "../engine/constants.h"
#include <cstring>
#include <cstdlib>
#include <map>
#include <string>
#include <list>
#include <vector>

using namespace std;
namespace stu
{

    class Node
    {
        public:
            map <string, Node *> children; //pointers to children
            bool isVisited_;               // Track if SO node has been isVisited_

            Node() {
                name_ = "";
                objectType_ = "";
                nChildren_ = 0;
                isBO_ = false;
                isRO_ = false;
                isSO_ = false;
                isList_ = false;
                isVisited_ = false;
                pos_ = -1;
                parent_ = NULL;
                child_ = NULL;
                next_ = NULL;
            }
            void setObjectType(string type){this->objectType_.assign(type);}
            void setBO(){this->isBO_ = true;}
            void setSO(){this->isSO_ = true;}
            void setRO(){this->isRO_ = true;}
            void setPos(int p){this->pos_ = p;}
            int pos(){return pos_;}
            void setList(bool b){this->isList_ = b;}
            void addChild(string s, Node *n){
                children.insert(pair<string, Node*>(s,n));
                n->parent_ = this;
            }
            void setPointingToType(string strtypename)
            {
                this->mpointingToType.assign(strtypename);
            }
            string pointingToType(){return this->mpointingToType;}
            bool isRO(){return isRO_;}
            bool isSO(){return isSO_;}
            bool isBO(){return isBO_;}
            bool isList(){return isList_;}
            Node * parent(){return parent_;}
            Node * child(){return child_;}
            Node * next(){return next_;}
            void setParent(Node *n){this->parent_ = n;}
            void setChild(Node *n){this->child_ = n;}
            void setNext(Node *n){this->next_ = n;}
            string name(){return this->name_;}
            void setTypeName(string name){this->name_.assign(name);}
            void setVariableName(string varName){this->mvariableName.assign(varName);}
            string variableName(){return this->mvariableName;}
            void setType(Type t){this->mtype = t;}
            Type type(){return this->mtype;}

        private:
            string name_;                // Name of the Type(eg "REGION")
            string mvariableName;        // Name of the variable (eg "region")
            string objectType_;
            Type mtype;                  // is populated with data type iif isBO
            int nChildren_;         // No. of chilren
            bool isBO_;                  // Tracks if nodes is BO
            bool isSO_;
            bool isRO_;
            string mpointingToType;      // only present if isRO == true
            Node * pointingToNode;      // points to the note which the RO is pointing
            // Only valid for RO objects
            bool isList_;
            int pos_;                    // Position in the list of children
            Node * parent_;
            Node * child_;
            Node * next_;
    };


    class TSSParser
    {
        public:
            //default constructor
            TSSParser();
            // grammar_ is a file name if // isFile == true
            TSSParser(string grammar_, bool isFile);
            ~TSSParser();

            //This function will break the objects from path string, and store corresponding
            //label and accesscode in pathVector. If the given string path is invalid, then
            //this function will return false
            bool storeAccessCode(string path, vector<PathComponent>&);

            //This function will simply get pathVector from Path p object and
            //extract the access code from it. It will then return an int array
            //that contains the access code.
            int* genAccessCode(Path *p);           // Generate access code

            //This following 4 functions will traverse the grammar_ tree
            //and check whether the last object specified in path is BO,
            // SO, List, or Ref.
            bool isBO(Path *p);
            bool isSO(Path *p);
            bool isList(Path *p);
            bool isRO(Path *p);
            string getPointingType(Path *p);
            string getType(Path *p);
            Node *gotoEnd(Path *p, bool&);

            //return the root's type , this holds the data type of the TSS that the grammar_ describes
            string getGrammarType();

            //If the type of a particular object in grammar_ is BO, then
            //following function will return the exact type of that BO.
            //Type could be Int, IntAR, Double, DoubleAR, String, or Byte
            Type getBOType(Path *p);

        private:
            string dataTypeName_; // name of the main data type
            bool isDataTypeFound_; // true if dataTypeName_ has valid value
            Node *head_;      // Actual data structure to store the grammar_
            Node *current_;
            string * grammar_;
            int childCounter_;

            // Depth first search of grammar_ to check connected components
            bool visit(Node * );
            Node * getParentNode();

            // check connected graph or not
            bool isConnected(map<string, Node *>);
            bool validateGrammar();   // Validate the given grammar_
    };
}
#endif	/* TSSPARSER_H */
