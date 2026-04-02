#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structures.h"
#include "../include/sorting.h"



Passenger* get_middle(Passenger* head){
    Passenger* slow = head;
    Passenger* fast = head->nextPassenger;

    while(fast && fast->nextPassenger){
        slow = slow->nextPassenger;
        fast = fast->nextPassenger->nextPassenger;
    }
    return slow;
}





Passenger* merge_coachNseat_sorted_passengers(Passenger* p1, Passenger* p2){
    if(p1 == NULL) return p2;
    if(p2 == NULL) return p1;

    Passenger* result = NULL;

    if(p1->coachNumber < p2->coachNumber || (p1->coachNumber == p2->coachNumber && p1->seatNumber <= p2->seatNumber)){
        result = p1;
        result->nextPassenger = merge_coachNseat_sorted_passengers(p1->nextPassenger, p2);
    } else {
        result = p2;
        result->nextPassenger = merge_coachNseat_sorted_passengers(p1, p2->nextPassenger);
    }

    return result;
}

Passenger* sortPassengersByCoachAndSeat(Passenger* head) {
    if(!head || !head->nextPassenger) return head;

    // Split the linked list into halves
    Passenger* middle = get_middle(head);
    Passenger* left = head;
    Passenger* right = middle->nextPassenger;
    middle->nextPassenger = NULL;

    // Recursively sort left and right halves
    left = sortPassengersByCoachAndSeat(left);
    right = sortPassengersByCoachAndSeat(right);

    // Merge the sorted halves
    return merge_coachNseat_sorted_passengers(left, right);
}



void mergeSortedArrays(Passenger** arr, int left, int mid, int right, SortType sortType){
    
    // Get the left array
    int n1 = mid - left + 1;
    Passenger** L = (Passenger**)malloc(n1 * sizeof(Passenger*));
    for(int i = 0; i < n1; i++) L[i] = arr[left  + i];
    
    // Get the right array
    int n2 = right - mid;
    Passenger** R = (Passenger**)malloc(n2 * sizeof(Passenger*));
    for(int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];
    
    int i = 0, j = 0, k = left;
    while(i < n1 && j < n2){
        if(sortType == SORT_BY_NAME){ // Compare by name
            if(strcmp(L[i]->name, R[j]->name) <= 0){
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        } else { // Compare by coach number
            if(L[i]->coachNumber <= R[j]->coachNumber){
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        }
    }

    while(i < n1) arr[k++] = L[i++];
    while(j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(Passenger** arr, int left, int right, SortType sortType){
    if(left < right){
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid, sortType);
        mergeSort(arr, mid + 1, right, sortType);

        mergeSortedArrays(arr, left, mid, right, sortType);
    }
}