#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structures.h"


// Creating seats List inside coach
Seat* createSeatList(int coachNumber, int capacity) {
    const char* berthTypes[] = {"L", "M", "U", "L", "M", "U", "SL", "SU"};

    Seat* head = NULL;
    Seat* current = NULL;

    for(int i = 0; i < capacity; i++){
        Seat* newSeat = (Seat*)malloc(sizeof(Seat));
        if(newSeat == NULL){
            printf("Memory allocation failed⚠️\n");
            exit(1);
        }
        
        newSeat->seatNumber = i + 1;
        strcpy(newSeat->berthType, berthTypes[i % 8]);
        newSeat->coachNumber = coachNumber;
        newSeat->isBooked = FALSE;
        newSeat->nextSeat = NULL;

        if(head == NULL){
            head = newSeat;
            current = head;
        }else{
            current->nextSeat = newSeat;
            current = newSeat;
        }
    }

    return head;
}

// Creating a coach 
Coach* createCoach(int coachNumber, const char* coachType, int capacity){
    Coach* newCoach = (Coach*)malloc(sizeof(Coach));

    if(newCoach == NULL){
        printf("Memory allocation failed⚠️\n");
        exit(1);
    }

    newCoach->coachNumber = coachNumber;
    strcpy(newCoach->coachType, coachType);
    newCoach->capacity = capacity;
    newCoach->nextCoach = NULL;
    
    // For engine n pantry no seats and for other coaches create seat list inside coach
    if(strcmp(coachType, "Engine") == 0 || strcmp(coachType, "Pantry") == 0){
        newCoach->seatList = NULL; 
    } else {
        newCoach->seatList = createSeatList(coachNumber, capacity);
    }
    return newCoach;
}


// Initializing the train with coaches and seats
Coach* initializeTrain(){
    //Attach engine
    Coach* head = createCoach(-1, "Engine", 0);
    Coach* current = head;

    // Attach 1AC( 1 coach )
    current->nextCoach = createCoach(1, "1AC", 16);
    current = current->nextCoach;

    // Attach pantry
    current->nextCoach = createCoach(-2, "Pantry", 0);
    current = current->nextCoach;

    int coaches;
    int startNumber;

    // Attach 2AC (2 coaches)
    coaches = 2;
    startNumber = 101;
    for(int i = 0; i < coaches; i++){
        current->nextCoach = createCoach(startNumber + i, "2AC", 48);
        current = current->nextCoach;
    }

    // Attach 3AC (3 coaches)
    coaches = 3;
    startNumber = 201;
    for(int i = 0; i < coaches; i++){
        current->nextCoach = createCoach(startNumber + i, "3AC", 64);
        current = current->nextCoach;
    }

    // Attach Sleeper (8 coaches)
    coaches = 8;
    startNumber = 301;
    for(int i = 0; i < coaches; i++){
        current->nextCoach = createCoach(startNumber + i, "Sleeper", 72);
        current = current->nextCoach;
    }

    return head;
}