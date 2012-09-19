#ifndef TSS_H
#define TSS_H

#include <iostream>
#include "constants.h"
#include "../path/Path.h"
#include <string>
#include <fstream>
#include <sstream>
#include <oci.h>
#include <stdexcept>
#include <time.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include "iBlob.h"
#include <sstream>

#include "Locator.h"
/** class definition for TSS.
TSS: Type System Specification
Provides a semantic way for access components
*/

using namespace std;

class TSS
{
    public:
        // constructor with tss file as input, isFile checks wether tssfile is a file or a string
        TSS(); // default constructor
        TSS(const char* tssfile,bool isFile=true);
        ~TSS();// destructor


        /* Initializaiton Functions : LEGACY FUNCTIONS PHASE THESE OUT*/
        void setTypeSpec(const char* tssfile, bool isFile = true);// associate type specification
        void setStoreHandle(void* b, bool ismem = false);// associate blob handler

        /* Create Path */
        Path createPath(void *);
        Path createPath(string strPath, void *);

        /* Insert Functions */

        friend Path::Path(const Path&);
        friend Path::Path(string strPath, TSSParser *tssp, iBlob *);

    private:
        TSSParser *tp;
        iBlob *iblob;
        string type;
};


#endif
