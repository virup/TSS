#ifndef UDT_CPP
#define UDT_CPP

#include "UDT.h"

UDT::UDT()
{
    bStorageAllocated = false;
    store = NULL;
}

int UDT::allocateLocation(iBlob *iblob)
{
    this->store = iblob;
    bStorageAllocated = true;
}
bool UDT::isStorageAllocated()
{
    return bStorageAllocated;
}

iBlob *UDT::getStore()
{
    return store;
}
#endif
