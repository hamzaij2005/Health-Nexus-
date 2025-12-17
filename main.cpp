#include <iostream>
#include "utils.h"
#include "patient.h"
#include "doctor.h"
#include "appointment.h"
#include "bed.h"
#include "pharmacy.h"
#include "billing.h"
#include "hospitalGraph.h"

using namespace std;

PatientManager patientMgr;
DoctorManager doctorMgr;
AppointmentManager appointmentMgr;
BedManager bedMgr;
PharmacyManager pharmacyMgr;
BillingManager billingMgr;
HospitalGraph hospitalGraph;

void showMainMenu();
void showPatientMenu();
void showDoctorMenu();
void showAppointmentMenu();
void showBedMenu();
void showPharmacyMenu();
void showBillingMenu();
void showGraphMenu();
void showAdminMenu();

int main()
{
    clearScreen();
    cout << "========================================" << endl;
    cout << "  HOSPITAL MANAGEMENT SYSTEM (HMS)" << endl;
    cout << "  DSA-Based Modular System" << endl;
    cout << "========================================" << endl
         << endl;
    cout << "System loaded successfully!" << endl;
    pause();

    int choice;
    do
    {
        showMainMenu();
        choice = getValidInt("Enter your choice: ");
        switch (choice)
        {
        case 1:
            showPatientMenu();
            break;
        case 2:
            showDoctorMenu();
            break;
        case 3:
            showAppointmentMenu();
            break;
        case 4:
            showBedMenu();
            break;
        case 5:
            showPharmacyMenu();
            break;
        case 6:
            showBillingMenu();
            break;
        case 7:
            showGraphMenu();
            break;
        case 8:
            showAdminMenu();
            break;
        case 9:
            clearScreen();
            cout << "Thank you for using HMS!" << endl;
            cout << "All data has been saved." << endl;
            cout << "Goodbye!" << endl
                 << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            pause();
        }
    } while (choice != 9);
    return 0;
}

void showMainMenu()
{
    clearScreen();
    cout << "========================================" << endl;
    cout << "         MAIN MENU" << endl;
    cout << "========================================" << endl
         << endl;
    cout << "1. Patient Management" << endl;
    cout << "2. Doctor Management" << endl;
    cout << "3. Appointment Management" << endl;
    cout << "4. Bed & Ward Management" << endl;
    cout << "5. Pharmacy Management" << endl;
    cout << "6. Billing Management" << endl;
    cout << "7. Hospital Graph (Departments)" << endl;
    cout << "8. Admin Panel" << endl;
    cout << "9. Exit" << endl;
    cout << "========================================" << endl;
}

void showPatientMenu()
{
    int choice;
    do
    {
        clearScreen();
        cout << "========================================" << endl;
        cout << "      PATIENT MANAGEMENT" << endl;
        cout << "========================================" << endl
             << endl;
        cout << "1. Register New Patient" << endl;
        cout << "2. View Patient Details" << endl;
        cout << "3. View All Patients" << endl;
        cout << "4. Update Patient Information" << endl;
        cout << "5. Delete Patient" << endl;
        cout << "6. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        choice = getValidInt("Enter your choice: ");
        switch (choice)
        {
        case 1:
            patientMgr.registerPatient();
            break;
        case 2:
        {
            int id = getValidInt("Enter Patient ID: ");
            patientMgr.viewPatient(id);
            pause();
            break;
        }
        case 3:
            patientMgr.viewAllPatients();
            break;
        case 4:
        {
            int id = getValidInt("Enter Patient ID: ");
            patientMgr.updatePatient(id);
            break;
        }
        case 5:
        {
            int id = getValidInt("Enter Patient ID: ");
            patientMgr.deletePatient(id);
            break;
        }
        }
    } while (choice != 6);
}

