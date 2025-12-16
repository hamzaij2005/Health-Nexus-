#include "doctor.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Doctor constructor
Doctor::Doctor(int id, std::string n, std::string spec, std::string dept, 
               std::string ph, std::string em, std::string qual, double fee)
    : doctorID(id), name(n), specialization(spec), department(dept), 
      phone(ph), email(em), qualification(qual), consultationFee(fee), 
      left(nullptr), right(nullptr) {}

// DoctorManager constructor
DoctorManager::DoctorManager() : root(nullptr) {
    loadFromFile();
}

// DoctorManager destructor
DoctorManager::~DoctorManager() {
    saveToFile();
    clearTree(root);
}

// Insert into BST
Doctor* DoctorManager::insertBST(Doctor* node, Doctor* newDoctor) {
    if (node == nullptr) {
        return newDoctor;
    }
    
    if (newDoctor->doctorID < node->doctorID) {
        node->left = insertBST(node->left, newDoctor);
    } else if (newDoctor->doctorID > node->doctorID) {
        node->right = insertBST(node->right, newDoctor);
    }
    
    return node;
}

// Search in BST
Doctor* DoctorManager::searchBST(Doctor* node, int id) {
    if (node == nullptr || node->doctorID == id) {
        return node;
    }
    
    if (id < node->doctorID) {
        return searchBST(node->left, id);
    } else {
        return searchBST(node->right, id);
    }
}

// Find minimum node
Doctor* DoctorManager::findMin(Doctor* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Delete from BST
Doctor* DoctorManager::deleteBST(Doctor* node, int id) {
    if (node == nullptr) {
        return node;
    }
    
    if (id < node->doctorID) {
        node->left = deleteBST(node->left, id);
    } else if (id > node->doctorID) {
        node->right = deleteBST(node->right, id);
    } else {
        if (node->left == nullptr) {
            Doctor* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            Doctor* temp = node->left;
            delete node;
            return temp;
        }
        
        Doctor* temp = findMin(node->right);
        node->doctorID = temp->doctorID;
        node->name = temp->name;
        node->specialization = temp->specialization;
        node->department = temp->department;
        node->phone = temp->phone;
        node->email = temp->email;
        node->qualification = temp->qualification;
        node->consultationFee = temp->consultationFee;
        node->assignedPatients = temp->assignedPatients;
        
        node->right = deleteBST(node->right, temp->doctorID);
    }
    
    return node;
}

// Inorder traversal
void DoctorManager::inorderTraversal(Doctor* node) {
    if (node != nullptr) {
        inorderTraversal(node->left);
        std::cout << "\n----------------------------------------\n";
        std::cout << "Doctor ID: " << node->doctorID << "\n";
        std::cout << "Name: " << node->name << "\n";
        std::cout << "Specialization: " << node->specialization << "\n";
        std::cout << "Department: " << node->department << "\n";
        std::cout << "Phone: " << node->phone << "\n";
        std::cout << "Email: " << node->email << "\n";
        std::cout << "Qualification: " << node->qualification << "\n";
        std::cout << "Consultation Fee: " << formatCurrency(node->consultationFee) << "\n";
        std::cout << "Assigned Patients: " << node->assignedPatients.size() << "\n";
        inorderTraversal(node->right);
    }
}

// Helper function to get all doctor IDs (forward declaration)
void getAllDoctorIDsHelper(Doctor* node, std::vector<int>& ids) {
    if (node != nullptr) {
        getAllDoctorIDsHelper(node->left, ids);
        ids.push_back(node->doctorID);
        getAllDoctorIDsHelper(node->right, ids);
    }
}

// Register new doctor
void DoctorManager::registerDoctor() {
    clearScreen();
    std::cout << "========== REGISTER NEW DOCTOR ==========\n\n";
    
    int id = generateID("doctors.txt");
    std::cout << "Generated Doctor ID: " << id << "\n\n";
    
    std::string name = getValidString("Enter Doctor Name: ");
    std::string specialization = getValidString("Enter Specialization: ");
    std::string department = getValidString("Enter Department: ");
    std::string phone = getValidString("Enter Phone Number: ");
    std::string email = getValidString("Enter Email: ");
    std::string qualification = getValidString("Enter Qualification: ");
    double fee = getValidInt("Enter Consultation Fee: ");
    
    Doctor* newDoctor = new Doctor(id, name, specialization, department, phone, email, qualification, fee);
    root = insertBST(root, newDoctor);
    
    std::cout << "\nDoctor registered successfully!\n";
    saveToFile();
    pause();
}

// View specific doctor
void DoctorManager::viewDoctor(int id) {
    Doctor* doctor = searchBST(root, id);
    if (doctor != nullptr) {
        std::cout << "\n========== DOCTOR DETAILS ==========\n";
        std::cout << "Doctor ID: " << doctor->doctorID << "\n";
        std::cout << "Name: " << doctor->name << "\n";
        std::cout << "Specialization: " << doctor->specialization << "\n";
        std::cout << "Department: " << doctor->department << "\n";
        std::cout << "Phone: " << doctor->phone << "\n";
        std::cout << "Email: " << doctor->email << "\n";
        std::cout << "Qualification: " << doctor->qualification << "\n";
        std::cout << "Consultation Fee: " << formatCurrency(doctor->consultationFee) << "\n";
        std::cout << "Assigned Patients: " << doctor->assignedPatients.size() << "\n";
        std::cout << "=====================================\n";
    } else {
        std::cout << "\nDoctor with ID " << id << " not found!\n";
    }
}

// View all doctors
void DoctorManager::viewAllDoctors() {
    clearScreen();
    std::cout << "========== ALL DOCTORS ==========\n";
    if (root == nullptr) {
        std::cout << "No doctors registered yet.\n";
    } else {
        inorderTraversal(root);
    }
    std::cout << "\n=====================================\n";
    pause();
}

// Update doctor information
void DoctorManager::updateInfo(int id) {
    Doctor* doctor = searchBST(root, id);
    if (doctor == nullptr) {
        std::cout << "\nDoctor with ID " << id << " not found!\n";
        pause();
        return;
    }
    
    clearScreen();
    std::cout << "========== UPDATE DOCTOR ==========\n";
    std::cout << "Current Information:\n";
    viewDoctor(id);
    std::cout << "\nEnter new information (press Enter to keep current value):\n\n";
    
    std::string input;
    
    std::cout << "Name [" << doctor->name << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) doctor->name = input;
    
    std::cout << "Specialization [" << doctor->specialization << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) doctor->specialization = input;
    
    std::cout << "Department [" << doctor->department << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) doctor->department = input;
    
    std::cout << "Phone [" << doctor->phone << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) doctor->phone = input;
    
    std::cout << "Email [" << doctor->email << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) doctor->email = input;
    
    std::cout << "Qualification [" << doctor->qualification << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) doctor->qualification = input;
    
    std::cout << "Consultation Fee [" << doctor->consultationFee << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) doctor->consultationFee = std::stod(input);
    
    std::cout << "\nDoctor information updated successfully!\n";
    saveToFile();
    pause();
}

