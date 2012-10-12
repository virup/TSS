#ifndef UDT_H
#define UDT_H

#include "iBlob.h"

class UDT
{
    protected:
        iBlob *store;
        bool bStorageAllocated;
        iBlob* getStore();

    public:
        UDT();
        int allocateLocation(iBlob *iblob);
        bool isStorageAllocated();
};
#endif
