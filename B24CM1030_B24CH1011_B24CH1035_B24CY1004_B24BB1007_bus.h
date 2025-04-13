#ifndef BUS_RESERVATION_H
#define BUS_RESERVATION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // For time validation functions

#define DATE_FORMAT_LEN 12  // Format: "YYYY-MM-DD"

// System Configuration Constants
#define MAX_SEATS 40
#define MAX_BUSES 5
#define MAX_USERS 100
#define MAX_BOOKINGS 200
#define TIME_FORMAT_LEN 6       // "HH:MM" format including null terminator
#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 15
#define MAX_EMAIL_LENGTH 50

// File Path Constants
#define BUS_DETAILS_FILE "bus_details.dat"
#define USER_DATABASE "users.dat"
#define BOOKINGS_FILE "bookings.dat"

// ======================
//  Data Structures
// ======================

// User Account Structure
typedef struct {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char fullName[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char email[MAX_EMAIL_LENGTH];
} User;

// Seat Information Structure
typedef struct {
    int seatNumber;
    int isBooked;
    char passengerName[MAX_NAME_LENGTH];
    char passengerPhone[MAX_PHONE_LENGTH];
    float farePaid;
} Seat;

// Complete Bus Information Structure
typedef struct {
    int busID;
    char busName[MAX_NAME_LENGTH];
    char startingPoint[MAX_NAME_LENGTH];
    char destination[MAX_NAME_LENGTH];
    char driverName[MAX_NAME_LENGTH];
    char driverPhone[MAX_PHONE_LENGTH];
    char driverEmail[MAX_EMAIL_LENGTH];
    float baseFare;
    char departureTime[TIME_FORMAT_LEN];   // Format: "HH:MM\0"
    char arrivalTime[TIME_FORMAT_LEN];     // Format: "HH:MM\0"
    char travelDate[DATE_FORMAT_LEN];      // ✅ ADDED: Travel date
    Seat seats[MAX_SEATS];
} Bus;

// Booking Record Structure
typedef struct {
    int busID;
    int seatNumber;
    char passengerName[MAX_NAME_LENGTH];
    char passengerPhone[MAX_PHONE_LENGTH];
    float farePaid;
    char departureTime[TIME_FORMAT_LEN];   // Snapshot of schedule
    char arrivalTime[TIME_FORMAT_LEN];     // Snapshot of schedule
    char travelDate[DATE_FORMAT_LEN];      // ✅ ADDED: Date booked for
} Booking;

// ======================
//  Global System State
// ======================
extern Bus buses[MAX_BUSES];
extern User users[MAX_USERS];
extern Booking bookings[MAX_BOOKINGS];
extern int userCount;
extern int bookingCount;
extern User currentUser;
extern int loggedIn;

// ======================
//  Utility Functions
// ======================
void clearScreen();
int isValidTimeFormat(const char *time);  // Validates HH:MM time format

// ======================
//  Bus Operations
// ======================
void initializeBuses();                   // Initializes bus data
int readBusDetailsFromFile();             // Loads buses from file
void displayBusDetails();                 // Shows all bus schedules
void viewSeats(int busID);                // Displays seat availability
int bookSeat(int busID, int seatNumber,   // Makes a new reservation
            const char passengerName[], 
            const char passengerPhone[]);
void viewMyBookings();                    // Shows user's reservations
void saveBookings();                      // Saves all bookings to file
void loadBookings();                      // Loads bookings from file

// ======================
//  User Operations
// ======================
void loadUsers();                         // Loads user accounts
void saveUsers();                         // Saves user accounts
int signUp();                             // New user registration
int signIn();                             // User login
void showAuthMenu();                      // Authentication interface

// ======================
//  Main Interface
// ======================
void mainMenu();                          // Primary user interface

#endif