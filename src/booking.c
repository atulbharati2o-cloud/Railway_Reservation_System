#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structures.h"
#include "../include/display.h"
#include "../include/booking.h"




//          ==================== Helper functions for booking ===================           //

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

// Function to add a passenger to the confirmed passenger list
void addPassengerToConfirmedList(Passenger** passengerListHead, Passenger* newPassenger){
    newPassenger->nextPassenger = NULL;
    if(*passengerListHead == NULL){
        *passengerListHead = newPassenger;
    } else{
        Passenger* current = *passengerListHead;
        while(current->nextPassenger != NULL){
            current = current->nextPassenger;
        }
        current->nextPassenger = newPassenger;
    }
}

// Find seats and book them and create Passenger record
int bookSeatsForGroup(Coach* coach, Passenger** passengerListHead, TempPassenger* group, int numberOfSeats, Bool ignorePreference, int pnr){
    int bookedCount = 0;

    Seat* currentSeat = coach->seatList;
    while(currentSeat != NULL && bookedCount < numberOfSeats){
        
        // If seat is empty then find passenger
        Bool foundPassenger = FALSE; // Flag to check if we found a passenger for the current seat
        if(!currentSeat->isBooked){
            for(int i = 0; i < numberOfSeats && !foundPassenger; i++){
                if(!group[i].gotSeat && (ignorePreference || strcmp(currentSeat->berthType, group[i].berthPreference) == 0)){
                    currentSeat->isBooked = TRUE; // Mark the seat as booked
                    
                    // Update the passenger's booking status
                    group[i].gotSeat = TRUE;
                    group[i].assignedCoach = coach->coachNumber;
                    group[i].assignedSeat = currentSeat->seatNumber;
                    strcpy(group[i].assignedBerth, currentSeat->berthType);
                    
                    
                    // Add the passenger to the passenger list
                    Passenger* newPassenger = (Passenger*)malloc(sizeof(Passenger));
                    if(newPassenger == NULL){
                        printf("ERROR: Memory allocation FAILED.\n");
                        exit(1);
                    }
                    
                    strcpy(newPassenger->name, group[i].name);
                    strcpy(newPassenger->gender, group[i].gender);
                    strcpy(newPassenger->DOB, group[i].DOB);
                    newPassenger->age = group[i].age;

                    newPassenger->pnrNumber = pnr; // Assign the same PNR to all passengers in the group
                    newPassenger->coachNumber = coach->coachNumber;
                    strcpy(newPassenger->coachType, coach->coachType);
                    newPassenger->seatNumber = currentSeat->seatNumber;
                    strcpy(newPassenger->berthType, currentSeat->berthType);
                    
                    // Add the new passenger to the confirmed passenger list
                    addPassengerToConfirmedList(passengerListHead, newPassenger);

                    bookedCount++;
                    foundPassenger = TRUE; // Set the flag to true since we found a passenger for this seat
                }
            }
        }

        currentSeat = currentSeat->nextSeat;
    }

    return bookedCount;
}


void initializeWaitlistManager(WaitlistManager* wm){
    wm->sleeperWL = NULL;
    wm->firstACWL = NULL;
    wm->secondACWL = NULL;
    wm->thirdACWL = NULL;

    wm->sleeperWLCount = 0;
    wm->firstACWLCount = 0;
    wm->secondACWLCount = 0;
    wm->thirdACWLCount = 0;
}






//          ========================= Waitlist Management ===================           //

// Function to get the head of the waitlist for a given coach type
Passenger** getWaitlistHead(WaitlistManager* wm, char* coachType){
    if(strcmp(coachType, "Sleeper") == 0){
        return &(wm->sleeperWL);
    } else if(strcmp(coachType, "1AC") == 0){
        return &(wm->firstACWL);
    } else if(strcmp(coachType, "2AC") == 0){
        return &(wm->secondACWL);
    } else if(strcmp(coachType, "3AC") == 0){
        return &(wm->thirdACWL);
    } else{
        return NULL; // Invalid coach type
    }
}


// Function to get the pointer to the waitlist counter for a given coach type
int* getWaitlistCounter(WaitlistManager* wm, char* coachType){
    if(strcmp(coachType, "Sleeper") == 0){
        return &(wm->sleeperWLCount);
    } else if(strcmp(coachType, "1AC") == 0){
        return &(wm->firstACWLCount);
    } else if(strcmp(coachType, "2AC") == 0){
        return &(wm->secondACWLCount);
    } else if(strcmp(coachType, "3AC") == 0){
        return &(wm->thirdACWLCount);
    } else{
        return NULL; // Invalid coach type
    }
}


