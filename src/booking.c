#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./booking.h"

//// If we check for each passenger and try to find a seat... TC = O(numberOfSeats * seatsInCoach) which can be very high for large groups and coaches.
// Bool canCoachFitGroup(Coach* coach, TempPassenger* group, int numberOfSeats){
//     for(int i = 0; i < numberOfSeats; i++){
//         if(group[i].gotSeat == TRUE) continue; // Skip passengers who already got a seat

//         Seat* currentSeat  = coach->seatList;
//         Bool foundSeat = FALSE;

//         while(currentSeat != NULL && !foundSeat){
//             if(!currentSeat->isBooked && strcmp(currentSeat->berthType, group[i].berthPreference) == 0){
//                 foundSeat = TRUE;
//             }
//             currentSeat = currentSeat->nextSeat;
//         }

//         if(!foundSeat) return FALSE;
//     }
//     return TRUE;
// }

int getBerthIndex(char* berthPreference){
    if(strcmp(berthPreference, "L") == 0) return 0;
    if(strcmp(berthPreference, "M") == 0) return 1;
    if(strcmp(berthPreference, "U") == 0) return 2;
    if(strcmp(berthPreference, "SL") == 0) return 3;
    if(strcmp(berthPreference, "SU") == 0) return 4;
    return -1; // Invalid berth preference
}

Bool canCoachFitGroup(Coach* coach, TempPassenger* group, int numberOfSeats){
    int requiredCount[5] = {0}; // L, M, U, SL, SU
    int availableCount[5] = {0}; // L, M, U, SL, SU

    // Count the required berths for the group
    for(int i = 0; i < numberOfSeats; i++){
        if(!group[i].gotSeat){
            int berthIndex = getBerthIndex(group[i].berthPreference);
            if(berthIndex != -1){
                requiredCount[berthIndex]++;
            }
        }
    }

    // Count the available berths in the coach
    Seat* currentSeat = coach->seatList;
    while(currentSeat != NULL){
        if(!currentSeat->isBooked){
            int berthIndex = getBerthIndex(currentSeat->berthType);
            if(berthIndex != -1){
                availableCount[berthIndex]++;
            }
        }
        currentSeat = currentSeat->nextSeat;
    }

    // Check if the coach has enough available berths to accommodate the whole group
    for(int i = 0; i < 5; i++){
        if(requiredCount[i] > availableCount[i]){
            return FALSE; // Not enough berths of this type
        }
    }
    return TRUE; // All required berths are available
}

int bookSeatsForGroup(Coach* coach, Passenger** passengerListHead, TempPassenger* group, int numberOfSeats, Bool ignorePreference){
    int bookedCount = 0;

    for(int i = 0; i < numberOfSeats; i++){
        Bool foundSeat = FALSE;
        if(!group[i].gotSeat){
            Seat* currentSeat = coach->seatList;
            while(currentSeat != NULL && !foundSeat){
                if(!currentSeat->isBooked && (ignorePreference || strcmp(currentSeat->berthType, group[i].berthPreference) == 0)){
                    currentSeat->isBooked = TRUE; // Mark the seat as booked
                    group[i].gotSeat = TRUE;
                    foundSeat = TRUE;


                    // Add the passenger to the passenger list
                    Passenger* newPassenger = (Passenger*)malloc(sizeof(Passenger));
                    
                    strcpy(newPassenger->name, group[i].name);
                    strcpy(newPassenger->gender, group[i].gender);
                    strcpy(newPassenger->DOB, group[i].DOB);
                    newPassenger->age = group[i].age;
                    
                    newPassenger->coachNumber = coach->coachNumber;
                    newPassenger->seatNumber = currentSeat->seatNumber;
                    strcpy(newPassenger->berthType, currentSeat->berthType);
                    
                    // Insert the new passenger at the head of the passenger list
                    newPassenger->nextPassenger = *passengerListHead;
                    *passengerListHead = newPassenger;
                    

                    bookedCount++;
                }
                currentSeat = currentSeat->nextSeat;
            }
        }
    }

    return bookedCount;
}

void executeBooking(Coach* trainHead, Passenger** passengerListHead, char* coachType, int numberOfSeats, TempPassenger* group){
    int totalSeatsBooked = 0;

    Coach* currentCoach = trainHead;
    while(currentCoach != NULL){
        if(strcmp(currentCoach->coachType, coachType) == 0){
            // Check if any coach of the specified type can accommodate the entire group
            if(canCoachFitGroup(currentCoach, group, numberOfSeats)){
                // Book seats for the group in this coach
                printf("\n Priority 1 Success: Whole group accomodated in Coach %d (%s)\n", currentCoach->coachNumber, currentCoach->coachType);
                bookSeatsForGroup(currentCoach, passengerListHead, group, numberOfSeats, FALSE);
                return; // Booking done, exit the function
            }
        }
        currentCoach = currentCoach->nextCoach;
    }


    

}

void bookTickets(Coach* trainHead, Passenger** passengerListHead){
    char coachType[10];
    int numberOfSeats;

    printf("---------------- Railway Reservation Form ----------------\n");
    printf("Enter Coach Type (Sleeper, 1AC, 2AC, 3AC): ");
    scanf("%s", coachType);
    printf("Enter Number of Seats to Book: ");
    scanf("%d", &numberOfSeats);

    // Temporary array to hold passenger details for the group booking
    TempPassenger* group = (TempPassenger*)malloc(numberOfSeats * sizeof(TempPassenger));

    for(int i = 0; i < numberOfSeats; i++){
        printf("\n-------- Details for Passenger %d ---------\n", i+1);

        printf("Name: ");
        scanf(" %[^\n]", group[i].name); 

        printf("Gender: ");
        scanf("%s", group[i].gender);

        printf("Date of Birth (DD/MM/YYYY): ");
        scanf("%s", group[i].DOB);

        printf("Age: ");
        scanf("%d", &group[i].age);

        printf("Berth Preference (L, M, U, SL, SU): ");
        scanf("%s", group[i].berthPreference);

        group[i].gotSeat = FALSE; // Initialize gotSeat to FALSE as we haven't allocated seats yet
    }

    executeBooking(trainHead, passengerListHead, coachType, numberOfSeats, group);

    free(group); // Free the temporary group array after booking is done
}