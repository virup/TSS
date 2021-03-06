#include "TSS.h"
#include "UDT.h"
#include "Segment.h"
#include <string.h>

class Segment::SegmentImpl
{
    private:
        const std::string grammarFile;

    public:
        const TSS t;
        SegmentImpl():grammarFile("segment/Segment.tss"), t(grammarFile, true){}
//        int insertValue(double x1, double y1, double x2, double y2, iBlob *);
//        void print(iBlob *);
};

Segment::Segment():segment(new SegmentImpl)
{}


Segment::~Segment()
{
    delete segment;
}


//int Segment::insertValue(double x1, double y1, double x2, double y2)
//{
//    return segment->insertValue(x1,y1, x2,y2, getStore());
//}

//void Segment::print()
//{
//    if(!isStorageAllocated())
//        return ;
//   segment->print(getStore());
//}

int Segment::insertValue(double x1, double y1,
                  double x2, double y2)
{
    if(!isStorageAllocated())
        return 0;

    Path pLpt = segment->t.createPath("Segment.lPt", store);
    Path pRpt = segment->t.createPath("Segment.rPt", store);

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

void Segment::print()
{

    if(!isStorageAllocated())
        return ;

    double x1, y1, x2, y2;

    Path pLpt = segment->t.createPath("Segment.lPt", store);
    Path pRpt = segment->t.createPath("Segment.rPt", store);

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
