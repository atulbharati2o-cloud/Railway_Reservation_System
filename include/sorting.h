#ifndef SORTING_H
#define SORTING_H

#include "./structures.h"

typedef enum { SORT_BY_NAME, SORT_BY_COACH } SortType;


Passenger* sortPassengersByCoachAndSeat(Passenger* head);



void mergeSort(Passenger** arr, int left, int right, SortType sortType);



#endif