#ifndef STU_OBJLOCORACLE_CPP
#define STU_OBJLOCORACLE_CPP

#include "ObjLocOracle.h"

using namespace stu;
ObjLocOracle::ObjLocOracle(OCILobLocator *l,
                           OCIError *e,
                           OCISvcCtx *s)
{
    this->lob = l;
    this->errhp = e;
    this->svchp = s;
}

/* Bind() : 1. Creates an iBLob object
 *          2. Linkes the iBlob to the UDT
 */
int ObjLocOracle::bind(UDT &object)
{
    iBlobStore *store = new iBlobOracleStore(this->lob,
                                             this->errhp,
                                             this->svchp);
    iBlob *p = new iBlob(store, false);
    object.allocateLocation(p);
}
#endif

