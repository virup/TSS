#ifndef TSS_CPP
#define TSS_CPP

#include "TSS.h"
#include "../path/Path.h"
#include "../TSSParser/TSSParser.h"
#include "constants.h"

static sword status;
static OCIEnv *envhp;
static OCIError *errhp;

static  OCISession *authp = (OCISession *) 0;
static  OCIServer *srvhp;
static  OCISvcCtx *svchp;
static  OCIStmt *stmthp;
static  OCIDefine *defnp = (OCIDefine *) 0;
static  OCIDefine *defnp2 = (OCIDefine *) 0;
static  OCILobLocator *mylob;
static  uint myid;


TSS::TSS(const char *tssfile, void *mylob, void *svchp, void *errhp,  string type="default")
{
    this->tp = new TSSParser(tssfile, false);
    this->type = type;
    this->lobWrapper = new OCILobWrapper;
    this->lobWrapper->lob=(OCILobLocator *)mylob;
    this->lobWrapper->cntxt=(OCISvcCtx*)svchp;
    this->lobWrapper->errhp=(OCIError*)errhp;
    this->iblob  = new iBlob(this->lobWrapper, true);
}

TSS::TSS(const char *grammarString, string type)
{
	this->tp = new TSSParser(grammarString, false);
	this->type = type;
}

//TODO: one argument should be the data type name. Has been removed?  
TSS::TSS(const char *grammarString, string serverPath, string username, string password, bool isFile)
{
	this->tp = new TSSParser(grammarString, false);
	//this->type = ;
	establishConnection(serverPath, username, password);
    this->lobWrapper->lob= mylob;
   this->lobWrapper->cntxt=svchp;
    this->lobWrapper->errhp=errhp;
    this->iblob  = new iBlob(this->lobWrapper, true);

}

//TODO: What does this function do exactly?
TSS::TSS(const char* tssfile, bool isFile)
{
	//need the TSS Parser Constructor that can handle file -> in ../TSSParser/Parser.h -> indicated that this will be implemented later.
}

TSS::TSS()
{}


//TODO: IMPORTANT to implement it. Otherwise there are memory leaks
TSS::~TSS()
{}

static void checkerr(OCIError *errhp, sword status)
{
    text errbuf[512];
    ub4 buflen;
    sb4 errcode;
    string er;

    if (status == OCI_SUCCESS) return;

    switch (status)
    {
        case OCI_SUCCESS_WITH_INFO:
            cout << "Error - OCI_SUCCESS_WITH_INFO" << endl;
            OCIErrorGet ((dvoid *) errhp, (ub4) 1, (text *) NULL, &errcode,
                    errbuf, (ub4) sizeof(errbuf), (ub4) OCI_HTYPE_ERROR);
            er=(char*)errbuf;
            cout << "Error -" << er << endl;
            break;
        case OCI_NEED_DATA:
            cout << "Error - OCI_NEED_DATA" << endl;
            break;
        case OCI_NO_DATA:
            cout << "Error - OCI_NO_DATA" << endl;
            break;
        case OCI_ERROR:
            OCIErrorGet ((dvoid *) errhp, (ub4) 1, (text *) NULL, &errcode,
                    errbuf, (ub4) sizeof(errbuf), (ub4) OCI_HTYPE_ERROR);
            er=(char*)errbuf;
            cout << "Error -" << er << endl;
            break;
        case OCI_INVALID_HANDLE:
            cout << "Error - OCI_INVALID_HANDLE" << endl;
            break;
        case OCI_STILL_EXECUTING:
            cout << "Error - OCI_STILL_EXECUTING" << endl;
            break;
        case OCI_CONTINUE:
            cout << "Error - OCI_CONTINUE" << endl;
            break;
    default:
            cout << "Error - " << status << endl;
            break;
    }
}



