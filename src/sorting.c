#include <stdio.h>
#include <stdlib.h>
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

Passenger* merge_sorted_passengers(Passenger* p1, Passenger* p2){
    if(p1 == NULL) return p2;
    if(p2 == NULL) return p1;

    Passenger* result = NULL;

    if(p1->coachNumber < p2->coachNumber || (p1->coachNumber == p2->coachNumber && p1->seatNumber <= p2->seatNumber)){
        result = p1;
        result->nextPassenger = merge_sorted_passengers(p1->nextPassenger, p2);
    } else {
        result = p2;
        result->nextPassenger = merge_sorted_passengers(p1, p2->nextPassenger);
    }

    return result;
}

void sortPassengersByCoachAndSeat(Passenger** headRef) {
    Passenger* head = *headRef;
    if(!head || !head->nextPassenger) return;

    // Split the linked list into halves
    Passenger* middle = get_middle(head);
    Passenger* left = head;
    Passenger* right = middle->nextPassenger;
    middle->nextPassenger = NULL;

    // Recursively sort left and right halves
    sortPassengersByCoachAndSeat(&left);
    sortPassengersByCoachAndSeat(&right);

    // Merge the sorted halves
    *headRef = merge_sorted_passengers(left, right);
}


