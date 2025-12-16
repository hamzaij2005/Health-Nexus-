#include "billing.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// BillItem constructor
BillItem::BillItem(std::string name, int qty, double price)
    : itemName(name), quantity(qty), unitPrice(price) {
    totalPrice = quantity * unitPrice;
}

// Bill constructor
Bill::Bill(int id, int pid, std::string d, double cf, double bc)
    : billID(id), patientID(pid), date(d), consultationFee(cf), 
      bedCharges(bc), totalAmount(0.0), status("Pending"), next(nullptr) {
    totalAmount = consultationFee + bedCharges;
}

// BillingManager constructor
BillingManager::BillingManager() : head(nullptr) {
    loadFromFile();
}

// BillingManager destructor
BillingManager::~BillingManager() {
    saveToFile();
    Bill* current = head;
    while (current != nullptr) {
        Bill* temp = current;
        current = current->next;
        delete temp;
    }
}

// Find bill by ID
Bill* BillingManager::findBill(int billID) {
    Bill* current = head;
    while (current != nullptr) {
        if (current->billID == billID) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Calculate total for a bill
double BillingManager::calculateTotal(Bill* bill) {
    if (bill == nullptr) return 0.0;
    
    double total = bill->consultationFee + bill->bedCharges;
    for (const auto& item : bill->items) {
        total += item.totalPrice;
    }
    return total;
}

// Display bill
void BillingManager::displayBill(Bill* bill) {
    if (bill == nullptr) return;
    
    std::cout << "\n========== BILL DETAILS ==========\n";
    std::cout << "Bill ID: " << bill->billID << "\n";
    std::cout << "Patient ID: " << bill->patientID << "\n";
    std::cout << "Date: " << bill->date << "\n";
    std::cout << "Status: " << bill->status << "\n";
    std::cout << "\n----------------------------------------\n";
    std::cout << "ITEMS:\n";
    
    if (bill->items.empty()) {
        std::cout << "No items added.\n";
    } else {
        for (const auto& item : bill->items) {
            std::cout << item.itemName << " x " << item.quantity 
                     << " @ " << formatCurrency(item.unitPrice) 
                     << " = " << formatCurrency(item.totalPrice) << "\n";
        }
    }
    
    std::cout << "\n----------------------------------------\n";
    std::cout << "Consultation Fee: " << formatCurrency(bill->consultationFee) << "\n";
    std::cout << "Bed Charges: " << formatCurrency(bill->bedCharges) << "\n";
    std::cout << "Items Total: " << formatCurrency(calculateTotal(bill) - bill->consultationFee - bill->bedCharges) << "\n";
    std::cout << "----------------------------------------\n";
    bill->totalAmount = calculateTotal(bill);
    std::cout << "TOTAL AMOUNT: " << formatCurrency(bill->totalAmount) << "\n";
    std::cout << "=====================================\n";
}

// Generate new bill
void BillingManager::generateBill() {
    clearScreen();
    std::cout << "========== GENERATE BILL ==========\n\n";
    
    int billID = generateID("billing.txt");
    int patientID = getValidInt("Enter Patient ID: ");
    std::string date = getCurrentDate();
    double consultationFee = getValidInt("Enter Consultation Fee: ");
    double bedCharges = getValidInt("Enter Bed Charges (0 if not applicable): ");
    
    Bill* newBill = new Bill(billID, patientID, date, consultationFee, bedCharges);
    
    // Add items
    char addMore = 'y';
    while (addMore == 'y' || addMore == 'Y') {
        std::string itemName = getValidString("Enter Item Name (Medicine/Service): ");
        int quantity = getValidInt("Enter Quantity: ");
        double unitPrice = getValidInt("Enter Unit Price: ");
        
        newBill->items.push_back(BillItem(itemName, quantity, unitPrice));
        
        std::cout << "Add more items? (y/n): ";
        std::cin >> addMore;
        std::cin.ignore();
    }
    
    newBill->totalAmount = calculateTotal(newBill);
    
    // Insert at beginning
    newBill->next = head;
    head = newBill;
    
    std::cout << "\nBill generated successfully!\n";
    displayBill(newBill);
    saveToFile();
    pause();
}

// View specific bill
void BillingManager::viewBill(int billID) {
    Bill* bill = findBill(billID);
    if (bill != nullptr) {
        displayBill(bill);
    } else {
        std::cout << "\nBill with ID " << billID << " not found!\n";
    }
    pause();
}

// View all bills
void BillingManager::viewAllBills() {
    clearScreen();
    std::cout << "========== ALL BILLS ==========\n";
    
    if (head == nullptr) {
        std::cout << "No bills generated yet.\n";
        pause();
        return;
    }
    
    Bill* current = head;
    while (current != nullptr) {
        displayBill(current);
        current = current->next;
    }
    
    std::cout << "\n============================\n";
    pause();
}

// Update bill
void BillingManager::updateBill(int billID) {
    Bill* bill = findBill(billID);
    if (bill == nullptr) {
        std::cout << "\nBill with ID " << billID << " not found!\n";
        pause();
        return;
    }
    
    clearScreen();
    std::cout << "========== UPDATE BILL ==========\n";
    displayBill(bill);
    
    std::cout << "\n1. Add Item\n";
    std::cout << "2. Update Consultation Fee\n";
    std::cout << "3. Update Bed Charges\n";
    int choice = getValidInt("Enter choice: ");
    
    if (choice == 1) {
        std::string itemName = getValidString("Enter Item Name: ");
        int quantity = getValidInt("Enter Quantity: ");
        double unitPrice = getValidInt("Enter Unit Price: ");
        addItemToBill(billID, itemName, quantity, unitPrice);
    } else if (choice == 2) {
        bill->consultationFee = getValidInt("Enter New Consultation Fee: ");
    } else if (choice == 3) {
        bill->bedCharges = getValidInt("Enter New Bed Charges: ");
    }
    
    bill->totalAmount = calculateTotal(bill);
    std::cout << "\nBill updated successfully!\n";
    saveToFile();
    pause();
}

// Add item to bill
void BillingManager::addItemToBill(int billID, std::string itemName, int quantity, double unitPrice) {
    Bill* bill = findBill(billID);
    if (bill != nullptr) {
        bill->items.push_back(BillItem(itemName, quantity, unitPrice));
        bill->totalAmount = calculateTotal(bill);
        std::cout << "\nItem added to bill successfully!\n";
        saveToFile();
    }
}

// Mark bill as paid
void BillingManager::markBillPaid(int billID) {
    Bill* bill = findBill(billID);
    if (bill != nullptr) {
        bill->status = "Paid";
        std::cout << "\nBill marked as paid!\n";
        saveToFile();
    } else {
        std::cout << "\nBill with ID " << billID << " not found!\n";
    }
    pause();
}

// Partition for QuickSort by amount
int BillingManager::partitionByAmount(Bill* arr[], int low, int high) {
    double pivot = arr[high]->totalAmount;
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j]->totalAmount <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// QuickSort by amount
void BillingManager::quickSortByAmount(Bill* arr[], int low, int high) {
    if (low < high) {
        int pi = partitionByAmount(arr, low, high);
        quickSortByAmount(arr, low, pi - 1);
        quickSortByAmount(arr, pi + 1, high);
    }
}

// Sort bills by amount
void BillingManager::sortBillsByAmount() {
    // Count bills
    int count = 0;
    Bill* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    
    if (count == 0) {
        std::cout << "No bills to sort.\n";
        return;
    }
    
    // Create array
    Bill** arr = new Bill*[count];
    current = head;
    for (int i = 0; i < count; i++) {
        arr[i] = current;
        current = current->next;
    }
    
    // Sort
    quickSortByAmount(arr, 0, count - 1);
    
    // Rebuild linked list
    head = arr[0];
    for (int i = 0; i < count - 1; i++) {
        arr[i]->next = arr[i + 1];
    }
    arr[count - 1]->next = nullptr;
    
    delete[] arr;
    
    std::cout << "\nBills sorted by amount successfully!\n";
    viewAllBills();
}

// Sort bills by date (simple implementation)
void BillingManager::sortBillsByDate() {
    std::cout << "\nSorting by date feature - basic implementation\n";
    // For full implementation, would need date parsing and comparison
    pause();
}

// Load bills from file
void BillingManager::loadFromFile() {
    std::ifstream file("billing.txt");
    if (!file.is_open()) {
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        int id, patientID;
        double cf, bc, total;
        std::string date, status;
        
        if (iss >> id >> patientID >> cf >> bc >> total) {
            std::getline(iss, date, '|');
            std::getline(iss, date, '|');
            std::getline(iss, status, '|');
            
            if (!date.empty() && date[0] == ' ') date = date.substr(1);
            if (status.empty()) status = "Pending";
            
            Bill* newBill = new Bill(id, patientID, date, cf, bc);
            newBill->totalAmount = total;
            newBill->status = status;
            
            // Load items
            std::string itemsStr;
            std::getline(iss, itemsStr);
            if (!itemsStr.empty()) {
                std::istringstream itemIss(itemsStr);
                std::string itemName;
                int quantity;
                double unitPrice;
                
                while (itemIss >> itemName >> quantity >> unitPrice) {
                    newBill->items.push_back(BillItem(itemName, quantity, unitPrice));
                }
            }
            
            newBill->next = head;
            head = newBill;
        }
    }
    file.close();
}

// Save bills to file
void BillingManager::saveToFile() {
    std::ofstream file("billing.txt");
    if (file.is_open()) {
        Bill* current = head;
        while (current != nullptr) {
            file << current->billID << " " << current->patientID << " " 
                 << current->consultationFee << " " << current->bedCharges << " " 
                 << current->totalAmount << " |" << current->date << "|" 
                 << current->status << "|";
            
            // Save items
            for (const auto& item : current->items) {
                file << item.itemName << " " << item.quantity << " " << item.unitPrice << " ";
            }
            file << "\n";
            
            current = current->next;
        }
        file.close();
    }
}



