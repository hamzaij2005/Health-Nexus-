#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <fstream>
#include <vector>

// Doctor structure
struct Doctor {
    int doctorID;
    std::string name;
    std::string specialization;
    std::string department;
    std::string phone;
    std::string email;
    std::string qualification;
    double consultationFee;
    
    // BST pointers
    Doctor* left;
    Doctor* right;
    
    // List of assigned patient IDs
    std::vector<int> assignedPatients;
    
    // Constructor
    Doctor(int id, std::string n, std::string spec, std::string dept, 
           std::string ph, std::string em, std::string qual, double fee);
};

// Doctor Management Class using BST
class DoctorManager {
private:
    Doctor* root;
    
    // Helper functions
    Doctor* insertBST(Doctor* node, Doctor* newDoctor);
    Doctor* searchBST(Doctor* node, int id);
    Doctor* deleteBST(Doctor* node, int id);
    Doctor* findMin(Doctor* node);
    void inorderTraversal(Doctor* node);
    void saveToFileHelper(Doctor* node, std::ofstream& file);
    void clearTree(Doctor* node);
    
public:
    DoctorManager();
    ~DoctorManager();
    
    // Main functions
    void registerDoctor();
    void viewDoctor(int id);
    void viewAllDoctors();
    void updateInfo(int id);
    void assignPatient(int doctorID, int patientID);
    void viewAssignedPatients(int doctorID);
    
    // File operations
    void loadFromFile();
    void saveToFile();
    
    // Check if doctor exists
    bool doctorExists(int id);
    
    // Get all doctor IDs
    std::vector<int> getAllDoctorIDs();
};

#endif



