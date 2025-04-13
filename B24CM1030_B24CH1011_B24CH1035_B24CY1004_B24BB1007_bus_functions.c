#include "B24CM1030_B24CH1011_B24CH1035_B24CY1004_B24BB1007_bus.h"
#include <time.h>

// Global variables
Booking bookings[MAX_BOOKINGS];
int bookingCount = 0;

void initializeBuses() {
    if (!readBusDetailsFromFile()) {
        const char *destinations[] = {"Delhi", "Mumbai", "Jaipur", "Udaipur", "Jaisalmer"};
        const float fares[] = {900.0, 1425.0, 525.0, 375.0, 450.0};
        const char *busNames[] = {"Rajasthan Express", "Marwar Travels", "Desert Queen", "Thar Riders", "Royal Safari"};
        const char *driverNames[] = {"Ravi Sharma", "Amit Verma", "Suresh Yadav", "Vikas Meena", "Manoj Singh"};
        const char *departureTimes[] = {"08:00", "10:30", "14:15", "18:45", "22:00"};
        const char *arrivalTimes[] = {"20:30", "23:45", "18:00", "06:30", "08:15"};

        for (int i = 0; i < MAX_BUSES; i++) {
            buses[i].busID = i + 1;
            strncpy(buses[i].busName, busNames[i % 5], sizeof(buses[i].busName));
            strcpy(buses[i].startingPoint, "Jodhpur");
            strcpy(buses[i].destination, destinations[i % 5]);
            strncpy(buses[i].driverName, driverNames[i % 5], sizeof(buses[i].driverName));
            sprintf(buses[i].driverPhone, "98765%04d", 1000 + i);
            sprintf(buses[i].driverEmail, "driver%d@indianbuses.in", i + 1);
            strcpy(buses[i].departureTime, departureTimes[i % 5]);
            strcpy(buses[i].arrivalTime, arrivalTimes[i % 5]);
            buses[i].baseFare = fares[i % 5];
        
            // 📅 Set today's date as the travel date
            time_t now = time(NULL);
            struct tm tm_now;
            struct tm *tm_ptr = localtime(&now);
            if (tm_ptr != NULL) {
            tm_now = *tm_ptr;
}

            strftime(buses[i].travelDate, DATE_FORMAT_LEN, "%Y-%m-%d", &tm_now);
        
            for (int j = 0; j < MAX_SEATS; j++) {
                buses[i].seats[j].seatNumber = j + 1;
                buses[i].seats[j].isBooked = 0;
                strcpy(buses[i].seats[j].passengerName, "");
                strcpy(buses[i].seats[j].passengerPhone, "");
                buses[i].seats[j].farePaid = 0.0;
            }
        }
    }
}

void saveBookings() {
    FILE *file = fopen("bookings.dat", "wb");
    if (file == NULL) {
        printf("Error saving bookings!\n");
        return;
    }
    
    fwrite(&bookingCount, sizeof(int), 1, file);
    for (int i = 0; i < bookingCount; i++) {
        fwrite(&bookings[i], sizeof(Booking), 1, file);
    }
    fclose(file);
}

void loadBookings() {
    FILE *file = fopen("bookings.dat", "rb");
    if (file == NULL) {
        bookingCount = 0;
        return;
    }
    
    fread(&bookingCount, sizeof(int), 1, file);
    for (int i = 0; i < bookingCount; i++) {
        if (fread(&bookings[i], sizeof(Booking), 1, file) != 1) {
            printf("Error reading booking data!\n");
            break;
        }
        
        int busID = bookings[i].busID;
        int seatNum = bookings[i].seatNumber;
        if (busID >= 1 && busID <= MAX_BUSES && seatNum >= 1 && seatNum <= MAX_SEATS) {
            int busIndex = busID - 1;
            int seatIndex = seatNum - 1;
            
            buses[busIndex].seats[seatIndex].isBooked = 1;
            strncpy(buses[busIndex].seats[seatIndex].passengerName, 
                   bookings[i].passengerName, 
                   sizeof(buses[busIndex].seats[seatIndex].passengerName));
            strncpy(buses[busIndex].seats[seatIndex].passengerPhone, 
                   bookings[i].passengerPhone, 
                   sizeof(buses[busIndex].seats[seatIndex].passengerPhone));
            buses[busIndex].seats[seatIndex].farePaid = bookings[i].farePaid;
            
            // Update times in booking record from bus data
            strncpy(bookings[i].departureTime, buses[busIndex].departureTime, TIME_FORMAT_LEN);
            strncpy(bookings[i].arrivalTime, buses[busIndex].arrivalTime, TIME_FORMAT_LEN);
        }
    }
    fclose(file);
}

