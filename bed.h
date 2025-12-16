#ifndef BED_H
#define BED_H

#include <string>
#include <fstream>

// Bed structure
struct Bed {
    int bedID;
    int wardNumber;
    std::string bedType; // General, ICU, Private, Semi-Private
    bool isOccupied;
    int patientID; // -1 if unoccupied
    std::string allocationDate;
    double dailyCharge;
    
    // Linked list pointer
    Bed* next;
    
    // Constructor
    Bed(int id, int ward, std::string type, bool occupied, int pid, std::string date, double charge);
};

// Bed Management Class using Linked List
class BedManager {
private:
    Bed* head;
    int bedCounter;
    
    // Helper functions
    Bed* findBed(int bedID);
    void displayBed(Bed* bed);
    
public:
    BedManager();
    ~BedManager();
    
    // Main functions
    void allocateBed();
    void freeBed(int bedID);
    void viewBeds();
    void viewAvailableBeds();
    void viewOccupiedBeds();
    void addBed();
    
    // File operations
    void loadFromFile();
    void saveToFile();
    
    // Check if bed is available
    bool isBedAvailable(int bedID);
};

#endif



