//
//  cm_sketch_test.cpp
//  
//
//  Created by James Gui on 11/24/18.
//

#include <stdio.h>
#include <iostream>
#include <map>
#include <cstdlib>
#include <cmath>
#include "cm_sketch.hpp"
using namespace std;

int main(int argc, char **argv){
    int ar_1 [10] = {1,2,3,4,5,6,7,8,9,10};
    int ar_2 [10] = {2,10,2,1,7,4,10,9,2,3};
    CMSketch c(0.01,0.1);

    unsigned int i;
    for (i = 0; i < 10; i++){
        c.update(ar_1[i]);
        c.update(ar_2[i]);
    }
    for (i = 1; i <= 10; i++){
        cout << c.degree(i) << endl;
    }
    return 0;
}