void showDoctorMenu()
{
    int choice;
    do
    {
        clearScreen();
        cout << "========================================" << endl;
        cout << "      DOCTOR MANAGEMENT" << endl;
        cout << "========================================" << endl
             << endl;
        cout << "1. Register New Doctor" << endl;
        cout << "2. View Doctor Details" << endl;
        cout << "3. View All Doctors" << endl;
        cout << "4. Update Doctor Information" << endl;
        cout << "5. Assign Patient to Doctor" << endl;
        cout << "6. View Assigned Patients" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        choice = getValidInt("Enter your choice: ");
        switch (choice)
        {
        case 1:
            doctorMgr.registerDoctor();
            break;
        case 2:
        {
            int id = getValidInt("Enter Doctor ID: ");
            doctorMgr.viewDoctor(id);
            pause();
            break;
        }
        case 3:
            doctorMgr.viewAllDoctors();
            break;
        case 4:
        {
            int id = getValidInt("Enter Doctor ID: ");
            doctorMgr.updateInfo(id);
            break;
        }
        case 5:
        {
            int doctorID = getValidInt("Enter Doctor ID: ");
            int patientID = getValidInt("Enter Patient ID: ");
            doctorMgr.assignPatient(doctorID, patientID);
            pause();
            break;
        }
        case 6:
        {
            int id = getValidInt("Enter Doctor ID: ");
            doctorMgr.viewAssignedPatients(id);
            pause();
            break;
        }
        }
    } while (choice != 7);
}

void showAppointmentMenu()
{
    int choice;
    do
    {
        clearScreen();
        cout << "========================================" << endl;
        cout << "    APPOINTMENT MANAGEMENT" << endl;
        cout << "========================================" << endl
             << endl;
        cout << "1. Book Appointment" << endl;
        cout << "2. View Appointment" << endl;
        cout << "3. View All Appointments" << endl;
        cout << "4. View Appointments by Doctor" << endl;
        cout << "5. Reschedule Appointment" << endl;
        cout << "6. Cancel Appointment" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        choice = getValidInt("Enter your choice: ");
        switch (choice)
        {
        case 1:
            appointmentMgr.bookAppointment();
            break;
        case 2:
        {
            int id = getValidInt("Enter Appointment ID: ");
            appointmentMgr.viewAppointmentByID(id);
            pause();
            break;
        }
        case 3:
            appointmentMgr.viewAllAppointments();
            break;
        case 4:
        {
            int id = getValidInt("Enter Doctor ID: ");
            appointmentMgr.viewAppointments(id);
            pause();
            break;
        }
        case 5:
        {
            int id = getValidInt("Enter Appointment ID: ");
            appointmentMgr.rescheduleAppointment(id);
            break;
        }
        case 6:
        {
            int id = getValidInt("Enter Appointment ID: ");
            appointmentMgr.cancelAppointment(id);
            break;
        }
        }
    } while (choice != 7);
}

void showBedMenu()
{
    int choice;
    do
    {
        clearScreen();
        cout << "========================================" << endl;
        cout << "      BED & WARD MANAGEMENT" << endl;
        cout << "========================================" << endl
             << endl;
        cout << "1. Add New Bed" << endl;
        cout << "2. Allocate Bed to Patient" << endl;
        cout << "3. Free Bed" << endl;
        cout << "4. View All Beds" << endl;
        cout << "5. View Available Beds" << endl;
        cout << "6. View Occupied Beds" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        choice = getValidInt("Enter your choice: ");
        switch (choice)
        {
        case 1:
            bedMgr.addBed();
            break;
        case 2:
            bedMgr.allocateBed();
            break;
        case 3:
        {
            int id = getValidInt("Enter Bed ID: ");
            bedMgr.freeBed(id);
            break;
        }
        case 4:
            bedMgr.viewBeds();
            break;
        case 5:
            clearScreen();
            bedMgr.viewAvailableBeds();
            pause();
            break;
        case 6:
            bedMgr.viewOccupiedBeds();
            break;
        }
    } while (choice != 7);
}

void showPharmacyMenu()
{
    int choice;
    do
    {
        clearScreen();
        cout << "========================================" << endl;
        cout << "      PHARMACY MANAGEMENT" << endl;
        cout << "========================================" << endl
             << endl;
        cout << "1. Add Medicine" << endl;
        cout << "2. View Medicine" << endl;
        cout << "3. View All Inventory" << endl;
        cout << "4. Update Stock" << endl;
        cout << "5. Remove Medicine" << endl;
        cout << "6. Search Medicine by Name" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        choice = getValidInt("Enter your choice: ");
        switch (choice)
        {
        case 1:
            pharmacyMgr.addMedicine();
            break;
        case 2:
        {
            int id = getValidInt("Enter Medicine ID: ");
            pharmacyMgr.viewMedicine(id);
            pause();
            break;
        }
        case 3:
            pharmacyMgr.viewInventory();
            break;
        case 4:
        {
            int id = getValidInt("Enter Medicine ID: ");
            int qty = getValidInt("Enter quantity to add (negative to subtract): ");
            pharmacyMgr.updateStock(id, qty);
            pause();
            break;
        }
        case 5:
        {
            int id = getValidInt("Enter Medicine ID: ");
            pharmacyMgr.removeMedicine(id);
            break;
        }
        case 6:
        {
            string name = getValidString("Enter Medicine Name: ");
            pharmacyMgr.searchMedicineByName(name);
            pause();
            break;
        }
        }
    } while (choice != 7);
}

