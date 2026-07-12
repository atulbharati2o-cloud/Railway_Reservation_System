#ifndef TRAIN_H
#define TRAIN_H

#include "tree_utils.h"

// Function to create a coach and initialize its seat tree
Coach* createCoach(int orderKey, int coachNumber, const char* coachType, int capacity);

// Function to initialize the train with predefined coaches and seats
Coach* initializeTrain();

#endif
