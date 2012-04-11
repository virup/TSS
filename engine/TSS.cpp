#ifndef TSS_CPP
#define TSS_CPP


TSS::TSS(const char *tssfile, void *mylob, void *svchp, void *errhp,  string type="default")
{
    this->tssp = new TSSParser(tssfile);
    this->type = type;
    this->lobWrapper = new OCILobWrapper;
    this->lobWrapper->lob=mylob;
    this->lobWrapper->cntxt=svchp;
    this->lobWrapper->errhp=errhp;
    this->iblob = new mSlob(lobWrapper,true);
}



Path TSS::createPath()
{
    Path p(this->tp, this->lobWrapper);
    return p;
}


Path TSS::createPath(string strPath)
{
   Path p(strpath, this->tp, this->iblob);
   return p;
}



int TSS::readInt(const Path &path)
{
    return path.readInt();
}

double TSS::readDouble(const Path &path)
{
    return path.readDouble();
}




string TSS::readString(const Path &path)
{
    return path.readString();
}

uint TSS::readIntArray(const Path &path, int* intBuf, uint bufsize)
{
    return path.readIntArray(intBuf, bufsize);
}


int TSS::readDoubleArray(const Path &path, double* doubleBuf, uint bufsize)
{
    return path.readDoubleArray(doubleBuf, bufsize);
}
int TSS::readBinary(const Path &path, unsigned char* doubleBuf, uint bufsize)
{
    return path.readBinary(doubleBuf, bufsize);
}


Path TSS::setInt(Path &path, string name, int value)
{
    path.setInt(name, value); // What is the 'name' for?
}
#endif

