#ifndef BFSDFS_H
#define BFSDFS_H

#include "graph.h"
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <algorithm>

struct TraversalResult {
    std::vector<int> visitedOrder;         
    std::vector<int> parent;                
    std::vector<int> distance;             
    std::unordered_set<int> connectedComponent;  
};

class BFS {
public:
    
    static TraversalResult traverse(Graph& graph, int startVertex);
    
    
    static std::unordered_set<int> getConnectedComponent(
        Graph& graph,
        int startVertex
    );
    
    static bool hasPath(Graph& graph, int source, int destination);
    
    
    static std::vector<int> findShortestPath(
        Graph& graph,
        int source,
        int destination
    );
};

class DFS {
public:
    
    static TraversalResult traverse(Graph& graph, int startVertex);
    
    static TraversalResult traverseRecursive(Graph& graph, int startVertex);
    
    
    static bool hasCycle(Graph& graph);
    
    static std::unordered_set<int> getConnectedComponent(
        Graph& graph,
        int startVertex
    );
    
private:
    
    static void dfsHelper(
        Graph& graph,
        int vertex,
        std::vector<bool>& visited,
        std::vector<int>& parent,
        std::vector<int>& visitedOrder,
        std::unordered_set<int>& component
    );
    
    
    static bool hasCycleHelper(
        Graph& graph,
        int vertex,
        std::vector<bool>& visited,
        std::vector<bool>& recStack
    );
};

#endif 
