#include "hospitalGraph.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <queue>
#include <climits>
#include <algorithm>

// GraphNode constructor
GraphNode::GraphNode(int id, std::string n, std::string t)
    : nodeID(id), name(n), type(t) {}

// HospitalGraph constructor
HospitalGraph::HospitalGraph() : nodeCounter(0) {
    loadFromFile();
    if (nodes.empty()) {
        initializeSampleData();
    }
}

// HospitalGraph destructor
HospitalGraph::~HospitalGraph() {
    saveToFile();
    for (auto node : nodes) {
        delete node;
    }
}

// Find node by ID
GraphNode* HospitalGraph::findNode(int nodeID) {
    for (auto node : nodes) {
        if (node->nodeID == nodeID) {
            return node;
        }
    }
    return nullptr;
}

// Find node by name
GraphNode* HospitalGraph::findNodeByName(const std::string& name) {
    for (auto node : nodes) {
        if (node->name == name) {
            return node;
        }
    }
    return nullptr;
}

// Add new node
void HospitalGraph::addNode(std::string name, std::string type) {
    // Check if node already exists
    if (findNodeByName(name) != nullptr) {
        std::cout << "\nNode with name '" << name << "' already exists!\n";
        return;
    }
    
    int id = ++nodeCounter;
    GraphNode* newNode = new GraphNode(id, name, type);
    nodes.push_back(newNode);
    
    std::cout << "\nNode added successfully! ID: " << id << "\n";
    saveToFile();
}

// Add edge between nodes
void HospitalGraph::addEdge(int fromID, int toID, int distance) {
    GraphNode* fromNode = findNode(fromID);
    GraphNode* toNode = findNode(toID);
    
    if (fromNode == nullptr || toNode == nullptr) {
        std::cout << "\nInvalid node IDs!\n";
        return;
    }
    
    // Check if edge already exists
    for (auto& neighbor : fromNode->neighbors) {
        if (neighbor.first->nodeID == toID) {
            std::cout << "\nEdge already exists! Updating distance.\n";
            neighbor.second = distance;
            saveToFile();
            return;
        }
    }
    
    fromNode->neighbors.push_back({toNode, distance});
    toNode->neighbors.push_back({fromNode, distance}); // Undirected graph
    
    std::cout << "\nEdge added successfully!\n";
    saveToFile();
}

// Add edge by name
void HospitalGraph::addEdgeByName(std::string fromName, std::string toName, int distance) {
    GraphNode* fromNode = findNodeByName(fromName);
    GraphNode* toNode = findNodeByName(toName);
    
    if (fromNode == nullptr || toNode == nullptr) {
        std::cout << "\nOne or both nodes not found!\n";
        return;
    }
    
    addEdge(fromNode->nodeID, toNode->nodeID, distance);
}

// Remove edge
void HospitalGraph::removeEdge(int fromID, int toID) {
    GraphNode* fromNode = findNode(fromID);
    GraphNode* toNode = findNode(toID);
    
    if (fromNode == nullptr || toNode == nullptr) {
        std::cout << "\nInvalid node IDs!\n";
        return;
    }
    
    // Remove from fromNode's neighbors
    fromNode->neighbors.erase(
        std::remove_if(fromNode->neighbors.begin(), fromNode->neighbors.end(),
            [toID](const std::pair<GraphNode*, int>& p) {
                return p.first->nodeID == toID;
            }),
        fromNode->neighbors.end()
    );
    
    // Remove from toNode's neighbors
    toNode->neighbors.erase(
        std::remove_if(toNode->neighbors.begin(), toNode->neighbors.end(),
            [fromID](const std::pair<GraphNode*, int>& p) {
                return p.first->nodeID == fromID;
            }),
        toNode->neighbors.end()
    );
    
    std::cout << "\nEdge removed successfully!\n";
    saveToFile();
}

