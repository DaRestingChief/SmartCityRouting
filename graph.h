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

struct Edge {
    int dest;
    double weight;
    std::string mode;
    int edgeId;
    
    Edge(int d, double w, const std::string& m, int id = -1)
        : dest(d), weight(w), mode(m), edgeId(id) {}
};

struct Station {
    int id;
    std::string name;
    double latitude;
    double longitude;
    std::vector<std::string> supportedModes;
    
    Station(int i, const std::string& n, double lat, double lon)
        : id(i), name(n), latitude(lat), longitude(lon) {}
};

class Graph {
private:
    int vertices;
    int edges;
    
    std::vector<std::list<Edge>> adjList;
    
    std::vector<std::vector<double>> adjMatrix;
    
    std::unordered_map<int, Station> stations;
    
    std::unordered_map<std::string, int> stationNameToId;
    
public:
    Graph(int v);
    
    void addStation(int id, const std::string& name, double lat, double lon);
    
    void addEdge(int src, int dest, double weight, const std::string& mode);
    
    void removeEdge(int src, int dest);
    
    void removeStation(int id);
    
    Station* getStation(int id);
    
    int getStationIdByName(const std::string& name);
    
    std::list<Edge>& getAdjacentStations(int stationId);
    
    double getEdgeWeight(int src, int dest) const;
    
    std::vector<std::vector<double>>& getAdjacencyMatrix();
    std::vector<std::list<Edge>>& getAdjacencyList();
    
    std::unordered_map<int, Station>& getAllStations();
    
    double getHeuristicDistance(int src, int dest) const;
    
    bool isConnected(int src, int dest) const;
    
    int getVertexCount() const { return vertices; }
    
    int getEdgeCount() const { return edges; }
    
    void clear();
};

#endif
