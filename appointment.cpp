#include "appointment.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <vector>

// Appointment constructor
Appointment::Appointment(int id, int pid, int did, std::string d, std::string t, std::string s, std::string r)
    : appointmentID(id), patientID(pid), doctorID(did), date(d), time(t), status(s), reason(r) {}

// AppointmentNode constructor
AppointmentNode::AppointmentNode(Appointment* appt) : appointment(appt), next(nullptr) {}

// DoctorQueue constructor
AppointmentManager::DoctorQueue::DoctorQueue(int did) : doctorID(did), front(nullptr), rear(nullptr) {}

// AppointmentManager constructor
AppointmentManager::AppointmentManager() : appointmentCounter(0) {
    for (int i = 0; i < MAX_DOCTORS; i++) {
        doctorQueues[i] = nullptr;
    }
    loadFromFile();
}

// AppointmentManager destructor
AppointmentManager::~AppointmentManager() {
    saveToFile();
    for (int i = 0; i < MAX_DOCTORS; i++) {
        if (doctorQueues[i] != nullptr) {
            clearQueue(i);
            delete doctorQueues[i];
        }
    }
}

// Enqueue appointment
void AppointmentManager::enqueue(int doctorID, Appointment* appointment) {
    if (doctorID < 0 || doctorID >= MAX_DOCTORS) {
        return;
    }
    
    if (doctorQueues[doctorID] == nullptr) {
        doctorQueues[doctorID] = new DoctorQueue(doctorID);
    }
    
    AppointmentNode* newNode = new AppointmentNode(appointment);
    
    if (doctorQueues[doctorID]->rear == nullptr) {
        doctorQueues[doctorID]->front = doctorQueues[doctorID]->rear = newNode;
    } else {
        doctorQueues[doctorID]->rear->next = newNode;
        doctorQueues[doctorID]->rear = newNode;
    }
}

// Dequeue appointment
Appointment* AppointmentManager::dequeue(int doctorID) {
    if (doctorID < 0 || doctorID >= MAX_DOCTORS || doctorQueues[doctorID] == nullptr) {
        return nullptr;
    }
    
    if (doctorQueues[doctorID]->front == nullptr) {
        return nullptr;
    }
    
    AppointmentNode* temp = doctorQueues[doctorID]->front;
    Appointment* appointment = temp->appointment;
    doctorQueues[doctorID]->front = doctorQueues[doctorID]->front->next;
    
    if (doctorQueues[doctorID]->front == nullptr) {
        doctorQueues[doctorID]->rear = nullptr;
    }
    
    delete temp;
    return appointment;
}

// Check if queue is empty
bool AppointmentManager::isQueueEmpty(int doctorID) {
    if (doctorID < 0 || doctorID >= MAX_DOCTORS || doctorQueues[doctorID] == nullptr) {
        return true;
    }
    return doctorQueues[doctorID]->front == nullptr;
}

// Clear queue
void AppointmentManager::clearQueue(int doctorID) {
    while (!isQueueEmpty(doctorID)) {
        Appointment* appt = dequeue(doctorID);
        if (appt != nullptr) {
            delete appt;
        }
    }
}

// Find appointment by ID
Appointment* AppointmentManager::findAppointment(int appointmentID) {
    for (int i = 0; i < MAX_DOCTORS; i++) {
        if (doctorQueues[i] != nullptr) {
            AppointmentNode* current = doctorQueues[i]->front;
            while (current != nullptr) {
                if (current->appointment->appointmentID == appointmentID) {
                    return current->appointment;
                }
                current = current->next;
            }
        }
    }
    return nullptr;
}

// Book new appointment
void AppointmentManager::bookAppointment() {
    clearScreen();
    std::cout << "========== BOOK APPOINTMENT ==========\n\n";
    
    int appointmentID = generateID("appointments.txt");
    int patientID = getValidInt("Enter Patient ID: ");
    int doctorID = getValidInt("Enter Doctor ID: ");
    std::string date = getValidString("Enter Date (DD/MM/YYYY): ");
    std::string time = getValidString("Enter Time (HH:MM): ");
    std::string reason = getValidString("Enter Reason for Visit: ");
    
    Appointment* newAppointment = new Appointment(appointmentID, patientID, doctorID, date, time, "Scheduled", reason);
    enqueue(doctorID, newAppointment);
    
    std::cout << "\nAppointment booked successfully!\n";
    std::cout << "Appointment ID: " << appointmentID << "\n";
    saveToFile();
    pause();
}

// Reschedule appointment
void AppointmentManager::rescheduleAppointment(int appointmentID) {
    Appointment* appointment = findAppointment(appointmentID);
    if (appointment == nullptr) {
        std::cout << "\nAppointment with ID " << appointmentID << " not found!\n";
        pause();
        return;
    }
    
    clearScreen();
    std::cout << "========== RESCHEDULE APPOINTMENT ==========\n";
    std::cout << "Current Appointment Details:\n";
    viewAppointmentByID(appointmentID);
    
    std::string newDate = getValidString("\nEnter New Date (DD/MM/YYYY): ");
    std::string newTime = getValidString("Enter New Time (HH:MM): ");
    
    appointment->date = newDate;
    appointment->time = newTime;
    appointment->status = "Rescheduled";
    
    std::cout << "\nAppointment rescheduled successfully!\n";
    saveToFile();
    pause();
}