// View entire graph
void HospitalGraph::viewGraph() {
    clearScreen();
    std::cout << "========== HOSPITAL GRAPH ==========\n\n";
    
    if (nodes.empty()) {
        std::cout << "Graph is empty.\n";
        pause();
        return;
    }
    
    for (auto node : nodes) {
        std::cout << "\n----------------------------------------\n";
        std::cout << "Node ID: " << node->nodeID << "\n";
        std::cout << "Name: " << node->name << "\n";
        std::cout << "Type: " << node->type << "\n";
        std::cout << "Connections:\n";
        
        if (node->neighbors.empty()) {
            std::cout << "  No connections\n";
        } else {
            for (const auto& neighbor : node->neighbors) {
                std::cout << "  -> " << neighbor.first->name 
                         << " (Distance: " << neighbor.second << ")\n";
            }
        }
    }
    
    std::cout << "\n=====================================\n";
    pause();
}

// View specific node
void HospitalGraph::viewNode(int nodeID) {
    GraphNode* node = findNode(nodeID);
    if (node != nullptr) {
        std::cout << "\n========== NODE DETAILS ==========\n";
        std::cout << "Node ID: " << node->nodeID << "\n";
        std::cout << "Name: " << node->name << "\n";
        std::cout << "Type: " << node->type << "\n";
        std::cout << "Number of Connections: " << node->neighbors.size() << "\n";
        std::cout << "=====================================\n";
    } else {
        std::cout << "\nNode with ID " << nodeID << " not found!\n";
    }
}

// View connections for a node
void HospitalGraph::viewConnections(int nodeID) {
    GraphNode* node = findNode(nodeID);
    if (node == nullptr) {
        std::cout << "\nNode with ID " << nodeID << " not found!\n";
        return;
    }
    
    std::cout << "\n========== CONNECTIONS FOR " << node->name << " ==========\n";
    
    if (node->neighbors.empty()) {
        std::cout << "No connections.\n";
    } else {
        for (const auto& neighbor : node->neighbors) {
            std::cout << "-> " << neighbor.first->name 
                     << " (Distance: " << neighbor.second << ")\n";
        }
    }
    std::cout << "==========================================\n";
}

