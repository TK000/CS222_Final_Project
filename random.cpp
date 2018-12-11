//
//  Algo1.cpp
//  
//
//  Created by Tomoka Kan on 11/6/18.
//
//

#include "Algorithms.hpp"
#include <vector>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <tuple>

#define N 1001 //number of vertices


std::vector<std::tuple<int,int>> edges;


int main(int argc, char* argv[]) {
    
    
    // read in the file
    std::ifstream graph;
    graph.open("amazon0302.txt");
    
    // sketch maintenance and update
    std::string line;
    while (std::getline(graph,line)) {
        std::istringstream iss(line);
        int u, v;
        if (!(iss >> u >> v)) {
            continue;
        } else if (u > 1000) {
            break;
        } else if (v > 1000) {
            continue;
        }
        
        //printf("%i, %i\n", u,v);
        edges.push_back(std::make_tuple(u,v));
    }

    int count = 0;
    srand(time(0));
    while (count < 100) {
        int u = rand() % 1000 + 1;
        int v = rand() % 1000 + 1;
        if (u == v) {
            continue;
        }
        if (std::find(edges.begin(),edges.end(),std::make_tuple(u,v)) != edges.end()) {
            continue;
        }
        if (std::find(edges.begin(),edges.end(),std::make_tuple(v,u)) != edges.end()) {
            continue;
        }
        printf("%i %i 0.0\n",u,v);
        count += 1;
    }
    
    // close file
    graph.close();
    return 0;
    
}

