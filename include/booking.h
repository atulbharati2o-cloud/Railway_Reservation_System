#ifndef BOOKING_H
#define BOOKING_H

#include "./structures.h"

// Temporary passenger details for booking
typedef struct TempPassenger {
    char name[50];
    char gender[10];
    char DOB[11]; 
    int age;
    char berthPreference[4]; 
    Bool gotSeat;
} TempPassenger;


// User interface to book tickets
void bookTickets(Coach* trainHead, Passenger** passengerListHead);

// Booking logic
void executeBooking(Coach* trainHead, Passenger** passengerListHead, char* coachType, int numberOfSeats, TempPassenger* group);

// Helper functions
int getBerthIndex(char* berthType);
Bool canCoachFitGroup(Coach* coach, TempPassenger* group, int numberOfSeats);
int bookSeatsForGroup(Coach* coach, Passenger** passengerListHead, TempPassenger* group, int numberOfSeats, Bool ignorePreference);
void addToWaitlist(Passenger** waitlistHead, TempPassenger person, char* coachType);


#endif