int readBusDetailsFromFile() {
    FILE *file = fopen(BUS_DETAILS_FILE, "rb");
    if (file == NULL) {
        printf("Bus details file not found. Using default buses.\n");
        return 0;
    }

    for (int i = 0; i < MAX_BUSES; i++) {
        if (fread(&buses[i], sizeof(Bus), 1, file) != 1) {
            printf("Error reading bus details. Using default buses.\n");
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

void displayBusDetails() {
    printf("\n+---------------------------------------------------------------------------------------------------------------------+\n");
    printf("|                                       BUS DETAILS                                                                     |\n");
    printf("+-----+----------------+----------------+----------------+----------------+----------+----------+-----------+-----------+\n");
    printf("| ID  | Bus Name       | From           | To             | Driver Name    | Depart   | Arrive   | Fare (Rs)| Date       |\n");
    printf("+-----+----------------+----------------+----------------+----------------+----------+----------+-----------+-----------+\n");
    
    for (int i = 0; i < MAX_BUSES; i++) {
        printf("| %-3d | %-14s | %-14s | %-14s | %-14s | %-8s | %-8s | %-8.2f | %-10s |\n",
            buses[i].busID,
            buses[i].busName,
            buses[i].startingPoint,
            buses[i].destination,
            buses[i].driverName,
            buses[i].departureTime,
            buses[i].arrivalTime,
            buses[i].baseFare,
            buses[i].travelDate);
    }
    
    printf("+-----+----------------+----------------+----------------+----------------+----------+----------+----------+\n");
}

void viewSeats(int busID) {
    if (busID < 1 || busID > MAX_BUSES) {
        printf("Invalid Bus ID!\n");
        return;
    }
    
    int busIndex = busID - 1;
    
    printf("\n+-----------------------------------------------------------------------------------------+\n");
    printf("| SEAT STATUS FOR %s (%s to %s) Dep: %s Arr: %s - Fare: Rs%.2f                 |\n", 
           buses[busIndex].busName, 
           buses[busIndex].startingPoint,
           buses[busIndex].destination,
           buses[busIndex].departureTime,
           buses[busIndex].arrivalTime,
           buses[busIndex].baseFare);
    printf("+-----------+----------------------+-------------------+------------------+-----------------+\n");
    printf("| Seat No.  | Status               | Passenger Name    | Phone           | Fare Paid       |\n");
    printf("+-----------+----------------------+-------------------+------------------+-----------------+\n");
    
    for (int i = 0; i < MAX_SEATS; i++) {
        printf("| %-9d | %-20s | %-17s | %-16s | Rs%-14.2f |\n", 
               buses[busIndex].seats[i].seatNumber,
               buses[busIndex].seats[i].isBooked ? "Booked" : "Available",
               buses[busIndex].seats[i].isBooked ? buses[busIndex].seats[i].passengerName : "-",
               buses[busIndex].seats[i].isBooked ? buses[busIndex].seats[i].passengerPhone : "-",
               buses[busIndex].seats[i].isBooked ? buses[busIndex].seats[i].farePaid : 0.0);
    }
    
    printf("+-----------+----------------------+-------------------+------------------+-----------------+\n");
}

int bookSeat(int busID, int seatNumber, const char passengerName[], const char passengerPhone[]) {
    if (busID < 1 || busID > MAX_BUSES) {
        printf("Invalid bus ID!\n");
        return 0;
    }

    if (seatNumber < 1 || seatNumber > MAX_SEATS) {
        printf("Invalid seat number! Please choose between 1-%d\n", MAX_SEATS);
        return 0;
    }

    int busIndex = busID - 1;
    int seatIndex = seatNumber - 1;

    if (buses[busIndex].seats[seatIndex].isBooked) {
        printf("Seat already booked!\n");
        return 0;
    }

    buses[busIndex].seats[seatIndex].isBooked = 1;
    strncpy(buses[busIndex].seats[seatIndex].passengerName, passengerName, 49);
    buses[busIndex].seats[seatIndex].passengerName[49] = '\0';
    strncpy(buses[busIndex].seats[seatIndex].passengerPhone, passengerPhone, 14);
    buses[busIndex].seats[seatIndex].passengerPhone[14] = '\0';
    buses[busIndex].seats[seatIndex].farePaid = buses[busIndex].baseFare;

    if (bookingCount < MAX_BOOKINGS) {
        bookings[bookingCount].busID = busID;
        bookings[bookingCount].seatNumber = seatNumber;
        strncpy(bookings[bookingCount].passengerName, passengerName, 49);
        bookings[bookingCount].passengerName[49] = '\0';
        strncpy(bookings[bookingCount].passengerPhone, passengerPhone, 14);
        bookings[bookingCount].passengerPhone[14] = '\0';
        bookings[bookingCount].farePaid = buses[busIndex].baseFare;

        // ✅ NEW: Store the travel date from the bus
        strncpy(bookings[bookingCount].travelDate, buses[busIndex].travelDate, DATE_FORMAT_LEN);

        // ✅ Already storing departure and arrival times
        strncpy(bookings[bookingCount].departureTime, buses[busIndex].departureTime, TIME_FORMAT_LEN);
        strncpy(bookings[bookingCount].arrivalTime, buses[busIndex].arrivalTime, TIME_FORMAT_LEN);

        bookingCount++;
    } else {
        printf("Warning: Maximum bookings reached!\n");
    }

    // ✅ Booking confirmation
    printf("\n+---------------------------------------------------------------+\n");
    printf("| BOOKING CONFIRMATION                                         |\n");
    printf("+---------------------------------------------------------------+\n");
    printf("| Bus: %-55s |\n", buses[busIndex].busName);
    printf("| Route: %s to %-45s |\n", buses[busIndex].startingPoint, buses[busIndex].destination);
    printf("| Date of Travel: %-44s |\n", buses[busIndex].travelDate);  // ✅ NEW line
    printf("| Departure: %-48s |\n", buses[busIndex].departureTime);
    printf("| Arrival: %-50s |\n", buses[busIndex].arrivalTime);
    printf("| Seat Number: %-47d |\n", seatNumber);
    printf("| Passenger Name: %-45s |\n", passengerName);
    printf("| Contact Phone: %-45s |\n", passengerPhone);
    printf("| Fare: Rs%-50.2f |\n", buses[busIndex].baseFare);
    printf("+---------------------------------------------------------------+\n");

    return 1;
}

void viewMyBookings() {
    printf("\n+------------------------------------------------------------------------------------------------------------------+\n");
    printf("| MY BOOKINGS                                                                                                     |\n");
    printf("+-----+----------------+----------------+----------------+------------+----------+----------+-----------+--------+\n");
    printf("| Bus | Bus Name       | From           | To             | Date       | Depart   | Arrive   | Seat No.  | Fare   |\n");
    printf("+-----+----------------+----------------+----------------+------------+----------+----------+-----------+--------+\n");
    
    int hasBookings = 0;
    
    for (int i = 0; i < bookingCount; i++) {
        if (strcmp(bookings[i].passengerName, currentUser.fullName) == 0) {
            int busIndex = bookings[i].busID - 1;
            if (busIndex >= 0 && busIndex < MAX_BUSES) {
                printf("| %-3d | %-14s | %-14s | %-14s | %-10s | %-8s | %-8s | %-9d | Rs%-6.2f |\n",
                       bookings[i].busID,
                       buses[busIndex].busName,
                       buses[busIndex].startingPoint,
                       buses[busIndex].destination,
                       bookings[i].travelDate,      
                       bookings[i].departureTime,
                       bookings[i].arrivalTime,
                       bookings[i].seatNumber,
                       bookings[i].farePaid);
                hasBookings = 1;
            }
        }
    }
    
    if (!hasBookings) {
        printf("| No bookings found for %-92s |\n", currentUser.fullName);
    }
    
    printf("+-----+----------------+----------------+----------------+------------+----------+----------+-----------+--------+\n");
}