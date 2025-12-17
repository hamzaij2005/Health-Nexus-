#ifndef APPOINTMENT_H
#define APPOINTMENT_H
#include <string>
#include <fstream>
#include <queue>
struct Appointment {
    int appointmentID;
    int patientID;
    int doctorID;
    std::string date;
    std::string time;
    std::string status; // Scheduled, Completed, Cancelled
    std::string reason;
    
    Appointment(int id, int pid, int did, std::string d, std::string t, std::string s, std::string r);
};

// Appointment Queue Node
struct AppointmentNode {
    Appointment* appointment;
    AppointmentNode* next;
    
    AppointmentNode(Appointment* appt);
};

// Appointment Management Class using Queue
class AppointmentManager {
private:
    // Queue for each doctor (using linked list implementation)
    struct DoctorQueue {
        int doctorID;
        AppointmentNode* front;
        AppointmentNode* rear;
        
        DoctorQueue(int did);
    };
    
    // Map doctor ID to their queue (simple array-based approach)
    static const int MAX_DOCTORS = 1000;
    DoctorQueue* doctorQueues[MAX_DOCTORS];
    int appointmentCounter;
    
    // Helper functions
    void enqueue(int doctorID, Appointment* appointment);
    Appointment* dequeue(int doctorID);
    bool isQueueEmpty(int doctorID);
    void clearQueue(int doctorID);
    
public:
    AppointmentManager();
    ~AppointmentManager();
    
    // Main functions
    void bookAppointment();
    void rescheduleAppointment(int appointmentID);
    void cancelAppointment(int appointmentID);
    void viewAppointments(int doctorID);
    void viewAllAppointments();
    void viewAppointmentByID(int appointmentID);
    
    // File operations
    void loadFromFile();
    void saveToFile();
    
    // Helper
    Appointment* findAppointment(int appointmentID);
};

#endif



