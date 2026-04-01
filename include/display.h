#ifndef DISPLAY_H
#define DISPLAY_H

#include "structures.h"

// To display the train layout with coaches and seats
void displayTrain(Coach* head);

// To display the passenger list with their details and seat info
void displayPassengers(Passenger* head);

// To display the waitlist with passenger details and their waitlist number
void displayWaitlist(Passenger* waitlistHead);

// To display passengers of a particular coach by their aplhabetical order of names
void displayParticularCoachByName(Passenger* head);

#endif