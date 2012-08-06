#include "iBlob.h"
#include <iostream>
#include "engine/TSS.h"

using namespace std;

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
static	uint myid;



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

void iBlobTest(OCILobLocator *mylob, OCIServer *srvhp, OCISvcCtx *svchp)
{
    /*
       Locator slobby,daddy;
       ub4 loblen;
       checkerr(errhp,OCILobTrim2(svchp,errhp,mylob,0));
       cout << "--------------------------------------------" << endl;
       cout << "Set to Empty!" << endl;
       (void) OCILobGetLength(svchp, errhp, mylob, &loblen);
       cout << "Size of Blob again: " << loblen << endl;


       OCILobWrapper *blob1=new OCILobWrapper;
       blob1->lob=mylob;
       blob1->cntxt=svchp;
       blob1->errhp=errhp;

       (void) OCILobGetLength(blob1->cntxt, blob1->errhp, blob1->lob, &loblen);
       cout << "Size of Blob again and again: " << loblen << endl;

       unsigned char buffer[3];

    //"THE CAT IN THE HAT";
    cout << "[Action] Building iBlob in memory ..." << endl;
    iBlob p(blob1,true);
    cout << "[Action] iBlob Built" << endl;

    p.beginBatch();
    p.printStats();
    daddy=p.locateGlobal();
    daddy.debug();
    if (daddy.getElements()==0) //then build an object node
    daddy.insert(0,OBJECT_LEVEL);
    slobby=p.locate(daddy,0);
    p.printStats();

    cout << "======================="<<endl;
    Locator l;
    l=p.insert(slobby,0);		

    //uint struc=p.addStruct(slobby); //added new structure
    slobby.insert(1); //add secondary struct
    slobby.debug();
    //Locator pinga(p.locate(slobby,struc));
    //pinga.debug();
    cout << "[Action] Added Struct" << endl;
    cout << "--Sentence: "; 
    Locator sent=p.insert(l,0); //insert empty "sentence object"


    //Locator sent=l.insert(0); //insert empty "sentence object"
    l.debug();
    buffer[0] = 'T';
    buffer[1] = 'H';
    buffer[2] = 'E';
    cout << "The: "; 
    (p.insert(buffer,3,sent,0)).debug();
    buffer[0] = 'C';
    buffer[1] = 'A';
    buffer[2] = 'T';
    cout << "Cat: "; 
    (p.insert(buffer,3,sent,1)).debug();

    buffer[0] = 'I';
    buffer[1] = 'N';
    cout << "In: "; 
    (p.insert(buffer,2,sent,2)).debug();

    buffer[0] = 'T';
    buffer[1] = 'H';
    buffer[2] = 'E';
    cout << "The2: "; 
    (p.insert(buffer,3,sent,3)).debug();

    buffer[0] = 'H';
    buffer[1] = 'A';
    buffer[2] = 'T';
    cout << "Hat: "; 
    (p.insert(buffer,3,sent,4)).debug();
    cout << "--Sentence: ";
    sent.debug();
    unsigned char buffer3[]="el gatos";

    //			l=p.locatemSlob();
    cout << "Located2!" << endl;
    l.debug();
    cout << "el gatos: "; 
    (p.insert(buffer3,8,l,1)).debug();

    unsigned char buffer2[]="IN FRENCH CHAT DANS CHAPEAU";

    //			l=p.locatemSlob();
    cout << "Located3!" << endl;
    l.debug();
    cout << "Frenchen: "; 
    (p.insert(buffer2,27,l,2)).debug();

    p.printStats();

    cout << "[ACTION] now start *NO RESEQUENCE* committing ..." << endl;
    p.commitBatch();
    cout << "***************************************************" << endl;
    cout << "*Done inserting!, now read with existing locators  *" << endl;
    cout << "*This should print correct result, because locators*" << endl;
    cout << "*are still valid after commit with no resquence    *" << endl;
    cout << "***************************************************" << endl;
    imSlobstream reader=p.getStream(l);

    //get( char* const c, const  uint size, uint & numBytesRead, bool & EndOfObj );
    unsigned char bufferr[500];
    uint nread=1;
    reader.read(bufferr,500,nread);
    for (uint k=0; k<nread; k++)
        cout << bufferr[k];
    cout << endl;

    cout << "Done Reading 1" << endl;

    cout <<"iBlob current length without secondary structure: ";
    cout << p.readLength()<<endl;


    cout << "-----------------------------------------------"<< endl;


    cout << "Will try Struct handling" <<endl;

    Locator newstr=p.locate(slobby,1);
    newstr.debug();

    Locator eng=p.locate(l,0);
    Locator french=p.locate(l,2);
    Locator spa=p.locate(l,1);


    cout << "Will insert into secondary" << endl;

    p.beginBatch();
    (p.insert(french,newstr,0)).debug();
    cout << "Inserted french, new count:" << newstr.getElements() << endl;
    (p.insert(eng,newstr,1)).debug();
    // compare the following two locate
    (p.locateRef(newstr,1)).debug();
    (p.locate(newstr,1)).debug();
    cout << "Inserted eng, new count:" << newstr.getElements() << endl;
    Locator spal=p.insert(spa,newstr,2);
    spal.debug();
    cout << "Inserted spa, new count:" << newstr.getElements() << endl;
    newstr.debug();
    p.printStats();

    p.seqIdxDebug();
    cout << "[ACTION] now start *RESEQUENCED* committing ..." << endl;
    p.commitBatch(true);
    p.printStats();
    cout << "***************************************************" << endl;
    cout << "*Done inserting!, now read with existing locators  *" << endl;
    cout << "*This should print INCORRECT result, because       *" << endl;
    cout << "*locators are not valid any more after resequence  *" << endl;
    cout << "***************************************************" << endl;

    //p2->strongRemove(l,2);

    newstr=p.locate(slobby,1);
    Locator french2=p.locate(newstr,0);

    imSlobstream reader2=p.getStream(french2);

    //get( char* const c, const  uint size, uint & numBytesRead, bool & EndOfObj );

    nread=1;
    reader2.read(bufferr,500,nread);
    for (uint k=0; k<nread; k++)
        cout << bufferr[k];
    cout << endl;

    cout << "-----------------------------------------------"<< endl;

    cout<<"********** Done Testing !! ****************"<<endl;
    */
}


