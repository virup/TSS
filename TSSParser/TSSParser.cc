#include "Path.h"
#include "TSSParser.h"
#include "Scanner.h"
#include <cstring>
#include <map>
#include <queue>
#include <string>
#include <iostream>
using namespace std;

TSSParser::TSSParser() {
    childCounter = 0;
    head = NULL;
}

TSSParser::TSSParser(string grammar, bool isFile) {
    //will take care of file input later
    this->grammar = new string(grammar);
    childCounter = 0;
    head = NULL;
}

TSSParser::~TSSParser() {
    //delete all the nodes in grammar tree
    delete grammar;

}

bool TSSParser::validateGrammar() {
    //first check if syntax is correct
    Scanner scanner(*grammar);
    if (!scanner.scan()) {
        cout << "Grammar has syntax errors\n";
        return false;
    } else {
        cout << "Building individual tree\n";
        //check semantics while building tree
        char * grammarC = new char[grammar->size() + 1];
        grammarC[grammar->size()] = 0;
        memcpy(grammarC, grammar->c_str(), grammar->size());

        char * tokens;
        string * stok;
        tokens = strtok(grammarC, "<>-=");
        while (tokens != NULL) {
            stok = new string(tokens);
            //cout << *stok << endl;
            tokens = strtok(NULL, "<>-=");
            buildTree(*stok);
            delete stok;
        }
        delete grammarC;
    }
    //cout << "vector size: " << nodes.size() << endl;
    //print();
    cout << "Building done\n";
    linkTrees();
    cout << "Linking done\n";
    // print();
    return true;

}

void TSSParser::buildTree(string &str) {
    //Couple of possible strings that come in here
    //1. onjectName : objectType
    //2. ;
    //3. [];
    //4. [](*pointerName);  -- Here we ingore (*pointerName) i.e we don't create node for it
    //5 (*pointerName) -- again, we don't create node for this
    //6 []
    //  (*pointerName);


    //type 1
    if (str.compare(";") == 0) {
        //cout << "Storing: " << head->name << endl;
        //store curent head in vector
        Node * myHead = new Node;
        copy(myHead, head);
        nodes.push_back(myHead);
        head = NULL;
        childCounter = 0;
    } else if (str.at(0) == '[') {
        //this means that previous node that we built
        //is list. Set the list flag
        //cout << "It is [];\n";
        current->isList = true;

        //check if we have ; at the end 
        if (str.at(str.size() - 1) == ';') {
            //cout << "Storing: " << head->name << endl;
            Node * myHead = new Node;
            copy(myHead, head);
            nodes.push_back(myHead);
            head = NULL;
            childCounter = 0;
        }
    } else if (str.at(0) == '(') {
        //skip this part. But check if we have ';' in end
        if (str.at(str.size() - 1) == ';') {
            //cout << "Storing: " << head->name << endl;
            Node * myHead = new Node;
            copy(myHead, head);
            nodes.push_back(myHead);
            head = NULL;
            childCounter = 0;
        }
    } else {
        //Only come here if we have string case 1

        if(head == NULL){
            head = new Node();
            head->no_of_children = 0;
            current = head;
        } else {
            //create new child node
            current = new Node();
            
            if (head->no_of_children == 0) {
                //only create 1 pointer to first child
                head->child = current;
                current->parent = head;
                current->pos = childCounter;
                ++head->no_of_children;
                ++childCounter;
            } else {
                //link child with its sibling
                Node * temp = head->child;
                while (temp->next != NULL)
                    temp = temp->next;
                temp->next = current;
                current->parent = head;
                current->pos = childCounter;
                ++head->no_of_children;
                ++childCounter;
            }
        }

        //extract name and type from the given string
        string temp = "";
        string::iterator iter;
        for (iter = str.begin(); iter < str.end(); iter++) {
            if (*iter == ':') {
                //this is our name
                current->name = temp;
                temp = "";
                ++iter;
            }
            temp += *iter;
        }

        //this is our type
        current->objectType = temp;
        //set appropriate flag
        if ((temp.compare("SO")) == 0)
            current->isSO = true;
        else if ((temp.compare("RO")) == 0)
            current->isRO = true;
        else {
            current->isBO = true;
            //now store the appropriate type
            if(temp.compare("I") == 0)
                current->type = Int;
            else if(temp.compare("IA") == 0)
                current->type = IntAR;
            else if(temp.compare("D") == 0)
                current->type = Double;
            else if(temp.compare("DA") == 0)
                current->type = DoubleAR;
            else if(temp.compare("S") == 0)
                current->type = String;
            else if(temp.compare("B") == 0)
                current->type = Byte;
        }

        //link this child to the head
       // if(!headNull) {
           if (head->child != NULL)
              head->children.insert(pair<string, Node*>(current->name, current));
        //}
    }

}

