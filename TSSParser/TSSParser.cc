#include "TSSParser.h"
#include "Scanner.h"
#include <cstring>
#include <map>
#include <queue>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
using namespace std;


void visitTree(TSSParser::Node *head)
{
    if(head->parent)
        cout<<" Parent = "<<head->parent->name<<"  ";
    cout<<"Self = "<<head->name<<"  "<<head->type<<endl;
    if(head->children.size()!= 0)
    {
        map<string, TSSParser::Node *>::iterator it = head->children.begin();
        for(; it != head->children.end(); it++)
            visitTree(it->second);
    }
    cout<<endl;
}

TSSParser::TSSParser() {
    childCounter = 0;
    head = NULL;
}

TSSParser::TSSParser(string grammar, bool isFile) {
    //will take care of file input later
    if(!isFile)
    {

    }
    else
    {


    }
    this->grammar = new string(grammar);
    childCounter = 0;
    head = NULL;
    validateGrammar_1();
    //buildTree_1(*this->grammar);
    visitTree(head);
}

TSSParser::~TSSParser() {
    //delete all the nodes in grammar tree
    ROPointers.clear();
    ROObjects.clear();
    delete grammar;

}


bool onlySpaces(std::string& str)
{
    for(uint i = 0; i < str.length(); i++)
        if(str[i] != ' ')
            return false;

    return true;
}


string removeChars(string& str, char avoidChars[])
{
    for (unsigned int i = 0; i < sizeof(avoidChars); ++i)
    {
        str.erase (std::remove(str.begin(), str.end(), avoidChars[i]),
                str.end());
    }
    return str;
}

string removeBraces(string &str)
{
    char avoidChars[] = "{}";
    return removeChars(str,avoidChars);
}

string removePlus(string str)
{
    char avoidChars[] = "+";
    string checkstr(str);
    return removeChars(checkstr,avoidChars);
}


string removeWhitespaces(string str)
{
    char avoidChars[] = " ";
    return removeChars(str,avoidChars);

}


bool checkChars(char s, string avoidChars)
{
    const char *p = avoidChars.c_str();
    for(uint i = 0; i < avoidChars.length(); i++)
        if(s == *(p+i))
            return true;

    return false;
}

void ltrim(string& s)
{
    int start = 0;
    for(uint i = 0; checkChars(s[i], ": ;{}"); ++i,start=i)
        ;

    s.assign(s.substr(start));

}


void rtrim(string& s)
{
    int end = s.length();
    for(uint i = s.length()-1; checkChars(s[i], ": ;{}"); i--)
        end = i;


    s.assign(s.substr(0, end));
}

string trimplus(string s)
{
    int end = s.length();
    for(uint i = s.length()-1; checkChars(s[i], "+"); i--)
        end = i;


    return (s.substr(0, end));

}



string trimsides(string& s)
{
    ltrim(s);
    rtrim(s);
    return s;

}

vector<string> TSSParser::tokenize(string s, string separator)
{
    std::stringstream ss(s);
    vector<std::string> elems;
    std::string item;
    while(std::getline(ss, item, *separator.c_str())) {
        if(!onlySpaces(item)){
            trimsides(item);
            elems.push_back(item);
        }
    }
    return elems;
}


string trim(string &str)
{
    char avoidChars[] = "& ;=";
    return removeChars(str,avoidChars);
}


bool isInside(vector<string> list, string word)
{
    for(uint i = 0; i < list.size(); i++)
        if(trim(list[i]).compare((trimplus(word)))==0)
            return true;

    return false;
}

bool checkAllBO(vector<string>& bo)
{
    vector<string> tagList;
    tagList.push_back("I");
    tagList.push_back("D");
    tagList.push_back("IA");
    tagList.push_back("DA");
    tagList.push_back("B");
    tagList.push_back("S");

    for(uint i = 0 ; i < bo.size();i++)
    {
        string tag = bo[i].substr(bo[i].find(":")+1,string::npos);
        if(!isInside(tagList,tag))
        {
            cerr<<"Wrong tags in "<<bo[i];
            throw "Wrong tag in ";
        }

    }
    return true;
}


