#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "/home/aistdev/ranjan/includes/iBLOB_V2/iBLOB/iBlob.h"
enum Type {Int, IntAR, Double, DoubleAR, String, Byte, Undefined};

//struct used to hold path information :: building block of a PATH object
struct PathComponent
{
	string label;
	int accessCode;
	Locator loc;
};
class TSSParser;
class Path;
#endif
