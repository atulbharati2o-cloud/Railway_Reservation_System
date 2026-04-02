#ifndef SORTING_H
#define SORTING_H

#include "./structures.h"

typedef enum { SORT_BY_NAME, SORT_BY_COACH } SortType;

Passenger* get_middle(Passenger* head);
Passenger* merge_sorted_passengers(Passenger* p1, Passenger* p2);

Passenger* sortPassengersByCoachAndSeat(Passenger* head);



void mergeSortedArrays(Passenger** arr, int left, int mid, int right, SortType sortType);
void mergeSort(Passenger** arr, int left, int right, SortType sortType);



#endif