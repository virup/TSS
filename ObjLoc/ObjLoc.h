#ifndef OBJLOC_h
#define OBJLOC_h

#include "UDT.h"

class ObjLoc
{
    public:
    virtual int bind(UDT &) = 0;
};

#endif
