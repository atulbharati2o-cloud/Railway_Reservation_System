#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/display.h"
#include "../include/sorting.h"

// ----------------------------- Passenger display helpers ------------------- //

static void displaySingleCoachLayout(Coach* currentCoach) {
    if(currentCoach->seatList != NULL) {
        printf("\n------------ Coach %d: %s (Capacity: %d) ------------\n\n", currentCoach->coachNumber, currentCoach->coachType, currentCoach->capacity);

        int seatCount = countSeats(currentCoach->seatList);
        Seat** seats = (Seat**)malloc(sizeof(Seat*) * seatCount);
        if(seats == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }

        int seatIndex = 0;
        collectSeatsInOrder(currentCoach->seatList, seats, &seatIndex);

        for(int j = 0; j < seatCount; j += 8) {
            printf("          [%02d|%s] [%02d|%s] [%02d|%s]      [%02d|%s]\n",
                   seats[j]->seatNumber, seats[j]->berthType,
                   seats[j + 1]->seatNumber, seats[j + 1]->berthType,
                   seats[j + 2]->seatNumber, seats[j + 2]->berthType,
                   seats[j + 7]->seatNumber, seats[j + 7]->berthType);
            printf("          [%02d|%s] [%02d|%s] [%02d|%s]      [%02d|%s]\n",
                   seats[j + 3]->seatNumber, seats[j + 3]->berthType,
                   seats[j + 4]->seatNumber, seats[j + 4]->berthType,
                   seats[j + 5]->seatNumber, seats[j + 5]->berthType,
                   seats[j + 6]->seatNumber, seats[j + 6]->berthType);
            printf("          --------------------      -------\n");
        }

        free(seats);
    } else {
        if(strcmp(currentCoach->coachType, "Engine") == 0) {
            printf("\n---------------------- Engine ----------------------\n\n");
        } else if(strcmp(currentCoach->coachType, "Pantry") == 0) {
            printf("\n---------------------- Pantry ----------------------\n\n");
        }
    }
}

static void displayTrainInOrder(Coach* head) {
    if(head == NULL) {
        return;
    }

    displayTrainInOrder(head->left);
    displaySingleCoachLayout(head);
    displayTrainInOrder(head->right);
}

void displayTrain(Coach* head) {
    printf("\n\n||================= Train Layout ==================||\n\n");

    if(head == NULL) {
        return;
    }

    displayTrainInOrder(head);
}

