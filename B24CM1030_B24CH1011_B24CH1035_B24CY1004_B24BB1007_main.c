#include "B24CM1030_B24CH1011_B24CH1035_B24CY1004_B24BB1007_bus.h"

// Initialize global variables
Bus buses[MAX_BUSES];
User users[MAX_USERS];
int userCount = 0;
User currentUser;
int loggedIn = 0;

void displayBookingDetails(int bookingIndex) {
    if (bookingIndex < 0 || bookingIndex >= bookingCount) return;
    
    int busIndex = bookings[bookingIndex].busID - 1;
    if (busIndex < 0 || busIndex >= MAX_BUSES) return;
    
    printf("\n+---------------------------------------------------------------+\n");
    printf("| BOOKING DETAILS                                             |\n");
    printf("+---------------------------------------------------------------+\n");
    printf("| Bus: %-55s |\n", buses[busIndex].busName);
    printf("| Route: %s to %-45s |\n", buses[busIndex].startingPoint, buses[busIndex].destination);
    printf("| Departure: %-48s |\n", bookings[bookingIndex].departureTime);
    printf("| Arrival: %-50s |\n", bookings[bookingIndex].arrivalTime);
    printf("| Seat Number: %-47d |\n", bookings[bookingIndex].seatNumber);
    printf("| Passenger Name: %-45s |\n", bookings[bookingIndex].passengerName);
    printf("| Contact Phone: %-45s |\n", bookings[bookingIndex].passengerPhone);
    printf("| Fare: Rs%-50.2f |\n", bookings[bookingIndex].farePaid);
    printf("+---------------------------------------------------------------+\n");
}

void mainMenu() {
    int choice, busID, seatNumber;
    
    loadBookings();

    while (1) {
        clearScreen();
        printf("\n+===================================================+\n");
        printf("|        BUS RESERVATION SYSTEM                   |\n");
        printf("| Logged in as: %-30s |\n", currentUser.fullName);
        printf("+===================================================+\n");
        printf("| 1. View All Buses & Schedules                   |\n");
        printf("| 2. View Seats for a Bus                         |\n");
        printf("| 3. Book a Seat                                  |\n");
        printf("| 4. View My Bookings                             |\n");
        printf("| 5. Logout                                       |\n");
        printf("+===================================================+\n");
        
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }
        
        while(getchar() != '\n');

        switch (choice) {
            case 1:
                clearScreen();
                displayBusDetails();
                printf("\nPress Enter to return to menu...");
                getchar();
                break;
                
            case 2:
                clearScreen();
                printf("Enter Bus ID (1-%d): ", MAX_BUSES);
                if (scanf("%d", &busID) != 1) {
                    printf("Invalid input!\n");
                    while(getchar() != '\n');
                    break;
                }
                while(getchar() != '\n');
                clearScreen();
                viewSeats(busID);
                printf("\nPress Enter to return to menu...");
                getchar();
                break;
                
            case 3:
                clearScreen();
                printf("Available Buses:\n");
                for (int i = 0; i < MAX_BUSES; i++) {
                    printf("%d. %s to %s (%s - %s)\n", 
                           buses[i].busID, buses[i].startingPoint, 
                           buses[i].destination, buses[i].departureTime,
                           buses[i].arrivalTime);
                }
                
                printf("\nEnter Bus ID (1-%d): ", MAX_BUSES);
                if (scanf("%d", &busID) != 1) {
                    printf("Invalid input!\n");
                    while(getchar() != '\n');
                    break;
                }
                
                printf("Enter Seat Number (1-%d): ", MAX_SEATS);
                if (scanf("%d", &seatNumber) != 1) {
                    printf("Invalid input!\n");
                    while(getchar() != '\n');
                    break;
                }
                while(getchar() != '\n');
                
                clearScreen();
                if (bookSeat(busID, seatNumber, currentUser.fullName, currentUser.phone)) {
                    saveBookings();
                }
                printf("\nPress Enter to return to menu...");
                getchar();
                break;
                
            case 4:
                clearScreen();
                printf("\n+-----------------------------------------------------------------------------------------+\n");
                printf("| MY BOOKINGS                                                                           |\n");
                printf("+-----+----------------+----------------+----------------+----------+----------+----------+\n");
                printf("| Bus | Bus Name       | From           | To             | Depart   | Arrive   | Seat No. |\n");
                printf("+-----+----------------+----------------+----------------+----------+----------+----------+\n");
                
                int hasBookings = 0;
                for (int i = 0; i < bookingCount; i++) {
                    if (strcmp(bookings[i].passengerName, currentUser.fullName) == 0) {
                        int busIdx = bookings[i].busID - 1;
                        if (busIdx >= 0 && busIdx < MAX_BUSES) {
                            printf("| %-3d | %-14s | %-14s | %-14s | %-8s | %-8s | %-8d |\n",
                                   bookings[i].busID,
                                   buses[busIdx].busName,
                                   buses[busIdx].startingPoint,
                                   buses[busIdx].destination,
                                   bookings[i].departureTime,
                                   bookings[i].arrivalTime,
                                   bookings[i].seatNumber);
                            hasBookings = 1;
                        }
                    }
                }
                
                if (!hasBookings) {
                    printf("| No bookings found for %-56s |\n", currentUser.fullName);
                }
                
                printf("+-----+----------------+----------------+----------------+----------+----------+----------+\n");
                
                // Option to view booking details
                if (hasBookings) {
                    printf("\nEnter booking number to view details (0 to return): ");
                    int viewChoice;
                    if (scanf("%d", &viewChoice) == 1 && viewChoice > 0) {
                        for (int i = 0; i < bookingCount; i++) {
                            if (strcmp(bookings[i].passengerName, currentUser.fullName) == 0 && 
                                bookings[i].busID == viewChoice) {
                                clearScreen();
                                displayBookingDetails(i);
                                break;
                            }
                        }
                    }
                    while(getchar() != '\n'); // Clear input buffer
                }
                
                printf("\nPress Enter to return to menu...");
                getchar();
                break;
                
            case 5:
                loggedIn = 0;
                printf("\nLogging out...\n");
                saveBookings();
                printf("Press Enter to continue...");
                while(getchar() != '\n');
                return;
                
            default:
                printf("\nInvalid choice! Press Enter to try again...");
                getchar();
        }
    }
}

int main() {
    loadUsers();
    initializeBuses();
    
    while (1) {
        if (!loggedIn) {
            showAuthMenu();
        }
        mainMenu();
    }
    
    return 0;
}