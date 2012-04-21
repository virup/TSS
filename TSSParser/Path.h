/* 
 * File:   Path.h
 * Author: Archesh
 *
 * Created on March 29, 2012, 3:34 PM
 */

#ifndef PATH_H
#define	PATH_H
#include <vector>
#include <string>
using namespace std;

struct PathComponent {
    string label;
    int accessCode; 
    PathComponent() {
        label = "";
        accessCode = -1;
    }
    //Locator loc;
};

class Path {
public:
    Path();
    Path(vector<PathComponent>);
    ~Path();
    vector <PathComponent> vect;
};


#endif	/* PATH_H */

