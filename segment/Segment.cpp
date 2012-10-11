#include "TSS.h"
#include "UDT.h"

#include <string.h>

class Segment::SegmentImpl
{
    private:
        const std::string grammarFile;
        const TSS t;

    public:
        SegmentImpl():grammarFile("segment/SegmentImpl.tss"), t(grammarFile, true):segment(new SegmentImpl){}
        int insertValue(double x1, double y1, double x2, double y2);
        void print();
        ~SegmentImpl();
};


Segment::~Segment()
{
    delete segment;
}


int Segment::insertValue(double x1, double y1, double x2, double y2)
{
    if(!segment->isStorageAllocated())
        return;
    segment->insertValue(x1,y1, x2,y2);
}

void print()
{
   if(!isStorageAllocated)
       return;
   segment->print();
}

int Segment::SegmentImpl::insertValue(double x1, double y1,
                  double x2, double y2)
{
    if(!isStorageAllocated())
        return 0;
    Path pLpt = t.createPath("SegmentImpl.lPt", store);
    Path pRpt = t.createPath("SegmentImpl.rPt", store);

    Path pLptLat = pLpt + "lat";
    pLptLat.set(x1);

    Path pLptLon = pLpt + "lon";
    pLptLon.set(y1);

    Path pRptLat = pRpt + "lat";
    pRptLat.set(x2);

    Path pRptLon = pRpt + "lon";
    pRptLon.set(y2);

    return 1;
}

void Segment::SegmentImpl::print()
{
    if(!isStorageAllocated())
        return ;

    double x1, y1, x2, y2;

    Path pLpt = t.createPath("SegmentImpl.lPt", store);
    Path pRpt = t.createPath("SegmentImpl.rPt", store);

    Path pLptLat = pLpt + "lat";
    pLptLat.read(x1);

    Path pLptLon = pLpt + "lon";
    pLptLon.read(y1);

    Path pRptLat = pRpt + "lat";
    pRptLat.read(x2);

    Path pRptLon = pRpt + "lon";
    pRptLon.read(y2);


    cout<<"("<<x1<<","<<y1<<")"<<"  ("<<x2<<","<<y2<<")"<<endl;
}