// Add a passenger to the waitlist queue
// Seatnumber here acts as the waitlist number for the passenger.
int addToWaitlist(WaitlistManager* wm, TempPassenger person, char* coachType, int pnr){

    Passenger** waitlistHead = getWaitlistHead(wm, coachType);
    int* waitlistCounter = getWaitlistCounter(wm, coachType);
    if(waitlistHead == NULL || waitlistCounter == NULL){
        printf("ERROR: Invalid coach type for waitlist\n");
        return -1;
    }

    Passenger* newWLPassenger = (Passenger*)malloc(sizeof(Passenger));

    strcpy(newWLPassenger->name, person.name);
    strcpy(newWLPassenger->gender, person.gender);
    strcpy(newWLPassenger->DOB, person.DOB);
    newWLPassenger->age = person.age;

    newWLPassenger->pnrNumber = pnr; // Assign the same PNR to all passengers in the group
    newWLPassenger->coachNumber = -1; // No coach assigned yet
    newWLPassenger->seatNumber = ++(*waitlistCounter); 
    strcpy(newWLPassenger->coachType, coachType);
    
    newWLPassenger->nextPassenger = NULL;

    if(*waitlistHead == NULL){
        *waitlistHead = newWLPassenger;
    } else{
        Passenger* current = *waitlistHead;
        while(current->nextPassenger != NULL){
            current = current->nextPassenger;
        }
        current->nextPassenger = newWLPassenger;
    }

    return newWLPassenger->seatNumber;
}





 
//           ========================= Booking Logic ===================           //

// Core Booking Logic
void executeBooking(Coach* trainHead, Passenger** passengerListHead, WaitlistManager* wm, char* coachType, int numberOfSeats, TempPassenger* group){
    static int nextPNR = 1000; // Static variable to generate unique PNR numbers for passengers
    int pnr = nextPNR++; // Assign current PNR and then increment for the next passenger

    int totalSeatsBooked = 0;

    // Priority 1: Try to accommodate the whole group with their preferred berths in the same coach
    Coach* currentCoach = trainHead;
    while(currentCoach != NULL){
        if(strcmp(currentCoach->coachType, coachType) == 0){
            // Check if any coach of the specified type can accommodate the entire group
            if(canCoachFitGroup(currentCoach, group, numberOfSeats)){
                // Book seats for the group in this coach
                printf("\nWhole group accomodated in Coach %d (%s)\n", currentCoach->coachNumber, currentCoach->coachType);
                totalSeatsBooked = bookSeatsForGroup(currentCoach, passengerListHead, group, numberOfSeats, FALSE, pnr);
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
                int bookedInThisCoach = bookSeatsForGroup(currentCoach, passengerListHead, group, numberOfSeats, FALSE, pnr);
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
                int bookedInThisCoach = bookSeatsForGroup(currentCoach, passengerListHead, group, numberOfSeats, TRUE, pnr); // True to ignore preferences in this step
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
                int wlNumber = addToWaitlist(wm, group[i], coachType, pnr);
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
            printf("PNR: %d | Name: %-20s | Status: WAITLISTED | WL Number: %d\n", pnr, group[i].name, group[i].assignedSeat);
        } else{
            printf("PNR: %d | Name: %-20s | Status: CONFIRMED  | Coach: %-3d | Seat: %-2d | Berth: %s\n", pnr, group[i].name, group[i].assignedCoach, group[i].assignedSeat, group[i].assignedBerth);
        }
    }
    printf("----------------------------------------------------------------------------\n\n");

}


// User interface to book tickets
void bookTicketsUI(Coach* trainHead, Passenger** passengerListHead, WaitlistManager* wm){
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

    executeBooking(trainHead, passengerListHead, wm, coachType, numberOfSeats, group);

    free(group); // Free the temporary group array after booking is done
}


// Function for testing bulk bookings from a CSV file
void processCSVBookings(Coach* trainHead, Passenger** passengerListHead, WaitlistManager* wm, const char* filename){
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
                executeBooking(trainHead, passengerListHead, wm, currentCoachType, numberOfSeats, group);
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
        executeBooking(trainHead, passengerListHead, wm, currentCoachType, numberOfSeats, group);
    }

    fclose(file);
    printf("----------Finished processing CSV bookings from file--------------\n");
}











