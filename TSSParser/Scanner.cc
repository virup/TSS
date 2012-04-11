/*
 * Scanner.cc
 *
 *
 */
#include "Scanner.h"
#include <iostream>
#define UPPER_START 64
#define UPPER_END 91
#define LOWER_START 96
#define LOWER_END 123
#define NUMBER_START 47
#define NUMBER_END 58

using namespace std;

Scanner::Scanner(string &grammar) {
    this->grammar = &grammar;
    removeWhiteSpaces();
    index = -1;
    errors = false;
    //grammar = removeWhiteSpaces(grammar);
    //cout << "Grammar : " << *(this->grammar) << endl;

}

Scanner::~Scanner() {

}

bool Scanner::scan() {
    if (grammar->length() != 0)
        return Program();
    else {
        cout << "WARNING : No grammar was passed\n";
        return true;
    }
}

bool Scanner::Program() {
    bool ret = false;
    while ((ch = getNextChar()) != '\0') {
        ret = Stmt();
        if (!ret) {
            cout << "Error in Program\n";
            cout << "Current Char -- " << ch << endl;
            return false;
        }
    }
    return ret;
}

bool Scanner::Stmt() {
    if (ch == '<') {
        ch = getNextChar();
        if (TagHead()) {
            ch = getNextChar();
            if (ch == '>') {
                ch = getNextChar();
                if (ch == '-') {
                    ch = getNextChar();
                    if (ch == '=') {

                        do {
                            ch = getNextChar();
                            //cout << ch << endl;
                            if (Tag()) {
                                ch = getNextChar();
                                if (!Reference())
                                    break;
                            } else
                                break;
                        } while (ch != '\0');

                        if (ch == ';' && !errors) {
                            return true;
                        } else {
                            cout << "Error : Missing ';' in stmt\n";
                            return false;
                        }

                    } else {
                        cout << "Error : Missing '=' in stmt\n";
                        return false;
                    }
                } else {
                    cout << "Error : Missing ':' in stmt\n";
                    return false;
                }
            } else {
                cout << "Error : Missing '>' in stmt\n";
                return false;
            }
        } else {
            cout << "Error in Taghead\n";
            cout << "Current Char -- " << ch << endl;
            return false;
        }
    } else {
        cout << "Error: Missing '<' in stmt\n";
        return false;
    }
}

bool Scanner::Tag() {
    //Try with Tag. If we find [, then call TagList function
    if (ch == '<') {
        if (Name()) {
            ch = getNextChar();
            if (Flag()) {
                ch = getNextChar();
                if (ch == '>') {
                    ch = getNextChar();

                    //see if we can build tagList
                    if (ch == '[') {
                        ch = getNextChar();
                        return TagList();
                    } else {
                        decrementIndex();
                        return true;
                    }
                } else {
                    errors = true;
                    cout << "Error : Missing '>' in Tag\n";
                    return false;
                }
            } else {
                errors = true;
                cout << "Error: Flag (inside Tag)\n";
                return false;
            }
        } else {
            errors = true;
            cout << "Possible errors\n";
            cout << "Missing ':' in Name - Flag\n";
            cout << "OR Invalid ident name\n";
            return false;
        }
    } else {
        //errors = true;
        //cout << "Error : Missing < in Tag\n";
        return false;
    }
}

bool Scanner::TagHead() {
    bool ret, retF;
    ret = Name();
    if (ret) {
        ch = getNextChar();
        retF = Flag();
    } else {
        errors = true;
        cout << "Possible errors\n";
        cout << "Missing ':' in TagHead\n";
        cout << "OR Invalid ident name\n";
        retF = false;
    }
    return (ret && retF);
}

bool Scanner::TagList() {
    //we already got [. So start from there
    if (ch == ']') {
        return true;
    } else {
        errors = true;
        cout << "Missing ']' in Taglist\n";
        return false;
    }

}

bool Scanner::Reference() {
    //see if we have (. If no, then its OK. 
    //Otherwise, rest of the statement must
    //be finished.
    if (ch == '(') {
        ch = getNextChar();
        if (ch == '*') {
            ch = getNextChar();
            if (Name()) {
                if (ch == ')')
                    return true;
                else {
                    errors = true;
                    cout << "ERROR : Missing ')' in Reference\n";
                }

            } else {
                errors = true;
                cout << "Invalid ident name\n";
                return false;
            }
        } else {
            errors = true;
            cout << "Error : Missing '*' in Reference\n";
            return false;
        }

    } else {
        decrementIndex();
        return true;
    }
}

bool Scanner::Name() {
    //first character should not be number
    int res = 0;
    if ((res = isAlphaNumeric()) == 0)
        return false;

    //rest of them can be any combination of valid alphanumeric characters
    do {
        ch = getNextChar();
        res = isAlphaNumeric();

    } while (res == 1 || res == 0);

    if (ch == ')' || ch == ':') {
        return true;
    } else {
        errors = true;
        return false;
    }
}


bool Scanner::Flag() {
    //ch = getNextChar();
    if (ch == 'R') {
        ch = getNextChar();
        if (ch == 'O') {
            return true;
        } else {
            errors = true;
            cout << "ERROR: Missing 'O' in Flag\n";
            return false;
        }
    } else if (ch == 'D' || ch == 'I') {
        ch = getNextChar();
        if (ch == 'A') {
            return true;
        }
        //getPreviousChar();
        decrementIndex();
        return true;

    } else if (ch == 'S') {
        ch = getNextChar();
        if (ch == 'O')
            return true;
        //getPreviousChar();
        decrementIndex();
        return true;

    } else if (ch == 'B')
        return true;

    return false;
}

int Scanner::isAlphaNumeric() {
    if ((ch > UPPER_START && ch < UPPER_END) || (ch > LOWER_START && ch < LOWER_END))
        return 1;
    else if (ch > NUMBER_START && ch < NUMBER_END)
        return 0;
    else
        return -1;
}

char Scanner::getNextChar() {
    ++index;
    if (index < (int) grammar->length()) {
        return grammar->at(index);
    } else
        return '\0';
}

void Scanner::decrementIndex() {
    --index;
}

void Scanner::removeWhiteSpaces() {
    string temp = "";
    string::iterator it;
    for (it = grammar->begin(); it < grammar->end(); it++) {
        if (isspace((int) *it))
            continue;
        temp += *it;
    }

    grammar->clear();
    grammar->append(temp);
}
