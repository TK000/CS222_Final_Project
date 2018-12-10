//
//  main.cpp
//  AdamicAdar
//
//  Created by James Gui on 12/10/18.
//  Copyright © 2018 James Gui. All rights reserved.
//

#include <iostream>
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

#define N 1000 //number of vertices

int L;

double l_eta[N];
double u_eta[N];

std::vector<int> S[N];
std::vector<int> S_inv[N];

double G(int u) {
    return (float) u / N;
}

std::vector<std::tuple<double, int, int>> C;


int main(int argc, char* argv[]) {
    
    // initialization
    for (int u = 0; u < N; u++) {
        l_eta[u] = u_eta[u] = 1;
    }
    
    // read in the file
    std::ifstream graph;
    graph.open("Amazon_truncated.txt");
    
    // sketch maintenance and update
    std::string line;
    while (std::getline(graph,line)) {
        std::istringstream iss(line);
        int u, v;
        if (!(iss >> u >> v)) {
            continue;
        }
        
        printf("%i, %i\n", u,v);
        
        if (std::find(S[u].begin(),S[u].end(),v) == S[u].end()) {
            if (S[u].size() < L) {
                S[u].push_back(v);
                S_inv[v].push_back(u);
            } else {
                if (G(v) <= l_eta[u]) {
                    int max = 0;
                    int max_2 = 0;
                    int k, k_star;
                    for (int w = 0; w < S[u].size(); w++) {
                        if (G(w) > max) {
                            max_2 = max;
                            max = G(w);
                            k_star = k;
                            k = w;
                        }
                    }
                    if (G(v) > G(k_star)) {
                        k_star = v;
                    }
                    S[u].erase(S[u].begin() + k-1);
                    S_inv[S[u].begin() + k-1].erase(u);
                    
                    S[u].push_back(v);
                    S_inv[v].push_back(u);
                    u_eta[u] = G(k);
                    l_eta[u] = G(k_star);
                }
            }
        }
        if (std::find(S[v].begin(),S[v].end(),u) == S[v].end()) {
            if (S[v].size() < L) {
                S[v].push_back(u);
                S_inv[v].push_back(u);
            } else {
                if (G(u) <= l_eta[v]) {
                    int max = 0;
                    int max_2 = 0;
                    int k, k_star;
                    for (int w = 0; w < S[v].size(); w++) {
                        if (G(w) > max) {
                            max_2 = max;
                            max = G(w);
                            k_star = k;
                            k = w;
                        }
                    }
                    if (G(u) > G(k_star)) {
                        k_star = u;
                    }
                    S[v].erase(S[v].begin() + k-1);
                    S_inv[S[u].begin() + k-1].erase(u);
                    
                    S[v].push_back(u);
                    S_inv[v].push_back(u);
                    u_eta[v] = G(k);
                    l_eta[v] = G(k_star);
                }
            }
        }
    }
    
    for (int u = 0; u < N; u++) {
        std::sort(S[u].begin(), S[u].end());
    }
    
    
    // common neighbors calculation (only keep top m)
    int m = 10;
    for (int u = 0; u < N-1; u++) {
        for (int v = u+1; v < N; v++) {
            //std::sort(S[u].begin(), S[u].end());
            //std::sort(S[v].begin(), S[v].end());
            int intersection = 0;
            int ui = 0;
            int vi = 0;
            while (ui < S[u].size() && vi < S[v].size()) {
                if (S[u][ui] == S[v][vi]) {
                    intersection++;
                    ui++;
                    vi++;
                } else if (S[u][ui] < S[v][vi]) {
                    ui++;
                } else {
                    vi++;
                }
            }
            float max = std::max((float) (l_eta[u] + u_eta[u]) / 2, (float) (l_eta[v] + u_eta[v]) / 2);
            double cn = (float) intersection / max;
            if (C.size() < m) {
                C.push_back(std::make_tuple(cn,u,v));
                std::sort(C.rbegin(), C.rend());
            } else if (std::get<0>(C[m-1]) < cn) {
                C.erase(C.begin() + m);
                C.push_back(std::make_tuple(cn,u,v));
                std::sort(C.rbegin(), C.rend());
            }
            
            if (u % 100 == 0 && v % 100 == 0) {
                printf("CALC:%i, %i\n", u,v);
            }
        }
    }
    
}
double AA(int u, int v, int d_max) {
    L = d_max;
    double AA = 0;
    std::sort(S_inv[u].begin(),S_inv[u].end());
    std::sort(S_inv[v].begin(),S_inv[v].end());
    
    std::vector<int> v_intersection;
    std::set_intersection(S_inv[u].begin(), S_inv[u].end(),
                          S_inv[v].begin(), S_inv[v].end(),
                          std::back_inserter(v_intersection));
    for(int node : v_intersection)
        if (l_eta[node] == 1 and u_eta[node] == 1){
            AA += 1/(log(S[node].size()));
        }
    
    return AA;
}

double RA(int u, int v, int d_max) {
    L = d_max;
    double AA = 0;
    std::sort(S_inv[u].begin(),S_inv[u].end());
    std::sort(S_inv[v].begin(),S_inv[v].end());
    
    std::vector<int> v_intersection;
    std::set_intersection(S_inv[u].begin(), S_inv[u].end(),
                          S_inv[v].begin(), S_inv[v].end(),
                          std::back_inserter(v_intersection));
    for(int node : v_intersection)
        if (l_eta[node] == 1 and u_eta[node] == 1){
            AA += 1/(S[node].size());
        }
    
    return AA;
}
