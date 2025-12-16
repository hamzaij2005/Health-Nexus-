#include "pharmacy.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Medicine constructor
Medicine::Medicine(int id, std::string n, std::string cat, int qty, double p, std::string exp, std::string man)
    : medicineID(id), name(n), category(cat), quantity(qty), price(p), 
      expiryDate(exp), manufacturer(man), left(nullptr), right(nullptr) {}

// PharmacyManager constructor
PharmacyManager::PharmacyManager() : root(nullptr) {
    loadFromFile();
}

// PharmacyManager destructor
PharmacyManager::~PharmacyManager() {
    saveToFile();
    clearTree(root);
}

// Insert into BST
Medicine* PharmacyManager::insertBST(Medicine* node, Medicine* newMedicine) {
    if (node == nullptr) {
        return newMedicine;
    }
    
    if (newMedicine->medicineID < node->medicineID) {
        node->left = insertBST(node->left, newMedicine);
    } else if (newMedicine->medicineID > node->medicineID) {
        node->right = insertBST(node->right, newMedicine);
    }
    
    return node;
}

// Search in BST by ID
Medicine* PharmacyManager::searchBST(Medicine* node, int id) {
    if (node == nullptr || node->medicineID == id) {
        return node;
    }
    
    if (id < node->medicineID) {
        return searchBST(node->left, id);
    } else {
        return searchBST(node->right, id);
    }
}

// Search by name (linear search through BST)
Medicine* PharmacyManager::searchByName(Medicine* node, const std::string& name) {
    if (node == nullptr) {
        return nullptr;
    }
    
    // Convert both to lowercase for case-insensitive search
    std::string nodeName = node->name;
    std::string searchName = name;
    std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::tolower);
    std::transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
    
    if (nodeName == searchName) {
        return node;
    }
    
    Medicine* leftResult = searchByName(node->left, name);
    if (leftResult != nullptr) {
        return leftResult;
    }
    
    return searchByName(node->right, name);
}

