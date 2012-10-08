#ifndef OBJLOCORACLE_H
#define OBJLOCORACLE_HH

#include "ObjLoc.h"
#include "iBlob.h"
#include "iBlobOracleStore.h"


class ObjLocOracle:public ObjLoc
{
    private:
        OCILobLocator *lob;
        OCIError *errhp;
        OCISvcCtx *svchp;


    public:
        ObjLocOracle(OCILobLocator *,
                OCIError *,
                OCISvcCtx *);
        int bind(UDT &);
};


#endif
