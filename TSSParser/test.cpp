/*
 * Test.cc
 *
 *  Created on: Mar 7, 2012
 *      Author: Archesh
 */

#include "TSSParser.h"
#include <iostream>
using namespace std;

int main() {


    TSSParser parser("<gObj:SO> -= <Object1:SO>(*object1Ref) <Object2:SO>[](*Object1Ref) <Object3:RO>[];"
                    "<Object1:SO> -= <Object1Base:DA>[] <o1struct:SO>(*object1StructRef);"
                    "<Object2:SO> -= <Object2Base:S>;"
                    "<o1struct:SO> -= <Object1StructureBase:IA>[];"
                    , false);

    if (parser.validateGrammar()) {
        vector<PathComponent> vect;
        if (parser.storeAccessCode("gObj.Object1.o1struct.Object1StructureBase[5]", vect)) {
            //print the vector
            cout << "Path: ";
            for (int i = 0; i < vect.size(); i++) {
                cout << vect.at(i).label;
                if (i != vect.size() - 1)
                    cout << "->";
            }

            cout << "\nAccess codes: ";
            for (int i = 0; i < vect.size(); i++) {
                cout << vect.at(i).accessCode;
                if (i != vect.size() - 1)
                    cout << ":";
            }
            cout << endl;
        }

//        Path p(vect);
//        cout << "Is BO? " << parser.isBO(p) << endl;
//        cout << "Is SO? " << parser.isSO(p) << endl;
//        cout << "Is List? " << parser.isList(p) << endl;
//        cout << "Is RO? " << parser.isRef(p) << endl;
//        cout << "Type: " << parser.getBOType(p) << endl;
//
    }
    else
        cout<<"Error in grammer";

    return 0;
}
