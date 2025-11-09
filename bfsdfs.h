#ifndef BFSDFS_H
#define BFSDFS_H

#include "graph.h"
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <algorithm>

/**
 * Breadth-First Search (BFS) and Depth-First Search (DFS)
 * 
 * Two fundamental graph traversal algorithms with different properties
 */

struct TraversalResult {
    std::vector<int> visitedOrder;          // Order of vertex visitation
    std::vector<int> parent;                // Parent of each vertex
    std::vector<int> distance;              // Distance from source (BFS only)
    std::unordered_set<int> connectedComponent;  // All nodes in component
};

class BFS {
public:
    /**
     * Breadth-First Search using Queue
     * 
     * Algorithm:
     * 1. Initialize queue with source vertex
     * 2. Mark source as visited
     * 3. While queue is not empty:
     *    - Dequeue vertex
     *    - For each unvisited adjacent vertex:
     *      - Mark as visited
     *      - Enqueue it
     *      - Record parent and distance
     * 
     * Data Structures Used:
     * - Queue (STL queue): For maintaining FIFO order
     * - Vector: For tracking visited, distances, parents
     * - Unordered Set: For O(1) visited checks (optional optimization)
     * 
     * Time Complexity: O(V + E)
     * Space Complexity: O(V)
     * 
     * Properties:
     * - Explores level by level
     * - Finds shortest path in unweighted graphs
     * - Complete: Always finds solution if it exists
     * - Optimal: Finds shortest path (unweighted)
     * 
     * Applications:
     * - Finding connected components
     * - Shortest path in unweighted graphs
     * - Level-order traversal
     * - Connectivity testing
     */
    static TraversalResult traverse(Graph& graph, int startVertex);
    
    /**
     * Find all vertices reachable from source
     */
    static std::unordered_set<int> getConnectedComponent(
        Graph& graph,
        int startVertex
    );
    
    /**
     * Check if path exists between two vertices
     * Time Complexity: O(V + E)
     */
    static bool hasPath(Graph& graph, int source, int destination);
    
    /**
     * Find shortest path between two vertices (unweighted)
     * Time Complexity: O(V + E)
     */
    static std::vector<int> findShortestPath(
        Graph& graph,
        int source,
        int destination
    );
};

class DFS {
public:
    /**
     * Depth-First Search using Stack (Iterative)
     * 
     * Algorithm:
     * 1. Initialize stack with source vertex
     * 2. Mark source as visited
     * 3. While stack is not empty:
     *    - Pop vertex
     *    - For each adjacent vertex:
     *      - If not visited:
     *        - Mark as visited
     *        - Push to stack
     *        - Record parent
     * 
     * Data Structures Used:
     * - Stack (STL stack): For maintaining LIFO order
     * - Vector: For tracking visited, parents
     * - Unordered Set: For visited vertex tracking
     * 
     * Time Complexity: O(V + E)
     * Space Complexity: O(V)
     * 
     * Properties:
     * - Explores as far as possible along each branch
     * - Goes deep first before backtracking
     * - Can be implemented recursively or iteratively
     * 
     * Applications:
     * - Topological sorting
     * - Cycle detection
     * - Finding strongly connected components
     * - Finding articulation points
     * - Maze solving
     */
    static TraversalResult traverse(Graph& graph, int startVertex);
    
    /**
     * Recursive DFS helper
     * More elegant but uses call stack
     */
    static TraversalResult traverseRecursive(Graph& graph, int startVertex);
    
    /**
     * Detect cycle in graph
     * Time Complexity: O(V + E)
     */
    static bool hasCycle(Graph& graph);
    
    /**
     * Find all vertices reachable from source
     */
    static std::unordered_set<int> getConnectedComponent(
        Graph& graph,
        int startVertex
    );
    
private:
    /**
     * Helper for recursive DFS
     */
    static void dfsHelper(
        Graph& graph,
        int vertex,
        std::vector<bool>& visited,
        std::vector<int>& parent,
        std::vector<int>& visitedOrder,
        std::unordered_set<int>& component
    );
    
    /**
     * Helper for cycle detection
     */
    static bool hasCycleHelper(
        Graph& graph,
        int vertex,
        std::vector<bool>& visited,
        std::vector<bool>& recStack
    );
};

#endif // BFSDFS_H
