#ifndef HOSPITAL_GRAPH_H
#define HOSPITAL_GRAPH_H

#include <string>
#include <vector>
#include <fstream>

// Graph Node structure (represents a department or location)
struct GraphNode {
    int nodeID;
    std::string name; // Department name or location
    std::string type; // Department, Ward, Emergency, etc.
    
    // Adjacency list
    std::vector<std::pair<GraphNode*, int>> neighbors; // (neighbor, distance/weight)
    
    GraphNode(int id, std::string n, std::string t);
};

// Hospital Graph Class using Adjacency List
class HospitalGraph {
private:
    std::vector<GraphNode*> nodes;
    int nodeCounter;
    
    // Helper functions
    GraphNode* findNode(int nodeID);
    GraphNode* findNodeByName(const std::string& name);
    void DFSHelper(GraphNode* node, std::vector<bool>& visited);
    
public:
    HospitalGraph();
    ~HospitalGraph();
    
    // Main functions
    void addNode(std::string name, std::string type);
    void addEdge(int fromID, int toID, int distance);
    void addEdgeByName(std::string fromName, std::string toName, int distance);
    void removeEdge(int fromID, int toID);
    void viewGraph();
    void viewNode(int nodeID);
    void viewConnections(int nodeID);
    
    // Graph algorithms
    void shortestPath(int fromID, int toID);
    void departmentConnections();
    void findConnectedComponents();
    
    // File operations
    void loadFromFile();
    void saveToFile();
    
    // Initialize with sample data
    void initializeSampleData();
};

#endif