int TSS::establishConnection(string connectionString, string username, string password)
{
    //create environment and    //allocate handles and data structures
    (void) OCIInitialize((ub4) OCI_DEFAULT, (dvoid *)0,
            (dvoid * (*)(dvoid *, size_t)) 0,
            (dvoid * (*)(dvoid *, dvoid *, size_t))0,
            (void (*)(dvoid *, dvoid *)) 0 );

    (void) OCIEnvInit( (OCIEnv **) &envhp, OCI_DEFAULT, (size_t) 0,
            (dvoid **) 0 );

    (void) OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &errhp, OCI_HTYPE_ERROR,
            (size_t) 0, (dvoid **) 0);

    // server contexts
    (void) OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &srvhp, OCI_HTYPE_SERVER,
            (size_t) 0, (dvoid **) 0);

    (void) OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &svchp, OCI_HTYPE_SVCCTX,
            (size_t) 0, (dvoid **) 0);

    //(void) OCIServerAttach( srvhp, errhp, (text *)"phoenix.cise.ufl.edu:1521/o$

    (void) OCIServerAttach( srvhp, errhp, (text *)connectionString.c_str(), strlen(connectionString.c_str()), 0);
    // set attribute server context in the service context
    (void) OCIAttrSet( (dvoid *) svchp, OCI_HTYPE_SVCCTX, (dvoid *)srvhp,
            (ub4) 0, OCI_ATTR_SERVER, (OCIError *) errhp);

    (void) OCIHandleAlloc((dvoid *) envhp, (dvoid **)&authp,
            (ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid **) 0);

    (void) OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
            (dvoid *) username.c_str(), (ub4) strlen((char *)username.c_str()),
            (ub4) OCI_ATTR_USERNAME, errhp);

    (void) OCIAttrSet((dvoid *) authp, (ub4) OCI_HTYPE_SESSION,
            (dvoid *) password.c_str(), (ub4) strlen((char *)password.c_str()),
            (ub4) OCI_ATTR_PASSWORD, errhp);
    //connect to server and being session
    checkerr(errhp, OCISessionBegin ( svchp,  errhp, authp, OCI_CRED_RDBMS,
                (ub4) OCI_DEFAULT));

    (void) OCIAttrSet((dvoid *) svchp, (ub4) OCI_HTYPE_SVCCTX,
            (dvoid *) authp, (ub4) 0,
            (ub4) OCI_ATTR_SESSION, errhp);

    checkerr(errhp, OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &stmthp,
                OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0));

    //first clear all rows
    checkerr(errhp, OCIStmtPrepare(stmthp, errhp, (text *)"DELETE FROM test_iblobs",
                (ub4)strlen("DELETE FROM test_iblobs"),
                (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));

    checkerr(errhp, OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
                (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_COMMIT_ON_SUCCESS));

    uint rows_deleted;
 checkerr(errhp, OCIAttrGet ( stmthp, OCI_HTYPE_STMT, &rows_deleted, 0,
            OCI_ATTR_ROW_COUNT, errhp ));

    cout << rows_deleted << " rows were deleted from the 'TEST_IBLOBS' table " << endl;

    string sqlQuery = "INSERT INTO test_iblobs (id, iblob) VALUES(1, EMPTY_BLOB())";

    //cout << "sqlQuery = " << sqlQuery << endl;
    checkerr(errhp, OCIStmtPrepare(stmthp, errhp, (text *)(sqlQuery.c_str()),
                (ub4)strlen(sqlQuery.c_str()), (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));

    status = OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
            (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT);


    if (status==OCI_SUCCESS || status==OCI_SUCCESS_WITH_INFO)
    {
        cout << "Inserted empty blob with id 1" << endl;

    }else{
 text errbuf[512];
        ub4 buflen;
        sb4 errcode;

        OCIErrorGet (errhp, (ub4) 1, (text *) NULL, &errcode,
                errbuf, (ub4) sizeof(errbuf), (ub4) OCI_HTYPE_ERROR);

        string er=(char*)errbuf;
        cout << "Error: " << er << endl;
        throw runtime_error("Unable to insert tuple.");
    }


    checkerr(errhp, OCITransCommit(svchp, errhp, (ub4)0));

    //allocate lob descriptor
    if (OCIDescriptorAlloc((dvoid *) envhp, (dvoid **) &mylob,
                (ub4)OCI_DTYPE_LOB, (size_t) 0, (dvoid **) 0))
    {
        cout << "FAILED: OCIDescriptorAlloc()" << endl;
        return OCI_ERROR;

    }
    //issue SQL and process data
    sqlQuery = "SELECT id, iblob FROM test_iblobs where id=1 FOR UPDATE";


    checkerr(errhp, OCIStmtPrepare(stmthp, errhp, (text *)sqlQuery.c_str(),
                (ub4)strlen(sqlQuery.c_str()),
                (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));

    checkerr(errhp, OCIDefineByPos(stmthp, &defnp, errhp, 1, (dvoid *) &myid,
                (sword) sizeof(int), SQLT_INT, (dvoid *) 0, (ub2 *)0,
                (ub2 *)0, OCI_DEFAULT));


    checkerr(errhp, OCIDefineByPos(stmthp, &defnp2, errhp, (ub4) 2,
                (dvoid *) &mylob, (sb4) -1, (ub2) SQLT_BLOB,
                (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) OCI_DEFAULT));

    // execute and fetch
 if (status = OCIStmtExecute(svchp, stmthp, errhp, (ub4) 1, (ub4) 0,
                (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT))
    {
        if (status == OCI_NO_DATA)
            cout << "No data returned!" << endl;
        else
        {
            checkerr(errhp, status);
            if (envhp)
                (void) OCIHandleFree((dvoid *) envhp, OCI_HTYPE_ENV);
            return OCI_ERROR;
        }
    }else

        if (status==OCI_SUCCESS || status==OCI_SUCCESS_WITH_INFO)
        {

            ub4 loblen = 0;
            cout << "Retrieved tuple with ID: " << myid << endl;
            timeval t1, t2;
	    double elapsedTime;
        }
    return 1;
    ///////////////////////////////////////////////////////
    //
}


void TSS::closeConnection()
{
    OCILobClose(svchp, errhp, mylob);
    checkerr(errhp, OCITransCommit(svchp, errhp, (ub4)0));
    if (envhp)
        (void) OCIHandleFree((dvoid *) envhp, OCI_HTYPE_ENV);

}



//TODO: Check if it works or not.
Path TSS::createPath()
{
    string str = "";
    Path *p = new Path(str,(this->tp), (this->iblob));
    return *p;
}


Path TSS::createPath(string strPath)
{
   Path *p = new Path(strPath, (this->tp), (this->iblob));
   return *p;
}

int TSS::readInt(Path &path)
{
    return path.readInt();
}

double TSS::readDouble(Path &path)
{
    return path.readDouble();
}

string TSS::readString(Path &path)
{
    return path.readString();
}

uint TSS::readIntArray(Path &path, int* intBuf, uint bufsize)
{
    return path.readIntArray(intBuf, bufsize);
}


int TSS::readDoubleArray(Path &path, double* doubleBuf, uint bufsize)
{
    return path.readDoubleArray(doubleBuf, bufsize);
}
int TSS::readBinary(Path &path, unsigned char* doubleBuf, uint bufsize)
{
    return path.readBinary(doubleBuf, bufsize);
}

Path TSS::setInt(Path &path, int value)
{
     path.setInt(value);
     return path;
}

Path TSS::setDouble(Path &path, double value)
{
	path.setDouble(value);
	return path;
}

Path TSS::setIntArray(Path &path, int array[], uint length)
{
	path.setIntArray(array, length);
	return path;
}

Path TSS::setDoubleArray(Path &path, double array[], uint length)
{
	path.setDoubleArray(array, length);
	return path;
}



//TODO: This logic has to be put in the Path class's function for setString.
Path TSS::setString(Path &path, string value)
{

	uint length = (unsigned int) value.size(); // this can be dangerous--WHY?
	//set the first 10 characters in the string to represent string length in decimal notation.
	//convert the length into a string
	string len;
	string toAppend;
	stringstream out;
	out << length;
	len = out.str();
	//len is now the string representation of the lenght of  the string
	//if the lenght is less that 10 digits (decimal notation), then pad the digits with 0's
	if(len.size() < 10)
	{
		for(int  i = 0 ; i < 10 - len.size() ; i++)
		{
			toAppend.append("0");
		}
        toAppend.append(len);
	}
	else
	{ //THROW EXCEPTION :: STRING TOO LARGE
		cout<< "String too large to fit in the iBlob " << "handle this better?" << endl;
	}
	value.append(toAppend);
	path.setBinary((unsigned char *) value.c_str(), length);
	return path;
}

Path TSS::setBinary(Path &path, unsigned char* binaryarray, uint length)
{
	path.setBinary(binaryarray, length);
	return path;
}



int TSS::count(Path &path)
{
	return path.count();
}


Path TSS::setRef(Path &objp1, Path &objp2, int idx)
{
    try{
	objp2.setRefTo(objp1, idx);
    return objp2;
    }
    catch(...)
    {
        cout<<"exception in setRef"<<endl;
    }
}

bool remove(Path &path, uint idx)
{
	return path.removeObj(idx);
}

#endif
