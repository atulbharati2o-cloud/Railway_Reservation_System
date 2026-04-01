#ifndef SORTING_H
#define SORTING_H

#include "./structures.h"

Passenger* get_middle(Passenger* head);

Passenger* merge_sorted_passengers(Passenger* p1, Passenger* p2);
Passenger* sortPassengersByCoachAndSeat(Passenger* head);


Passenger* merge_name_sorted_passengers(Passenger* p1, Passenger* p2);
Passenger* sortPassengersByName(Passenger* head);

#endif