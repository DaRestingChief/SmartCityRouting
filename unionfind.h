#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>
#include <algorithm>

class UnionFind {
private:
    std::vector<int> parent;        
    std::vector<int> rank;         
    std::vector<int> setSize; 
    int numComponents;              
public:  
    UnionFind(int n); 
    int find(int x); 
    bool unite(int x, int y); 
    bool isConnected(int x, int y);  
    int getSizeOfSet(int x);   
    int getNumComponents() const {
        return numComponents;
    } 
    void reset(int n);
};

#endif
