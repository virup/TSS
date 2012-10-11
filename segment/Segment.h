#include "TSS.h"
#include "UDT.h"

#include <string.h>

class Segment:public UDT
{
    private:
        class SegmentImpl;
        SegmentImpl *segment;

    public:
        Segment() {}
        int insertValue(double x1, double y1, double x2, double y2);
        void print();
        ~Segment();
};
