#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/booking.h"
#include "../include/display.h"

typedef struct PassengerSnapshot {
    int bookingOrder;   // Unique key used to remove the passenger from the confirmed AVL tree
    int coachNumber;    // Needed to free the correct seat during cancellation
    int seatNumber;     // Exact seat that was occupied
    char coachType[10]; // Used to promote the right waitlist after a seat becomes free
} PassengerSnapshot;

static int nextPNR = 1000;        // Shared reservation id for one booking request / group
static int nextBookingOrder = 1;  // Unique AVL key for each confirmed passenger node







// ============================ QUESTION 1 ============================ //
// Reservation flow with preferences, cross-coach fallback, and waitlist handling.


// ----------------------- Q1 Helper functions ------------------------ //

int getBerthIndex(const char* berthType) {
    if(strcmp(berthType, "L") == 0) return 0;
    if(strcmp(berthType, "M") == 0) return 1;
    if(strcmp(berthType, "U") == 0) return 2;
    if(strcmp(berthType, "SL") == 0) return 3;
    if(strcmp(berthType, "SU") == 0) return 4;
    return -1;
}

Bool canCoachFitGroup(Coach* coach, TempPassenger* group, int numberOfSeats) {
    if(coach == NULL || coach->seatList == NULL) {
        return FALSE;
    }

    // First try to book the whole group in one coach.
    // We compare how many berths of each type are needed with how many are still open in this coach.
    int requiredCount[5] = {0};
    int availableCount[5] = {0};

    for(int i = 0; i < numberOfSeats; i++) {
        if(!group[i].gotSeat) {
            int berthIndex = getBerthIndex(group[i].berthPreference);
            if(berthIndex != -1) {
                requiredCount[berthIndex]++;
            }
        }
    }

    int seatCount = countSeats(coach->seatList);
    Seat** seats = (Seat**)malloc(sizeof(Seat*) * seatCount);
    if(seats == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectSeatsInOrder(coach->seatList, seats, &index);

    for(int i = 0; i < seatCount; i++) {
        if(!seats[i]->isBooked) {
            int berthIndex = getBerthIndex(seats[i]->berthType);
            if(berthIndex != -1) {
                availableCount[berthIndex]++;
            }
        }
    }

    free(seats);

    for(int i = 0; i < 5; i++) {
        if(requiredCount[i] > availableCount[i]) {
            return FALSE;
        }
    }

    return TRUE;
}

static int bookSeatsForGroup(Coach* coach, Passenger** passengerListHead, TempPassenger* group, int numberOfSeats, Bool ignorePreference, int pnr) {
    int bookedCount = 0;

    int seatCount = countSeats(coach->seatList);
    if(seatCount == 0) {
        return 0;
    }

    Seat** seats = (Seat**)malloc(sizeof(Seat*) * seatCount);
    if(seats == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectSeatsInOrder(coach->seatList, seats, &index);

    // Seats are traversed in seat order so the allocation matches the visible coach layout.
    // When ignorePreference is FALSE, we respect berth preferences exactly.
    for(int s = 0; s < seatCount && bookedCount < numberOfSeats; s++) {
        Seat* currentSeat = seats[s];

        if(currentSeat->isBooked) {
            continue;
        }

        Bool foundPassenger = FALSE;
        for(int i = 0; i < numberOfSeats && !foundPassenger; i++) {
            if(!group[i].gotSeat && (ignorePreference || strcmp(currentSeat->berthType, group[i].berthPreference) == 0)) {
                currentSeat->isBooked = TRUE;
                group[i].gotSeat = TRUE;
                group[i].assignedCoach = coach->coachNumber;
                group[i].assignedSeat = currentSeat->seatNumber;
                strcpy(group[i].assignedBerth, currentSeat->berthType);

                Passenger* newPassenger = createPassengerNode(group[i].name, group[i].gender, group[i].DOB, group[i].age);
                newPassenger->pnrNumber = pnr;
                newPassenger->coachNumber = coach->coachNumber;
                newPassenger->seatNumber = currentSeat->seatNumber;
                strcpy(newPassenger->berthType, currentSeat->berthType);
                strcpy(newPassenger->coachType, coach->coachType);
                newPassenger->bookingOrder = nextBookingOrder++;
                *passengerListHead = insertPassengerByBookingOrder(*passengerListHead, newPassenger);

                bookedCount++;
                foundPassenger = TRUE;
            }
        }
    }

    free(seats);
    return bookedCount;
}

// ------------------ Q1 Waitlist initialization -------------------- //

void initializeWaitlistManager(WaitlistManager* wm) {
    wm->sleeperWL = NULL;
    wm->firstACWL = NULL;
    wm->secondACWL = NULL;
    wm->thirdACWL = NULL;

    wm->sleeperWLCount = 0;
    wm->firstACWLCount = 0;
    wm->secondACWLCount = 0;
    wm->thirdACWLCount = 0;
}

static Passenger** getWaitlistHead(WaitlistManager* wm, const char* coachType) {
    if(strcmp(coachType, "Sleeper") == 0) {
        return &(wm->sleeperWL);
    }
    if(strcmp(coachType, "1AC") == 0) {
        return &(wm->firstACWL);
    }
    if(strcmp(coachType, "2AC") == 0) {
        return &(wm->secondACWL);
    }
    if(strcmp(coachType, "3AC") == 0) {
        return &(wm->thirdACWL);
    }
    return NULL;
}

static int* getWaitlistCounter(WaitlistManager* wm, const char* coachType) {
    // The counter stores the next waitlist number to assign for that coach type.
    if(strcmp(coachType, "Sleeper") == 0) {
        return &(wm->sleeperWLCount);
    }
    if(strcmp(coachType, "1AC") == 0) {
        return &(wm->firstACWLCount);
    }
    if(strcmp(coachType, "2AC") == 0) {
        return &(wm->secondACWLCount);
    }
    if(strcmp(coachType, "3AC") == 0) {
        return &(wm->thirdACWLCount);
    }
    return NULL;
}

static int addToWaitlist(WaitlistManager* wm, TempPassenger person, const char* coachType, int pnr) {
    Passenger** waitlistHead = getWaitlistHead(wm, coachType);
    int* waitlistCounter = getWaitlistCounter(wm, coachType);

    if(waitlistHead == NULL || waitlistCounter == NULL) {
        printf("ERROR: Invalid coach type for waitlist\n");
        return -1;
    }

    Passenger* newWLPassenger = createPassengerNode(person.name, person.gender, person.DOB, person.age);
    newWLPassenger->pnrNumber = pnr;
    newWLPassenger->coachNumber = -1;
    newWLPassenger->seatNumber = ++(*waitlistCounter);
    strcpy(newWLPassenger->coachType, coachType);

    *waitlistHead = insertWaitlistPassenger(*waitlistHead, newWLPassenger);
    return newWLPassenger->seatNumber;
}

// ---------------------- Q1 Booking functions ---------------------- //

void executeBooking(Coach* trainHead, Passenger** passengerListHead, WaitlistManager* wm, const char* coachType, int numberOfSeats, TempPassenger* group) {
    int pnr = nextPNR++;
    int totalSeatsBooked = 0;

    int coachCount = countCoaches(trainHead);
    Coach** coaches = (Coach**)malloc(sizeof(Coach*) * coachCount);
    if(coaches == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectCoachesInOrder(trainHead, coaches, &index);

    // Step 1: try to place the whole group in one coach while honoring every berth preference.
    for(int i = 0; i < coachCount; i++) {
        if(strcmp(coaches[i]->coachType, coachType) == 0 && canCoachFitGroup(coaches[i], group, numberOfSeats)) {
            printf("\nWhole group accomodated in Coach %d (%s)\n", coaches[i]->coachNumber, coaches[i]->coachType);
            totalSeatsBooked = bookSeatsForGroup(coaches[i], passengerListHead, group, numberOfSeats, FALSE, pnr);
            break;
        }
    }

    // Step 2: if one coach cannot fit the full group, spread the group across coaches of the same type.
    if(totalSeatsBooked < numberOfSeats) {
        printf("\nSame-coach booking not possible. Splitting group across multiple %s coaches with berth preferences\n", coachType);
        for(int i = 0; i < coachCount && totalSeatsBooked < numberOfSeats; i++) {
            if(strcmp(coaches[i]->coachType, coachType) == 0) {
                int bookedInThisCoach = bookSeatsForGroup(coaches[i], passengerListHead, group, numberOfSeats, FALSE, pnr);
                totalSeatsBooked += bookedInThisCoach;
            }
        }

        if(totalSeatsBooked == numberOfSeats) {
            printf("\nWhole group accomodated with preferences across multiple %s coaches\n\n", coachType);
        }
    }

    // Step 3: if preferences still block some passengers, fill remaining seats without preference filtering.
    if(totalSeatsBooked < numberOfSeats) {
        printf("\nStill %d passengers without seats. Filling them in any available seats in %s coaches without preferences\n", numberOfSeats - totalSeatsBooked, coachType);
        for(int i = 0; i < coachCount && totalSeatsBooked < numberOfSeats; i++) {
            if(strcmp(coaches[i]->coachType, coachType) == 0) {
                int bookedInThisCoach = bookSeatsForGroup(coaches[i], passengerListHead, group, numberOfSeats, TRUE, pnr);
                totalSeatsBooked += bookedInThisCoach;
            }
        }
    }

    // Any unassigned passengers are moved to the coach-type waitlist.
    if(totalSeatsBooked == numberOfSeats) {
        printf("----SUCCESS: All %d passengers got confirmed seats in %s coaches----\n\n", numberOfSeats, coachType);
    } else {
        int unbooked = numberOfSeats - totalSeatsBooked;
        printf("\n----ALERT: %s coaches completely filled.----\n", coachType);
        printf("\n----PARTIAL SUCCESS: %d tickets confirmed. %d tickets are in waiting list----\n\n", totalSeatsBooked, unbooked);

        for(int i = 0; i < numberOfSeats; i++) {
            if(!group[i].gotSeat) {
                int wlNumber = addToWaitlist(wm, group[i], coachType, pnr);
                group[i].assignedCoach = -1;
                group[i].assignedSeat = wlNumber;
            }
        }
    }

    printf("\n------------------------------------ TICKET SUMMARY ----------------------------------\n");
    for(int i = 0; i < numberOfSeats; i++) {
        if(group[i].assignedCoach == -1) {
            printf("PNR: %d | Name: %-20s | Status: WAITLISTED | WL Number: %d\n", pnr, group[i].name, group[i].assignedSeat);
        } else {
            printf("PNR: %d | Name: %-20s | Status: CONFIRMED  | Coach: %-3d | Seat: %-2d | Berth: %s\n", pnr, group[i].name, group[i].assignedCoach, group[i].assignedSeat, group[i].assignedBerth);
        }
    }
    printf("----------------------------------------------------------------------------\n\n");

    free(coaches);
}

void bookTicketsUI(Coach* trainHead, Passenger** passengerListHead, WaitlistManager* wm) {
    // User interface to book tickets
    char coachType[10];
    int numberOfSeats;

    printf("---------------- Railway Reservation Form ----------------\n");
    printf("Enter Coach Type (Sleeper, 1AC, 2AC, 3AC): ");
    scanf("%s", coachType);
    printf("Enter Number of Seats to Book: ");
    scanf("%d", &numberOfSeats);

    TempPassenger* group = (TempPassenger*)malloc(numberOfSeats * sizeof(TempPassenger));
    if(group == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for(int i = 0; i < numberOfSeats; i++) {
        printf("\n-------- Details for Passenger %d ---------\n", i + 1);

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

        group[i].gotSeat = FALSE;
        group[i].assignedCoach = -1;
        group[i].assignedSeat = -1;
        group[i].assignedBerth[0] = '\0';
    }

    executeBooking(trainHead, passengerListHead, wm, coachType, numberOfSeats, group);
    free(group);
}

// ---------------------- Q1 CSV booking ---------------------------- //

void processCSVBookings(Coach* trainHead, Passenger** passengerListHead, WaitlistManager* wm, const char* filename) {
    // Read grouped bookings from CSV and process each group as one reservation request.
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        printf("ERROR: Could not open file %s\n", filename);
        return;
    }

    char line[256];
    int currentGroupID = -1;
    char currentCoachType[10];
    int numberOfSeats = 0;
    TempPassenger group[15]; // The file format limits one group to 15 passengers in this implementation.

    printf("----------Processing CSV bookings from file--------------\n");

    fgets(line, sizeof(line), file);

    while(fgets(line, sizeof(line), file)) {
        int groupID, age;
        char coachType[10], name[50], gender[10], DOB[11], berthPreference[4];

        if(sscanf(line, "%d,%9[^,],%49[^,],%9[^,],%10[^,],%d,%3[^,\n\r]", &groupID, coachType, name, gender, DOB, &age, berthPreference) == 7) {
            if(currentGroupID == -1) {
                currentGroupID = groupID;
                strcpy(currentCoachType, coachType);
            }

            if(groupID != currentGroupID) {
                printf("\n..... Processing Group %d, Coach Type: %s, Number of Seats: %d .....\n", currentGroupID, currentCoachType, numberOfSeats);
                executeBooking(trainHead, passengerListHead, wm, currentCoachType, numberOfSeats, group);
                currentGroupID = groupID;
                strcpy(currentCoachType, coachType);
                numberOfSeats = 0;
            }

            strcpy(group[numberOfSeats].name, name);
            strcpy(group[numberOfSeats].gender, gender);
            strcpy(group[numberOfSeats].DOB, DOB);
            group[numberOfSeats].age = age;
            strcpy(group[numberOfSeats].berthPreference, berthPreference);
            group[numberOfSeats].gotSeat = FALSE;
            group[numberOfSeats].assignedCoach = -1;
            group[numberOfSeats].assignedSeat = -1;
            group[numberOfSeats].assignedBerth[0] = '\0';
            numberOfSeats++;
        }
    }

    if(numberOfSeats > 0) {
        printf("\n..... Processing Group %d, Coach Type: %s, Number of Seats: %d .....\n", currentGroupID, currentCoachType, numberOfSeats);
        executeBooking(trainHead, passengerListHead, wm, currentCoachType, numberOfSeats, group);
    }

    fclose(file);
    printf("----------Finished processing CSV bookings from file--------------\n");
}







// ============================ QUESTION 6 ============================ //
// Cancellation (full/partial), seat release, and waitlist advancement.


// ----------------------- Q6 Helper functions ------------------------ //

static void freeSeat(Coach* coachListHead, int coachNumber, int seatNumber) {
    // Locate the coach , then free the exact seat node.
    Coach* coach = findCoachByNumber(coachListHead, coachNumber);
    if(coach == NULL || coach->seatList == NULL) {
        return;
    }

    Seat* seat = findSeatByNumber(coach->seatList, seatNumber);
    if(seat != NULL) {
        seat->isBooked = FALSE;
    }
}

// Promote the earliest waitlisted passengers into newly freed seats.
static void promoteFromWaitlist(Passenger** passengerListHead, WaitlistManager* wm, Coach* coachListHead, const char* coachType) {
    Passenger** waitlistHead = getWaitlistHead(wm, coachType);
    if(waitlistHead == NULL || *waitlistHead == NULL) {
        return;
    }

    int coachCount = countCoaches(coachListHead);
    Coach** coaches = (Coach**)malloc(sizeof(Coach*) * coachCount);
    if(coaches == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int index = 0;
    collectCoachesInOrder(coachListHead, coaches, &index);

    for(int i = 0; i < coachCount && *waitlistHead != NULL; i++) {
        if(strcmp(coaches[i]->coachType, coachType) != 0 || coaches[i]->seatList == NULL) {
            continue;
        }

        int seatCount = countSeats(coaches[i]->seatList);
        Seat** seats = (Seat**)malloc(sizeof(Seat*) * seatCount);
        if(seats == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }

        int seatIndex = 0;
        collectSeatsInOrder(coaches[i]->seatList, seats, &seatIndex);

        for(int s = 0; s < seatCount && *waitlistHead != NULL; s++) {
            if(seats[s]->isBooked) {
                continue;
            }

            // extractMinPassenger gives the oldest waitlisted passenger first.
            Passenger* passengerToPromote = extractMinPassenger(waitlistHead);
            if(passengerToPromote == NULL) {
                break;
            }

            passengerToPromote->coachNumber = coaches[i]->coachNumber;
            passengerToPromote->seatNumber = seats[s]->seatNumber;
            strcpy(passengerToPromote->berthType, seats[s]->berthType);
            strcpy(passengerToPromote->coachType, coaches[i]->coachType);
            passengerToPromote->bookingOrder = nextBookingOrder++;
            passengerToPromote->left = NULL;
            passengerToPromote->right = NULL;
            passengerToPromote->height = 1;

            seats[s]->isBooked = TRUE;
            *passengerListHead = insertPassengerByBookingOrder(*passengerListHead, passengerToPromote);
        }

        free(seats);
    }

    free(coaches);
}

// Rebuild waitlist numbering after cancellations/promotions.
static void updateWaitlistNumbers(WaitlistManager* wm) {
    rebuildWaitlistNumbers(&wm->sleeperWL);
    wm->sleeperWLCount = countPassengers(wm->sleeperWL);

    rebuildWaitlistNumbers(&wm->firstACWL);
    wm->firstACWLCount = countPassengers(wm->firstACWL);

    rebuildWaitlistNumbers(&wm->secondACWL);
    wm->secondACWLCount = countPassengers(wm->secondACWL);

    rebuildWaitlistNumbers(&wm->thirdACWL);
    wm->thirdACWLCount = countPassengers(wm->thirdACWL);
}

// Capture matching passengers before deleting them from the AVL tree.
static void collectSnapshotsByPNR(Passenger* root, int pnr, PassengerSnapshot* snapshots, int* index) {
    if(root == NULL) {
        return;
    }

    // We capture the data before deletion because the AVL node may be freed immediately after.
    collectSnapshotsByPNR(root->left, pnr, snapshots, index);
    if(root->pnrNumber == pnr) {
        snapshots[*index].bookingOrder = root->bookingOrder; // Needed to remove the passenger from the confirmed AVL tree
        snapshots[*index].coachNumber = root->coachNumber;
        snapshots[*index].seatNumber = root->seatNumber; 
        strcpy(snapshots[*index].coachType, root->coachType); // Needed for promoting the right waitlist after cancellation
        (*index)++;
    }
    collectSnapshotsByPNR(root->right, pnr, snapshots, index);
}

void cancelFullTicket(Passenger** passengerListHead, WaitlistManager* wm, Coach* CoachListHead, int pnr) {
    int totalPassengers = countPassengers(*passengerListHead);
    if(totalPassengers == 0) {
        printf("\n----ALERT: No passengers found with PNR %d----\n\n", pnr);
        return;
    }

    PassengerSnapshot* snapshots = (PassengerSnapshot*)malloc(sizeof(PassengerSnapshot) * totalPassengers);
    if(snapshots == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int matched = 0;
    collectSnapshotsByPNR(*passengerListHead, pnr, snapshots, &matched);

    if(matched == 0) {
        free(snapshots);
        printf("\n----ALERT: No passengers found with PNR %d----\n\n", pnr);
        return;
    }

    for(int i = 0; i < matched; i++) {
        freeSeat(CoachListHead, snapshots[i].coachNumber, snapshots[i].seatNumber);
        *passengerListHead = deletePassengerByBookingOrder(*passengerListHead, snapshots[i].bookingOrder);
        promoteFromWaitlist(passengerListHead, wm, CoachListHead, snapshots[i].coachType);
    }

    updateWaitlistNumbers(wm);
    free(snapshots);
    printf("\n----SUCCESS: Cancelled %d passengers under PNR %d----\n\n", matched, pnr);
}

void cancelSelectedPassengers(Passenger** passengerListHead, WaitlistManager* wm, Coach* CoachListHead, int pnr, int indices[], int k) {
    // Partial cancellation: remove only the passengers whose positions match the user input.
    int totalPassengers = countPassengers(*passengerListHead);
    if(totalPassengers == 0) {
        printf("\n----ALERT: No passengers found with PNR %d----\n\n", pnr);
        return;
    }

    PassengerSnapshot* snapshots = (PassengerSnapshot*)malloc(sizeof(PassengerSnapshot) * totalPassengers);
    if(snapshots == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    int matched = 0;
    collectSnapshotsByPNR(*passengerListHead, pnr, snapshots, &matched);

    if(matched == 0) {
        free(snapshots);
        printf("\n----ALERT: No passengers found with PNR %d----\n\n", pnr);
        return;
    }

    int cancelledCount = 0;
    for(int i = 0; i < matched; i++) {
        int shouldCancel = 0;
        for(int j = 0; j < k; j++) {
            if(indices[j] == i + 1) {
                shouldCancel = 1;
                break;
            }
        }

        if(shouldCancel) {
            freeSeat(CoachListHead, snapshots[i].coachNumber, snapshots[i].seatNumber);
            *passengerListHead = deletePassengerByBookingOrder(*passengerListHead, snapshots[i].bookingOrder);
            promoteFromWaitlist(passengerListHead, wm, CoachListHead, snapshots[i].coachType);
            cancelledCount++;
        }
    }

    updateWaitlistNumbers(wm);
    free(snapshots);

    if(cancelledCount > 0) {
        printf("\n----SUCCESS: Cancelled %d selected passengers under PNR %d----\n\n", cancelledCount, pnr);
    } else {
        printf("\n----ALERT: No passengers found with PNR %d----\n\n", pnr);
    }
}

// ------------------------- Q6 Cancellation UI ----------------------- //

void cancelTicketUI(Passenger** passengerListHead, WaitlistManager* wm, Coach* CoachListHead) {
    // Ask the user whether they want a full cancellation or only selected passengers.
    int pnr;
    printf("Enter PNR number: ");
    scanf("%d", &pnr);

    displayPassengersByPNR(*passengerListHead, pnr);

    printf("\n1. Cancel Full Ticket\n2. Cancel Selected Passengers\nChoose an option: ");
    int choice;
    scanf("%d", &choice);

    if(choice == 1) {
        cancelFullTicket(passengerListHead, wm, CoachListHead, pnr);
    } else if(choice == 2) {
        int k;
        printf("Enter number of passengers to cancel: ");
        scanf("%d", &k);

        int indices[15];
        printf("Enter indices: ");
        for(int i = 0; i < k; i++) {
            scanf("%d", &indices[i]);
        }

        cancelSelectedPassengers(passengerListHead, wm, CoachListHead, pnr, indices, k);
    }
}