void iBlobTest_readonly(OCILobLocator *mylob, OCIServer *srvhp, OCISvcCtx *svchp)
{
    /*
       Locator slobby,daddy;
       ub4 loblen;

       OCILobWrapper *blob1=new OCILobWrapper;
       blob1->lob=mylob;
       blob1->cntxt=svchp;
       blob1->errhp=errhp;

       (void) OCILobGetLength(blob1->cntxt, blob1->errhp, blob1->lob, &loblen);
       cout << "Size of Blob again: " << loblen << endl;

    //"THE CAT IN THE HAT";
    cout << "Will build iBlob" << endl;
    iBlob p(blob1);
    cout << "iBlob Built" << endl;
    p.printStats();
    p.seqIdxDebug();
    //p.debug();

    daddy = p.locateGlobal();
    daddy.debug();
    slobby = p.locate(daddy,0);
    slobby.debug();
    Locator l = p.locate(slobby,0);
    l.debug();

    imSlobstream reader=p.getStream(l);

    //get( char* const c, const  uint size, uint & numBytesRead, bool & EndOfObj );
    unsigned char bufferr[500];
    uint nread=1;
    reader.read(bufferr,500,nread);
    for (uint k=0; k<nread; k++)
    cout << bufferr[k];
    cout << endl;


    cout << "Done Reading 1" << endl;
    Locator newstr;
    if (slobby.getElements()>1)
    {
    newstr=p.locate(slobby,1);
    Locator french2=p.locate(newstr,0);

    imSlobstream reader2=p.getStream(french2);
    nread=1;
    reader2.read(bufferr,500,nread);
    for (uint k=0; k<nread; k++)
    cout << bufferr[k];
    cout << endl;
    cout << "Done Reading 2" << endl;

    }
    */
}

