#include "doctor.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;

Doctor::Doctor(int id, string n, string spec, string dept, string ph, string em, string qual, double fee): doctorID(id), name(n), specialization(spec), department(dept), phone(ph), email(em), qualification(qual), consultationFee(fee), left(nullptr), right(nullptr) {}

DoctorManager::DoctorManager() : root(nullptr) {
    loadFromFile();
}
DoctorManager::~DoctorManager() {
    saveToFile();
    clearTree(root);
}
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
Doctor* DoctorManager::findMin(Doctor* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

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
void DoctorManager::inorderTraversal(Doctor* node) {
    if (node != nullptr) {
        inorderTraversal(node->left);
        cout << "----------------------------------------" << endl;
        cout << "Doctor ID: " << node->doctorID << endl;
        cout << "Name: " << node->name << endl;
        cout << "Specialization: " << node->specialization << endl;
        cout << "Department: " << node->department << endl;
        cout << "Phone: " << node->phone << endl;
        cout << "Email: " << node->email << endl;
        cout << "Qualification: " << node->qualification << endl;
        cout << "Consultation Fee: " << formatCurrency(node->consultationFee) << endl;
        cout << "Assigned Patients: " << node->assignedPatients.size() << endl;
        inorderTraversal(node->right);
    }
}
void getAllDoctorIDsHelper(Doctor* node, vector<int>& ids) {
    if (node != nullptr) {
        getAllDoctorIDsHelper(node->left, ids);
        ids.push_back(node->doctorID);
        getAllDoctorIDsHelper(node->right, ids);
    }
}
void DoctorManager::registerDoctor() {
    clearScreen();
    cout << "========== REGISTER NEW DOCTOR ==========" << endl << endl;
    
    int id = generateID("doctors.txt");
    cout << "Generated Doctor ID: " << id << endl << endl;
    
    string name = getValidString("Enter Doctor Name: ");
    string specialization = getValidString("Enter Specialization: ");
    string department = getValidString("Enter Department: ");
    string phone = getValidString("Enter Phone Number: ");
    string email = getValidString("Enter Email: ");
    string qualification = getValidString("Enter Qualification: ");
    double fee = getValidInt("Enter Consultation Fee: ");
    
    Doctor* newDoctor = new Doctor(id, name, specialization, department, phone, email, qualification, fee);
    root = insertBST(root, newDoctor);
    
    cout << endl << "Doctor registered successfully!" << endl;
    saveToFile();
    pause();
}

void DoctorManager::viewDoctor(int id) {
    Doctor* doctor = searchBST(root, id);
    if (doctor != nullptr) {
        cout << "========== DOCTOR DETAILS ==========" << endl;
        cout << "Doctor ID: " << doctor->doctorID << endl;
        cout << "Name: " << doctor->name << endl;
        cout << "Specialization: " << doctor->specialization << endl;
        cout << "Department: " << doctor->department << endl;
        cout << "Phone: " << doctor->phone << endl;
        cout << "Email: " << doctor->email << endl;
        cout << "Qualification: " << doctor->qualification << endl;
        cout << "Consultation Fee: " << formatCurrency(doctor->consultationFee) << endl;
        cout << "Assigned Patients: " << doctor->assignedPatients.size() << endl;
        cout << "=====================================" << endl;
    } else {
        cout << "Doctor with ID " << id << " not found!" << endl;
    }
}

void DoctorManager::viewAllDoctors() {
    clearScreen();
    cout << "========== ALL DOCTORS ==========" << endl;
    if (root == nullptr) {
        cout << "No doctors registered yet." << endl;
    } else {
        inorderTraversal(root);
    }
    cout << "=====================================" << endl;
    pause();
}

void DoctorManager::updateInfo(int id) {
    Doctor* doctor = searchBST(root, id);
    if (doctor == nullptr) {
        cout << "Doctor with ID " << id << " not found!" << endl;
        pause();
        return;
    }
    
    clearScreen();
    cout << "========== UPDATE DOCTOR ==========" << endl;
    cout << "Current Information:" << endl;
    viewDoctor(id);
    cout << "Enter new information (press Enter to keep current value):" << endl << endl;
    
    string input;
    
    cout << "Name [" << doctor->name << "]: ";
    getline(cin, input);
    if (!input.empty()) doctor->name = input;
    
    cout << "Specialization [" << doctor->specialization << "]: ";
    getline(cin, input);
    if (!input.empty()) doctor->specialization = input;
    
    cout << "Department [" << doctor->department << "]: ";
    getline(cin, input);
    if (!input.empty()) doctor->department = input;
    
    cout << "Phone [" << doctor->phone << "]: ";
    getline(cin, input);
    if (!input.empty()) doctor->phone = input;
    
    cout << "Email [" << doctor->email << "]: ";
    getline(cin, input);
    if (!input.empty()) doctor->email = input;
    
    cout << "Qualification [" << doctor->qualification << "]: ";
    getline(cin, input);
    if (!input.empty()) doctor->qualification = input;
    
    cout << "Consultation Fee [" << doctor->consultationFee << "]: ";
    getline(cin, input);
    if (!input.empty()) doctor->consultationFee = stod(input);
    
    cout << endl << "Doctor information updated successfully!" << endl;
    saveToFile();
    pause();
}

void DoctorManager::assignPatient(int doctorID, int patientID) {
    Doctor* doctor = searchBST(root, doctorID);
    if (doctor == nullptr) {
        cout << "Doctor with ID " << doctorID << " not found!" << endl;
        return;
    }
    
    if (find(doctor->assignedPatients.begin(), doctor->assignedPatients.end(), patientID) != doctor->assignedPatients.end()) {
        cout << "Patient already assigned to this doctor!" << endl;
        return;
    }
    doctor->assignedPatients.push_back(patientID);
    cout << "Patient " << patientID << " assigned to Doctor " << doctor->name << " successfully!" << endl;
    saveToFile();
}

void DoctorManager::viewAssignedPatients(int doctorID) {
    Doctor* doctor = searchBST(root, doctorID);
    if (doctor == nullptr) {
        cout << "Doctor with ID " << doctorID << " not found!" << endl;
        return;
    }
    
    cout << "========== ASSIGNED PATIENTS ==========" << endl;
    cout << "Doctor: " << doctor->name << " (ID: " << doctor->doctorID << ")" << endl << endl;
    
    if (doctor->assignedPatients.empty()) {
        cout << "No patients assigned yet." << endl;
    } else {
        cout << "Patient IDs: ";
        for (size_t i = 0; i < doctor->assignedPatients.size(); i++) {
            cout << doctor->assignedPatients[i];
            if (i < doctor->assignedPatients.size() - 1) cout << ", ";
        }
        cout << endl;
    }
    cout << "======================================" << endl;
}
bool DoctorManager::doctorExists(int id) {
    return searchBST(root, id) != nullptr;
}

vector<int> DoctorManager::getAllDoctorIDs() {
    vector<int> ids;
    getAllDoctorIDsHelper(root, ids);
    return ids;
}

void DoctorManager::loadFromFile() {
    ifstream file("doctors.txt");
    if (!file.is_open()) return;
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        int id;
        double fee;
        string name, specialization, department, phone, email, qualification;
        
        if (iss >> id >> fee) {
            getline(iss, name, '|');
            getline(iss, name, '|');
            getline(iss, specialization, '|');
            getline(iss, department, '|');
            getline(iss, phone, '|');
            getline(iss, email, '|');
            getline(iss, qualification, '|');
            
            if (!name.empty() && name[0] == ' ') name = name.substr(1);
            
            Doctor* newDoctor = new Doctor(id, name, specialization, department, phone, email, qualification, fee);
            
            string patientsStr;
            getline(iss, patientsStr, '|');
            if (!patientsStr.empty()) {
                istringstream pIss(patientsStr);
                int patientID;
                while (pIss >> patientID) newDoctor->assignedPatients.push_back(patientID);
            }
            
            root = insertBST(root, newDoctor);
        }
    }
    file.close();
}
void DoctorManager::saveToFileHelper(Doctor* node, ofstream& file) {
    if (node != nullptr) {
        saveToFileHelper(node->left, file);
        file << node->doctorID << " " << node->consultationFee << " |" << node->name 
             << "|" << node->specialization << "|" << node->department << "|" 
             << node->phone << "|" << node->email << "|" << node->qualification << "|";
        
        for (size_t i = 0; i < node->assignedPatients.size(); i++) {
            file << node->assignedPatients[i];
            if (i < node->assignedPatients.size() - 1) file << " ";
        }
        file << "|\n";
        
        saveToFileHelper(node->right, file);
    }
}
void DoctorManager::saveToFile() {
    ofstream file("doctors.txt");
    if (file.is_open()) {
        saveToFileHelper(root, file);
        file.close();
    }
}

void DoctorManager::clearTree(Doctor* node) {
    if (node != nullptr) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}
