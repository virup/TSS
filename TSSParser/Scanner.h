/*
 * Scanner.h
 * The grammar used for this scanner is given below
 * 
 * Program   ::= stmt+
 * stmt      ::= '<' Taghead '>' '-=' (Tag Reference* | Taglist Reference*)+ ';'
 * Taghead   ::= Name ':' Flag
 * Taglist   ::= Tag '[]'
 * Tag       ::= '<' Name ':' Flag '>'
 * Reference ::= '( *' Name ')'
 * Name      ::= ident
 * Flag      ::= 'SO' | 'RO' | 'I' | 'D' | 'IA' | 'DA' | 'S' | 'B'
 * ident     ::= identstart | identpart*
 * identstart::= A-Z | a-z
 * identpart ::= identstart | (0-9)*  
 * 
 * 
 * All the nonterminal symbols have function of their own. Only exception are
 * ident, identpart, and identstart as this has been taken care in Name function
 * 
 * 
 */

#ifndef SCANNER_H_
#define SCANNER_H_

#include <string>
using namespace std;

class Scanner {
private:
    string * grammar;
    int index;
    char ch;
    bool errors;

    bool Program();
    bool Stmt();
    bool TagHead();
    bool TagList();
    bool Tag();
    bool Reference();
    bool Name();
    bool Flag();
    char getNextChar();


    void decrementIndex();
    int isAlphaNumeric();
    void removeWhiteSpaces();


public:
    //constructor that accepts user grammar
    //in form of a string
    Scanner(string &);
    ~Scanner();

    //scans the given string and returns
    //true if it fits the TSS grammar
    bool scan();

};


#endif /* SCANNER_H_ */