// Cancel appointment
void AppointmentManager::cancelAppointment(int appointmentID) {
    Appointment* appointment = findAppointment(appointmentID);
    if (appointment == nullptr) {
        std::cout << "\nAppointment with ID " << appointmentID << " not found!\n";
        pause();
        return;
    }
    
    appointment->status = "Cancelled";
    std::cout << "\nAppointment cancelled successfully!\n";
    saveToFile();
    pause();
}

// View appointments for a doctor
void AppointmentManager::viewAppointments(int doctorID) {
    if (doctorID < 0 || doctorID >= MAX_DOCTORS || doctorQueues[doctorID] == nullptr) {
        std::cout << "\nNo appointments found for Doctor ID " << doctorID << "\n";
        return;
    }
    
    std::cout << "\n========== APPOINTMENTS FOR DOCTOR " << doctorID << " ==========\n";
    
    AppointmentNode* current = doctorQueues[doctorID]->front;
    int count = 0;
    
    while (current != nullptr) {
        if (current->appointment->status != "Cancelled") {
            count++;
            std::cout << "\n----------------------------------------\n";
            std::cout << "Appointment ID: " << current->appointment->appointmentID << "\n";
            std::cout << "Patient ID: " << current->appointment->patientID << "\n";
            std::cout << "Date: " << current->appointment->date << "\n";
            std::cout << "Time: " << current->appointment->time << "\n";
            std::cout << "Status: " << current->appointment->status << "\n";
            std::cout << "Reason: " << current->appointment->reason << "\n";
        }
        current = current->next;
    }
    
    if (count == 0) {
        std::cout << "No active appointments.\n";
    }
    std::cout << "\n==========================================\n";
}

// View all appointments
void AppointmentManager::viewAllAppointments() {
    clearScreen();
    std::cout << "========== ALL APPOINTMENTS ==========\n";
    
    bool found = false;
    for (int i = 0; i < MAX_DOCTORS; i++) {
        if (doctorQueues[i] != nullptr && !isQueueEmpty(i)) {
            found = true;
            viewAppointments(i);
        }
    }
    
    if (!found) {
        std::cout << "No appointments found.\n";
    }
    std::cout << "\n=====================================\n";
    pause();
}

// View appointment by ID
void AppointmentManager::viewAppointmentByID(int appointmentID) {
    Appointment* appointment = findAppointment(appointmentID);
    if (appointment != nullptr) {
        std::cout << "\n========== APPOINTMENT DETAILS ==========\n";
        std::cout << "Appointment ID: " << appointment->appointmentID << "\n";
        std::cout << "Patient ID: " << appointment->patientID << "\n";
        std::cout << "Doctor ID: " << appointment->doctorID << "\n";
        std::cout << "Date: " << appointment->date << "\n";
        std::cout << "Time: " << appointment->time << "\n";
        std::cout << "Status: " << appointment->status << "\n";
        std::cout << "Reason: " << appointment->reason << "\n";
        std::cout << "========================================\n";
    } else {
        std::cout << "\nAppointment with ID " << appointmentID << " not found!\n";
    }
}

// Load appointments from file
void AppointmentManager::loadFromFile() {
    std::ifstream file("appointments.txt");
    if (!file.is_open()) {
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        int id, patientID, doctorID;
        std::string date, time, status, reason;
        
        if (iss >> id >> patientID >> doctorID) {
            std::getline(iss, date, '|');
            std::getline(iss, date, '|');
            std::getline(iss, time, '|');
            std::getline(iss, status, '|');
            std::getline(iss, reason, '|');
            
            if (!date.empty() && date[0] == ' ') date = date.substr(1);
            if (status.empty()) status = "Scheduled";
            
            Appointment* newAppointment = new Appointment(id, patientID, doctorID, date, time, status, reason);
            enqueue(doctorID, newAppointment);
            
            if (id > appointmentCounter) {
                appointmentCounter = id;
            }
        }
    }
    file.close();
}

// Save appointments to file
void AppointmentManager::saveToFile() {
    std::ofstream file("appointments.txt");
    if (file.is_open()) {
        for (int i = 0; i < MAX_DOCTORS; i++) {
            if (doctorQueues[i] != nullptr) {
                AppointmentNode* current = doctorQueues[i]->front;
                while (current != nullptr) {
                    file << current->appointment->appointmentID << " " 
                         << current->appointment->patientID << " " 
                         << current->appointment->doctorID << " |" 
                         << current->appointment->date << "|" 
                         << current->appointment->time << "|" 
                         << current->appointment->status << "|" 
                         << current->appointment->reason << "|\n";
                    current = current->next;
                }
            }
        }
        file.close();
    }
}