// Find minimum node
Medicine* PharmacyManager::findMin(Medicine* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Delete from BST
Medicine* PharmacyManager::deleteBST(Medicine* node, int id) {
    if (node == nullptr) {
        return node;
    }
    
    if (id < node->medicineID) {
        node->left = deleteBST(node->left, id);
    } else if (id > node->medicineID) {
        node->right = deleteBST(node->right, id);
    } else {
        if (node->left == nullptr) {
            Medicine* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            Medicine* temp = node->left;
            delete node;
            return temp;
        }
        
        Medicine* temp = findMin(node->right);
        node->medicineID = temp->medicineID;
        node->name = temp->name;
        node->category = temp->category;
        node->quantity = temp->quantity;
        node->price = temp->price;
        node->expiryDate = temp->expiryDate;
        node->manufacturer = temp->manufacturer;
        
        node->right = deleteBST(node->right, temp->medicineID);
    }
    
    return node;
}

// Inorder traversal
void PharmacyManager::inorderTraversal(Medicine* node) {
    if (node != nullptr) {
        inorderTraversal(node->left);
        std::cout << "\n----------------------------------------\n";
        std::cout << "Medicine ID: " << node->medicineID << "\n";
        std::cout << "Name: " << node->name << "\n";
        std::cout << "Category: " << node->category << "\n";
        std::cout << "Quantity: " << node->quantity << "\n";
        std::cout << "Price: " << formatCurrency(node->price) << "\n";
        std::cout << "Expiry Date: " << node->expiryDate << "\n";
        std::cout << "Manufacturer: " << node->manufacturer << "\n";
        inorderTraversal(node->right);
    }
}

// Add new medicine
void PharmacyManager::addMedicine() {
    clearScreen();
    std::cout << "========== ADD MEDICINE ==========\n\n";
    
    int id = generateID("pharmacy.txt");
    std::cout << "Generated Medicine ID: " << id << "\n\n";
    
    std::string name = getValidString("Enter Medicine Name: ");
    std::string category = getValidString("Enter Category: ");
    int quantity = getValidInt("Enter Quantity: ");
    double price = getValidInt("Enter Price: ");
    std::string expiryDate = getValidString("Enter Expiry Date (DD/MM/YYYY): ");
    std::string manufacturer = getValidString("Enter Manufacturer: ");
    
    Medicine* newMedicine = new Medicine(id, name, category, quantity, price, expiryDate, manufacturer);
    root = insertBST(root, newMedicine);
    
    std::cout << "\nMedicine added successfully!\n";
    saveToFile();
    pause();
}

// Update stock
void PharmacyManager::updateStock(int medicineID, int quantity) {
    Medicine* medicine = searchBST(root, medicineID);
    if (medicine == nullptr) {
        std::cout << "\nMedicine with ID " << medicineID << " not found!\n";
        return;
    }
    
    medicine->quantity += quantity;
    if (medicine->quantity < 0) {
        medicine->quantity = 0;
    }
    
    std::cout << "\nStock updated! New quantity: " << medicine->quantity << "\n";
    saveToFile();
}

// Remove medicine
void PharmacyManager::removeMedicine(int medicineID) {
    Medicine* medicine = searchBST(root, medicineID);
    if (medicine == nullptr) {
        std::cout << "\nMedicine with ID " << medicineID << " not found!\n";
        pause();
        return;
    }
    
    std::cout << "\nRemoving medicine: " << medicine->name << "\n";
    root = deleteBST(root, medicineID);
    std::cout << "Medicine removed successfully!\n";
    saveToFile();
    pause();
}

// View inventory
void PharmacyManager::viewInventory() {
    clearScreen();
    std::cout << "========== PHARMACY INVENTORY ==========\n";
    if (root == nullptr) {
        std::cout << "No medicines in inventory.\n";
    } else {
        inorderTraversal(root);
    }
    std::cout << "\n========================================\n";
    pause();
}

// View specific medicine
void PharmacyManager::viewMedicine(int medicineID) {
    Medicine* medicine = searchBST(root, medicineID);
    if (medicine != nullptr) {
        std::cout << "\n========== MEDICINE DETAILS ==========\n";
        std::cout << "Medicine ID: " << medicine->medicineID << "\n";
        std::cout << "Name: " << medicine->name << "\n";
        std::cout << "Category: " << medicine->category << "\n";
        std::cout << "Quantity: " << medicine->quantity << "\n";
        std::cout << "Price: " << formatCurrency(medicine->price) << "\n";
        std::cout << "Expiry Date: " << medicine->expiryDate << "\n";
        std::cout << "Manufacturer: " << medicine->manufacturer << "\n";
        std::cout << "=====================================\n";
    } else {
        std::cout << "\nMedicine with ID " << medicineID << " not found!\n";
    }
}

// Search medicine by name
void PharmacyManager::searchMedicineByName(const std::string& name) {
    Medicine* medicine = searchByName(root, name);
    if (medicine != nullptr) {
        viewMedicine(medicine->medicineID);
    } else {
        std::cout << "\nMedicine '" << name << "' not found!\n";
    }
}

// Check if medicine exists
bool PharmacyManager::medicineExists(int id) {
    return searchBST(root, id) != nullptr;
}

// Get medicine price
double PharmacyManager::getMedicinePrice(int id) {
    Medicine* medicine = searchBST(root, id);
    if (medicine != nullptr) {
        return medicine->price;
    }
    return 0.0;
}

// Load medicines from file
void PharmacyManager::loadFromFile() {
    std::ifstream file("pharmacy.txt");
    if (!file.is_open()) {
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        int id, quantity;
        double price;
        std::string name, category, expiryDate, manufacturer;
        
        if (iss >> id >> quantity >> price) {
            std::getline(iss, name, '|');
            std::getline(iss, name, '|');
            std::getline(iss, category, '|');
            std::getline(iss, expiryDate, '|');
            std::getline(iss, manufacturer, '|');
            
            if (!name.empty() && name[0] == ' ') name = name.substr(1);
            
            Medicine* newMedicine = new Medicine(id, name, category, quantity, price, expiryDate, manufacturer);
            root = insertBST(root, newMedicine);
        }
    }
    file.close();
}

// Save helper
void PharmacyManager::saveToFileHelper(Medicine* node, std::ofstream& file) {
    if (node != nullptr) {
        saveToFileHelper(node->left, file);
        file << node->medicineID << " " << node->quantity << " " << node->price 
             << " |" << node->name << "|" << node->category << "|" 
             << node->expiryDate << "|" << node->manufacturer << "|\n";
        saveToFileHelper(node->right, file);
    }
}

// Save medicines to file
void PharmacyManager::saveToFile() {
    std::ofstream file("pharmacy.txt");
    if (file.is_open()) {
        saveToFileHelper(root, file);
        file.close();
    }
}

// Clear tree
void PharmacyManager::clearTree(Medicine* node) {
    if (node != nullptr) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}