bool reorderStatements(vector<string>& statements)
{
   vector<string> declarations;

    for(uint i = 0; i < statements.size(); )
    {
        if(statements[i].find("{") != string::npos)
        {
            declarations.push_back(statements[i]);
            statements.erase(statements.begin() + i);
        }
        else
            i++;
    }

    if(declarations.size() != 3)
    {
        cerr<<"Not all required statements declared"<<endl;
        throw "Not all required statements declared";
    }
    else
    {
        statements.insert(statements.begin(), declarations.begin(), declarations.end());
    }
}



Type getType(string s)
{
    string type = s.substr(s.find(":")+1, string::npos);
    cout<<"s = "<<s<<"  Type = "<< type<<endl;

    if(type.compare("I"))
        cout<<"INT"<<endl;

    if(type.compare("IA"))
        cout<<"INT ARR";

    if(type.compare("D"))
        cout<<"Double";

    if(type.compare("DA"))
        cout<<"DOUBLE ARR";

    if(type.compare("S"))
        cout<<"STRING";

    if(type.compare("B"))
        cout<<"BYTE";

    if(type.compare("I")==0)
        return Int;

    if(type.compare("IA")==0)
        return IntAR;

    if(type.compare("D")==0)
        return Double;

    if(type.compare("DA")==0)
        return DoubleAR;

    if(type.compare("S")==0)
        return String;

    if(type.compare("B")==0)
        return Byte;

    return Undefined;

}


bool isList(string s)
{
    return (s.find("+") == s.length()-1)? true: false;
}


bool TSSParser::visit(TSSParser::Node *n)
{
    if(n->visited)
        return true;
    else{
        n->visited = true;
        for(map<string, Node *>::iterator it = n->children.begin();
                it != n->children.end(); it++)
            visit(it->second);
    }

}


TSSParser::Node * TSSParser::checkTreeAndReturnHead(map<string, TSSParser::Node*> nodeMaps)
{
   TSSParser::Node *head = NULL;
   map<string, TSSParser::Node *>::iterator it;
   int parentMissing = 0;
   for(it = nodeMaps.begin(); it != nodeMaps.end(); it++)
       if(it->second->parent == NULL && it->second->isBO != true)
       {
           parentMissing++;
           head = it->second;
        }

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
            throw "Hierarchy not well formed";
       }

   return head;
}


void printlist(vector<string> l)
{
    cout<<"================="<<endl;
    for(uint i = 0; i < l.size(); i++)
        cout<<l[i]<<endl;
    cout<<"================="<<endl;

}


