#ifndef MST_H
#define MST_H

#include "graph.h"
#include "unionfind.h"
#include <bits/stdc++.h>

struct MSTEdge {
    int src;
    int dest;
    double weight;
    std::string mode;
    
    bool operator<(const MSTEdge& other) const {
        return weight < other.weight;
    }
    
    bool operator>(const MSTEdge& other) const {
        return weight > other.weight;
    }
};

struct MSTResult {
    std::vector<MSTEdge> edges;         
    double totalWeight;                 
    int vertexCount;                    
    int edgeCount;                  
};

class MST {
public:
    static MSTResult kruskal(Graph& graph);
    
    static MSTResult prim(Graph& graph, int startVertex = 0);
    
    static MSTResult kruskalWithDSU(Graph& graph);
    
private:
    
    static std::vector<MSTEdge> extractAllEdges(Graph& graph);
};

#endif // MST_H