void TSSParser::linkTrees() {
    cout << "Linking started\n";
    //remove first head from the list
    head = new Node();
    copy(head, nodes.front());
    delete nodes.front();
    nodes.pop_front();

    //init queue for BFS
    queue <Node*> myQueue;

    //cout << head->name << "'s children: " << head->children.size() << endl;
    current = head->child;

    while (current != NULL) {
        if (current->isSO)
            myQueue.push(current);
        current = current->next;
    }


    //fire up BFS. Note that we won't have any cycles in our graph, 
    //and such, we will not need to check whether a node has been already visited
    while (!myQueue.empty()) {

        //copy(current, myQueue.front());
        //delete myQueue.front();
        current = myQueue.front();


        //find current in list
        bool found = false;
        int i = 0;
        while (i < nodes.size()) {
            //cout << "Current name= " << current->name << ":: Front: " << nodes.front()->name << endl;
            if (current->name.compare(nodes.front()->name) == 0 && nodes.front()->isSO) {
                // cout << "Match found\n";
                found = true;
                //link the 2 nodes, push it in queue, and remove it from the list

                linkNodes(current, nodes.front());
                delete nodes.front();
                nodes.pop_front();

                //push children
                current = current->child;
                while (current != NULL) {
                    if (current->isSO)
                        myQueue.push(current);
                    current = current->next;
                }

                //delete temp;
                break;
            }

            Node * temp = new Node();
            copy(temp, nodes.front());
            delete nodes.front();
            nodes.pop_front();
            nodes.push_back(temp);

            ++i;
        }

        if (!found) {
            cout << "ERROR! " << current->name << " is not defined\n";
            exit(1);
        }

        found = false;
        //delete current;
        myQueue.pop();
    }

}

bool TSSParser::storeAccessCode(string path, vector<PathComponent>& pathVector) {
    //first step is tokenize the path string. The format of such string will be as follow
    //objectName.objectName[index].objectName[index].objectName
    //where objectName is of type string, and index will be some number. If we find a number, 
    //we simply ignore it.
    cout << "Storing access code\n";
    bool returnVal = true;
    char * tmp;
    string * str;
    PathComponent pathComp;
    tmp = strtok((char*) path.c_str(), ".[]");
    while (tmp != NULL) {
        //cout << tmp << endl;
        str = new string(tmp);
        if (!isdigit(str->at(0))) {
            pathComp.label = *str;
            pathVector.push_back(pathComp);
        }

        tmp = strtok(NULL, ".[]");
    }

    Node * current = head;
    //now we have the path stored in vector. Traverse the tree with this path and store the
    //access code in the pathVector
    if (head->name.compare(pathVector.at(0).label) == 0) {
        pathVector.at(0).accessCode = 0;
        for (int i = 1; i < pathVector.size(); i++) {
            //get next child
            if (current->children.find(pathVector.at(i).label) != current->children.end()) {
                current = current->children.at(pathVector.at(i).label);
                pathVector.at(i).accessCode = current->pos;
            } else {
                cout << pathVector.at(i).label << " is not a valid path\n";
                returnVal = false;
                break;
            }

        }
    } else {
        cout << "Path is invalid\n";
        returnVal = false;
    }

    return returnVal;
}

int * TSSParser::genAccessCode(Path p) {
    int * accessCodes = new int[p.vect.size()];
    for (int i = 0; i < p.vect.size(); i++)
        accessCodes[i] = p.vect.at(i).accessCode;
    return accessCodes;
}

bool TSSParser::isBO(Path p) {
    bool retVal = true;
    Node * current = head;
    if (p.vect.at(0).label.compare(head->name) == 0) {
        for (int i = 1; i < p.vect.size(); i++) {
            if (current->children.find(p.vect.at(i).label) != current->children.end()) {
                current = current->children.at(p.vect.at(i).label);
            } else {
                cout << p.vect.at(i).label << " is not a valid path\n";
                retVal = false;
                break;
            }
        }
    } else {
        retVal = false;
        cout << "Path is invalid\n";
    }
    
    if(retVal) {
        if(current->isBO)
            retVal = true;
        else
            retVal = false;
    }
        
    
    return retVal;
}

