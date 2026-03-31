#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum { FALSE, TRUE } Bool;

// Seat Node for seatList in Coach
typedef struct Seat {
    int seatNumber;
    char berthType[4]; // L, M, U, SL, SU
    int coachNumber; 
    Bool isBooked;
    struct Seat* nextSeat;
} Seat;

// Coach Node for coachList in Train
typedef struct Coach {
    int coachNumber;
    char coachType[10]; // Sleeper, 1AC, 2AC, 3AC, Engine, Pantry
    int capacity;
    struct Coach* nextCoach;
    Seat* seatList; // Pointer to the head of the internal linked list of seats in the coach
} Coach;

// Passenger Node for passengerList
typedef struct Passenger {
    char name[50];
    char gender[10];
    char DOB[11]; 
    int age;
    int coachNumber;
    int seatNumber;
    char berthType[4]; 
    struct Passenger* nextPassenger;
} Passenger;


#endif