// Shortest path using BFS (for unweighted) or Dijkstra's (for weighted)
void HospitalGraph::shortestPath(int fromID, int toID) {
    GraphNode* fromNode = findNode(fromID);
    GraphNode* toNode = findNode(toID);
    
    if (fromNode == nullptr || toNode == nullptr) {
        std::cout << "\nInvalid node IDs!\n";
        return;
    }
    
    if (fromID == toID) {
        std::cout << "\nSource and destination are the same!\n";
        return;
    }
    
    // Using BFS for unweighted graph (simple implementation)
    std::queue<GraphNode*> q;
    std::vector<bool> visited(nodes.size() + 1, false);
    std::vector<int> parent(nodes.size() + 1, -1);
    std::vector<int> distance(nodes.size() + 1, INT_MAX);
    
    q.push(fromNode);
    visited[fromNode->nodeID] = true;
    distance[fromNode->nodeID] = 0;
    
    while (!q.empty()) {
        GraphNode* current = q.front();
        q.pop();
        
        if (current->nodeID == toID) {
            break;
        }
        
        for (const auto& neighbor : current->neighbors) {
            if (!visited[neighbor.first->nodeID]) {
                visited[neighbor.first->nodeID] = true;
                distance[neighbor.first->nodeID] = distance[current->nodeID] + neighbor.second;
                parent[neighbor.first->nodeID] = current->nodeID;
                q.push(neighbor.first);
            }
        }
    }
    
    if (distance[toID] == INT_MAX) {
        std::cout << "\nNo path found between " << fromNode->name 
                 << " and " << toNode->name << "!\n";
        return;
    }
    
    // Reconstruct path
    std::vector<int> path;
    int current = toID;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }
    std::reverse(path.begin(), path.end());
    
    std::cout << "\n========== SHORTEST PATH ==========\n";
    std::cout << "From: " << fromNode->name << "\n";
    std::cout << "To: " << toNode->name << "\n";
    std::cout << "Distance: " << distance[toID] << " units\n";
    std::cout << "Path: ";
    
    for (size_t i = 0; i < path.size(); i++) {
        GraphNode* node = findNode(path[i]);
        std::cout << node->name;
        if (i < path.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << "\n=====================================\n";
}

// Department connections
void HospitalGraph::departmentConnections() {
    clearScreen();
    std::cout << "========== DEPARTMENT CONNECTIONS ==========\n\n";
    
    for (auto node : nodes) {
        if (node->type == "Department") {
            std::cout << "\n" << node->name << ":\n";
            viewConnections(node->nodeID);
        }
    }
    
    std::cout << "\n==========================================\n";
    pause();
}

// DFS Helper
void HospitalGraph::DFSHelper(GraphNode* node, std::vector<bool>& visited) {
    visited[node->nodeID] = true;
    for (const auto& neighbor : node->neighbors) {
        if (!visited[neighbor.first->nodeID]) {
            DFSHelper(neighbor.first, visited);
        }
    }
}

// Find connected components
void HospitalGraph::findConnectedComponents() {
    std::vector<bool> visited(nodes.size() + 1, false);
    int componentCount = 0;
    
    for (auto node : nodes) {
        if (!visited[node->nodeID]) {
            componentCount++;
            std::cout << "\nComponent " << componentCount << ":\n";
            DFSHelper(node, visited);
            
            // Print nodes in this component
            for (auto n : nodes) {
                if (visited[n->nodeID]) {
                    std::cout << "  - " << n->name << "\n";
                }
            }
        }
    }
    
    std::cout << "\nTotal Connected Components: " << componentCount << "\n";
}

// Initialize sample data
void HospitalGraph::initializeSampleData() {
    addNode("Emergency", "Department");
    addNode("Cardiology", "Department");
    addNode("Pediatrics", "Department");
    addNode("Surgery", "Department");
    addNode("Pharmacy", "Department");
    addNode("Ward A", "Ward");
    addNode("Ward B", "Ward");
    addNode("ICU", "Ward");
    
    addEdgeByName("Emergency", "Cardiology", 5);
    addEdgeByName("Emergency", "Surgery", 3);
    addEdgeByName("Cardiology", "Ward A", 2);
    addEdgeByName("Surgery", "ICU", 4);
    addEdgeByName("Pediatrics", "Ward B", 3);
    addEdgeByName("Pharmacy", "Ward A", 6);
    addEdgeByName("Pharmacy", "Ward B", 6);
    
    std::cout << "\nSample graph data initialized!\n";
}

// Load graph from file
void HospitalGraph::loadFromFile() {
    std::ifstream file("hospitalGraph.txt");
    if (!file.is_open()) {
        return;
    }
    
    std::string line;
    // Load nodes
    while (std::getline(file, line) && line != "EDGES:") {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        int id;
        std::string name, type;
        
        if (iss >> id) {
            std::getline(iss, name, '|');
            std::getline(iss, name, '|');
            std::getline(iss, type, '|');
            
            if (!name.empty() && name[0] == ' ') name = name.substr(1);
            
            GraphNode* newNode = new GraphNode(id, name, type);
            nodes.push_back(newNode);
            
            if (id > nodeCounter) {
                nodeCounter = id;
            }
        }
    }
    
    // Load edges
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        int fromID, toID, distance;
        
        if (iss >> fromID >> toID >> distance) {
            addEdge(fromID, toID, distance);
        }
    }
    
    file.close();
}

// Save graph to file
void HospitalGraph::saveToFile() {
    std::ofstream file("hospitalGraph.txt");
    if (file.is_open()) {
        // Save nodes
        for (auto node : nodes) {
            file << node->nodeID << " |" << node->name << "|" << node->type << "|\n";
        }
        
        file << "EDGES:\n";
        
        // Save edges (only once per pair)
        std::vector<std::pair<int, int>> savedEdges;
        for (auto node : nodes) {
            for (const auto& neighbor : node->neighbors) {
                int fromID = node->nodeID;
                int toID = neighbor.first->nodeID;
                
                // Check if already saved
                bool alreadySaved = false;
                for (const auto& saved : savedEdges) {
                    if ((saved.first == fromID && saved.second == toID) ||
                        (saved.first == toID && saved.second == fromID)) {
                        alreadySaved = true;
                        break;
                    }
                }
                
                if (!alreadySaved) {
                    file << fromID << " " << toID << " " << neighbor.second << "\n";
                    savedEdges.push_back({fromID, toID});
                }
            }
        }
        
        file.close();
    }
}



