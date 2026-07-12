#ifndef TREE_UTILS_H
#define TREE_UTILS_H

#include "structures.h"

// Coach AVL helpers
Coach* createCoachNode(int orderKey, int coachNumber, const char* coachType, int capacity);
Seat* createSeatNode(int coachNumber, int seatNumber, const char* berthType);
Passenger* createPassengerNode(const char* name, const char* gender, const char* DOB, int age);

Coach* insertCoachNode(Coach* root, Coach* node);
Coach* findCoachByNumber(Coach* root, int coachNumber);
void collectCoachesInOrder(Coach* root, Coach** arr, int* index);
int countCoaches(Coach* root);
Coach* buildCoachTreeFromArray(Coach** arr, int start, int end);

// Seat AVL helpers
Seat* insertSeatNode(Seat* root, Seat* node);
Seat* findSeatByNumber(Seat* root, int seatNumber);
void collectSeatsInOrder(Seat* root, Seat** arr, int* index);
int countSeats(Seat* root);

// Passenger AVL helpers
Passenger* insertPassengerByBookingOrder(Passenger* root, Passenger* node);
Passenger* deletePassengerByBookingOrder(Passenger* root, int bookingOrder);
Passenger* findPassengerByBookingOrder(Passenger* root, int bookingOrder);
void collectPassengersInOrder(Passenger* root, Passenger** arr, int* index);
void collectPassengersByPNR(Passenger* root, int pnr, Passenger** arr, int* index);
void collectPassengersByCoachNumber(Passenger* root, int coachNumber, Passenger** arr, int* index);
int countPassengers(Passenger* root);
Passenger* insertWaitlistPassenger(Passenger* root, Passenger* node);
Passenger* extractMinPassenger(Passenger** root);
Passenger* buildPassengerTreeFromArray(Passenger** arr, int start, int end);
void rebuildWaitlistNumbers(Passenger** root);

#endif
