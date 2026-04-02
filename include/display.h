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
void displayPassengersOfCoachSortedByName(Passenger* head);





// To display all passengers sorted by their names
void displayAllPassengersSortedByName(Passenger* head);

// To display all passengers sorted by their coach numbers
void displayAllPassengersSortedByCoachNumber(Passenger* head);

// To display all passengers who were booked L or SL 
void displayAllPassengersIn_L_or_SL_Berths(Passenger* head);

// To display all passengers above 60 years of age who didn't get L or SL berth
void displaySeniorCitizensWithoutL_or_SL_Berths(Passenger* head);

#endif