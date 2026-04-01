#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/booking.h"

//// If we check for each passenger and try to find a seat... TC = O(numberOfSeats * seatsInCoach) which can be very high for large groups and coaches.
// Bool canCoachFitGroup(Coach* coach, TempPassenger* group, int numberOfSeats){
//     for(int i = 0; i < numberOfSeats; i++){
//         if(group[i].gotSeat == TRUE) continue; // Skip passengers who already got a seat
//
//         Seat* currentSeat  = coach->seatList;
//         Bool foundSeat = FALSE;
//
//         while(currentSeat != NULL && !foundSeat){
//             if(!currentSeat->isBooked && strcmp(currentSeat->berthType, group[i].berthPreference) == 0){
//                 foundSeat = TRUE;
//             }
//             currentSeat = currentSeat->nextSeat;
//         }
//
//         if(!foundSeat) return FALSE;
//     }
//     return TRUE;
// }

int getBerthIndex(char* berthType){
    if(strcmp(berthType, "L") == 0) return 0;
    if(strcmp(berthType, "M") == 0) return 1;
    if(strcmp(berthType, "U") == 0) return 2;
    if(strcmp(berthType, "SL") == 0) return 3;
    if(strcmp(berthType, "SU") == 0) return 4;
    return -1; // Invalid berth type
}

// Check if a coach has enough available berths to accommodate the whole group based on their preferences. TC = O(seatsInCoach + numberOfSeats) 
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

// Find seats and book them and create Passenger record
int bookSeatsForGroup(Coach* coach, Passenger** passengerListHead, TempPassenger* group, int numberOfSeats, Bool ignorePreference){
    int bookedCount = 0;

    for(int i = 0; i < numberOfSeats; i++){
        Bool foundSeat = FALSE;
        if(!group[i].gotSeat){
            Seat* currentSeat = coach->seatList;
            while(currentSeat != NULL && !foundSeat){
                if(!currentSeat->isBooked && (ignorePreference || strcmp(currentSeat->berthType, group[i].berthPreference) == 0)){

                    currentSeat->isBooked = TRUE; // Mark the seat as booked
                    
                    // Update the passenger's booking status
                    group[i].gotSeat = TRUE;
                    group[i].assignedCoach = coach->coachNumber;
                    group[i].assignedSeat = currentSeat->seatNumber;
                    strcpy(group[i].assignedBerth, currentSeat->berthType);
                    
                    
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
                    
                    
                    foundSeat = TRUE;
                    bookedCount++;
                }
                currentSeat = currentSeat->nextSeat;
            }
        }
    }

    return bookedCount;
}

// Add a passenger to the waitlist queue
// Seatnumber here acts as the waitlist number for the passenger.
int addToWaitlist(Passenger** waitlistHead, TempPassenger person, char* coachType){
    Passenger* newWLPassenger = (Passenger*)malloc(sizeof(Passenger));

    strcpy(newWLPassenger->name, person.name);
    strcpy(newWLPassenger->gender, person.gender);
    strcpy(newWLPassenger->DOB, person.DOB);
    newWLPassenger->age = person.age;
    newWLPassenger->coachNumber = -1; // No coach assigned yet
    newWLPassenger->seatNumber = -1; // No seat assigned yet
    
    newWLPassenger->nextPassenger = NULL;

    if(*waitlistHead == NULL){
        newWLPassenger->seatNumber = 1;
        *waitlistHead = newWLPassenger;
    } else{
        Passenger* current = *waitlistHead;
        int count = 1;
        while(current->nextPassenger != NULL){
            count++;
            current = current->nextPassenger;
        }
        newWLPassenger->seatNumber = count + 1;
        current->nextPassenger = newWLPassenger;
    }

    return newWLPassenger->seatNumber;
}

