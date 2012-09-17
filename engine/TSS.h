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

class TSS
{
    public:
        /* Constructors & destructor */
        TSS(); // default constructor
        // constructor with tss file as input, isFile checks wether tssfile is a file or a string
        TSS(const char* tssfile, bool isFile = true);
        TSS(const char* tssfile, bool , void*, string);
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

        /* Count Function : count number of objects are present beneath a given object in the iBlob heirarchy : if the path points to a Base Object the result is -1 */
        //TODO: What happens when the path point to a list? if it points to a
        //SO?
        int count(Path &path);

        friend Path::Path(const Path&);
        friend Path::Path(string strPath, TSSParser *tssp, iBlob *);

    private:
        TSSParser *tp;
        //OCILobWrapper *lobWrapper;
        iBlob *iblob;
        string type;

        /* Locate Functions */
        Path locate(string path);
        Path locate(const Path &path, string name); // locate object by its component name, iff. path is a SO and is not a list
        Path locate(const Path &path, uint idx); // locate object by idx iff. path is a list

};


#endif