//         =================== Helper functions for cancellation and waitlist management ===================           //


// Function to free a seat when a ticket is cancelled
void freeSeat(Coach* coachListHead, int coachNumber, int seatNumber){
    Coach* currentCoach = coachListHead;
    while(currentCoach != NULL){
        if(currentCoach->coachNumber == coachNumber){
            Seat* currentSeat = currentCoach->seatList;
            while(currentSeat != NULL){
                if(currentSeat->seatNumber == seatNumber){
                    currentSeat->isBooked = FALSE; // Mark the seat as available
                    return;
                }
                currentSeat = currentSeat->nextSeat;
            }
        }
        currentCoach = currentCoach->nextCoach;
    }
}


// Function to upadate waitlist number of a coach type
void updateSingleWaitlist(Passenger* waitlistHead){
    int waitlistNumber = 1;
    Passenger* current = waitlistHead;
    while(current != NULL){
        current->seatNumber = waitlistNumber++; // Update the waitlist number
        current = current->nextPassenger;
    }
}


// Function to get the current count of passengers in a waitlist
int getCurrentWaitlistCount(Passenger* waitlistHead){
    int count = 0;
    Passenger* current = waitlistHead;
    while(current != NULL){
        count++;
        current = current->nextPassenger;
    }
    return count;
}

// Function to update waitlist numbers after promotion
void updateWaitlistNumbers(WaitlistManager* wm){
    // Sleeper
    updateSingleWaitlist(wm->sleeperWL);
    wm->sleeperWLCount = getCurrentWaitlistCount(wm->sleeperWL);

    // 1AC
    updateSingleWaitlist(wm->firstACWL);
    wm->firstACWLCount = getCurrentWaitlistCount(wm->firstACWL);

    // 2AC
    updateSingleWaitlist(wm->secondACWL);
    wm->secondACWLCount = getCurrentWaitlistCount(wm->secondACWL);

    // 3AC
    updateSingleWaitlist(wm->thirdACWL);
    wm->thirdACWLCount = getCurrentWaitlistCount(wm->thirdACWL);
}


// Function to promote passengers from waitlist to confirmed list after a cancellation
void promoteFromWaitlist(Passenger** passengerListHead, WaitlistManager* wm, Coach* coachListHead, char* coachType){

    // Get the head of the waitlist for the given coach type
    Passenger** waitlistHead = getWaitlistHead(wm, coachType);
    if(waitlistHead == NULL){
        printf("ERROR: Invalid Coach Type\n");
        return;
    }
    if(*waitlistHead == NULL){
        return; // No passengers in waitlist for this coach type
    }

    Coach* currentCoach = coachListHead;
    while(currentCoach != NULL){
        if(strcmp(currentCoach->coachType, coachType) == 0){
            
            Seat* currentSeat = currentCoach->seatList;
            while(currentSeat != NULL){
                if(!currentSeat->isBooked){

                    // Found an available seat - promote the first passenger from the waitlist of this coach to this seat
                    Passenger* passengerToPromote = *waitlistHead;
                    *waitlistHead = (*waitlistHead)->nextPassenger; // Remove the passenger from the waitlist

                    // Update the passenger's booking details
                    passengerToPromote->coachNumber = currentCoach->coachNumber;
                    passengerToPromote->seatNumber = currentSeat->seatNumber;
                    strcpy(passengerToPromote->berthType, currentSeat->berthType);
                    strcpy(passengerToPromote->coachType, currentCoach->coachType);

                    currentSeat->isBooked = TRUE; // Mark the seat as booked

                    // Add the promoted passenger to the confirmed passenger list
                    addPassengerToConfirmedList(passengerListHead, passengerToPromote);
                }

                currentSeat = currentSeat->nextSeat;
            }
        }

        currentCoach = currentCoach->nextCoach;
    }
}








//        =================== Cancellation Logic & Functions ===================           //



