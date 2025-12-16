#include "patient.h"
#include "utils.h"
#include <iostream>
#include <sstream>

// Patient constructor
Patient::Patient(int id, std::string n, int a, std::string g, std::string addr, 
                 std::string ph, std::string bg, std::string mh)
    : patientID(id), name(n), age(a), gender(g), address(addr), 
      phone(ph), bloodGroup(bg), medicalHistory(mh), left(nullptr), right(nullptr) {}

// PatientManager constructor
PatientManager::PatientManager() : root(nullptr) {
    loadFromFile();
}

// PatientManager destructor
PatientManager::~PatientManager() {
    saveToFile();
    clearTree(root);
}

// Insert into BST
Patient* PatientManager::insertBST(Patient* node, Patient* newPatient) {
    if (node == nullptr) {
        return newPatient;
    }
    
    if (newPatient->patientID < node->patientID) {
        node->left = insertBST(node->left, newPatient);
    } else if (newPatient->patientID > node->patientID) {
        node->right = insertBST(node->right, newPatient);
    }
    
    return node;
}

// Search in BST
Patient* PatientManager::searchBST(Patient* node, int id) {
    if (node == nullptr || node->patientID == id) {
        return node;
    }
    
    if (id < node->patientID) {
        return searchBST(node->left, id);
    } else {
        return searchBST(node->right, id);
    }
}

// Find minimum node
Patient* PatientManager::findMin(Patient* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Delete from BST
Patient* PatientManager::deleteBST(Patient* node, int id) {
    if (node == nullptr) {
        return node;
    }
    
    if (id < node->patientID) {
        node->left = deleteBST(node->left, id);
    } else if (id > node->patientID) {
        node->right = deleteBST(node->right, id);
    } else {
        // Node to delete found
        if (node->left == nullptr) {
            Patient* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            Patient* temp = node->left;
            delete node;
            return temp;
        }
        
        // Node with two children
        Patient* temp = findMin(node->right);
        node->patientID = temp->patientID;
        node->name = temp->name;
        node->age = temp->age;
        node->gender = temp->gender;
        node->address = temp->address;
        node->phone = temp->phone;
        node->bloodGroup = temp->bloodGroup;
        node->medicalHistory = temp->medicalHistory;
        
        node->right = deleteBST(node->right, temp->patientID);
    }
    
    return node;
}

// Inorder traversal (for viewing all patients)
void PatientManager::inorderTraversal(Patient* node) {
    if (node != nullptr) {
        inorderTraversal(node->left);
        std::cout << "\n----------------------------------------\n";
        std::cout << "Patient ID: " << node->patientID << "\n";
        std::cout << "Name: " << node->name << "\n";
        std::cout << "Age: " << node->age << "\n";
        std::cout << "Gender: " << node->gender << "\n";
        std::cout << "Address: " << node->address << "\n";
        std::cout << "Phone: " << node->phone << "\n";
        std::cout << "Blood Group: " << node->bloodGroup << "\n";
        std::cout << "Medical History: " << node->medicalHistory << "\n";
        inorderTraversal(node->right);
    }
}

// Register new patient
void PatientManager::registerPatient() {
    clearScreen();
    std::cout << "========== REGISTER NEW PATIENT ==========\n\n";
    
    int id = generateID("patients.txt");
    std::cout << "Generated Patient ID: " << id << "\n\n";
    
    std::string name = getValidString("Enter Patient Name: ");
    int age = getValidInt("Enter Age: ");
    std::string gender = getValidString("Enter Gender (M/F/Other): ");
    std::string address = getValidString("Enter Address: ");
    std::string phone = getValidString("Enter Phone Number: ");
    std::string bloodGroup = getValidString("Enter Blood Group: ");
    std::string medicalHistory = getValidString("Enter Medical History (or 'None'): ");
    
    Patient* newPatient = new Patient(id, name, age, gender, address, phone, bloodGroup, medicalHistory);
    root = insertBST(root, newPatient);
    
    std::cout << "\nPatient registered successfully!\n";
    saveToFile();
    pause();
}

// View specific patient
void PatientManager::viewPatient(int id) {
    Patient* patient = searchBST(root, id);
    if (patient != nullptr) {
        std::cout << "\n========== PATIENT DETAILS ==========\n";
        std::cout << "Patient ID: " << patient->patientID << "\n";
        std::cout << "Name: " << patient->name << "\n";
        std::cout << "Age: " << patient->age << "\n";
        std::cout << "Gender: " << patient->gender << "\n";
        std::cout << "Address: " << patient->address << "\n";
        std::cout << "Phone: " << patient->phone << "\n";
        std::cout << "Blood Group: " << patient->bloodGroup << "\n";
        std::cout << "Medical History: " << patient->medicalHistory << "\n";
        std::cout << "=====================================\n";
    } else {
        std::cout << "\nPatient with ID " << id << " not found!\n";
    }
}

// View all patients
void PatientManager::viewAllPatients() {
    clearScreen();
    std::cout << "========== ALL PATIENTS ==========\n";
    if (root == nullptr) {
        std::cout << "No patients registered yet.\n";
    } else {
        inorderTraversal(root);
    }
    std::cout << "\n=====================================\n";
    pause();
}

// Update patient information
void PatientManager::updatePatient(int id) {
    Patient* patient = searchBST(root, id);
    if (patient == nullptr) {
        std::cout << "\nPatient with ID " << id << " not found!\n";
        pause();
        return;
    }
    
    clearScreen();
    std::cout << "========== UPDATE PATIENT ==========\n";
    std::cout << "Current Information:\n";
    viewPatient(id);
    std::cout << "\nEnter new information (press Enter to keep current value):\n\n";
    
    std::string input;
    
    std::cout << "Name [" << patient->name << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) patient->name = input;
    
    std::cout << "Age [" << patient->age << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) patient->age = std::stoi(input);
    
    std::cout << "Gender [" << patient->gender << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) patient->gender = input;
    
    std::cout << "Address [" << patient->address << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) patient->address = input;
    
    std::cout << "Phone [" << patient->phone << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) patient->phone = input;
    
    std::cout << "Blood Group [" << patient->bloodGroup << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) patient->bloodGroup = input;
    
    std::cout << "Medical History [" << patient->medicalHistory << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) patient->medicalHistory = input;
    
    std::cout << "\nPatient information updated successfully!\n";
    saveToFile();
    pause();
}

