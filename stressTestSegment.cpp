#include <iostream>
#include "TSS.h"
#include "Segment.h"
#include "ObjLocOracle.h"
#include <stdlib.h>
#include "iBlob.h"
#include <time.h>
using namespace std;


#define COUNT 20
#define MAX 100

static char *username = (char *) "aist";
static char *password = (char *) "aist11g";

static OCIEnv *envhp;
static OCIError *errhp;
static sword status;


static	OCISession *authp = (OCISession *) 0;
static	OCIServer *srvhp;
static	OCISvcCtx *svchp;
static	OCIStmt	*stmthp;
static	OCIDefine *defnp = (OCIDefine *) 0;
static	OCIDefine *defnp2 = (OCIDefine *) 0;
static	OCILobLocator *mylob;

static uint myid;

// Function to calculate time diffence
double diffclock(clock_t clock1,clock_t clock2)
{
    double diffticks=clock1-clock2;
    double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
    return diffms;
}

//function to get a random double number
double fRand(double fMin, double fMax)
{
        double f = (double)rand() / RAND_MAX;
        return fMin + f * (fMax - fMin);
}

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




int prepareBLOB_In_DB(string connectionString,string username, string password)
{
    string sqlQuery;
    //create environment and 	//allocate handles and data structures
    (void) OCIInitialize((ub4) OCI_DEFAULT, (dvoid *)0,
            (dvoid * (*)(dvoid *, size_t)) 0,
            (dvoid * (*)(dvoid *, dvoid *, size_t))0,
            (void (*)(dvoid *, dvoid *)) 0 );

    (void) OCIEnvInit( (OCIEnv **) &envhp, OCI_DEFAULT, (size_t) 0,
            (dvoid **) 0 );

    (void) OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &errhp, OCI_HTYPE_ERROR,
            (size_t) 0, (dvoid **) 0);

    /* server contexts */
    (void) OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &srvhp, OCI_HTYPE_SERVER,
            (size_t) 0, (dvoid **) 0);

    (void) OCIHandleAlloc( (dvoid *) envhp, (dvoid **) &svchp, OCI_HTYPE_SVCCTX,
            (size_t) 0, (dvoid **) 0);

    //(void) OCIServerAttach( srvhp, errhp, (text *)"phoenix.cise.ufl.edu:1521/orcl", strlen("phoenix.cise.ufl.edu:1521/orcl"), 0);

    (void) OCIServerAttach( srvhp, errhp, (text *)connectionString.c_str(), strlen(connectionString.c_str()), 0);
    /* set attribute server context in the service context */
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


    //allocate lob descriptor
    if (OCIDescriptorAlloc((dvoid *) envhp, (dvoid **) &mylob,
                (ub4)OCI_DTYPE_LOB, (size_t) 0, (dvoid **) 0))
    {
        cout << "FAILED: OCIDescriptorAlloc()" << endl;
        return OCI_ERROR;
    }

    //issue SQL and process data
    sqlQuery = "SELECT id, s.state.data FROM usstates s where id=1 FOR UPDATE";


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
        }
    return 1;
    ///////////////////////////////////////////////////////
    //
}

void closeConnection(){
    OCILobClose(svchp, errhp, mylob);
    checkerr(errhp, OCITransCommit(svchp, errhp, (ub4)0));
    if (envhp)
        (void) OCIHandleFree((dvoid *) envhp, OCI_HTYPE_ENV);
}



void write_to_blob_TSS(int number)
{
    // ESTABLISH CONNECTION TO THE DATABASE
    // and create the datastructures required for Oracle connection
    // like OCILobLocator *, ServerContext etc.
    prepareBLOB_In_DB(string("phoenix.cise.ufl.edu:1521/orcl"), string(username), string(password));

    double x1, x2, y1,y2;
    try
    {
        // Create an uninitialized Segment object
        Segment seg;

        // Create a Oracle Store Location
        ObjLoc *ol = new ObjLocOracle(mylob, errhp, svchp);

        // Bind the Segment to the Oracle Store Location
        ol->bind(seg);

        // Start the timer
        clock_t begin=clock();

        for(int i = 0; i < number; i++)
        {
            x1 = fRand(0,MAX);
            y1 = fRand(0,MAX);
            x2 = fRand(0,MAX);
            y2 = fRand(0,MAX);
            // Work on the Segment object using provided functions
            seg.insertValue(x1,y1, x2, y2);
        }
        clock_t end=clock();
        cout << "Time elapsed: " << double(diffclock(end,begin)) << " ms"<< endl;
    }
    catch(string s)
    {
        cout<<"Error happened"<<endl;
        cout<<s<<endl;
        return;
    }
    catch(char *s)
    {
        cout<<"Error happened"<<endl;
        cout<<s<<endl;
    }
    closeConnection();
}

void write_to_blob(int number)
{
    // ESTABLISH CONNECTION TO THE DATABASE
    prepareBLOB_In_DB(string("phoenix.cise.ufl.edu:1521/orcl"), string(username), string(password));

    //iBlobStore * store = new iBlobOracleStore(mylob, errhp, svchp);
    iBlobStore * store = new iBlobOracleStore(mylob, errhp, svchp);
    iBlob p (store, false);

    // Start the timer
    clock_t begin=clock();

    /* IBLOB FUNCTIONS USED TO CREATE THE SEGMENT OBJECT */
    for(int i = 0; i < number; i++)
    {
        cout<<i<<endl;
        Locator lGlobal = p.locateGlobal();
        cout<<"Done 1 "<<endl;
        Locator l;
        if(lGlobal.getElements()  == 0)
        {cout<<"Here"<<endl;
            l = p.insert(lGlobal, 0);
        }
        cout<<"Done 2 "<<endl;
        Locator lLeftPt = l.insert(0);
        cout<<"Done 3 "<<endl;
        Locator lRightPt = l.insert(1);
        cout<<"Done 4 "<<endl;
        Locator lLPTx = lLeftPt.insert(0);
        cout<<"Done 5 "<<endl;
        Locator lLPTy = lLeftPt.insert(1);
        cout<<"Done 6 "<<endl;
        Locator lRPTx = lRightPt.insert(0);
        cout<<"Done 7 "<<endl;
        Locator lRPTy = lRightPt.insert(1);
        cout<<"Done 8 "<<endl;
        double x1 = fRand(0,MAX);
        double y1 = fRand(0,MAX);
        double x2 = fRand(0,MAX);
        double y2 = fRand(0,MAX);
        cout<<"Done 8.5 "<<endl;
        p.insertVal(x1, lLeftPt, 0);
        cout<<"Done 9 "<<endl;
        p.insertVal(y1, lLPTy, 0);
        cout<<"Done 10 "<<endl;
        p.insertVal(x2, lRPTx, 0);
        cout<<"Done 11 "<<endl;
        p.insertVal(y2, lRPTy, 0);
        cout<<"Done 12 "<<endl;
    }

    /****************************************************/
    clock_t end=clock();
    cout << "Time elapsed: " << double(diffclock(end,begin)) << " ms"<< endl;

    closeConnection();
}
int main(int argc, char* argv[])
{
    write_to_blob_TSS(COUNT);
    write_to_blob(COUNT);
    return 1;
}
