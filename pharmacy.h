#ifndef PHARMACY_H
#define PHARMACY_H

#include <string>
#include <fstream>

// Medicine structure
struct Medicine {
    int medicineID;
    std::string name;
    std::string category; // Antibiotic, Painkiller, Vitamin, etc.
    int quantity;
    double price;
    std::string expiryDate;
    std::string manufacturer;
    
    // BST pointers
    Medicine* left;
    Medicine* right;
    
    // Constructor
    Medicine(int id, std::string n, std::string cat, int qty, double p, std::string exp, std::string man);
};

// Pharmacy Management Class using BST
class PharmacyManager {
private:
    Medicine* root;
    
    // Helper functions
    Medicine* insertBST(Medicine* node, Medicine* newMedicine);
    Medicine* searchBST(Medicine* node, int id);
    Medicine* searchByName(Medicine* node, const std::string& name);
    Medicine* deleteBST(Medicine* node, int id);
    Medicine* findMin(Medicine* node);
    void inorderTraversal(Medicine* node);
    void saveToFileHelper(Medicine* node, std::ofstream& file);
    void clearTree(Medicine* node);
    
public:
    PharmacyManager();
    ~PharmacyManager();
    
    // Main functions
    void addMedicine();
    void updateStock(int medicineID, int quantity);
    void removeMedicine(int medicineID);
    void viewInventory();
    void viewMedicine(int medicineID);
    void searchMedicineByName(const std::string& name);
    
    // File operations
    void loadFromFile();
    void saveToFile();
    
    // Check if medicine exists
    bool medicineExists(int id);
    double getMedicinePrice(int id);
};

#endif



