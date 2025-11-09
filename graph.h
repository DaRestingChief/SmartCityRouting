#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <queue>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <limits>

/**
 * Edge structure representing a connection between two nodes
 * Contains: destination node, weight, mode of transport, and edge ID
 */
struct Edge {
    int dest;           // Destination station ID
    double weight;      // Weight: distance/time/cost
    std::string mode;   // Transport mode: bus, metro, bike, walk
    int edgeId;         // Unique edge identifier
    
    Edge(int d, double w, const std::string& m, int id = -1)
        : dest(d), weight(w), mode(m), edgeId(id) {}
};

/**
 * Station structure representing a node in the transportation network
 * Contains: station ID, name, coordinates, and transport mode support
 */
struct Station {
    int id;
    std::string name;
    double latitude;
    double longitude;
    std::vector<std::string> supportedModes;  // bus, metro, bike, walk
    
    Station(int i, const std::string& n, double lat, double lon)
        : id(i), name(n), latitude(lat), longitude(lon) {}
};

/**
 * Graph class implementing multiple representations:
 * 1. Adjacency List: Vector of lists for efficient traversal
 * 2. Adjacency Matrix: 2D array for quick edge lookup
 * Uses unordered_map for O(1) station name lookup
 */
class Graph {
private:
    int vertices;
    int edges;
    
    // Adjacency List representation: vector of lists
    std::vector<std::list<Edge>> adjList;
    
    // Adjacency Matrix representation: 2D vector
    std::vector<std::vector<double>> adjMatrix;
    
    // Station storage: ID -> Station mapping
    std::unordered_map<int, Station> stations;
    
    // Name to ID mapping for fast lookup: O(1) average case
    std::unordered_map<std::string, int> stationNameToId;
    
public:
    /**
     * Constructor: Initialize graph with given number of vertices
     * Both adjacency list and matrix are initialized
     */
    Graph(int v);
    
    /**
     * Add a station to the graph
     * Time Complexity: O(1) average for unordered_map insertion
     */
    void addStation(int id, const std::string& name, double lat, double lon);
    
    /**
     * Add a weighted edge between two stations
     * Updates both adjacency list and matrix representations
     * Time Complexity: O(1) for list insertion, O(1) for matrix update
     */
    void addEdge(int src, int dest, double weight, const std::string& mode);
    
    /**
     * Remove an edge from the graph
     */
    void removeEdge(int src, int dest);
    
    /**
     * Remove a station from the graph
     */
    void removeStation(int id);
    
    /**
     * Get station by ID
     * Time Complexity: O(1) average
     */
    Station* getStation(int id);
    
    /**
     * Get station ID by name
     * Time Complexity: O(1) average
     */
    int getStationIdByName(const std::string& name);
    
    /**
     * Get all adjacent stations for a given station
     * Time Complexity: O(degree of vertex)
     */
    std::list<Edge>& getAdjacentStations(int stationId);
    
    /**
     * Get edge weight between two stations
     * Time Complexity: O(1) using adjacency matrix
     */
    double getEdgeWeight(int src, int dest) const;
    
    /**
     * Toggle between adjacency list and matrix representation for different algorithms
     */
    std::vector<std::vector<double>>& getAdjacencyMatrix();
    std::vector<std::list<Edge>>& getAdjacencyList();
    
    /**
     * Get all stations in the graph
     */
    std::unordered_map<int, Station>& getAllStations();
    
    /**
     * Calculate Euclidean distance between two stations (for heuristic in A*)
     * Time Complexity: O(1)
     */
    double getHeuristicDistance(int src, int dest) const;
    
    /**
     * Check if two stations are connected
     */
    bool isConnected(int src, int dest) const;
    
    /**
     * Get number of vertices
     */
    int getVertexCount() const { return vertices; }
    
    /**
     * Get number of edges
     */
    int getEdgeCount() const { return edges; }
    
    /**
     * Clear the graph
     */
    void clear();
};

#endif // GRAPH_H
