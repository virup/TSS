#include "Segment.h"

int Segment::insertValue(double x1, double y1,
                  double x2, double y2)
{
    if(!isStorageAllocated())
        return 0;
    Path pLpt = t.createPath("Segment.lPt", store);
    Path pRpt = t.createPath("Segment.rPt", store);

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

    Path pLpt = t.createPath("Segment.lPt", store);
    Path pRpt = t.createPath("Segment.rPt", store);

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
