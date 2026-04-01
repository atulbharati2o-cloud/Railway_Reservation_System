#include <stdio.h>
#include "../include/display.h"

void displayTrain(Coach* head){

   printf("//================================= Train Layout =================================//\n\n");

    Coach* currentCoach = head;
    while(currentCoach != NULL){
        if(currentCoach->seatList != NULL){
            printf("--------------------- Coach %d: %s (Capacity: %d) ---------------------\n", currentCoach->coachNumber, currentCoach->coachType, currentCoach->capacity);

            Seat* currentSeat = currentCoach->seatList;
            while(currentSeat != NULL){
                Seat* bay[8];
                for(int i = 0; i < 8 && currentSeat != NULL; i++){
                    bay[i] = currentSeat;
                    currentSeat = currentSeat->nextSeat;
                }

                printf("[%02d|%s] [%02d|%s] [%02d|%s]      [%02d|%s]\n",
                                bay[0]->seatNumber, bay[0]->berthType,
                                bay[1]->seatNumber, bay[1]->berthType,
                                bay[2]->seatNumber, bay[2]->berthType,
                                bay[7]->seatNumber, bay[7]->berthType
                );
                printf("[%02d|%s] [%02d|%s] [%02d|%s]      [%02d|%s]\n",
                                bay[3]->seatNumber, bay[3]->berthType,
                                bay[4]->seatNumber, bay[4]->berthType,
                                bay[5]->seatNumber, bay[5]->berthType,
                                bay[6]->seatNumber, bay[6]->berthType
                );

                printf("--------------------      -------\n");

            }
        }
        currentCoach = currentCoach->nextCoach;
    }
}


void displayPassengers(Passenger* head){
    if(head == NULL){
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    printf("//============================== Confirmed Passengers ==============================//\n\n");
    Passenger* current = head;
    while(current != NULL){
        printf("Name: %s | Gender: %s | DOB: %s | Age: %d | Coach: %d | Seat: %d | Berth: %s\n",
                current->name,
                current->gender,
                current->DOB,
                current->age,
                current->coachNumber,
                current->seatNumber,
                current->berthType
            );

        current = current->nextPassenger;
    }
    printf("\n===================================================================================//\n");
}

void displayWaitlist(Passenger* waitlistHead){
    if(waitlistHead == NULL){
        printf("\nNo passengers in waitlist.\n\n");
        return;
    }

    printf("//============================== Waitlisted Passengers ==============================//\n\n");
    Passenger* current = waitlistHead;
    while(current != NULL){
        printf("Waitlist No: %d | Name: %s | Gender: %s | DOB: %s | Age: %d\n",
                current->seatNumber, // Using seatNumber as waitlist number
                current->name,          
                current->gender,
                current->DOB,
                current->age
            );

        current = current->nextPassenger;
    }
    printf("\n===================================================================================//\n");
}       