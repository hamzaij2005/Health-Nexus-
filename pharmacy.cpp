#include "pharmacy.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

Medicine::Medicine(int id, string n, string cat, int qty, double p, string exp, string man): medicineID(id), name(n), category(cat), quantity(qty), price(p),expiryDate(exp), manufacturer(man), left(nullptr), right(nullptr) {}

PharmacyManager::PharmacyManager() : root(nullptr) {
    loadFromFile();
}
PharmacyManager::~PharmacyManager() {
    saveToFile();
    clearTree(root);
}
Medicine* PharmacyManager::insertBST(Medicine* node, Medicine* newMedicine) {
    if (node == nullptr) return newMedicine;

    if (newMedicine->medicineID < node->medicineID) {
        node->left = insertBST(node->left, newMedicine);
    } else if (newMedicine->medicineID > node->medicineID) {
        node->right = insertBST(node->right, newMedicine);
    }

    return node;
}

Medicine* PharmacyManager::searchBST(Medicine* node, int id) {
    if (node == nullptr || node->medicineID == id) return node;

    if (id < node->medicineID) return searchBST(node->left, id);
    else return searchBST(node->right, id);
}

Medicine* PharmacyManager::searchByName(Medicine* node, const string& name) {
    if (node == nullptr) return nullptr;

    string nodeName = node->name;
    string searchName = name;
    transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::tolower);
    transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

    if (nodeName == searchName) return node;

    Medicine* leftResult = searchByName(node->left, name);
    if (leftResult != nullptr) return leftResult;

    return searchByName(node->right, name);
}
Medicine* PharmacyManager::findMin(Medicine* node) {
    while (node->left != nullptr) node = node->left;
    return node;
}
Medicine* PharmacyManager::deleteBST(Medicine* node, int id) {
    if (node == nullptr) return node;

    if (id < node->medicineID) node->left = deleteBST(node->left, id);
    else if (id > node->medicineID) node->right = deleteBST(node->right, id);
    else {
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

void PharmacyManager::inorderTraversal(Medicine* node) {
    if (node != nullptr) {
        inorderTraversal(node->left);
        cout << "----------------------------------------" << endl;
        cout << "Medicine ID: " << node->medicineID << endl;
        cout << "Name: " << node->name << endl;
        cout << "Category: " << node->category << endl;
        cout << "Quantity: " << node->quantity << endl;
        cout << "Price: " << formatCurrency(node->price) << endl;
        cout << "Expiry Date: " << node->expiryDate << endl;
        cout << "Manufacturer: " << node->manufacturer << endl;
        inorderTraversal(node->right);
    }
}

void PharmacyManager::addMedicine() {
    clearScreen();
    cout << "========== ADD MEDICINE ==========" << endl << endl;

    int id = generateID("pharmacy.txt");
    cout << "Generated Medicine ID: " << id << endl << endl;

    string name = getValidString("Enter Medicine Name: ");
    string category = getValidString("Enter Category: ");
    int quantity = getValidInt("Enter Quantity: ");
    double price = getValidInt("Enter Price: ");
    string expiryDate = getValidString("Enter Expiry Date (DD/MM/YYYY): ");
    string manufacturer = getValidString("Enter Manufacturer: ");

    Medicine* newMedicine = new Medicine(id, name, category, quantity, price, expiryDate, manufacturer);
    root = insertBST(root, newMedicine);

    cout << "Medicine added successfully!" << endl;
    saveToFile();
    pause();
}
void PharmacyManager::updateStock(int medicineID, int quantity) {
    Medicine* medicine = searchBST(root, medicineID);
    if (medicine == nullptr) {
        cout << "Medicine with ID " << medicineID << " not found!" << endl;
        return;
    }
    medicine->quantity += quantity;
    if (medicine->quantity < 0) medicine->quantity = 0;

    cout << "Stock updated! New quantity: " << medicine->quantity << endl;
    saveToFile();
}
void PharmacyManager::removeMedicine(int medicineID) {
    Medicine* medicine = searchBST(root, medicineID);
    if (medicine == nullptr) {
        cout << "Medicine with ID " << medicineID << " not found!" << endl;
        pause();
        return;
    }
    cout << "Removing medicine: " << medicine->name << endl;
    root = deleteBST(root, medicineID);
    cout << "Medicine removed successfully!" << endl;
    saveToFile();
    pause();
}
void PharmacyManager::viewInventory() {
    clearScreen();
    cout << "========== PHARMACY INVENTORY ==========" << endl;
    if (root == nullptr) {
        cout << "No medicines in inventory." << endl;
    } else {
        inorderTraversal(root);
    }
    cout << "========================================" << endl;
    pause();
}
void PharmacyManager::viewMedicine(int medicineID) {
    Medicine* medicine = searchBST(root, medicineID);
    if (medicine != nullptr) {
        cout << "========== MEDICINE DETAILS ==========" << endl;
        cout << "Medicine ID: " << medicine->medicineID << endl;
        cout << "Name: " << medicine->name << endl;
        cout << "Category: " << medicine->category << endl;
        cout << "Quantity: " << medicine->quantity << endl;
        cout << "Price: " << formatCurrency(medicine->price) << endl;
        cout << "Expiry Date: " << medicine->expiryDate << endl;
        cout << "Manufacturer: " << medicine->manufacturer << endl;
        cout << "=====================================" << endl;
    } else {
        cout << "Medicine with ID " << medicineID << " not found!" << endl;
    }
}
void PharmacyManager::searchMedicineByName(const string& name) {
    Medicine* medicine = searchByName(root, name);
    if (medicine != nullptr) {
        viewMedicine(medicine->medicineID);
    } else {
        cout << "Medicine '" << name << "' not found!" << endl;
    }
}

bool PharmacyManager::medicineExists(int id) {
    return searchBST(root, id) != nullptr;
}

double PharmacyManager::getMedicinePrice(int id) {
    Medicine* medicine = searchBST(root, id);
    if (medicine != nullptr) return medicine->price;
    return 0.0;
}
void PharmacyManager::loadFromFile() {
    ifstream file("pharmacy.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        int id, quantity;
        double price;
        string name, category, expiryDate, manufacturer;
        if (iss >> id >> quantity >> price) {
            getline(iss, name, '|');
            getline(iss, name, '|');
            getline(iss, category, '|');
            getline(iss, expiryDate, '|');
            getline(iss, manufacturer, '|');

            if (!name.empty() && name[0] == ' ') name = name.substr(1);

            Medicine* newMedicine = new Medicine(id, name, category, quantity, price, expiryDate, manufacturer);
            root = insertBST(root, newMedicine);
        }
    }
    file.close();
}
void PharmacyManager::saveToFileHelper(Medicine* node, ofstream& file) {
    if (node != nullptr) {
        saveToFileHelper(node->left, file);
        file << node->medicineID << " " << node->quantity << " " << node->price
             << " |" << node->name << "|" << node->category << "|" 
             << node->expiryDate << "|" << node->manufacturer << "|" << endl;
        saveToFileHelper(node->right, file);
    }
}

void PharmacyManager::saveToFile() {
    ofstream file("pharmacy.txt");
    if (file.is_open()) {
        saveToFileHelper(root, file);
        file.close();
    }
}

void PharmacyManager::clearTree(Medicine* node) {
    if (node != nullptr) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}