int prepareBLOB_In_DB(string connectionString,string username, string password)
{
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

int main(int argc, char* argv[])
{
    // ESTABLISH CONNECTION TO THE DATABASE
    prepareBLOB_In_DB(string("phoenix.cise.ufl.edu:1521/orcl"), string(username), string(password));


    // DECLARE A GRAMMAR FOR THE NEW DATA TYPE
    /*char *regionGrammar =
        "<region:SO> -= <face:SO>[]<index:RO>[];"
        "<face:SO>   -= <outerCycle:SO><holeCycle:SO>[];"
        "<outerCycle:SO> -= <segment:DA>[](*index);"
        "<holeCycle:SO>  -= <segment:DA>[];";
    */

    char *regionGrammar = "S = {region, face, outerCycle, holeCycle};"
                         "R = {index};"
                         "B = {segment:DA};"
                         "region ::= face index+;"
                         "face ::= outerCycle holeCycle+;"
                         "outerCycle ::= segment:DA+ index;"
                         "holeCycle ::=  segment:DA+;"
                         "index ::= &segment:DA;";



    // CREATE A TSS OBJECT AND PROVIDE THE GRAMMAR
    TSS t(regionGrammar, mylob, svchp, errhp, "Region");

    //exit(0);
	//cout << "BEGIN TSS CONSTRUCTOR" << endl;
	//TSS t(regionGrammar, "Region");
	//t.establishConnection(string("phoenix.cise.ufl.edu:1521/orcl"),string(username), string(password));

	//TSS t(regionGrammar,string("phoenix.cise.ufl.edu:1521/orcl"),string(username), string(password), false, "Region");

    cout << "DEFINE DATA SEGMENTS " << endl;

    // DEFINE SOME SEGMENT DATA
    double f0OSeg0[] = {1.0,3.0,1.5,0.5};
    double f0OSeg1[] = {1.5,0.5,3.0,0.5};
    double f0OSeg2[] = {3.0,0.5,3.0,2.0};
    double f0OSeg3[] = {3.0, 2.0, 1.0,3.0};
    double f0HSeg0[] = {2.0,1.0, 2.5,1.0};
    double f0HSeg1[] = {2.5,1.0,2.0,2.0};
    double f0HSeg2[] = {2.0,2.0,2.0,1.0};

    double f1OSeg0[] = {5.0,1.0,7.0,1.0};
    double f1OSeg1[] = {7.0,1.0,6.0,3.0};
    double f1OSeg2[] = {6.0,3.0,5.0,1.0};


    //Path testPath = t.createPath();
    // CREATE SOME REQUIRED PATHS
    Path pface0 = t.createPath("region.face[0]");
    Path pface0O = pface0+"outerCycle";

    Path pface0OSeg0 = pface0O + "segment[0]";
    Path pface0OSeg1 = pface0O + "segment[1]";
    Path pface0OSeg2 = pface0O + "segment[2]";
    Path pface0OSeg3 = pface0O + "segment[3]";


    Path pface0H = pface0+"holeCycle[0]";
    Path pface0HSeg0 = pface0H + "segment[0]";
    Path pface0HSeg1 = pface0H + "segment[1]";
    Path pface0HSeg2 = pface0H + "segment[2]";



    Path pface1 = t.createPath("region.face[1]");
    Path pface1O = pface1+"outerCycle";
    Path pface1OSeg0 = pface1O + "segment[0]";
    Path pface1OSeg1 = pface1O + "segment[1]";
    Path pface1OSeg2 = pface1O + "segment[2]";

    //Reference path
    Path pindex = t.createPath("region.index[0]");

    cout<<"Paths constructed"<<endl;


    // WRITE VALUES TO THE PATHS
    try{
        cout<<pface0OSeg0.set(f0OSeg0,sizeof(f0OSeg0));
        cout<<pface0OSeg1.set(f0OSeg1,sizeof(f0OSeg1));
        cout<<pface0OSeg2.set(f0OSeg2,sizeof(f0OSeg2));
        cout<<pface0OSeg3.set(f0OSeg3,sizeof(f0OSeg3));

        cout<<pface0HSeg0.set(f0HSeg0,sizeof(f0HSeg0));
        cout<<pface0HSeg1.set(f0HSeg1,sizeof(f0HSeg1));
        cout<<pface0HSeg2.set(f0HSeg2,sizeof(f0HSeg2));

        cout<<pface1OSeg0.set(f1OSeg0,sizeof(f1OSeg0));
        cout<<pface1OSeg1.set(f1OSeg1,sizeof(f1OSeg1));
        cout<<pface1OSeg2.set(f1OSeg2,sizeof(f1OSeg2));

        cout<<endl<<"Writing the pointer"<<endl;
        cout<<pindex.setRefTo(pface0OSeg0, 0);
    }
    catch(...)
    {
        cout<<"error";
    }

    cout<<endl<<"Reading back"<<endl;
    double p[4];

    Path pp1 = t.createPath("region.face[0].outerCycle.segment[0]");
    Path pp2 = t.createPath("region.face[0].outerCycle.segment[1]");
    Path pp3 = t.createPath("region.face[1].outerCycle.segment[2]");
    Path pp4 = t.createPath("region.index[0]");

    // READ BACK THE VALUE FROM face[0].outerCycle.Segment[1]
    try{
        pp4.readDoubleArray(p,sizeof(p));
    	//cout << t.readString(pregionlabel);
    }
    catch(...)
    {
        cout<<"Error in reading"<<endl;
        return 0;
    }

    cout<<"("<<p[0]<<","<<p[1]<<") - ("<<p[2]<<","<<p[3]<<")"<<endl;

    // CLOSE THE CONNECTIONS
    //OCILobClose(svchp, errhp, mylob);
    //checkerr(errhp, OCITransCommit(svchp, errhp, (ub4)0));
    //if (envhp)
        //(void) OCIHandleFree((dvoid *) envhp, OCI_HTYPE_ENV);
	t.closeConnection();
    return 0;
}
