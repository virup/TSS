#ifndef STU_OBJLOC_H
#define STU_OBJLOC_H

#include "UDT.h"
namespace stu
{
    class ObjLoc
    {
        public:
            /* Pure virtual function to be reimplemented
             * by all subclasses */
            virtual int bind(UDT &) = 0;
    };
}
#endif
