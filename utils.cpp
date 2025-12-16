#include "utils.h"
#include <limits>
#include <cstdlib>

// Validate and get integer input
int getValidInt(const std::string& prompt) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input! Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cin.ignore(); // Clear newline
    return value;
}

// Validate and get string input
std::string getValidString(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    while (value.empty()) {
        std::cout << "Input cannot be empty! Please enter again: ";
        std::getline(std::cin, value);
    }
    return value;
}

// Generate unique ID by reading existing IDs from file
int generateID(const std::string& filename) {
    std::ifstream file(filename);
    int maxID = 0;
    int currentID;
    
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (iss >> currentID) {
                if (currentID > maxID) {
                    maxID = currentID;
                }
            }
        }
        file.close();
    }
    
    return maxID + 1;
}

// Get current date as string
std::string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << ltm->tm_mday << "/"
        << std::setw(2) << (1 + ltm->tm_mon) << "/"
        << (1900 + ltm->tm_year);
    
    return oss.str();
}

// Clear screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Pause execution
void pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

// Format currency
std::string formatCurrency(double amount) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << "Rs. " << amount;
    return oss.str();
}