void showBillingMenu()
{
    int choice;
    do
    {
        clearScreen();
        cout << "========================================" << endl;
        cout << "      BILLING MANAGEMENT" << endl;
        cout << "========================================" << endl
             << endl;
        cout << "1. Generate Bill" << endl;
        cout << "2. View Bill" << endl;
        cout << "3. View All Bills" << endl;
        cout << "4. Update Bill" << endl;
        cout << "5. Mark Bill as Paid" << endl;
        cout << "6. Sort Bills by Amount" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        choice = getValidInt("Enter your choice: ");
        switch (choice)
        {
        case 1:
            billingMgr.generateBill();
            break;
        case 2:
        {
            int id = getValidInt("Enter Bill ID: ");
            billingMgr.viewBill(id);
            break;
        }
        case 3:
            billingMgr.viewAllBills();
            break;
        case 4:
        {
            int id = getValidInt("Enter Bill ID: ");
            billingMgr.updateBill(id);
            break;
        }
        case 5:
        {
            int id = getValidInt("Enter Bill ID: ");
            billingMgr.markBillPaid(id);
            break;
        }
        case 6:
            billingMgr.sortBillsByAmount();
            break;
        }
    } while (choice != 7);
}

void showGraphMenu()
{
    int choice;
    do
    {
        clearScreen();
        cout << "========================================" << endl;
        cout << "    HOSPITAL GRAPH (DEPARTMENTS)" << endl;
        cout << "========================================" << endl
             << endl;
        cout << "1. View Graph" << endl;
        cout << "2. Add Node (Department/Location)" << endl;
        cout << "3. Add Edge (Connection)" << endl;
        cout << "4. View Node Details" << endl;
        cout << "5. View Connections" << endl;
        cout << "6. Find Shortest Path" << endl;
        cout << "7. View Department Connections" << endl;
        cout << "8. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        choice = getValidInt("Enter your choice: ");
        switch (choice)
        {
        case 1:
            hospitalGraph.viewGraph();
            break;
        case 2:
        {
            string name = getValidString("Enter Node Name: ");
            string type = getValidString("Enter Type (Department/Ward/etc): ");
            hospitalGraph.addNode(name, type);
            pause();
            break;
        }
        case 3:
        {
            int fromID = getValidInt("Enter From Node ID: ");
            int toID = getValidInt("Enter To Node ID: ");
            int distance = getValidInt("Enter Distance: ");
            hospitalGraph.addEdge(fromID, toID, distance);
            pause();
            break;
        }
        case 4:
        {
            int id = getValidInt("Enter Node ID: ");
            hospitalGraph.viewNode(id);
            pause();
            break;
        }
        case 5:
        {
            int id = getValidInt("Enter Node ID: ");
            hospitalGraph.viewConnections(id);
            pause();
            break;
        }
        case 6:
        {
            int fromID = getValidInt("Enter From Node ID: ");
            int toID = getValidInt("Enter To Node ID: ");
            hospitalGraph.shortestPath(fromID, toID);
            pause();
            break;
        }
        case 7:
            hospitalGraph.departmentConnections();
            break;
        }
    } while (choice != 8);
}

void showAdminMenu()
{
    int choice;
    do
    {
        clearScreen();
        cout << "========================================" << endl;
        cout << "         ADMIN PANEL" << endl;
        cout << "========================================" << endl
             << endl;
        cout << "1. View System Statistics" << endl;
        cout << "2. View All Data Summary" << endl;
        cout << "3. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        choice = getValidInt("Enter your choice: ");
        switch (choice)
        {
        case 1:
        {
            clearScreen();
            cout << "========== SYSTEM STATISTICS ==========" << endl
                 << endl;
            cout << "Note: Detailed statistics can be viewed" << endl;
            cout << "in individual modules." << endl;
            cout << "=====================================" << endl;
            pause();
            break;
        }
        case 2:
        {
            clearScreen();
            cout << "========== DATA SUMMARY ==========" << endl
                 << endl;
            cout << "Use individual modules to view detailed data." << endl;
            cout << "=====================================" << endl;
            pause();
            break;
        }
        }
    } while (choice != 3);
}
