#include "../include/structures.h"
#include "../include/display.h"
#include "../include/train.h"

int main(){
    Coach* myTrain = initializeTrain();
    displayTrain(myTrain);

    return 0;
}