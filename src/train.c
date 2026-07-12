#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/train.h"

static Seat* buildSeatTreeForCoach(int coachNumber, int capacity) {
    const char* berthTypes[] = {"L", "M", "U", "L", "M", "U", "SL", "SU"};
    Seat* root = NULL;

    for(int i = 0; i < capacity; i++) {
        Seat* seat = createSeatNode(coachNumber, i + 1, berthTypes[i % 8]);
        root = insertSeatNode(root, seat);
    }

    return root;
}

Coach* createCoach(int orderKey, int coachNumber, const char* coachType, int capacity) {
    Coach* coach = createCoachNode(orderKey, coachNumber, coachType, capacity);

    if(strcmp(coachType, "Engine") != 0 && strcmp(coachType, "Pantry") != 0) {
        coach->seatList = buildSeatTreeForCoach(coachNumber, capacity);
    }

    return coach;
}

Coach* initializeTrain() {
    Coach* head = NULL;
    int orderKey = 0;

    head = insertCoachNode(head, createCoach(orderKey++, -1, "Engine", 0));
    head = insertCoachNode(head, createCoach(orderKey++, 1, "1AC", 16));
    head = insertCoachNode(head, createCoach(orderKey++, -2, "Pantry", 0));

    for(int i = 0; i < 2; i++) {
        head = insertCoachNode(head, createCoach(orderKey++, 101 + i, "2AC", 48));
    }

    for(int i = 0; i < 3; i++) {
        head = insertCoachNode(head, createCoach(orderKey++, 201 + i, "3AC", 64));
    }

    for(int i = 0; i < 8; i++) {
        head = insertCoachNode(head, createCoach(orderKey++, 301 + i, "Sleeper", 72));
    }

    return head;
}
