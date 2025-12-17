#include "appointment.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
using namespace std;
Appointment::Appointment(int id, int pid, int did, string d, string t, string s, string r): appointmentID(id), patientID(pid), doctorID(did),date(d), time(t), status(s), reason(r) {}
AppointmentNode::AppointmentNode(Appointment* appt): appointment(appt), next(nullptr) {}
AppointmentManager::DoctorQueue::DoctorQueue(int did): doctorID(did), front(nullptr), rear(nullptr) {}
AppointmentManager::AppointmentManager() : appointmentCounter(0) {
    for (int i = 0; i < MAX_DOCTORS; i++)
        doctorQueues[i] = nullptr;

    loadFromFile();
}
AppointmentManager::~AppointmentManager() {
    saveToFile();
    for (int i = 0; i < MAX_DOCTORS; i++) {
        if (doctorQueues[i] != nullptr) {
            clearQueue(i);
            delete doctorQueues[i];
        }
    }
}
void AppointmentManager::enqueue(int doctorID, Appointment* appointment) {//Queue operation
    if (doctorID < 0 || doctorID >= MAX_DOCTORS)
        return;
    if (doctorQueues[doctorID] == nullptr)
        doctorQueues[doctorID] = new DoctorQueue(doctorID);
    AppointmentNode* newNode = new AppointmentNode(appointment);

    if (doctorQueues[doctorID]->rear == nullptr) {
        doctorQueues[doctorID]->front = newNode;
        doctorQueues[doctorID]->rear = newNode;
    } else {
        doctorQueues[doctorID]->rear->next = newNode;
        doctorQueues[doctorID]->rear = newNode;
    }
}
Appointment* AppointmentManager::dequeue(int doctorID) {
    if (doctorID < 0 || doctorID >= MAX_DOCTORS || doctorQueues[doctorID] == nullptr)
        return nullptr;

    if (doctorQueues[doctorID]->front == nullptr)
        return nullptr;

    AppointmentNode* temp = doctorQueues[doctorID]->front;
    Appointment* appointment = temp->appointment;
    doctorQueues[doctorID]->front = temp->next;
    if (doctorQueues[doctorID]->front == nullptr)
        doctorQueues[doctorID]->rear = nullptr;

    delete temp;
    return appointment;
}
bool AppointmentManager::isQueueEmpty(int doctorID) {
    if (doctorID < 0 || doctorID >= MAX_DOCTORS || doctorQueues[doctorID] == nullptr)
        return true;

    return doctorQueues[doctorID]->front == nullptr;
}
void AppointmentManager::clearQueue(int doctorID) {
    while (!isQueueEmpty(doctorID)) {
        Appointment* appt = dequeue(doctorID);
        delete appt;
    }
}

// Find appointment by ID
Appointment* AppointmentManager::findAppointment(int appointmentID) {
    for (int i = 0; i < MAX_DOCTORS; i++) {
        if (doctorQueues[i] != nullptr) {
            AppointmentNode* current = doctorQueues[i]->front;
            while (current != nullptr) {
                if (current->appointment->appointmentID == appointmentID)
                    return current->appointment;
                current = current->next;
            }
        }
    }
    return nullptr;
}

