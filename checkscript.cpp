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

std::vector<std::tuple<int,int>> pred_e;


int main(int argc, char* argv[]) {
    
    // initialization
    std::ifstream preds;
    preds.open("output.txt");
    std::string line;
    while (std::getline(preds,line)) {
        std::istringstream iss(line);
        int u, v;
        float f;
        if (!(iss >> u >> v >> f)) {
            continue;
        }
        pred_e.push_back(std::make_tuple(u,v));
    }
    preds.close();

    
    // read in the file
    std::ifstream graph;
    graph.open("amazon0601.txt");
    int count = 0;
    
    // sketch maintenance and update
    //std::string line;
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

        if (std::find(pred_e.begin(),pred_e.end(),std::make_tuple(u,v)) != pred_e.end()) {
            count+= 1;
        } else if (std::find(pred_e.begin(),pred_e.end(),std::make_tuple(v,u)) != pred_e.end()) {
            count += 1;
        }
    }
        
    
    // close file
    graph.close();
    printf("%i\n",count);
    return 0;
    
}

