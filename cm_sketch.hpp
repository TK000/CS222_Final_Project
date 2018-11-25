//
//  cm_sketch.hpp
//  
//
//  Created by James Gui on 11/24/18.
//

#ifndef cm_sketch_hpp
#define cm_sketch_hpp

#include <stdio.h>

#endif /* cm_sketch_hpp */

// prime number for hashing
# define P 2147483647l

// Count-Min sketch for counting vertex degree
class CMSketch {
    unsigned int w,d;
    
    // error, between 0 and 1
    float epsilon;
    // accuracy, between 0 and 1
    float gamma;
    
    // parameters for hash function in Zp
    // aj*x + bj mod p
    unsigned int aj, bj;
    
    
    // actual sketch data structure, 2D array
    int** count;
    
    // hash function parameters; dx2 array for d functions
    int** hasharray;
    
    // generate two parameter for hash function
    int genhash();
    
public:
    //constructor
    CMSketch(float epsilon, float gamma);
    
    // increment count
    void update(int vertex);
    
    // return estimate of degree of vertex v
    unsigned int degree(int vertex);
    
    // destructor
    ~CMSketch();
    
};