//string removePlus(string s)
//{
//    return s.substr(0,s.find("+")-1);
//}
//
bool TSSParser::validateGrammar_1(){
    vector<string> statements = tokenize(*this->grammar, ";");
    vector<string> so;
    vector<string> ro;
    vector<string> bo;


    reorderStatements(statements);

    map<string, TSSParser::Node*> nodeMaps;
    map<string, TSSParser::Node*> boMaps;

    for(uint i = 0; i < statements.size(); i++)
    {
        if(statements[i].find("{") != string::npos)
        {
            vector<string> set = tokenize(statements[i], "=");
            if(set.size() != 2)
            {
                cerr<<"Error in the statement : "<<statements[i]<<endl;
                throw "Error in statement";
            }
            else
            {
                string nameList = removeBraces(set[1]);

                vector<string> names = tokenize(nameList, ",");
                if(set[0].find("S") != string::npos)
                {
                    so.insert(so.begin(), names.begin(), names.end());
                    for(uint lv = 0; lv < so.size(); lv++)
                    {
                        Node *n = new Node;
                        n->setName(so[lv]);
                        n->setObjectType("SO");
                        n->setType(Undefined);
                        n->setSO();
                        nodeMaps.insert(pair<string, Node *>(trimsides(so[lv]), n));
                    }
                }
                else if(set[0].find("R") != string::npos)
                {
                    ro.insert(ro.begin(), names.begin(), names.end());
                    for(uint lv = 0; lv < ro.size(); lv++)
                    {
                        Node *n = new Node;
                        n->setName(ro[lv]);
                        n->setObjectType("SO");
                        n->setRO();
                        n->setType(Undefined);
                        nodeMaps.insert(pair<string, Node *>(ro[lv], n));
                    }
                }
                else if(set[0].find("B") != string::npos)
                {
                    bo.insert(bo.begin(), names.begin(), names.end());
                    checkAllBO(bo);
                    for(uint lv = 0; lv < bo.size();  lv++)
                    {
                        Node *n = new Node;
                        n->setName(bo[lv].substr(0,bo[lv].find(":")));
                        n->setObjectType("BO");
                        n->setBO();
                        n->setType(getType(bo[lv]));
                        bo[lv].assign(bo[lv].substr(0, bo[lv].find(":")));
                        boMaps.insert(pair<string, Node *>(bo[lv].substr(0,bo[lv].find(":")), n));
                    }
                }
            }
        }
        else // normal statment
        {
            string statement = statements[i];
            size_t pos = statement.find("::=");
            if(pos == string::npos)
            {
                cerr<<"Not a well formed statement : "<<statements[i]<<endl;
                throw "Not a well formed statment";
            }
            string RHS = statement.substr(pos + 3 );
            string LHS = statement.substr(0,pos);
            LHS = trim(LHS);
            vector<string> RHSs = tokenize(RHS, " ");
            Node *earlierChild = NULL;
            if(isInside(so,LHS)) // LHS == SO
            {
                Node *nLHS = nodeMaps[trimsides(LHS)];
                earlierChild = NULL;

                for(uint j= 0; j < RHSs.size(); j++)
                {

                    string curedString(RHSs[j].substr(0, RHSs[j].find(":")));
                    if(!isInside(so,curedString) && !isInside(ro,curedString) && !isInside(bo, curedString))
                    {
                        cerr<<RHSs[j]<<"Not found in "<<statements[i]<<endl;
                        throw "Error in statements";
                    }
                    if(RHSs[j] == LHS)
                    {
                        cerr<<RHSs[j]<<"is recursive in "<<statements[i]<<endl;
                        throw "Error in statements";
                    }

                    // create the tree

                    if(isInside(bo, curedString))
                    {
                        string s(trimplus(RHSs[j]));
                        s.assign(s.substr(0,s.find(":")));
                        Node * n = boMaps.find(s)->second;
                        Node *newNode = new Node();
                        newNode->setName(n->name);
                        newNode->setObjectType(n->objectType);
                        newNode->setBO();
                        newNode->setType(n->type);
                        newNode->setPos(j);
                        newNode->setList(::isList(RHSs[j]));
                        if(earlierChild!=NULL)
                        {
                            cout<<"Earlier child not NULL"<<endl;
                            earlierChild->setNext(newNode);
                        }

                        nLHS->addChild(trimsides(curedString),newNode);
                    }
                    else
                    {
                        Node * n = nodeMaps.find(trimplus(RHSs[j]))->second;
                        n->setPos(j);
                        n->setList(::isList(RHSs[j]));
                        if(earlierChild!=NULL)
                            earlierChild->setNext(n);
                        nLHS->addChild(trimplus(RHSs[j]),n);
                    }
                }
            }
            else if(isInside(ro, LHS)) //LHS == RO
            {

                if(RHSs.size() > 1)
                {
                    cerr<<"RO should point to only one object "<<statements[i]<<endl;
                    throw "Error in statement";
                }
                if(RHSs[0][0] != '&')
                {
                    cerr<<"Expected &SO or &RO in the right hand side of  "<<statements[i]<<endl;
                    throw "Error in statement";
                }

                string curedString(RHSs[0].substr(0, RHSs[0].find(":")));
                curedString.assign(trimplus(curedString.substr(1)));
                if(!isInside(so,curedString) && !isInside(bo, curedString))
                {
                    cerr<<RHSs[0]<<" not in S set or B set in "<<statements[i]<<endl;
                    throw "Error in statement";
                }
                    Node * n = nodeMaps.find(LHS)->second;
                    n->setObjectType(curedString);
            }
            else
            {
                cerr<<LHS<< " not in S set or R set in "<<statements[i]<<endl;
                throw "Error in statement";
            }
        }
    }
    head = checkTreeAndReturnHead(nodeMaps);
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

    for (uint i = 0; i < ROPointers.size(); i++) {
        cout << "Pointer(" << i << ") --> " << ROPointers.at(i) << endl;
    }
    for (uint i = 0; i < ROObjects.size(); i++) {
        cout << "Object(" << i << ") --> " << ROObjects.at(i) << endl;
    }

    linkTrees();
    cout << "Linking done\n";
    // print();
    return true && matchRO();
}

