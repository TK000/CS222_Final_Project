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

int L = 10;

double l_eta[N];
double u_eta[N];

std::vector<int> S[N];

double G(int u) {
    return (float) u / N;
}

std::vector<std::tuple<double, int, int>> C;
//int edges[N*N];
std::vector<std::tuple<int,int>> edges;


int main(int argc, char* argv[]) {
    
    // initialization
    for (int u = 0; u < N; u++) {
        l_eta[u] = u_eta[u] = 1;
    }
    
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
        
        printf("%i, %i\n", u,v);
        edges.push_back(std::make_tuple(u,v));
        
        if (std::find(S[u].begin(),S[u].end(),v) == S[u].end()) {
            if (S[u].size() < L) {
                S[u].push_back(v);
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
                    S[u].push_back(v);
                    u_eta[u] = G(k);
                    l_eta[u] = G(k_star);
                }
            }
        }
        if (std::find(S[v].begin(),S[v].end(),u) == S[v].end()) {
            if (S[v].size() < L) {
                S[v].push_back(u);
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
                    S[v].push_back(u);
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
    int m = 100;
    for (int u = 0; u < N-1; u++) {
        for (int v = u+1; v < N; v++) {
            if (std::find(edges.begin(),edges.end(),std::make_tuple(u,v)) == edges.end()
                && std::find(edges.begin(),edges.end(),std::make_tuple(v,u)) == edges.end()) {
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
    
    // print top m common neighbors
    for (int i = 0; i < C.size(); i++) {
        printf("%i %i %f\n", std::get<1>(C[i]), std::get<2>(C[i]), std::get<0>(C[i]));
    }

    // debugging
    for (int u = 0; u < N; u++) {
        //printf("size:%i\n", S[u].size());
        /*for (int i = 0; i < S[u].size(); i++) {
            printf("%i,",S[u][i]);
        }*/
    }
    
    // close file
    graph.close();
    return 0;
    
}

