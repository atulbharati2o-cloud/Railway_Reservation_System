#ifndef BOOKING_H
#define BOOKING_H

#include "tree_utils.h"

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
    int assignedCoach; // Final coach number, or -1 if the passenger stays in waitlist
    int assignedSeat;  // Final seat number, or waitlist position if not confirmed
    char assignedBerth[4]; // Berth actually assigned after allocation
} TempPassenger;

// ============= INITIALIZATION OF WAITLIST MANAGER ============= //
void initializeWaitlistManager(WaitlistManager* wm);

// ========= BOOKING ============== //
// User interface to book tickets
void bookTicketsUI(Coach* trainHead, Passenger** passengerListHead, WaitlistManager* wm);

// Booking logic
void executeBooking(Coach* trainHead, Passenger** passengerListHead, WaitlistManager* wm, const char* coachType, int numberOfSeats, TempPassenger* group);

// ============= CSV BOOKING ============== //
void processCSVBookings(Coach* trainHead, Passenger** passengerListHead, WaitlistManager* wm, const char* filename);

// ============= CANCEL BOOKING ============== //
void cancelTicketUI(Passenger** passengerListHead, WaitlistManager* wm, Coach* CoachListHead);
void cancelFullTicket(Passenger** passengerListHead, WaitlistManager* wm, Coach* CoachListHead, int pnr);
void cancelSelectedPassengers(Passenger** passengerListHead, WaitlistManager* wm, Coach* CoachListHead, int pnr, int indices[], int k);

#endif
