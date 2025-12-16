#ifndef BILLING_H
#define BILLING_H

#include <string>
#include <fstream>
#include <vector>

// Bill Item structure
struct BillItem {
    std::string itemName;
    int quantity;
    double unitPrice;
    double totalPrice;
    
    BillItem(std::string name, int qty, double price);
};

// Bill structure
struct Bill {
    int billID;
    int patientID;
    std::string date;
    std::vector<BillItem> items;
    double consultationFee;
    double bedCharges;
    double totalAmount;
    std::string status; // Paid, Pending, Cancelled
    
    // Linked list pointer
    Bill* next;
    
    // Constructor
    Bill(int id, int pid, std::string d, double cf, double bc);
};

// Billing Management Class using Linked List
class BillingManager {
private:
    Bill* head;
    
    // Helper functions
    Bill* findBill(int billID);
    void displayBill(Bill* bill);
    double calculateTotal(Bill* bill);
    
public:
    BillingManager();
    ~BillingManager();
    
    // Main functions
    void generateBill();
    void viewBill(int billID);
    void viewAllBills();
    void updateBill(int billID);
    void addItemToBill(int billID, std::string itemName, int quantity, double unitPrice);
    void markBillPaid(int billID);
    
    // File operations
    void loadFromFile();
    void saveToFile();
    
    // Sorting functions (using QuickSort)
    void sortBillsByAmount();
    void sortBillsByDate();
    void quickSortByAmount(Bill* arr[], int low, int high);
    int partitionByAmount(Bill* arr[], int low, int high);
};

#endif



