#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <fstream>

struct Patient {
    int patientID;
    std::string name;
    int age;
    std::string gender;
    std::string address;
    std::string phone;
    std::string bloodGroup;
    std::string medicalHistory;
    
    Patient* left;
    Patient* right;
    

    Patient(int id, std::string n, int a, std::string g, std::string addr, 
            std::string ph, std::string bg, std::string mh);
};

class PatientManager {
private:
    Patient* root;
    

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
 
    void registerPatient();
    void viewPatient(int id);
    void viewAllPatients();
    void updatePatient(int id);
    void deletePatient(int id);

    void loadFromFile();
    void saveToFile();

    bool patientExists(int id);
};

#endif



