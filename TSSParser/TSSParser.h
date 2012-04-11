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
#include <map>
#include <string>
#include <list>

using namespace std;

enum Type {Int, IntAR, Double, DoubleAR, String, Byte};
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


    public:

        TSSParser(); //default constructor
        TSSParser(string grammar, bool isFile); // grammar is a file name if 
                                                // isFile == true
        ~TSSParser();
         bool validateGrammar();   // Validate the given grammar
        int* genAccessCode(Path p);           // Generate access code

        bool isBO(Path p);
        bool isSO(Path p);
        bool isList(Path p);
        bool isRef(Path p);

        Type getBOType(Path p);
        bool validatePath(Node *, string Name); // Validate a given path
};
