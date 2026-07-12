#ifndef SORTING_H
#define SORTING_H

#include "tree_utils.h"

typedef enum {
    SORT_BY_NAME,
    SORT_BY_COACH,
    SORT_BY_COACH_AND_SEAT
} SortType;

// Merge sort for passenger views
void mergeSort(Passenger** arr, int left, int right, SortType sortType);

#endif
