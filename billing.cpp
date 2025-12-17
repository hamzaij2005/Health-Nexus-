#include "billing.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

// BillItem constructor
BillItem::BillItem(string name, int qty, double price)
    : itemName(name), quantity(qty), unitPrice(price) {
    totalPrice = quantity * unitPrice;
}

// Bill constructor
Bill::Bill(int id, int pid, string d, double cf, double bc)
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
double BillingManager::calculateTotal(Bill* bill) {
    if (bill == nullptr) return 0.0;

    double total = bill->consultationFee + bill->bedCharges;
    for (const auto& item : bill->items) {
        total += item.totalPrice;
    }
    return total;
}
void BillingManager::displayBill(Bill* bill) {
    if (bill == nullptr) return;

    cout << endl << "========== BILL DETAILS ==========" << endl;
    cout << "Bill ID: " << bill->billID << endl;
    cout << "Patient ID: " << bill->patientID << endl;
    cout << "Date: " << bill->date << endl;
    cout << "Status: " << bill->status << endl;

    cout << endl << "----------------------------------------" << endl;
    cout << "ITEMS:" << endl;

    if (bill->items.empty()) {
        cout << "No items added." << endl;
    } else {
        for (const auto& item : bill->items) {
            cout << item.itemName << " x " << item.quantity
                 << " @ " << formatCurrency(item.unitPrice)
                 << " = " << formatCurrency(item.totalPrice) << endl;
        }
    }

    cout << endl << "----------------------------------------" << endl;
    cout << "Consultation Fee: " << formatCurrency(bill->consultationFee) << endl;
    cout << "Bed Charges: " << formatCurrency(bill->bedCharges) << endl;

    double itemsTotal = calculateTotal(bill)
                        - bill->consultationFee
                        - bill->bedCharges;

    cout << "Items Total: " << formatCurrency(itemsTotal) << endl;
    cout << "----------------------------------------" << endl;

    bill->totalAmount = calculateTotal(bill);
    cout << "TOTAL AMOUNT: " << formatCurrency(bill->totalAmount) << endl;
    cout << "=====================================" << endl;
}

// Generate bill
void BillingManager::generateBill() {
    clearScreen();
    cout << "========== GENERATE BILL ==========" << endl << endl;

    int billID = generateID("billing.txt");
    int patientID = getValidInt("Enter Patient ID: ");
    string date = getCurrentDate();
    double consultationFee = getValidInt("Enter Consultation Fee: ");
    double bedCharges = getValidInt("Enter Bed Charges (0 if not applicable): ");

    Bill* newBill = new Bill(billID, patientID, date, consultationFee, bedCharges);

    char addMore = 'y';
    while (addMore == 'y' || addMore == 'Y') {
        string itemName = getValidString("Enter Item Name: ");
        int quantity = getValidInt("Enter Quantity: ");
        double unitPrice = getValidInt("Enter Unit Price: ");

        newBill->items.push_back(BillItem(itemName, quantity, unitPrice));

        cout << "Add more items? (y/n): ";
        cin >> addMore;
        cin.ignore();
    }

    newBill->totalAmount = calculateTotal(newBill);
    newBill->next = head;
    head = newBill;

    cout << endl << "Bill generated successfully!" << endl;
    displayBill(newBill);
    saveToFile();
    pause();
}

// View bill
void BillingManager::viewBill(int billID) {
    Bill* bill = findBill(billID);
    if (bill != nullptr) {
        displayBill(bill);
    } else {
        cout << "Bill with ID " << billID << " not found!" << endl;
    }
    pause();
}

// View all bills
void BillingManager::viewAllBills() {
    clearScreen();
    cout << "========== ALL BILLS ==========" << endl;

    if (head == nullptr) {
        cout << "No bills generated yet." << endl;
        pause();
        return;
    }

    Bill* current = head;
    while (current != nullptr) {
        displayBill(current);
        current = current->next;
    }
    pause();
}

// Partition (QuickSort)
int BillingManager::partitionByAmount(Bill* arr[], int low, int high) {
    double pivot = arr[high]->totalAmount;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j]->totalAmount <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// QuickSort
void BillingManager::quickSortByAmount(Bill* arr[], int low, int high) {
    if (low < high) {
        int pi = partitionByAmount(arr, low, high);
        quickSortByAmount(arr, low, pi - 1);
        quickSortByAmount(arr, pi + 1, high);
    }
}

// Sort bills
void BillingManager::sortBillsByAmount() {
    int count = 0;
    Bill* current = head;

    while (current != nullptr) {
        count++;
        current = current->next;
    }

    if (count == 0) {
        cout << "No bills to sort." << endl;
        return;
    }

    Bill** arr = new Bill*[count];
    current = head;
    for (int i = 0; i < count; i++) {
        arr[i] = current;
        current = current->next;
    }

    quickSortByAmount(arr, 0, count - 1);

    head = arr[0];
    for (int i = 0; i < count - 1; i++) {
        arr[i]->next = arr[i + 1];
    }
    arr[count - 1]->next = nullptr;

    delete[] arr;

    cout << "Bills sorted by amount successfully!" << endl;
    viewAllBills();
}
