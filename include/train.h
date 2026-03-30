#ifndef TRAIN_H
#define TRAIN_H

#include "structures.h"

// Function to create a linked list of seats for a coach
Seat* createSeatList(int coachNumber, int capacity);

// Function to create a coach and initialize its seat list
Coach* createCoach(int coachNumber, const char* coachType, int capacity);

// Function to initialize the train with predefined coaches and seats
Coach* initializeTrain();

#endif