void displayPassengers(Passenger* head) {
    // Confirmed passengers are already stored by booking order, so the tree walk shows reservation sequence.
    int count = countPassengers(head);

    if(count == 0) {
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    Passenger** arr = (Passenger**)malloc(sizeof(Passenger*) * count);
    if(arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectPassengersInOrder(head, arr, &index);

    printf("\n============================== Confirmed Passengers ==============================\n\n");
    for(int i = 0; i < count; i++) {
        printf("PNR: %d | Name: %-20s | Coach: %-3d | Seat: %-2d | Berth: %s\n",
               arr[i]->pnrNumber,
               arr[i]->name,
               arr[i]->coachNumber,
               arr[i]->seatNumber,
               arr[i]->berthType);
    }
    printf("\n===================================================================================\n");
    free(arr);
}

// ----------------------------- Waitlist display --------------------------- //

void displaySingleWaitlist(Passenger* waitlistHead, const char* coachType) {
    // Waitlist nodes use seatNumber as the waiting-list position, so this shows the waiting order.
    int count = countPassengers(waitlistHead);

    if(count == 0) {
        printf("\nNo passengers in %s waitlist.\n\n", coachType);
        return;
    }

    Passenger** arr = (Passenger**)malloc(sizeof(Passenger*) * count);
    if(arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectPassengersInOrder(waitlistHead, arr, &index);

    printf("\n---------- %s WAITLIST ----------\n\n", coachType);
    for(int i = 0; i < count; i++) {
        printf("WL %-2d | Name: %-20s | PNR: %-3d\n", arr[i]->seatNumber, arr[i]->name, arr[i]->pnrNumber);
    }
    free(arr);
}

void displayAllWaitlist(WaitlistManager* wm) {
    // Show every coach-type waitlist separately so the user can see where each passenger is waiting.
    printf("\n============================== WAITLISTS ==============================\n\n");

    displaySingleWaitlist(wm->sleeperWL, "Sleeper");
    displaySingleWaitlist(wm->firstACWL, "1AC");
    displaySingleWaitlist(wm->secondACWL, "2AC");
    displaySingleWaitlist(wm->thirdACWL, "3AC");

    printf("\n=======================================================================\n");
}






// ============================ QUESTION 2 ============================ //
// Sort all passengers by coach-number and seat-number.

void displayAllPassengersSortedByCoachAndSeat(Passenger* head) {
    int count = countPassengers(head);

    if(count == 0) {
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    Passenger** arr = (Passenger**)malloc(sizeof(Passenger*) * count);
    if(arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectPassengersInOrder(head, arr, &index);
    mergeSort(arr, 0, count - 1, SORT_BY_COACH_AND_SEAT);

    printf("\n============================== All Passengers Sorted by Coach Number and Seat Number =============================\n\n");
    for(int i = 0; i < count; i++) {
        printf("PNR: %d | Name: %-20s | Gender: %-6s | DOB: %s | Age: %-2d | Coach: %-3d | Seat: %-2d | Berth: %s\n",
               arr[i]->pnrNumber,
               arr[i]->name,
               arr[i]->gender,
               arr[i]->DOB,
               arr[i]->age,
               arr[i]->coachNumber,
               arr[i]->seatNumber,
               arr[i]->berthType);
    }

    free(arr);
}






// ============================ QUESTION 3 ============================ //
// Sort passengers of a particular coach by passenger-name alphabetically.

void displayPassengersOfCoachSortedByName(Passenger* head) {
    if(head == NULL) {
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    int coachNumber;
    printf("\nEnter the coach number: ");
    scanf("%d", &coachNumber);

    int total = countPassengers(head);
    Passenger** arr = (Passenger**)malloc(sizeof(Passenger*) * total);
    if(arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int count = 0;
    collectPassengersByCoachNumber(head, coachNumber, arr, &count);

    if(count == 0) {
        printf("\nNo passengers found for coach %d.\n", coachNumber);
        free(arr);
        return;
    }

    mergeSort(arr, 0, count - 1, SORT_BY_NAME);

    printf("\n============================== Passengers of Coach %d Sorted by Name =============================\n\n", coachNumber);
    for(int i = 0; i < count; i++) {
        printf("PNR: %d | Name: %-20s | Gender: %-6s | DOB: %s | Age: %-2d | Seat: %-2d | Berth: %s\n",
               arr[i]->pnrNumber,
               arr[i]->name,
               arr[i]->gender,
               arr[i]->DOB,
               arr[i]->age,
               arr[i]->seatNumber,
               arr[i]->berthType);
    }

    free(arr);
}






// ============================ QUESTION 4 ============================ //
// Display functions.

void displayAllPassengersSortedByName(Passenger* head) {
    int count = countPassengers(head);

    if(count == 0) {
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    Passenger** arr = (Passenger**)malloc(sizeof(Passenger*) * count);
    if(arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectPassengersInOrder(head, arr, &index);
    mergeSort(arr, 0, count - 1, SORT_BY_NAME);

    printf("\n============================== All Passengers Sorted by Name =============================\n\n");
    for(int i = 0; i < count; i++) {
        printf("PNR: %d | Name: %-20s | Gender: %-6s | DOB: %s | Age: %-2d | Coach: %-3d | Seat: %-2d | Berth: %s\n",
               arr[i]->pnrNumber,
               arr[i]->name,
               arr[i]->gender,
               arr[i]->DOB,
               arr[i]->age,
               arr[i]->coachNumber,
               arr[i]->seatNumber,
               arr[i]->berthType);
    }

    free(arr);
}


void displayAllPassengersSortedByCoachNumber(Passenger* head) {
    int count = countPassengers(head);

    if(count == 0) {
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    Passenger** arr = (Passenger**)malloc(sizeof(Passenger*) * count);
    if(arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectPassengersInOrder(head, arr, &index);
    mergeSort(arr, 0, count - 1, SORT_BY_COACH);

    printf("\n============================== All Passengers Sorted by Coach Number =============================\n\n");
    for(int i = 0; i < count; i++) {
        printf("PNR: %d | Name: %-20s | Gender: %-6s | DOB: %s | Age: %-2d | Coach: %-3d | Seat: %-2d | Berth: %s\n",
               arr[i]->pnrNumber,
               arr[i]->name,
               arr[i]->gender,
               arr[i]->DOB,
               arr[i]->age,
               arr[i]->coachNumber,
               arr[i]->seatNumber,
               arr[i]->berthType);
    }

    free(arr);
}


void displayAllPassengersIn_L_or_SL_Berths(Passenger* head) {
    int count = countPassengers(head);

    if(count == 0) {
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    Passenger** arr = (Passenger**)malloc(sizeof(Passenger*) * count);
    if(arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectPassengersInOrder(head, arr, &index);

    int found = 0;
    printf("\n============================== Passengers in L or SL Berths =============================\n\n");
    for(int i = 0; i < count; i++) {
        if(strcmp(arr[i]->berthType, "L") == 0 || strcmp(arr[i]->berthType, "SL") == 0) {
            printf("PNR: %-3d | Name: %-20s | Age: %-2d | Berth: %-2s\n", arr[i]->pnrNumber, arr[i]->name, arr[i]->age, arr[i]->berthType);
            found = 1;
        }
    }

    if(!found) {
        printf("No passengers in L or SL berths found.\n");
    }

    free(arr);
}


void displaySeniorCitizensWithoutL_or_SL_Berths(Passenger* head) {
    int count = countPassengers(head);

    if(count == 0) {
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    Passenger** arr = (Passenger**)malloc(sizeof(Passenger*) * count);
    if(arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectPassengersInOrder(head, arr, &index);

    int found = 0;
    printf("\n============================== Senior Citizens without L or SL Berths =============================\n\n");
    for(int i = 0; i < count; i++) {
        if(arr[i]->age > 60 && strcmp(arr[i]->berthType, "L") != 0 && strcmp(arr[i]->berthType, "SL") != 0) {
            printf("PNR: %-3d | Name: %-20s | Age: %-2d | Berth: %-2s\n", arr[i]->pnrNumber, arr[i]->name, arr[i]->age, arr[i]->berthType);
            found = 1;
        }
    }

    if(!found) {
        printf("No senior citizens without L or SL berths found.\n");
    }

    free(arr);
}






// ============================ QUESTION 5 ============================ //
// Display number of available seats of each type of coach.

static void countAvailableSeatsInCoach(Coach* coach, int arr[4]) {
    int index = -1;
    if(strcmp(coach->coachType, "1AC") == 0) index = 0;
    else if(strcmp(coach->coachType, "2AC") == 0) index = 1;
    else if(strcmp(coach->coachType, "3AC") == 0) index = 2;
    else if(strcmp(coach->coachType, "Sleeper") == 0) index = 3;

    if(index == -1 || coach->seatList == NULL) {
        return;
    }

    int seatCount = countSeats(coach->seatList);
    Seat** seats = (Seat**)malloc(sizeof(Seat*) * seatCount);
    if(seats == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int seatIndex = 0;
    collectSeatsInOrder(coach->seatList, seats, &seatIndex);
    for(int j = 0; j < seatCount; j++) {
        if(!seats[j]->isBooked) {
            arr[index]++;
        }
    }
    free(seats);
}


static void countAvailableSeatsInOrder(Coach* head, int arr[4]) {
    if(head == NULL) {
        return;
    }

    countAvailableSeatsInOrder(head->left, arr);
    countAvailableSeatsInCoach(head, arr);
    countAvailableSeatsInOrder(head->right, arr);
}


void displayNumberOfAvailableSeatsInEachCoach(Coach* head) {
    int arr[4] = {0};
    if(head == NULL) {
        printf("\nNo coaches found.\n");
        return;
    }

    countAvailableSeatsInOrder(head, arr);

    printf("\n============================== Available Seats in Each Coach =============================\n\n");
    printf("Coach Type | Available Seats\n");
    printf("-----------|----------------\n");
    printf("1AC        | %d\n", arr[0]);
    printf("2AC        | %d\n", arr[1]);
    printf("3AC        | %d\n", arr[2]);
    printf("Sleeper    | %d\n", arr[3]);
}






// ============================ QUESTION 7 ============================ //
// Range search by lexicographic passenger-name bounds (inclusive).

void displayPassengersByNameRange(Passenger* head) {
    int count = countPassengers(head);
    if(count == 0) {
        printf("\nNo confirmed passengers yet.\n\n");
        return;
    }

    char name1[50], name2[50];
    printf("Enter first name (N1): ");
    scanf(" %49[^\n]", name1);
    printf("Enter second name (N2): ");
    scanf(" %49[^\n]", name2);

    char lower[50], upper[50];
    if(strcmp(name1, name2) <= 0) {
        strcpy(lower, name1);
        strcpy(upper, name2);
    } else {
        strcpy(lower, name2);
        strcpy(upper, name1);
    }

    Passenger** arr = (Passenger**)malloc(sizeof(Passenger*) * count);
    if(arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectPassengersInOrder(head, arr, &index);
    mergeSort(arr, 0, count - 1, SORT_BY_NAME);

    int found = 0;
    printf("\n============================== Passengers with Names in Range [%s, %s] ==============================\n\n", lower, upper);
    for(int i = 0; i < count; i++) {
        if(strcmp(arr[i]->name, lower) >= 0 && strcmp(arr[i]->name, upper) <= 0) {
            printf("PNR: %d | Name: %-20s | Gender: %-6s | DOB: %s | Age: %-2d | Coach: %-3d | Seat: %-2d | Berth: %s\n",
                   arr[i]->pnrNumber,
                   arr[i]->name,
                   arr[i]->gender,
                   arr[i]->DOB,
                   arr[i]->age,
                   arr[i]->coachNumber,
                   arr[i]->seatNumber,
                   arr[i]->berthType);
            found = 1;
        }
    }

    if(!found) {
        printf("No passengers found in the given name range.\n");
    }

    free(arr);
}





// ----------------------------- PNR lookup --------------------------------- //

void displayPassengersByPNR(Passenger* head, int pnr) {
    int count = countPassengers(head);
    if(count == 0) {
        printf("\nNo passengers found with PNR %d\n", pnr);
        return;
    }

    Passenger** arr = (Passenger**)malloc(sizeof(Passenger*) * count);
    if(arr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectPassengersByPNR(head, pnr, arr, &index);

    printf("\n------- Passengers under PNR %d -------\n", pnr);
    if(index == 0) {
        printf("No passengers found with PNR %d\n", pnr);
        free(arr);
        return;
    }

    for(int i = 0; i < index; i++) {
        printf("%d. %s | Coach: %d | Seat: %d\n", i + 1, arr[i]->name, arr[i]->coachNumber, arr[i]->seatNumber);
    }

    free(arr);
}
