#include "bed.h"
#include "utils.h"
#include <iostream>
#include <sstream>

// Bed constructor
Bed::Bed(int id, int ward, std::string type, bool occupied, int pid, std::string date, double charge)
    : bedID(id), wardNumber(ward), bedType(type), isOccupied(occupied), 
      patientID(pid), allocationDate(date), dailyCharge(charge), next(nullptr) {}

// BedManager constructor
BedManager::BedManager() : head(nullptr), bedCounter(0) {
    loadFromFile();
}

// BedManager destructor
BedManager::~BedManager() {
    saveToFile();
    Bed* current = head;
    while (current != nullptr) {
        Bed* temp = current;
        current = current->next;
        delete temp;
    }
}

// Find bed by ID
Bed* BedManager::findBed(int bedID) {
    Bed* current = head;
    while (current != nullptr) {
        if (current->bedID == bedID) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Display bed information
void BedManager::displayBed(Bed* bed) {
    if (bed == nullptr) return;
    
    std::cout << "\n----------------------------------------\n";
    std::cout << "Bed ID: " << bed->bedID << "\n";
    std::cout << "Ward Number: " << bed->wardNumber << "\n";
    std::cout << "Bed Type: " << bed->bedType << "\n";
    std::cout << "Status: " << (bed->isOccupied ? "Occupied" : "Available") << "\n";
    std::cout << "Daily Charge: " << formatCurrency(bed->dailyCharge) << "\n";
    
    if (bed->isOccupied) {
        std::cout << "Patient ID: " << bed->patientID << "\n";
        std::cout << "Allocation Date: " << bed->allocationDate << "\n";
    }
    std::cout << "----------------------------------------\n";
}

// Add new bed
void BedManager::addBed() {
    clearScreen();
    std::cout << "========== ADD NEW BED ==========\n\n";
    
    int bedID = generateID("beds.txt");
    int wardNumber = getValidInt("Enter Ward Number: ");
    std::string bedType = getValidString("Enter Bed Type (General/ICU/Private/Semi-Private): ");
    double dailyCharge = getValidInt("Enter Daily Charge: ");
    
    Bed* newBed = new Bed(bedID, wardNumber, bedType, false, -1, "", dailyCharge);
    
    // Insert at beginning
    newBed->next = head;
    head = newBed;
    
    std::cout << "\nBed added successfully! Bed ID: " << bedID << "\n";
    saveToFile();
    pause();
}

// Allocate bed to patient
void BedManager::allocateBed() {
    clearScreen();
    std::cout << "========== ALLOCATE BED ==========\n\n";
    
    viewAvailableBeds();
    
    int bedID = getValidInt("\nEnter Bed ID to allocate: ");
    Bed* bed = findBed(bedID);
    
    if (bed == nullptr) {
        std::cout << "\nBed with ID " << bedID << " not found!\n";
        pause();
        return;
    }
    
    if (bed->isOccupied) {
        std::cout << "\nBed is already occupied!\n";
        pause();
        return;
    }
    
    int patientID = getValidInt("Enter Patient ID: ");
    std::string date = getCurrentDate();
    
    bed->isOccupied = true;
    bed->patientID = patientID;
    bed->allocationDate = date;
    
    std::cout << "\nBed allocated successfully!\n";
    displayBed(bed);
    saveToFile();
    pause();
}

// Free bed
void BedManager::freeBed(int bedID) {
    Bed* bed = findBed(bedID);
    
    if (bed == nullptr) {
        std::cout << "\nBed with ID " << bedID << " not found!\n";
        pause();
        return;
    }
    
    if (!bed->isOccupied) {
        std::cout << "\nBed is already free!\n";
        pause();
        return;
    }
    
    std::cout << "\nFreeing bed " << bedID << " (Patient ID: " << bed->patientID << ")\n";
    bed->isOccupied = false;
    bed->patientID = -1;
    bed->allocationDate = "";
    
    std::cout << "Bed freed successfully!\n";
    saveToFile();
    pause();
}

// View all beds
void BedManager::viewBeds() {
    clearScreen();
    std::cout << "========== ALL BEDS ==========\n";
    
    if (head == nullptr) {
        std::cout << "No beds registered yet.\n";
        pause();
        return;
    }
    
    Bed* current = head;
    while (current != nullptr) {
        displayBed(current);
        current = current->next;
    }
    
    std::cout << "\n============================\n";
    pause();
}

// View available beds
void BedManager::viewAvailableBeds() {
    std::cout << "========== AVAILABLE BEDS ==========\n";
    
    bool found = false;
    Bed* current = head;
    
    while (current != nullptr) {
        if (!current->isOccupied) {
            found = true;
            displayBed(current);
        }
        current = current->next;
    }
    
    if (!found) {
        std::cout << "No available beds.\n";
    }
    std::cout << "\n==================================\n";
}

// View occupied beds
void BedManager::viewOccupiedBeds() {
    clearScreen();
    std::cout << "========== OCCUPIED BEDS ==========\n";
    
    bool found = false;
    Bed* current = head;
    
    while (current != nullptr) {
        if (current->isOccupied) {
            found = true;
            displayBed(current);
        }
        current = current->next;
    }
    
    if (!found) {
        std::cout << "No occupied beds.\n";
    }
    std::cout << "\n==================================\n";
    pause();
}

// Check if bed is available
bool BedManager::isBedAvailable(int bedID) {
    Bed* bed = findBed(bedID);
    return (bed != nullptr && !bed->isOccupied);
}

// Load beds from file
void BedManager::loadFromFile() {
    std::ifstream file("beds.txt");
    if (!file.is_open()) {
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        int id, ward, patientID;
        bool occupied;
        double charge;
        std::string type, date;
        
        if (iss >> id >> ward >> occupied >> patientID >> charge) {
            std::getline(iss, type, '|');
            std::getline(iss, type, '|');
            std::getline(iss, date, '|');
            
            if (!type.empty() && type[0] == ' ') type = type.substr(1);
            if (date.empty()) date = "";
            
            Bed* newBed = new Bed(id, ward, type, occupied, patientID, date, charge);
            newBed->next = head;
            head = newBed;
            
            if (id > bedCounter) {
                bedCounter = id;
            }
        }
    }
    file.close();
}

// Save beds to file
void BedManager::saveToFile() {
    std::ofstream file("beds.txt");
    if (file.is_open()) {
        Bed* current = head;
        while (current != nullptr) {
            file << current->bedID << " " << current->wardNumber << " " 
                 << (current->isOccupied ? 1 : 0) << " " << current->patientID << " " 
                 << current->dailyCharge << " |" << current->bedType << "|" 
                 << current->allocationDate << "|\n";
            current = current->next;
        }
        file.close();
    }
}