// Delete patient
void PatientManager::deletePatient(int id) {
    Patient* patient = searchBST(root, id);
    if (patient == nullptr) {
        std::cout << "\nPatient with ID " << id << " not found!\n";
        pause();
        return;
    }
    
    std::cout << "\nDeleting patient: " << patient->name << "\n";
    root = deleteBST(root, id);
    std::cout << "Patient deleted successfully!\n";
    saveToFile();
    pause();
}

// Check if patient exists
bool PatientManager::patientExists(int id) {
    return searchBST(root, id) != nullptr;
}

// Load patients from file
void PatientManager::loadFromFile() {
    std::ifstream file("patients.txt");
    if (!file.is_open()) {
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        int id, age;
        std::string name, gender, address, phone, bloodGroup, medicalHistory;
        
        if (iss >> id >> age) {
            std::getline(iss, name, '|');
            std::getline(iss, name, '|');
            std::getline(iss, gender, '|');
            std::getline(iss, address, '|');
            std::getline(iss, phone, '|');
            std::getline(iss, bloodGroup, '|');
            std::getline(iss, medicalHistory, '|');
            
            // Clean up strings
            if (!name.empty() && name[0] == ' ') name = name.substr(1);
            
            Patient* newPatient = new Patient(id, name, age, gender, address, phone, bloodGroup, medicalHistory);
            root = insertBST(root, newPatient);
        }
    }
    file.close();
}

// Save helper for file writing
void PatientManager::saveToFileHelper(Patient* node, std::ofstream& file) {
    if (node != nullptr) {
        saveToFileHelper(node->left, file);
        file << node->patientID << " " << node->age << " |" << node->name 
             << "|" << node->gender << "|" << node->address << "|" 
             << node->phone << "|" << node->bloodGroup << "|" 
             << node->medicalHistory << "|\n";
        saveToFileHelper(node->right, file);
    }
}

// Save patients to file
void PatientManager::saveToFile() {
    std::ofstream file("patients.txt");
    if (file.is_open()) {
        saveToFileHelper(root, file);
        file.close();
    }
}

// Clear tree
void PatientManager::clearTree(Patient* node) {
    if (node != nullptr) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}



