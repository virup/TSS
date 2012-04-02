#ifndef TSS_H
#define TSS_H


TSS::TSS(const char *tssfile, void *iblob, string type="default")
{
    this->tssp = new TSSParser(tssfile);
    this->iBlob = iBlob;
    this->type = type;
}


Path TSS::createPath()
{
    Path p(this->tssp, this->iblob);
    return p;
}


Path TSS::createPath(string strPath)
{
   Path p(this->tssp, this->blob, strPath);
   return p; 
}


#endif

