#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef enum { FALSE, TRUE } Bool;

typedef struct Seat {
    int seatNumber;
    char berthType[4]; // L, M, U, SL, SU
    int coachNumber;
    Bool isBooked;
    struct Seat* left;
    struct Seat* right;
    int height;
} Seat;


typedef struct Coach {
    int coachNumber;
    int orderKey;      // Internal AVL key that keeps the train layout in physical order
    char coachType[10]; // Sleeper, 1AC, 2AC, 3AC, Engine, Pantry
    int capacity;      // Number of seats for this coach type
    Seat* seatList;    // Root of the AVL tree that stores seats inside this coach
    struct Coach* left;
    struct Coach* right;
    int height;
} Coach;


typedef struct Passenger {
    char name[50];
    char gender[10];
    char DOB[11];
    int age;
    int pnrNumber;     // Shared by all passengers booked together in one request
    int coachNumber;   // Actual coach assigned to the passenger
    int seatNumber;    // Actual seat number, or waitlist position for WL nodes
    char berthType[4]; // Final berth assigned after booking
    char coachType[10];
    int bookingOrder;  // Unique AVL key for confirmed-passenger storage
    struct Passenger* left;
    struct Passenger* right;
    int height;
} Passenger;

typedef struct WaitlistManager {
    Passenger* sleeperWL;   // Root of Sleeper waitlist AVL
    Passenger* firstACWL;   // Root of 1AC waitlist AVL
    Passenger* secondACWL;  // Root of 2AC waitlist AVL
    Passenger* thirdACWL;   // Root of 3AC waitlist AVL
    int sleeperWLCount;
    int firstACWLCount;
    int secondACWLCount;
    int thirdACWLCount;
} WaitlistManager;

#endif
