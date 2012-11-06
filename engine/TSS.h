#ifndef STU_TSS_H
#define STU_TSS_H

#include <iostream>
#include "constants.h"
#include "Path.h"
#include <string>
#include <fstream>
#include <sstream>
#include <sstream>
#include <string.h>

/** class definition for TSS.
TSS: Type System Specification
Provides a semantic way for access components
*/

using namespace std;
namespace stu{
class TSS
{
    public:
        // constructor with tss file as input, isFile checks wether tssfile is a file or a string
        TSS(); // default constructor


        /* if isFile = true, then char *tssfile is the name of the file
         * else, it is the actual grammar */
        TSS(const char* tssfile,bool isFile=true);
        TSS(string tssfile,bool isFile=true);
        ~TSS();// destructor


        /* Create Path objects using these functions only */
        Path createPath(void *) const;
        Path createPath(string strPath, void *) const ;

        /*Path constructors used internally*/
        friend Path::Path(const Path&);
        friend Path::Path(string strPath, TSSParser *tssp, iBlob *);

    private:
        TSSParser *tp_;
};
}

#endif