bool TSSParser::isSO(Path p) {
    bool retVal = true;
    Node * current = head;
    if (p.vect.at(0).label.compare(head->name) == 0) {
        for (int i = 1; i < p.vect.size(); i++) {
            if (current->children.find(p.vect.at(i).label) != current->children.end()) {
                current = current->children.at(p.vect.at(i).label);
            } else {
                cout << p.vect.at(i).label << " is not a valid path\n";
                retVal = false;
                break;
            }
        }
    } else {
        retVal = false;
        cout << "Path is invalid\n";
    }
    
    if(retVal) {
        if(current->isSO)
            retVal = true;
        else
            retVal = false;
    }
        
    
    return retVal;
}

bool TSSParser::isList(Path p) {
    bool retVal = true;
    Node * current = head;
    if (p.vect.at(0).label.compare(head->name) == 0) {
        for (int i = 1; i < p.vect.size(); i++) {
            if (current->children.find(p.vect.at(i).label) != current->children.end()) {
                current = current->children.at(p.vect.at(i).label);
            } else {
                cout << p.vect.at(i).label << " is not a valid path\n";
                retVal = false;
                break;
            }
        }
    } else {
        retVal = false;
        cout << "Path is invalid\n";
    }
    
    if(retVal) {
        if(current->isList)
            retVal = true;
        else
            retVal = false;
    }
        
    
    return retVal;
}

bool TSSParser::isRef(Path p) {
    bool retVal = true;
    Node * current = head;
    if (p.vect.at(0).label.compare(head->name) == 0) {
        for (int i = 1; i < p.vect.size(); i++) {
            if (current->children.find(p.vect.at(i).label) != current->children.end()) {
                current = current->children.at(p.vect.at(i).label);
            } else {
                cout << p.vect.at(i).label << " is not a valid path\n";
                retVal = false;
                break;
            }
        }
    } else {
        retVal = false;
        cout << "Path is invalid\n";
    }
    
    if(retVal) {
        if(current->isRO)
            retVal = true;
        else
            retVal = false;
    }
        
    
    return retVal;
}

Type TSSParser::getBOType(Path p) {
    Node * current = head;
    if (p.vect.at(0).label.compare(head->name) == 0) {
        for (int i = 1; i < p.vect.size(); i++) {
            if (current->children.find(p.vect.at(i).label) != current->children.end()) {
                current = current->children.at(p.vect.at(i).label);
            } else {
                cout << p.vect.at(i).label << " is not a valid path\n";
                return Undefined;
            }
        }
    } else {
        cout << "Path is invalid\n";
        return Undefined;
    }
    
    if(current->isBO)
        return current->type;
    else {
        cout << "Object is not BO\n";
        return Undefined;
    }
            
}

void TSSParser::print() {
    cout << "Printing\n";
    queue <Node*> printQueue;

    printQueue.push(head);

    while (!printQueue.empty()) {
        Node * temp = printQueue.front();
        int size = temp->children.size();
        cout << "\nParent: " << temp->name << " :: children: " << size << endl;

        //push children
        temp = temp->child;

        for (int i = 0; i < size; i++) {
            cout << "\tChild(" << i << "): " << temp->parent->children.at(temp->name)->name << endl;
            if (temp->isSO)
                printQueue.push(temp->parent->children.at(temp->name));
            temp = temp->next;
        }

        //pop parent
        printQueue.pop();
    }

}

void TSSParser::cleanUp() {

}

void TSSParser::copy(Node *a, Node *b) {
    a->child = b->child;
    a->children.insert(b->children.begin(), b->children.end());
    a->name = b->name;
    a->next = b->next;
    a->no_of_children = b->no_of_children;
    a->objectType = b->objectType;
    a->parent = b->parent;
    a->pos = b->pos;
    a->type = b->type;
    a->visited = b->visited;
    a->isBO = b->isBO;
    a->isList = b->isList;
    a->isRO = b->isRO;
    a->isSO = b->isSO;
}

void TSSParser::linkNodes(Node* a, Node* b) {
    a->child = b->child;
    a->children.insert(b->children.begin(), b->children.end());
    a->no_of_children = b->no_of_children;
}