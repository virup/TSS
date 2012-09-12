#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstring>
#include <set>
#include <map>
#include "/home/aistdev/ranjan/includes/iBLOB_V2/iBLOB/iBlob.h"

enum Type { UNDEFINED,
            CHAR, UCHAR,
            INT, UINT,
            SHORT, USHORT,
            LINT, ULINT,
            DOUBLE, LDOUBLE,
            FLOAT, WCHART, BOOL};

//struct used to hold path information :: building block of a PATH object
struct PathComponent
{
    std::string label;
	int accessCode;
	Locator loc;
};
// base data types allowed
const std::string baseTypesAllowed[] = {"char", "unsigned_char",
                                        "int", "unsigned_int",
                                        "short", "unsigned_short",
                                        "long_int", "unsigned_long_int",
                                        "double", "long_double",
                                        "float", "wchar_t", "bool"};

const std::set<std::string> baseTypes(baseTypesAllowed, baseTypesAllowed+13);


class TSSParser;
class Path;
#endif
