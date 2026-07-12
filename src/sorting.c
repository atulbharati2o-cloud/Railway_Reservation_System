#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sorting.h"

static int comparePassengers(Passenger* left, Passenger* right, SortType sortType) {
    if(sortType == SORT_BY_NAME) {
        return strcmp(left->name, right->name);
    }

    if(sortType == SORT_BY_COACH) {
        return left->coachNumber - right->coachNumber;
    }

    if(sortType == SORT_BY_COACH_AND_SEAT) {
        if(left->coachNumber != right->coachNumber) {
            return left->coachNumber - right->coachNumber;
        }
        return left->seatNumber - right->seatNumber;
    }

    return 0;
}

static void mergeSortedArrays(Passenger** arr, int left, int mid, int right, SortType sortType) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Passenger** leftArr = (Passenger**)malloc(sizeof(Passenger*) * n1);
    Passenger** rightArr = (Passenger**)malloc(sizeof(Passenger*) * n2);
    if(leftArr == NULL || rightArr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for(int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for(int i = 0; i < n2; i++) {
        rightArr[i] = arr[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while(i < n1 && j < n2) {
        if(comparePassengers(leftArr[i], rightArr[j], sortType) <= 0) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while(i < n1) {
        arr[k++] = leftArr[i++];
    }
    while(j < n2) {
        arr[k++] = rightArr[j++];
    }

    free(leftArr);
    free(rightArr);
}

void mergeSort(Passenger** arr, int left, int right, SortType sortType) {
    if(left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, sortType);
    mergeSort(arr, mid + 1, right, sortType);
    mergeSortedArrays(arr, left, mid, right, sortType);
}
