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
double alpha = 0.25;

double Sl_eta[N];
double Su_eta[N];
double Tl_eta[N];
double Tu_eta[N];

std::vector<int> S[N];
std::vector<int> T[N];

double G(int u) {
    return (float) u / N;
}

std::vector<std::tuple<double, int, int>> C;
std::vector<std::tuple<int,int>> edges;

void update(std::vector<int> (&S)[N], double (&l_eta)[N], double (&u_eta)[N], int u, int v) {
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
}

int intersection(std::vector<int> (&S)[N], int u, int v) {
    int i = 0;
    int ui = 0;
    int vi = 0;
    while (ui < S[u].size() && vi < S[v].size()) {
        if (S[u][ui] == S[v][vi]) {
            i++;
            ui++;
            vi++;
        } else if (S[u][ui] < S[v][vi]) {
            ui++;
        } else {
            vi++;
        }
    }
    return i;
}


int main(int argc, char* argv[]) {
    
    // initialization
    for (int u = 0; u < N; u++) {
        Sl_eta[u] = Su_eta[u] = Tl_eta[u] = Tu_eta[u] = 1;
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

        edges.push_back(std::make_tuple(u,v));
        
        printf("%i, %i\n", u,v);
        
        for (int w = 0; w < S[u].size(); w++) {
            update(T, Tl_eta, Tu_eta, u,w);
            update(T, Tl_eta, Tu_eta, w,u);
        }
        for (int x = 0; x < S[v].size(); x++) {
            update(T, Tl_eta, Tu_eta, v,x);
            update(T, Tl_eta, Tu_eta, x,v);
        }
        update(S, Sl_eta, Su_eta, u,v);
        update(S, Sl_eta, Su_eta, v,u);
    }
    
    for (int u = 0; u < N; u++) {
        std::sort(T[u].begin(),T[u].end());
        std::sort(S[u].begin(),S[u].end());
    }

    // local path calculation (only keep top m)
    int m = 100;
    for (int u = 0; u < N-1; u++) {
        for (int v = u+1; v < N; v++) {
            if (std::find(edges.begin(),edges.end(),std::make_tuple(u,v)) == edges.end()
                && std::find(edges.begin(),edges.end(),std::make_tuple(v,u)) == edges.end()) {
                int S_intersection = intersection(S,u,v);
                int T_intersection = intersection(T,u,v);
                float S_max = std::max((float) (Sl_eta[u] + Su_eta[u]) / 2, (float) (Sl_eta[v] + Su_eta[v]) / 2);
                float T_max = std::max((float) (Tl_eta[u] + Tu_eta[u]) / 2, (float) (Tl_eta[v] + Tu_eta[v]) / 2);
                double Slp = (float) S_intersection / S_max;
                double Tlp = (float) T_intersection / T_max;
                double lp = Slp + (double) (alpha*Tlp);

                if (C.size() < m) {
                    C.push_back(std::make_tuple(lp,u,v));
                    std::sort(C.rbegin(), C.rend());
                } else if (std::get<0>(C[m-1]) < lp) {
                    C.erase(C.begin() + m);
                    C.push_back(std::make_tuple(lp,u,v));
                    std::sort(C.rbegin(), C.rend());
                }
                /*if (C.size() < m) {
                    C.push_back(std::make_tuple(Slp + alpha*Tlp,u,v));
                    std::sort(C.rbegin(), C.rend());
                } else if (std::get<0>(C[m-1]) < Slp + alpha*Tlp) {
                    C.erase(C.begin() + m);
                    C.push_back(std::make_tuple(Slp + alpha*Tlp,u,v));
                    std::sort(C.rbegin(), C.rend());
                }*/
                if (u % 100 == 0 && v % 100 == 0) {
                    printf("CALC:%i, %i\n", u,v);
                }
            }
        }
    }
    
    // print top m local paths
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

