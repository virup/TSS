/*
 * Test.cc
 *
 *  Created on: Mar 7, 2012
 *      Author: Archesh
 */

#include "TSSParser.h"
#include <iostream.h>

int main () {
	    
        
        TSSParser parser ("<Region1:SO> -= <regionLabel:S><face:SO><index:SO>;"
        "<face:SO> -= <faceLabel:S><outerCycle:SO><holeCycle:SO>[];"
        "<index:SO> -= <indexLabel:S><SegmentIndex:RO>[];"
        "<outerCycle:SO> -= <segment:DA> [] (*segmentIndex);"
        "<holeCycle:SO> -= <segment:B>[];", false);
        
        parser.validateGrammar();
        
	return 0;
}
