#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structures.h"
#include "../include/display.h"
#include "../include/train.h"
#include "../include/booking.h"
#include "../include/sorting.h"

int main(){
    Coach* myTrain = initializeTrain();

    Passenger* passengerListHead = NULL;
    WaitlistManager wm;

    initializeWaitlistManager(&wm);

    int choice;
    do{
        printf("\n===== Train Ticket Booking System =====\n");
        printf("1. Display Train Layout\n");
        printf("2. Book Tickets (Manual)\n");
        printf("3. Book Tickets (Load from CSV)\n");
        printf("4. Display Confirmed Passengers\n");
        printf("5. Display Waitlist Passengers\n");
        printf("6. Sort Passengers by Coach Number and Seat Number\n");
        printf("7. Display Passengers of a Particular Coach (Sorted by Name)\n");
        printf("8. Display All Passengers sorted by Name\n");
        printf("9. Display Passengers sorted by Coach Number\n");
        printf("10. Display All Passengers who were booked L or SL Berths\n");
        printf("11. Display Senior Citizens(age > 60) without L or SL Berths\n");
        printf("12. Display Available Seats in Each Coach\n");
        printf("13. Reverse Train Layout\n");
        printf("14. Display Passengers by PNR\n");
        printf("15. Cancel Ticket\n");
        printf("16. Exit\n");


        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
                displayTrain(myTrain);
                break;
            case 2:
                bookTicketsUI(myTrain, &passengerListHead, &wm);
                break;
            case 3:
                processCSVBookings(myTrain, &passengerListHead, &wm, "bookings.csv");
                break;
            case 4:
                displayPassengers(passengerListHead);
                break;
            case 5:
                displayAllWaitlist(&wm);
                break;
            case 6:
                passengerListHead = sortPassengersByCoachAndSeat(passengerListHead);
                displayPassengers(passengerListHead);
                break;
            case 7:
                displayPassengersOfCoachSortedByName(passengerListHead);
                break;
            case 8:
                displayAllPassengersSortedByName(passengerListHead);
                break;
            case 9:
                displayAllPassengersSortedByCoachNumber(passengerListHead);
                break;
            case 10:
                displayAllPassengersIn_L_or_SL_Berths(passengerListHead);
                break;
            case 11:
                displaySeniorCitizensWithoutL_or_SL_Berths(passengerListHead);
                break;
            case 12:
                displayNumberOfAvailableSeatsInEachCoach(myTrain);
                break;
            case 13:
                reverseTrainLayout(myTrain);
                displayTrain(myTrain);
                break;
            case 14: {
                int pnr;
                printf("Enter PNR number: ");
                scanf("%d", &pnr);
                displayPassengersByPNR(passengerListHead, pnr);
                break;
            }
            case 15:
                cancelTicketUI(&passengerListHead, &wm, myTrain);
                break;
            case 16:
                printf("Exiting the system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 16);
    return 0;
}