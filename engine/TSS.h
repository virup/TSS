#ifndef TSS_H
#define TSS_H

#include <iostream>
#include "../../ranjan/includes/iBLOB_V2/iBLOB/iBlob.h"
#include "../../ranjan/includes/iBLOB_V2/iBLOB/Locator.h"
#include "constants.h"
#include <string>


/** class definition for TSS.
TSS: Type System Specification
Provides a semantic way for access components
*/

using namespace std;


static text *username = (text *) "aist";
static text *password = (text *) "aist11g";

class TSS
{
    public:
        /* Constructors & destructor */
        TSS(); // default constructor
        // constructor with tss file as input, isFile checks wether tssfile is a file or a string
        TSS(const char* tssfile, bool isFile = true);
        ~TSS();// destructor

        /* Initializaiton Functions */
        setTypeSpec(const char* tssfile, bool isFile = true);// associate type specification
        setStoreHandle(void* b, bool ismem = false);// associate blob handler

        /*// append for SO List iff.  path.isSO == true & path.isList == true
          Path append(const Path &path); // append a new structured element to the SO list
          */

        /* Create Path */
        Path createPath();
        Path createPath(string strPath);

        /* Insert Functions */

        // insert for SO iff.  path.isSO == true & tssp.isSO(path.path+"."+name) == true
        Path set(const Path &path, string name, bool isList = false); // set structured object by its name

        // insert bo iff.  path.isSO == true & tssp.isBO(path.path+"."+name) == true
        Path setInt(Path &path, string name, int value); // set the integer value for a base object
        Path setDouble(Path &path, string name, double value); // set the double value for a base object
        Path setIntArray(Path &path, string name, int array[], uint length); // set the integer array for a base object
        Path setDoubleArray(Path &path, string name, double array[], uint length); // set the double array for a base object
        Path setString(Path &path, string name, string value); // set the string value for a base object
        Path setBinary(Path &path, string name, const unsigned char* binaryarray, uint length); // set the binary value for a base object


        Path setRef(const Path &objp1, Path &objp2, string name2); // set the ref in objp2.name2 pointing to objp1
        Path appendRef(const Path &objp1, Path &objp2); // append the ref pointing to objp1 into objp2, if objp2 is a list of references

        // append element to bo list iff. path.isBO == true & path.isList == true
        Path appendInt(const Path &path); // append a new structured element to the SO list
        Path appendDouble(const Path &path); // append a new structured element to the SO list
        Path appendIntArray(const Path &path); // append a new structured element to the SO list
        Path appendDoubleArray(const Path &path); // append a new structured element to the SO list
        Path appendString(const Path &path); // append a new structured element to the SO list
        Path appendBinary(const Path &path); // append a new structured element to the SO list

        /* Read Functions iff. path.isBO == true */
        int readInt(const Path &path);       // read a int value from a BO
        double readDouble(const Path &path); // read a double value from a BO
        string readString(const Path &path); // read a string value from a BO
        uint readIntArray(const Path &path, int* intBuf, uint bufsize); // read a int array from a BO
        int readDoubleArray(const Path &path, double* doubleBuf, uint bufsize); // read a double array from a BO
        int readBinary(const Path &path, unsigned char* doubleBuf, uint bufsize); // read a binary value from a BO

        /* Remove Function : */
        bool remove(const Path &path,  uint idx) // remove the idx element in path iff. path is a list

        friend Path::Path(TSSParser *tssp);
        friend Path::Path(string strPath, TSSParser *tssp);



    private:
        TSSParser *tp;
        OCILobWrapper *lobWrapper;
        mSlob *iblob;
        string type;

        typedef struct OCILobWrapper{
            OCILobLocator* lob;
            OCISvcCtx*	cntxt;
            OCIError* errhp;
        } OCILobWrapper;



        /* Locate Functions */
        Path locate(string path);
        Path locate(const Path &path, string name); // locate object by its component name, iff. path is a SO and is not a list
        Path locate(const Path &path, uint idx); // locate object by idx iff. path is a list
};


#endif
