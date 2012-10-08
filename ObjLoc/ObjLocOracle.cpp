#ifndef OBJLOCORACLE_CPP
#define OBJLOCORACLE_CPP

#include "ObjLocOracle.h"

ObjLocOracle::ObjLocOracle(OCILobLocator *l,
                           OCIError *e,
                           OCISvcCtx *s)
{
    this->lob = l;
    this->errhp = e;
    this->svchp = s;
}

int ObjLocOracle::bind(UDT &object)
{
    iBlobStore *store = new iBlobOracleStore(this->lob,
                                             this->errhp,
                                             this->svchp);
    iBlob *p = new iBlob(store, false);
    object.allocateLocation(p);
}
#endif

