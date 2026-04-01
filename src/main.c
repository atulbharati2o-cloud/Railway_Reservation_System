#include <stdio.h>
#include <stdlib.h>
#include "../include/structures.h"
#include "../include/display.h"
#include "../include/train.h"
#include "../include/booking.h"

int main(){
    Coach* myTrain = initializeTrain();
    //displayTrain(myTrain);

    Passenger* passengerListHead = NULL;
    Passenger* waitlistHead = NULL;

    int choice;
    do{
        printf("\n===== Train Ticket Booking System =====\n");
        printf("1. Display Train Layout\n");
        printf("2. Book Tickets (Manual)\n");
        printf("3. Book Tickets (Load from CSV)\n");
        printf("4. Display Confirmed Passengers\n");
        printf("5. Display Waitlist Passengers\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
                displayTrain(myTrain);
                break;
            case 2:
                bookTickets(myTrain, &passengerListHead, &waitlistHead);
                break;
            case 3:
                processCSVBookings(myTrain, &passengerListHead, &waitlistHead, "bookings.csv");
                break;
            case 4:
                displayPassengers(passengerListHead);
                break;
            case 5:
                displayWaitlist(waitlistHead);
                break;
            case 6:
                printf("Exiting the system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }while(choice != 6);

    return 0;
}