// Function to cancel the entire ticket (all passengers under the same PNR)
void cancelFullTicket(Passenger** passengerListHead, WaitlistManager* wm, Coach* CoachListHead, int pnr){
    Passenger* curr = *passengerListHead;
    Passenger* prev = NULL; // keeps track of previous passenger 

    int cancelledCount = 0;

    // Traverse the passenger list, find passengers with given PNR and cancel their tickets
    while(curr != NULL){
        if(curr->pnrNumber == pnr){
            
            // Mark the seat as available
            freeSeat(CoachListHead, curr->coachNumber, curr->seatNumber);

            char coachType[10];
            strcpy(coachType, curr->coachType);

            // Remove the passenger from the passenger list
            if(prev == NULL){
                *passengerListHead = curr->nextPassenger; // If head needs to be removed
            } else{
                prev->nextPassenger = curr->nextPassenger; // Bypass the current
            }

            // Free the memory allocated for the current passenger
            Passenger* temp = curr;
            curr = curr->nextPassenger; 
            free(temp);

            cancelledCount++;

            promoteFromWaitlist(passengerListHead, wm, CoachListHead, coachType); // After cancelling a ticket, promote passengers from waitlist  to confirmed list 
        } else{
            prev = curr;
            curr = curr->nextPassenger;
        }
    }

    updateWaitlistNumbers(wm); // Update waitlist numbers after promotion

    if(cancelledCount > 0){
        printf("\n----SUCCESS: Cancelled %d passengers under PNR %d----\n\n", cancelledCount, pnr);
    } else{
        printf("\n----ALERT: No passengers found with PNR %d----\n\n", pnr);
    }
}


// Function to cancel selected passengers from a ticket (partial cancellation)
void cancelSelectedPassengers(Passenger** passengerListHead, WaitlistManager* wm, Coach* CoachListHead, int pnr, int indices[], int k){
    Passenger* curr = *passengerListHead;
    Passenger* prev = NULL; // keeps track of previous passenger 

    int cancelledCount = 0;
    int currentIndex = 1; // To track the index of passengers under the same PNR

    // Traverse the passenger list, find passengers with given PNR and cancel the selected passengers based on indices
    while(curr != NULL){
        if(curr->pnrNumber == pnr){

            // Check if the current passenger's index is in the list of indices to cancel
            int shouldCancel = 0;
            for(int i = 0; i < k; i++){
                if(indices[i] == currentIndex){
                    shouldCancel = 1;
                    break;
                }
            }

            if(shouldCancel){

                // Mark the seat as available
                freeSeat(CoachListHead, curr->coachNumber, curr->seatNumber);

                char coachType[10];
                strcpy(coachType, curr->coachType);

                // Remove the passenger from the passenger list
                if(prev == NULL){
                    *passengerListHead = curr->nextPassenger; // If head needs to be removed
                } else{
                    prev->nextPassenger = curr->nextPassenger; // Bypass the current
                }

                // Free the memory allocated for the current passenger
                Passenger* temp = curr;
                curr = curr->nextPassenger;
                free(temp);

                cancelledCount++;

                promoteFromWaitlist(passengerListHead, wm, CoachListHead, coachType); // After cancelling a ticket, promote passengers from waitlist  to confirmed list
            } else{
                prev = curr;
                curr = curr->nextPassenger;
            }

            currentIndex++; // Increment the index for passengers under the same PNR

        } else{
            prev = curr;
            curr = curr->nextPassenger;
        }
    }

    updateWaitlistNumbers(wm); // Update waitlist numbers after promotion

    if(cancelledCount > 0){
        printf("\n----SUCCESS: Cancelled %d selected passengers under PNR %d----\n\n", cancelledCount, pnr);
    } else{
        printf("\n----ALERT: No passengers found with PNR %d----\n\n", pnr);
    }
}


// Cancel Ticket UI for full cancellation and partial cancellation
void cancelTicketUI(Passenger** passengerListHead, WaitlistManager* wm, Coach* CoachListHead){
    int pnr;
    printf("Enter PNR number: ");
    scanf("%d", &pnr);

    displayPassengersByPNR(*passengerListHead, pnr);

    printf("\n1. Cancel Full Ticket\n2. Cancel Selected Passengers\nChoose an option: ");
    int choice;
    scanf("%d", &choice);

    if(choice == 1){
        // Cancel all passengers with the given PNR
        cancelFullTicket(passengerListHead, wm, CoachListHead, pnr);
    } else if(choice == 2){
        int k;
        printf("Enter number of passengers to cancel: ");
        scanf("%d", &k);

        int indices[15]; // Assuming maximum group size is 15
        printf("Enter indices: ");
        for(int i = 0; i < k; i++){
            scanf("%d", &indices[i]);
        }

        cancelSelectedPassengers(passengerListHead, wm, CoachListHead, pnr, indices, k);
    }
}