#ifndef BOOKING_H
#define BOOKING_H

#include "./structures.h"

// Temporary passenger details for booking
typedef struct TempPassenger {

    // Details needed for booking
    char name[50];
    char gender[10];
    char DOB[11]; 
    int age;
    char berthPreference[4]; 

    // Details assigned during booking process
    Bool gotSeat;
    int assignedCoach; 
    int assignedSeat;
    char assignedBerth[4];
} TempPassenger;


// User interface to book tickets
void bookTickets(Coach* trainHead, Passenger** passengerListHead, Passenger** waitlistHead);

// Booking logic
void executeBooking(Coach* trainHead, Passenger** passengerListHead, Passenger** waitlistHead, char* coachType, int numberOfSeats, TempPassenger* group);

// Helper functions
int getBerthIndex(char* berthType);
Bool canCoachFitGroup(Coach* coach, TempPassenger* group, int numberOfSeats);
int bookSeatsForGroup(Coach* coach, Passenger** passengerListHead, TempPassenger* group, int numberOfSeats, Bool ignorePreference);
int addToWaitlist(Passenger** waitlistHead, TempPassenger person, char* coachType);

// Function for testing 
void processCSVBookings(Coach* trainHead, Passenger** passengerListHead, Passenger** waitlistHead, const char* filename);

#endif