// Book new appointment
void AppointmentManager::bookAppointment() {
    clearScreen();
    cout << "========== BOOK APPOINTMENT ==========" << endl << endl;

    int appointmentID = generateID("appointments.txt");
    int patientID = getValidInt("Enter Patient ID: ");
    int doctorID = getValidInt("Enter Doctor ID: ");
    string date = getValidString("Enter Date (DD/MM/YYYY): ");
    string time = getValidString("Enter Time (HH:MM): ");
    string reason = getValidString("Enter Reason for Visit: ");

    Appointment* newAppointment =
        new Appointment(appointmentID, patientID, doctorID, date, time, "Scheduled", reason);

    enqueue(doctorID, newAppointment);
    cout << endl << "Appointment booked successfully!" << endl;
    cout << "Appointment ID: " << appointmentID << endl;
    saveToFile();
    pause();
}
// Reschedule appointment
void AppointmentManager::rescheduleAppointment(int appointmentID) {
    Appointment* appointment = findAppointment(appointmentID);
    if (appointment == nullptr) {
        cout << endl << "Appointment not found!" << endl;
        pause();
        return;
    }
    clearScreen();
    cout << "========== RESCHEDULE APPOINTMENT ==========" << endl;
    viewAppointmentByID(appointmentID);

    string newDate = getValidString("Enter New Date: ");
    string newTime = getValidString("Enter New Time: ");

    appointment->date = newDate;
    appointment->time = newTime;
    appointment->status = "Rescheduled";

    cout << endl << "Appointment rescheduled successfully!" << endl;
    saveToFile();
    pause();
}

// Cancel appointment
void AppointmentManager::cancelAppointment(int appointmentID) {
    Appointment* appointment = findAppointment(appointmentID);
    if (appointment == nullptr) {
        cout << endl << "Appointment not found!" << endl;
        pause();
        return;
    }

    appointment->status = "Cancelled";
    cout << endl << "Appointment cancelled successfully!" << endl;
    saveToFile();
    pause();
}

// View appointments for a doctor
void AppointmentManager::viewAppointments(int doctorID) {
    if (doctorQueues[doctorID] == nullptr) {
        cout << endl << "No appointments found." << endl;
        return;
    }

    AppointmentNode* current = doctorQueues[doctorID]->front;
    while (current != nullptr) {
        cout << endl << "Appointment ID: " << current->appointment->appointmentID << endl;
        cout << "Patient ID: " << current->appointment->patientID << endl;
        cout << "Date: " << current->appointment->date << endl;
        cout << "Time: " << current->appointment->time << endl;
        cout << "Status: " << current->appointment->status << endl;
        cout << "Reason: " << current->appointment->reason << endl;
        current = current->next;
    }
}

// View appointment by ID
void AppointmentManager::viewAppointmentByID(int appointmentID) {
    Appointment* appointment = findAppointment(appointmentID);
    if (appointment != nullptr) {
        cout << endl << "Appointment ID: " << appointment->appointmentID << endl;
        cout << "Patient ID: " << appointment->patientID << endl;
        cout << "Doctor ID: " << appointment->doctorID << endl;
        cout << "Date: " << appointment->date << endl;
        cout << "Time: " << appointment->time << endl;
        cout << "Status: " << appointment->status << endl;
        cout << "Reason: " << appointment->reason << endl;
    } else {
        cout << endl << "Appointment not found!" << endl;
    }
}

// Load appointments from file
void AppointmentManager::loadFromFile() {
    ifstream file("appointments.txt");
    if (!file)
        return;

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        int id, pid, did;
        string date, time, status, reason;

        iss >> id >> pid >> did;
        getline(iss, date, '|');
        getline(iss, date, '|');
        getline(iss, time, '|');
        getline(iss, status, '|');
        getline(iss, reason, '|');

        Appointment* appt = new Appointment(id, pid, did, date, time, status, reason);
        enqueue(did, appt);
    }
    file.close();
}

// Save appointments to file
void AppointmentManager::saveToFile() {
    ofstream file("appointments.txt");
    for (int i = 0; i < MAX_DOCTORS; i++) {
        if (doctorQueues[i] != nullptr) {
            AppointmentNode* current = doctorQueues[i]->front;
            while (current != nullptr) {
                file << current->appointment->appointmentID << " "
                     << current->appointment->patientID << " "
                     << current->appointment->doctorID << "|"
                     << current->appointment->date << "|"
                     << current->appointment->time << "|"
                     << current->appointment->status << "|"
                     << current->appointment->reason << "|" << endl;
                current = current->next;
            }
        }
    }
    file.close();
}
