#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <fstream>

// Patient structure
struct Patient {
    int patientID;
    std::string name;
    int age;
    std::string gender;
    std::string address;
    std::string phone;
    std::string bloodGroup;
    std::string medicalHistory;
    
    // BST pointers
    Patient* left;
    Patient* right;
    
    // Constructor
    Patient(int id, std::string n, int a, std::string g, std::string addr, 
            std::string ph, std::string bg, std::string mh);
};

// Patient Management Class using BST
class PatientManager {
private:
    Patient* root;
    
    // Helper functions
    Patient* insertBST(Patient* node, Patient* newPatient);
    Patient* searchBST(Patient* node, int id);
    Patient* deleteBST(Patient* node, int id);
    Patient* findMin(Patient* node);
    void inorderTraversal(Patient* node);
    void saveToFileHelper(Patient* node, std::ofstream& file);
    void clearTree(Patient* node);
    
public:
    PatientManager();
    ~PatientManager();
    
    // Main functions
    void registerPatient();
    void viewPatient(int id);
    void viewAllPatients();
    void updatePatient(int id);
    void deletePatient(int id);
    
    // File operations
    void loadFromFile();
    void saveToFile();
    
    // Check if patient exists
    bool patientExists(int id);
};

#endif



