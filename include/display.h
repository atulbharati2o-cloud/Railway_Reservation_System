#ifndef DISPLAY_H
#define DISPLAY_H

#include "structures.h"

// To display the train layout with coaches and seats
void displayTrain(Coach* head);




// To display the passenger list with their details and seat info
void displayPassengers(Passenger* head);

// To display passengers in the waitlist of a particular coach type
void displaySingleWaitlist(Passenger* waitlistHead, char* coachType);

// To display passengers in all waitlists
void displayAllWaitlist(WaitlistManager* wm);





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





// To display number of available seats in each coach
void displayNumberOfAvailableSeatsInEachCoach(Coach* head);






// To display passengers by PNR number
void displayPassengersByPNR(Passenger* head, int pnr);

#endif