void TSSParser::buildTree(string &str) {
    //Couple of possible strings that come in here
    //1. onjectName : objectType
    //2. ;
    //3. [];
    //4. [](*pointerName);  -- Here we ingore (*pointerName) i.e we don't create node for it
    //5 (*pointerName) -- again, we don't create node for this
    //6 []



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

        //now, after ']', we can have a RO pointer. Check if this is the case

        if (str.size() > 3) {
            //yes, it is the case. Extract the name of the RO pointer, and store it in vector.

            string temp = "";
            int index = 4;
            while (str.at(index) != ')') {
                temp += str.at(index);
                ++index;
            }

            //store this in RO pointer vector
            ROPointers.push_back(temp);
        }

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
        //Extract RO pointer. 
        string temp = "";
        int index = 2;
        while (str.at(index) != ')') {
            temp += str.at(index);
            ++index;
        }

        //store this in RO pointer vector
        ROPointers.push_back(temp);

        //check if we have ';' in end
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

        if (head == NULL) {
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
        else if ((temp.compare("RO")) == 0) {
            current->isRO = true;
            //also store this in ROObject vector
            string temp (current->name);
            ROObjects.push_back(temp);
        }else {
            current->isBO = true;
            //now store the appropriate type
            if (temp.compare("I") == 0)
                current->type = Int;
            else if (temp.compare("IA") == 0)
                current->type = IntAR;
            else if (temp.compare("D") == 0)
                current->type = Double;
            else if (temp.compare("DA") == 0)
                current->type = DoubleAR;
            else if (temp.compare("S") == 0)
                current->type = String;
            else if (temp.compare("B") == 0)
                current->type = Byte;
        }

        //link this child to the head
        // if(!headNull) {
        if (head->child != NULL)
            head->children.insert(pair<string, Node*>(current->name, current));
        //}
    }
    //cout<<"===================="<<endl;
    //cout<<"Head =  "<<head<<endl;

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


void printChildren(map<string, TSSParser::Node *> maplist)
{
    map<string, TSSParser::Node *>:: iterator it;
    cout<<"Children are: "<<endl;
    for(it = maplist.begin(); it!= maplist.end(); it++)
    {
        cout<<it->first<<endl;
    }

}


bool TSSParser::storeAccessCode(string strpath, vector<PathComponent>& pathVector) {
    //first step is tokenize the path string. The format of such string will be as follow
    //objectName.objectName[index].objectName[index].objectName
    //where objectName is of type string, and index will be some number. If we find a number, 
    //we simply ignore it.
    cout<<strpath<<endl;
    bool returnVal = true;
    char * tmp;
    string * str;
    PathComponent pathComp;
    char *path = new char[strpath.length() + 1 ];
    strpath.copy(path,strpath.length());
    path[strpath.length()] = 0 ;
    tmp = strtok((char*) path, ".[]");
    while (tmp != NULL) {
        str = new string(tmp);
        if (!isdigit(str->at(0))) {
            pathComp.label = *str;
            pathVector.push_back(pathComp);
        }
        else
        {
            pathComp.label.assign("List");
            pathComp.accessCode = atoi(str->c_str());
            pathVector.push_back(pathComp);
        }
        tmp = strtok(NULL, ".[]");
    }

    Node * current = this->head;
    //now we have the path stored in vector. Traverse the tree with this path and store the
    //access code in the pathVector
    //cout<<"storeAccessCode   "<<pathVector.at(0).label<<endl;
    if (head->name.compare(pathVector.at(0).label) == 0) {
        pathVector.at(0).accessCode = 0;
        for (int i = 1; i < pathVector.size(); i++) {
            //get next child
            if(pathVector[i].label.compare("List") == 0) continue;
            
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

int * TSSParser::genAccessCode(Path *p) {
    int * accessCodes = new int[p->vPath.size()];
    for (int i = 0; i < p->vPath.size(); i++)
        accessCodes[i] = p->vPath.at(i).accessCode;
    return accessCodes;
}

bool TSSParser::isBO(Path *p) {
    bool retVal = true;
    Node * current = head;
    if (p->vPath.at(0).label.compare(head->name) == 0) {
        for (int i = 1; i < p->vPath.size(); i++) {
            if(p->vPath[i].label.compare("List") == 0) continue;
            if (current->children.find(p->vPath.at(i).label) != current->children.end()) {
                current = current->children.at(p->vPath.at(i).label);
            } else {
                cout << p->vPath.at(i).label << " is not a valid path\n";
                retVal = false;
                break;
            }
        }
    } else {
        retVal = false;
        cout << "Path is invalid\n";
    }

    if (retVal) {
        if (current->isBO)
            retVal = true;
        else
            retVal = false;
    }


    return retVal;
}

bool TSSParser::isSO(Path *p) {
    bool retVal = true;
    Node * current = head;
    if (p->vPath.at(0).label.compare(head->name) == 0) {
        for (int i = 1; i < p->vPath.size(); i++) {
            if(p->vPath[i].label.compare("List") == 0) continue;
            if (current->children.find(p->vPath.at(i).label) != current->children.end()) {
                current = current->children.at(p->vPath.at(i).label);
            } else {
                cout << p->vPath.at(i).label << " is not a valid path\n";
                retVal = false;
                break;
            }
        }
    } else {
        retVal = false;
        cout << "Path is invalid\n";
    }

    if (retVal) {
        if (current->isSO)
            retVal = true;
        else
            retVal = false;
    }


    return retVal;
}

bool TSSParser::isList(Path *p) {
    bool retVal = true;
    Node * current = head;
    if (p->vPath.at(0).label.compare(head->name) == 0) {
        for (int i = 1; i < p->vPath.size(); i++) {
            if(p->vPath[i].label.compare("List") == 0) continue;
            if (current->children.find(p->vPath.at(i).label) != current->children.end()) {
                current = current->children.at(p->vPath.at(i).label);
            } else {
                cout << p->vPath.at(i).label << " is not a valid path\n";
                retVal = false;
                break;
            }
        }
    } else {
        retVal = false;
        cout << "Path is invalid\n";
    }

    if (retVal) {
        if (current->isList)
            retVal = true;
        else
            retVal = false;
    }


    return retVal;
}

bool TSSParser::isRef(Path *p) {
    bool retVal = true;
    Node * current = head;
    if (p->vPath.at(0).label.compare(head->name) == 0) {
        for (int i = 1; i < p->vPath.size(); i++) {
            if(p->vPath[i].label.compare("List") == 0) continue;
            if (current->children.find(p->vPath.at(i).label) != current->children.end()) {
                current = current->children.at(p->vPath.at(i).label);
            } else {
                cout << p->vPath.at(i).label << " is not a valid path\n";
                retVal = false;
                break;
            }
        }
    } else {
        retVal = false;
        cout << "Path is invalid\n";
    }

    if (retVal) {
        if (current->isRO)
            retVal = true;
        else
            retVal = false;
    }


    return retVal;
}

Type TSSParser::getBOType(Path *p) {
    Node * current = head;
    if (p->vPath.at(0).label.compare(head->name) == 0) {
        for (int i = 1; i < p->vPath.size(); i++) {
            if(p->vPath[i].label.compare("List") == 0) continue;
            if (current->children.find(p->vPath.at(i).label) != current->children.end()) {
                current = current->children.at(p->vPath.at(i).label);
            } else {
                cout << p->vPath.at(i).label << " is not a valid path\n";
                return Undefined;
            }
        }
    } else {
        cout << "Path is invalid\n";
        return Undefined;
    }

    if (current->isBO)
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

bool TSSParser :: matchRO() {
    int pointerSize = ROPointers.size();
    int objectSize = ROObjects.size();
    bool found = false;
    if(pointerSize != objectSize) {
        cout << "Either pointer to RO Object is missing OR RO object is undefined\n ";
        return false;
    }else {
        for(int i = 0; i < pointerSize; i++) {
            for (int j = 0; j < objectSize; j++) {
                if(ROPointers.at(i).compare(ROObjects.at(j)) == 0) {
                    found = true;
                    break;
                }
            }

            if(!found) {
                cout << "RO Object or RO pointer not found\n";
                return false;
            }

            found = false;
        }
    }

    return true;
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

string TSSParser::getGrammarType()
{
    return head->name;
}
