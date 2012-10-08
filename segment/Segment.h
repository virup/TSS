#include "TSS.h"
#include "UDT.h"

#include <string.h>

class Segment:public UDT
{
    private:
        const std::string grammarFile;
        const TSS t;

    public:
        Segment():grammarFile("segment/Segment.tss"), t(grammarFile, true)
         {}
        int insertValue(double x1, double y1, double x2, double y2);
        void print();

};
