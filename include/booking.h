#ifndef BOOKING_H
#define BOOKING_H

#include "./structures.h"

// User interface to book tickets
void bookTickets(Coach* trainHead, Passenger** passengerListHead);

void executeBooking(Coach* trainHead, Passenger** passengerListHead, char* coachType, int numberOfSeats, TempPassenger* group);


#endif