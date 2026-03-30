#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum { FALSE, TRUE } Bool;

// Seat Node
typedef struct Seat {
    int seatNumber;
    char berthType[3]; // L, M, U, SL, SU
    char coachType[10]; // Sleeper, 1AC, 2AC, 3AC
    Bool isBooked;
    struct Seat* nextSeat;
} Seat;

// Coach Node
typedef struct Coach {
    int coachNumber;
    char coachType[10]; // Sleeper, 1AC, 2AC, 3AC, engine, pantry
    int capacity;
    struct Coach* nextCoach;
    Seat* seatList; // Pointer to the head of the internal linked list of seats in the coach
} Coach;

// Passenger Node
typedef struct Passenger {
    char name[50];
    char gender[10];
    char DOB[11]; // DD/MM/YYYY format
    int age;
    int coachNumber;
    int seatNumber;
    struct Passenger* nextPassenger;
} Passenger;

#endif