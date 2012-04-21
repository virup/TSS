/* 
 * File:   TSSParser.h
 * Author: Archesh
 *
 * Created on March 29, 2012, 3:28 PM
 */

#ifndef TSSPARSER_H
#define	TSSPARSER_H



#endif	/* TSSPARSER_H */

#include "Path.h"
#include "Scanner.h"
#include <cstring>
#include <cstdlib>
#include <map>
#include <string>
#include <list>
#include <vector>

using namespace std;

enum Type {Int, IntAR, Double, DoubleAR, String, Byte, Undefined};



class TSSParser
{
    private:
        
       struct Node
        {
            string name;                // Name of the node
            string objectType;
            Type type;                  // Data type iif isBO
            int no_of_children;         // No. of chilren
            bool isBO;                  // Tracks if nodes is BO
            bool isSO;
            bool isRO;
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
        };

        Node *head;      // Actual data structure to store the grammar
        Node *current;
        string * grammar;
        int childCounter;
        list<Node*> nodes;
        Node * getParentNode();
       
        //makes a copy of 'b' and stores it in 'a'
        //this function is need to store heads in vector
        void copy(Node *a, Node *b);
        
        void linkNodes(Node *a ,Node *b);
        
        //this function makes individual tree of each line
        //provided in the grammar. Then, it stores the head
        //of each tree in the list.
        void buildTree(string &);
        
        //this function links all the trees together in to 1 tree
        //using BFS algorithm
        void linkTrees();
        
        //this function prints the individual trees stored in the list
        void print();

        //visit all nodes and delete it to free up memory
        void cleanUp();
        

    public:

        TSSParser(); //default constructor
        TSSParser(string grammar, bool isFile); // grammar is a file name if 
                                                // isFile == true
        ~TSSParser();
         bool validateGrammar();   // Validate the given grammar
        
        //This function will break the objects from path string, and store corresponding
        //label and accesscode in pathVector. If the given string path is invalid, then
        //this function will return false
        bool storeAccessCode(string path, vector<PathComponent> &pathVector); 
        
        //This function will simply get pathVector from Path p object and
        //extract the access code from it. It will then return an int array
        //that contains the access code.
        int* genAccessCode(Path p);           // Generate access code

        //This following 4 functions will traverse the grammar tree
        //and check whether the last object specified in path is BO, 
        // SO, List, or Ref.
        bool isBO(Path p);
        bool isSO(Path p);
        bool isList(Path p);
        bool isRef(Path p);

        
        //If the type of a particular object in grammar is BO, then 
        //following function will return the exact type of that BO. 
        //Type could be Int, IntAR, Double, DoubleAR, String, or Byte
        Type getBOType(Path p);
        
        
};