void executeBooking(Coach* trainHead, Passenger** passengerListHead, Passenger** waitlistHead, char* coachType, int numberOfSeats, TempPassenger* group){
    int totalSeatsBooked = 0;

    // Priority 1: Try to accommodate the whole group with their preferred berths in the same coach
    Coach* currentCoach = trainHead;
    while(currentCoach != NULL){
        if(strcmp(currentCoach->coachType, coachType) == 0){
            // Check if any coach of the specified type can accommodate the entire group
            if(canCoachFitGroup(currentCoach, group, numberOfSeats)){
                // Book seats for the group in this coach
                printf("\nWhole group accomodated in Coach %d (%s)\n", currentCoach->coachNumber, currentCoach->coachType);
                totalSeatsBooked = bookSeatsForGroup(currentCoach, passengerListHead, group, numberOfSeats, FALSE);
                break; // Exit the loop as booking is done
            }
        }
        currentCoach = currentCoach->nextCoach;
    }

    // Priority 2: try to accommodate the group with their preferred berths across multiple coaches of the same type
    if(totalSeatsBooked < numberOfSeats){
        printf("\nSame-coach booking not possible. Splitting group across multiple %s coaches with berth preferences\n", coachType);
        currentCoach = trainHead;
        while(currentCoach != NULL && totalSeatsBooked < numberOfSeats){
            if(strcmp(currentCoach->coachType, coachType) == 0){
                int bookedInThisCoach = bookSeatsForGroup(currentCoach, passengerListHead, group, numberOfSeats, FALSE);
                totalSeatsBooked += bookedInThisCoach;
            }
            currentCoach = currentCoach->nextCoach;
        }

        if(totalSeatsBooked == numberOfSeats){
            printf("\nWhole group accomodated with preferences across multiple %s coaches\n\n", coachType);
        }
    }
    

    // Priority 3: try to accommodate maximum possible passengers with their preferred berths and then fill the remaining passengers in any available seats in coaches of the same type
    // We have already booked as many passengers as possible with their preferences in the previous step, now we just need to fill the remaining passengers without caring about preferences
    if(totalSeatsBooked < numberOfSeats){
        printf("\nStill %d passengers without seats. Filling them in any available seats in %s coaches without preferences\n", numberOfSeats - totalSeatsBooked, coachType);
        currentCoach = trainHead;
        while(currentCoach != NULL && totalSeatsBooked < numberOfSeats){
            if(strcmp(currentCoach->coachType, coachType) == 0){
                int bookedInThisCoach = bookSeatsForGroup(currentCoach, passengerListHead, group, numberOfSeats, TRUE); // True to ignore preferences in this step
                totalSeatsBooked += bookedInThisCoach;
            }
            currentCoach = currentCoach->nextCoach;
        }
    }

    if(totalSeatsBooked == numberOfSeats){
        printf("----SUCCESS: All %d passengers got confirmed seats in %s coaches----\n\n", numberOfSeats, coachType);
    } 

    // Waitlist the remaining passengers
    else{
        int unbooked = numberOfSeats - totalSeatsBooked;
        printf("\n----ALERT: %s coaches completely filled.----\n", coachType);
        printf("\n----PARTIAL SUCCESS: %d tickets confirmed. %d tickets are in waiting list----\n\n", totalSeatsBooked, unbooked);

        for(int i = 0; i < numberOfSeats; i++){
            if(!group[i].gotSeat){
                int wlNumber = addToWaitlist(waitlistHead, group[i], coachType);
                group[i].gotSeat = TRUE; 
                group[i].assignedCoach = -1; // No coach assigned yet
                group[i].assignedSeat = wlNumber; // Using seatNumber field to store waitlist
            }
        }
    }

    

    // TICKET SUMMARY
    printf("\n------------------------------------ TICKET SUMMARY ----------------------------------\n");
    for(int i = 0; i < numberOfSeats; i++){
        if(group[i].assignedCoach == -1){
            printf("Passenger: %s | Status: WAITLISTED | Waitlist Number: %d\n", group[i].name, group[i].assignedSeat);
        } else{
            printf("Passenger: %s | Status: CONFIRMED | Coach: %d | Seat: %d | Berth: %s\n", group[i].name, group[i].assignedCoach, group[i].assignedSeat, group[i].assignedBerth);
        }
    }
    printf("----------------------------------------------------------------------------\n\n");

}

// User interface to book tickets
void bookTickets(Coach* trainHead, Passenger** passengerListHead, Passenger** waitlistHead){
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

    executeBooking(trainHead, passengerListHead, waitlistHead, coachType, numberOfSeats, group);

    free(group); // Free the temporary group array after booking is done
}

// Function for testing bulk bookings from a CSV file
void processCSVBookings(Coach* trainHead, Passenger** passengerListHead, Passenger** waitlistHead, const char* filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        printf("ERROR: Could not open file %s\n", filename);
        return;
    }

    char line[256];
    int currentGroupID = -1;
    char currentCoachType[10];
    int numberOfSeats = 0;

    TempPassenger group[15]; // Assuming maximum group size is 15

    printf("----------Processing CSV bookings from file--------------\n");

    fgets(line, sizeof(line), file); // Skip header line

    // Read each line from the CSV file
    while(fgets(line, sizeof(line), file)){
        int groupID, age;
        char coachType[10], name[50], gender[10], DOB[11], berthPreference[4];

        if(sscanf(line, "%d,%9[^,],%49[^,],%9[^,],%10[^,],%d,%3[^,\n\r]", &groupID, coachType, name, gender, DOB, &age, berthPreference) == 7){

            if(currentGroupID == -1){
                currentGroupID = groupID;
                strcpy(currentCoachType, coachType);
            }

            if(groupID != currentGroupID){
                printf("\n..... Processing Group %d, Coach Type: %s, Number of Seats: %d .....\n", currentGroupID, currentCoachType, numberOfSeats);
                executeBooking(trainHead, passengerListHead, waitlistHead, currentCoachType, numberOfSeats, group);
                currentGroupID = groupID;
                strcpy(currentCoachType, coachType);
                numberOfSeats = 0;
            }

            // Add passenger details to the current group
            strcpy(group[numberOfSeats].name, name);
            strcpy(group[numberOfSeats].gender, gender);
            strcpy(group[numberOfSeats].DOB, DOB);  
            group[numberOfSeats].age = age;
            strcpy(group[numberOfSeats].berthPreference, berthPreference);
            group[numberOfSeats].gotSeat = FALSE; 

            numberOfSeats++;
        }
            
    }

    // Book the last group after reading the file
    if(numberOfSeats > 0){
        printf("\n..... Processing Group %d, Coach Type: %s, Number of Seats: %d .....\n", currentGroupID, currentCoachType, numberOfSeats);
        executeBooking(trainHead, passengerListHead, waitlistHead, currentCoachType, numberOfSeats, group);
    }

    fclose(file);
    printf("----------Finished processing CSV bookings from file--------------\n");