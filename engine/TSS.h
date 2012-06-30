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

#include "../../ranjan/includes/iBLOB_V2/iBLOB/Locator.h"
/** class definition for TSS.
TSS: Type System Specification
Provides a semantic way for access components
*/

using namespace std;

typedef struct OCILobWrapper{
    OCILobLocator* lob;
    OCISvcCtx*	cntxt;
    OCIError* errhp;
} OCILobWrapper;


class TSS
{
    public:
        /* Constructors & destructor */
        TSS(); // default constructor
        // constructor with tss file as input, isFile checks wether tssfile is a file or a string
        TSS(const char* tssfile, bool isFile = true);
        TSS(const char* tssfile, void*, void*,void*, string);
        ~TSS();// destructor

        //handles case when TSS grammar  is a string
        TSS(const char* grammarString, string serverPath, string username, string password, bool isFile = false);
        TSS(const char* tssGrammarString, string type);

        /* Initializaiton Functions : LEGACY FUNCTIONS PHASE THESE OUT*/
        void setTypeSpec(const char* tssfile, bool isFile = true);// associate type specification
        void setStoreHandle(void* b, bool ismem = false);// associate blob handler

        /*functions to Establish and Close connections to the iBlob*/
        int establishConnection(string username, string passwd, string serverpath);
        void closeConnection();

        /* Create Path */
        Path createPath();
        Path createPath(string strPath);

        /* Insert Functions */

        // insert BO iff.  path.isSO == true & tssp.isBO(path.path+"."+name) == true
        Path setInt(Path &path, int value); // set the integer value for a base object
        Path setDouble(Path &path, double value); // set the double value for a base object
        Path setIntArray(Path &path, int array[], uint length); // set the integer array for a base object
        Path setDoubleArray(Path &path, double array[], uint length); // set the double array for a base object
        Path setString(Path &path, string value); // set the string value for a base object
        Path setBinary(Path &path, unsigned char* binaryarray, uint length); // set the binary value for a base object
        Path setRef(Path &objp1, Path &objp2, int idx); // set the ref in objp2.index pointing to objp1

        /* Read Functions iff. path.isBO == true */
        int readInt(Path &path);       // read a int value from a BO
        double readDouble(Path &path); // read a double value from a BO
        string readString(Path &path); // read a string value from a BO
        uint readIntArray(Path &path, int* intBuf, uint bufsize); // read a int array from a BO
        int readDoubleArray(Path &path, double* doubleBuf, uint bufsize); // read a double array from a BO
        int readBinary(Path &path, unsigned char* doubleBuf, uint bufsize); // read a binary value from a BO

        /* Remove Function : Remove Objects from the iBlob */
        bool remove(Path &path,  uint idx); // remove the idx element in path iff. path is a list

        /* Count Function : count number of objects are present beneath a given object in the iBlob heirarchy : if the path points to a Base Object the result is -1 */
        //TODO: What happens when the path point to a list? if it points to a
        //SO?
        int count(Path &path);

        friend Path::Path(const Path&);
        friend Path::Path(string strPath, TSSParser *tssp, iBlob *);

    private:
        TSSParser *tp;
        OCILobWrapper *lobWrapper;
        iBlob *iblob;
        string type;

        /* Locate Functions */
        Path locate(string path);
        Path locate(const Path &path, string name); // locate object by its component name, iff. path is a SO and is not a list
        Path locate(const Path &path, uint idx); // locate object by idx iff. path is a list

};


#endif
