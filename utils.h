#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

// Utility functions for the Hospital Management System

// Function to validate integer input
int getValidInt(const std::string& prompt);

// Function to validate string input (non-empty)
std::string getValidString(const std::string& prompt);

// Function to generate unique ID
int generateID(const std::string& filename);

// Function to get current date as string
std::string getCurrentDate();

// Function to clear screen (cross-platform)
void clearScreen();

// Function to pause execution
void pause();

// Function to format currency
std::string formatCurrency(double amount);

#endif



