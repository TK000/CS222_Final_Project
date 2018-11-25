//
//  cm_sketch.cpp
//  
//
//  Created by James Gui on 11/24/18.
//

#include "cm_sketch.hpp"
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <limits>
#include <iostream>

using namespace std;


CMSketch::CMSketch(float eps, float gam){
    epsilon = eps;
    gamma = gam;
    w = ceil(exp(1)/epsilon);
    d = ceil(log(1/gamma));
    
    // initialize sketch
    count = new int* [d];
    unsigned int i,j;
    for (i = 0; i < d; i++){
        count[i] = new int [w];
        for (j = 0; j < w; j++){
            count[i][j] = 0;
        }
    }
    
    // initialize pseudorandom number generator seed
    srand(time(NULL));
    hasharray = new int* [d];
    for (i = 0; i < d; i++){
        hasharray[i] = new int[2];
        hasharray[i][0] = genhash();
        hasharray[i][1] = genhash();
    }
}

int CMSketch::genhash(){
    return int(float(rand())*float(P)/float(RAND_MAX) + 1);
}

// destructor
CMSketch::~CMSketch() {
    // free count
    unsigned int i;
    for (i = 0; i < d; i++) {
        delete[] count[i];
    }
    delete[] count;
    
    // free hasharray
    for (i = 0; i < d; i++) {
        delete[] hasharray[i];
    }
    delete[] hasharray;
}

// increment count; when edge (u,v) is read, call update(u); update(v)
void CMSketch::update(int vertex) {
    unsigned int hashval = 0;
    // increment counter for the hashed location in each of the d arrays
    for (unsigned int j = 0; j < d; j++) {
        hashval = ((long)hasharray[j][0] * vertex + hasharray[j][1]) % P % w;
        count[j][hashval]++;
    }
}

// return estimated degree of the vertex
unsigned int CMSketch::degree(int vertex){
    int minval = numeric_limits<int>::max();
    unsigned int hashval = 0;
    for (unsigned int j = 0; j < d; j++) {
        hashval = ((long)hasharray[j][0] * vertex + hasharray[j][1])% P % w;
        minval = min(minval, count[j][hashval]);
    }
    return minval;
}