// Assign patient to doctor
void DoctorManager::assignPatient(int doctorID, int patientID) {
    Doctor* doctor = searchBST(root, doctorID);
    if (doctor == nullptr) {
        std::cout << "\nDoctor with ID " << doctorID << " not found!\n";
        return;
    }
    
    // Check if patient already assigned
    if (std::find(doctor->assignedPatients.begin(), doctor->assignedPatients.end(), patientID) 
        != doctor->assignedPatients.end()) {
        std::cout << "\nPatient already assigned to this doctor!\n";
        return;
    }
    
    doctor->assignedPatients.push_back(patientID);
    std::cout << "\nPatient " << patientID << " assigned to Doctor " << doctor->name << " successfully!\n";
    saveToFile();
}

// View assigned patients
void DoctorManager::viewAssignedPatients(int doctorID) {
    Doctor* doctor = searchBST(root, doctorID);
    if (doctor == nullptr) {
        std::cout << "\nDoctor with ID " << doctorID << " not found!\n";
        return;
    }
    
    std::cout << "\n========== ASSIGNED PATIENTS ==========\n";
    std::cout << "Doctor: " << doctor->name << " (ID: " << doctor->doctorID << ")\n\n";
    
    if (doctor->assignedPatients.empty()) {
        std::cout << "No patients assigned yet.\n";
    } else {
        std::cout << "Patient IDs: ";
        for (size_t i = 0; i < doctor->assignedPatients.size(); i++) {
            std::cout << doctor->assignedPatients[i];
            if (i < doctor->assignedPatients.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "======================================\n";
}

// Check if doctor exists
bool DoctorManager::doctorExists(int id) {
    return searchBST(root, id) != nullptr;
}

// Get all doctor IDs
std::vector<int> DoctorManager::getAllDoctorIDs() {
    std::vector<int> ids;
    getAllDoctorIDsHelper(root, ids);
    return ids;
}

// Load doctors from file
void DoctorManager::loadFromFile() {
    std::ifstream file("doctors.txt");
    if (!file.is_open()) {
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        int id;
        double fee;
        std::string name, specialization, department, phone, email, qualification;
        
        if (iss >> id >> fee) {
            std::getline(iss, name, '|');
            std::getline(iss, name, '|');
            std::getline(iss, specialization, '|');
            std::getline(iss, department, '|');
            std::getline(iss, phone, '|');
            std::getline(iss, email, '|');
            std::getline(iss, qualification, '|');
            
            if (!name.empty() && name[0] == ' ') name = name.substr(1);
            
            Doctor* newDoctor = new Doctor(id, name, specialization, department, phone, email, qualification, fee);
            
            // Load assigned patients
            std::string patientsStr;
            std::getline(iss, patientsStr, '|');
            if (!patientsStr.empty()) {
                std::istringstream pIss(patientsStr);
                int patientID;
                while (pIss >> patientID) {
                    newDoctor->assignedPatients.push_back(patientID);
                }
            }
            
            root = insertBST(root, newDoctor);
        }
    }
    file.close();
}

// Save helper
void DoctorManager::saveToFileHelper(Doctor* node, std::ofstream& file) {
    if (node != nullptr) {
        saveToFileHelper(node->left, file);
        file << node->doctorID << " " << node->consultationFee << " |" << node->name 
             << "|" << node->specialization << "|" << node->department << "|" 
             << node->phone << "|" << node->email << "|" << node->qualification << "|";
        
        // Save assigned patients
        for (size_t i = 0; i < node->assignedPatients.size(); i++) {
            file << node->assignedPatients[i];
            if (i < node->assignedPatients.size() - 1) {
                file << " ";
            }
        }
        file << "|\n";
        
        saveToFileHelper(node->right, file);
    }
}

// Save doctors to file
void DoctorManager::saveToFile() {
    std::ofstream file("doctors.txt");
    if (file.is_open()) {
        saveToFileHelper(root, file);
        file.close();
    }
}

// Clear tree
void DoctorManager::clearTree(Doctor* node) {
    if (node != nullptr) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}

