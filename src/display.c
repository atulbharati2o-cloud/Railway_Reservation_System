#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/display.h"
#include "../include/sorting.h"

void displayTrain(Coach* head){

   printf("\n\n||================= Train Layout ==================||\n\n");

    Coach* currentCoach = head;
    while(currentCoach != NULL){
        if(currentCoach->seatList != NULL){
            printf("\n------------ Coach %d: %s (Capacity: %d) ------------\n\n", currentCoach->coachNumber, currentCoach->coachType, currentCoach->capacity);

            Seat* currentSeat = currentCoach->seatList;
            while(currentSeat != NULL){
                Seat* bay[8];
                for(int i = 0; i < 8 && currentSeat != NULL; i++){
                    bay[i] = currentSeat;
                    currentSeat = currentSeat->nextSeat;
                }

                printf("          [%02d|%s] [%02d|%s] [%02d|%s]      [%02d|%s]\n",
                                bay[0]->seatNumber, bay[0]->berthType,
                                bay[1]->seatNumber, bay[1]->berthType,
                                bay[2]->seatNumber, bay[2]->berthType,
                                bay[7]->seatNumber, bay[7]->berthType
                );
                printf("          [%02d|%s] [%02d|%s] [%02d|%s]      [%02d|%s]\n",
                                bay[3]->seatNumber, bay[3]->berthType,
                                bay[4]->seatNumber, bay[4]->berthType,
                                bay[5]->seatNumber, bay[5]->berthType,
                                bay[6]->seatNumber, bay[6]->berthType
                );

                printf("          --------------------      -------\n");

            }
        } else{
            if(strcmp(currentCoach->coachType, "Engine") == 0){
                printf("\n---------------------- Engine ----------------------\n\n");
            } else if(strcmp(currentCoach->coachType, "Pantry") == 0){
                printf("\n---------------------- Pantry ----------------------\n\n");
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

    printf("\n============================== Confirmed Passengers ==============================\n\n");
    Passenger* current = head;
    while(current != NULL){
        printf("PNR: %d | Name: %-20s | Coach: %-3d | Seat: %-2d | Berth: %s\n", current->pnrNumber, current->name, current->coachNumber, current->seatNumber, current->berthType);
        current = current->nextPassenger;
    }
    printf("\n===================================================================================\n");
}


void displaySingleWaitlist(Passenger* waitlistHead, char* coachType){
    if(waitlistHead == NULL){
        printf("\nNo passengers in %s waitlist.\n\n", coachType);
        return;
    }

    printf("\n---------- %s WAITLIST ----------\n\n", coachType);
    Passenger* current = waitlistHead;
    while(current != NULL){
        printf("WL %-2d | Name: %-20s | PNR: %-3d\n", current->seatNumber, current->name, current->pnrNumber);
        current = current->nextPassenger;
    }
}

void displayAllWaitlist(WaitlistManager* wm){
    printf("\n============================== WAITLISTS ==============================\n\n");

    displaySingleWaitlist(wm->sleeperWL, "Sleeper");
    displaySingleWaitlist(wm->firstACWL, "1AC");
    displaySingleWaitlist(wm->secondACWL, "2AC");
    displaySingleWaitlist(wm->thirdACWL, "3AC");

    printf("\n=======================================================================\n");
}       







void displayPassengersOfCoachSortedByName(Passenger* head){
    if(head == NULL){
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    printf("\nEnter the coach number: ");
    int coachNumber;
    scanf("%d", &coachNumber);

    // First, count number of passengers in the given coach
    int count = 0;
    Passenger* current = head;
    while(current != NULL){
        if(current->coachNumber == coachNumber){
            count++;
        }
        current = current->nextPassenger;
    }

    Passenger** arr = (Passenger**)malloc(count * sizeof(Passenger*));

    current = head;
    int index = 0;
    while(current != NULL){
        if(current->coachNumber == coachNumber){
            arr[index++] = current;
        }
        current = current->nextPassenger;
    }

    mergeSort(arr, 0, count - 1, SORT_BY_NAME); // Sort by name

    // Display sorted passengers of the specified coach
    printf("\n============================== Passengers of Coach %d Sorted by Name =============================\n\n", coachNumber);
    for(int i = 0; i < count; i++){
        printf("PNR: %d | Name: %-20s | Gender: %-6s | DOB: %s | Age: %-2d | Seat: %-2d | Berth: %s\n",
                arr[i]->pnrNumber,
                arr[i]->name,
                arr[i]->gender,
                arr[i]->DOB,
                arr[i]->age,
                arr[i]->seatNumber,
                arr[i]->berthType
            );
    }
    
    free(arr);
}






void displayAllPassengersSortedByName(Passenger* head){
    if(head == NULL){
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    // Count total passengers
    int count = 0;
    Passenger* current = head;
    while(current != NULL){
        count++;
        current = current->nextPassenger;
    }

    Passenger** arr = (Passenger**)malloc(count * sizeof(Passenger*));

    current = head;
    int index = 0;
    while(current != NULL){
        arr[index++] = current;
        current = current->nextPassenger;
    }

    mergeSort(arr, 0, count - 1, SORT_BY_NAME); // Sort by name

    // Display all passengers sorted by name
    printf("\n============================== All Passengers Sorted by Name =============================\n\n");
    for(int i = 0; i < count; i++){
        printf("PNR: %d | Name: %-20s | Gender: %-6s | DOB: %s | Age: %-2d | Coach: %-3d | Seat: %-2d | Berth: %s\n",
                arr[i]->pnrNumber,
                arr[i]->name,
                arr[i]->gender,
                arr[i]->DOB,
                arr[i]->age,
                arr[i]->coachNumber,
                arr[i]->seatNumber,
                arr[i]->berthType
            );
    }
    free(arr);
}



void displayAllPassengersSortedByCoachNumber(Passenger* head){
    if(head == NULL){
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    // Count total passengers
    int count = 0;
    Passenger* current = head;
    while(current != NULL){
        count++;
        current = current->nextPassenger;
    }

    Passenger** arr = (Passenger**)malloc(count * sizeof(Passenger*));

    current = head;
    int index = 0;
    while(current != NULL){
        arr[index++] = current;
        current = current->nextPassenger;
    }

    mergeSort(arr, 0, count - 1, SORT_BY_COACH); // Sort by coach number

    // Display all passengers sorted by coach number
    printf("\n============================== All Passengers Sorted by Coach Number =============================\n\n");
    for(int i = 0; i < count; i++){
        printf("PNR: %d | Name: %-20s | Gender: %-6s | DOB: %s | Age: %-2d | Coach: %-3d | Seat: %-2d | Berth: %s\n",
                arr[i]->pnrNumber,
                arr[i]->name,
                arr[i]->gender,
                arr[i]->DOB,
                arr[i]->age,
                arr[i]->coachNumber,
                arr[i]->seatNumber,
                arr[i]->berthType
            );
    }
    free(arr);
}



void displayAllPassengersIn_L_or_SL_Berths(Passenger* head){
    if(head == NULL){
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    int count = 0;

    printf("\n============================== Passengers in L or SL Berths =============================\n\n");
    Passenger* current = head;
    while(current != NULL){
        if(strcmp(current->berthType, "L") == 0 || strcmp(current->berthType, "SL") == 0){
            printf("PNR: %-3d | Name: %-20s | Berth: %-2s\n", current->pnrNumber, current->name, current->berthType);
        }
        count++;
        current = current->nextPassenger;
    }

    if(count == 0){
        printf("No passengers in L or SL berths found.\n");
    }
}



void displaySeniorCitizensWithoutL_or_SL_Berths(Passenger* head){
    if(head == NULL){
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    int count = 0;

    printf("\n============================== Senior Citizens without L or SL Berths =============================\n\n");
    Passenger* current = head;
    while(current != NULL){
        if(current->age > 60 && strcmp(current->berthType, "L") != 0 && strcmp(current->berthType, "SL") != 0){
            printf("PNR: %-3d | Name: %-20s | Age: %-2d | Berth: %-2s\n", current->pnrNumber, current->name, current->age, current->berthType);
            count++;
        }
        current = current->nextPassenger;
    }

    if(count == 0){
        printf("No senior citizens without L or SL berths found.\n");
    }
}






void displayNumberOfAvailableSeatsInEachCoach(Coach* head){
    
    int arr[4] = {0}; // 1AC, 2AC, 3AC, Sleeper
    Coach* currentCoach = head;
    while(currentCoach != NULL){
        int index = -1;
        if(strcmp(currentCoach->coachType, "1AC") == 0) index = 0;
        else if(strcmp(currentCoach->coachType, "2AC") == 0) index = 1;
        else if(strcmp(currentCoach->coachType, "3AC") == 0) index = 2;
        else if(strcmp(currentCoach->coachType, "Sleeper") == 0) index = 3;

        if(index != -1){
            Seat* currentSeat = currentCoach->seatList;
            while(currentSeat != NULL){
                if(!currentSeat->isBooked){
                    arr[index]++;
                }
                currentSeat = currentSeat->nextSeat;
            }
        }

        currentCoach = currentCoach->nextCoach;
    }

    // Display
    printf("\n============================== Available Seats in Each Coach =============================\n\n");
    printf("Coach Type | Available Seats\n");
    printf("-----------|----------------\n");
    printf("1AC        | %d\n", arr[0]);
    printf("2AC        | %d\n", arr[1]);
    printf("3AC        | %d\n", arr[2]);
    printf("Sleeper    | %d\n", arr[3]);

}






// Display Passengers by PNR number
void displayPassengersByPNR(Passenger* head, int pnr){
    int index = 1;
    printf("\n------- Passengers under PNR %d -------\n", pnr);

    Passenger* current = head;
    while(current != NULL){
        if(current->pnrNumber == pnr){
            printf("%d. %s | Coach: %d | Seat: %d\n", index, current->name, current->coachNumber, current->seatNumber);
            index++;
        }
        current = current->nextPassenger;
    }

    if(index == 1){
        printf("No passengers found with PNR %d\n", pnr);
    }
}
