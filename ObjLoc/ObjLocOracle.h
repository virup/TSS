#ifndef STU_OBJLOCORACLE_H
#define STU_OBJLOCORACLE_H

#include "ObjLoc.h"
#include "iBlob.h"
#include "iBlobOracleStore.h"

namespace stu
{
class ObjLocOracle:public ObjLoc
{
    private:
        OCILobLocator *lob;
        OCIError *errhp;
        OCISvcCtx *svchp;


    public:
        /* Oracle OCI specific constructor */
        ObjLocOracle(OCILobLocator *,
                OCIError *,
                OCISvcCtx *);
        int bind(UDT &);
};
}

#endif
