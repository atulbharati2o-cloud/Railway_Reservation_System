#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tree_utils.h"

/*
 * QUICK NAVIGATION
 * 1) Common helpers
 * 2) Coach AVL helpers
 * 3) Seat AVL helpers
 * 4) Passenger AVL helpers
 */






// ============================ COMMON HELPERS ============================ //

static int maxInt(int a, int b) {
    return a > b ? a : b;
}

static void* xmalloc(size_t size) {
    void* ptr = malloc(size);
    if(ptr == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    return ptr;
}







// ============================ COACH AVL HELPERS ============================ //
// Node creation, balancing, insert/search/traversal for coach tree.

Coach* createCoachNode(int orderKey, int coachNumber, const char* coachType, int capacity) {
    Coach* node = (Coach*)xmalloc(sizeof(Coach));
    node->coachNumber = coachNumber;
    node->orderKey = orderKey;
    strcpy(node->coachType, coachType);
    node->capacity = capacity;
    node->seatList = NULL;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}


// -------------------------- Coach balance helpers -------------------------- //

static int coachHeight(Coach* node) {
    return node ? node->height : 0;
}

static int coachBalance(Coach* node) {
    return node ? coachHeight(node->left) - coachHeight(node->right) : 0;
}

static Coach* coachRotateRight(Coach* y) {
    Coach* x = y->left;
    Coach* t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = 1 + maxInt(coachHeight(y->left), coachHeight(y->right));
    x->height = 1 + maxInt(coachHeight(x->left), coachHeight(x->right));
    return x;
}

static Coach* coachRotateLeft(Coach* x) {
    Coach* y = x->right;
    Coach* t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = 1 + maxInt(coachHeight(x->left), coachHeight(x->right));
    y->height = 1 + maxInt(coachHeight(y->left), coachHeight(y->right));
    return y;
}


// ----------------------- Coach core tree operations ------------------------ //

Coach* insertCoachNode(Coach* root, Coach* node) {
    if(root == NULL) {
        return node;
    }

    if(node->orderKey < root->orderKey) {
        root->left = insertCoachNode(root->left, node);
    } else if(node->orderKey > root->orderKey) {
        root->right = insertCoachNode(root->right, node);
    } else {
        return root;
    }
    
    root->height = 1 + maxInt(coachHeight(root->left), coachHeight(root->right));
    int balance = coachBalance(root);

    if(balance > 1 && node->orderKey < root->left->orderKey) {
        return coachRotateRight(root);
    }
    if(balance < -1 && node->orderKey > root->right->orderKey) {
        return coachRotateLeft(root);
    }
    if(balance > 1 && node->orderKey > root->left->orderKey) {
        root->left = coachRotateLeft(root->left);
        return coachRotateRight(root);
    }
    if(balance < -1 && node->orderKey < root->right->orderKey) {
        root->right = coachRotateRight(root->right);
        return coachRotateLeft(root);
    }

    return root;
}

Coach* findCoachByNumber(Coach* root, int coachNumber) {
    // Coach numbers are not the AVL key, so we must search both subtrees.
    if(root == NULL) {
        return NULL;
    }
    if(root->coachNumber == coachNumber) {
        return root;
    }

    Coach* found = findCoachByNumber(root->left, coachNumber);
    if(found != NULL) {
        return found;
    }
    return findCoachByNumber(root->right, coachNumber);
}

void collectCoachesInOrder(Coach* root, Coach** arr, int* index) {
    // In-order traversal returns coaches in the same sequence used by the train layout.
    if(root == NULL) {
        return;
    }
    collectCoachesInOrder(root->left, arr, index);
    arr[(*index)++] = root;
    collectCoachesInOrder(root->right, arr, index);
}

int countCoaches(Coach* root) {
    if(root == NULL) {
        return 0;
    }
    return 1 + countCoaches(root->left) + countCoaches(root->right);
}

Coach* buildCoachTreeFromArray(Coach** arr, int start, int end) {
    if(start > end) {
        return NULL;
    }

    // Rebuild a balanced coach tree from an already ordered array.
    int mid = start + (end - start) / 2;
    Coach* root = arr[mid];
    root->left = buildCoachTreeFromArray(arr, start, mid - 1);
    root->right = buildCoachTreeFromArray(arr, mid + 1, end);
    root->height = 1 + maxInt(coachHeight(root->left), coachHeight(root->right));
    return root;
}







// ============================ SEAT AVL HELPERS ============================ //
// Node creation, balancing, insert/search/traversal for seat tree.

Seat* createSeatNode(int coachNumber, int seatNumber, const char* berthType) {
    Seat* node = (Seat*)xmalloc(sizeof(Seat));
    node->seatNumber = seatNumber;
    strcpy(node->berthType, berthType);
    node->coachNumber = coachNumber;
    node->isBooked = FALSE;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}


// --------------------------- Seat balance helpers -------------------------- //

static int seatHeight(Seat* node) {
    return node ? node->height : 0;
}

static int seatBalance(Seat* node) {
    return node ? seatHeight(node->left) - seatHeight(node->right) : 0;
}

static Seat* seatRotateRight(Seat* y) {
    Seat* x = y->left;
    Seat* t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = 1 + maxInt(seatHeight(y->left), seatHeight(y->right));
    x->height = 1 + maxInt(seatHeight(x->left), seatHeight(x->right));
    return x;
}

static Seat* seatRotateLeft(Seat* x) {
    Seat* y = x->right;
    Seat* t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = 1 + maxInt(seatHeight(x->left), seatHeight(x->right));
    y->height = 1 + maxInt(seatHeight(y->left), seatHeight(y->right));
    return y;
}


// ------------------------ Seat core tree operations ------------------------ //

Seat* insertSeatNode(Seat* root, Seat* node) {
    // Insert the seat by seatNumber and rebalance the coach's seat tree.
    if(root == NULL) {
        return node;
    }

    if(node->seatNumber < root->seatNumber) {
        root->left = insertSeatNode(root->left, node);
    } else if(node->seatNumber > root->seatNumber) {
        root->right = insertSeatNode(root->right, node);
    } else {
        return root;
    }

    root->height = 1 + maxInt(seatHeight(root->left), seatHeight(root->right));
    int balance = seatBalance(root);

    // LL case
    if(balance > 1 && node->seatNumber < root->left->seatNumber) {
        return seatRotateRight(root);
    }
    // RR case
    if(balance < -1 && node->seatNumber > root->right->seatNumber) {
        return seatRotateLeft(root);
    }
    // LR case
    if(balance > 1 && node->seatNumber > root->left->seatNumber) {
        root->left = seatRotateLeft(root->left);
        return seatRotateRight(root);
    }
    // RL case
    if(balance < -1 && node->seatNumber < root->right->seatNumber) {
        root->right = seatRotateRight(root->right);
        return seatRotateLeft(root);
    }

    return root;
}

Seat* findSeatByNumber(Seat* root, int seatNumber) {
    // Seats are keyed by seatNumber, so search can follow the AVL ordering directly.
    if(root == NULL) {
        return NULL;
    }
    if(root->seatNumber == seatNumber) {
        return root;
    }
    if(seatNumber < root->seatNumber) {
        return findSeatByNumber(root->left, seatNumber);
    }
    return findSeatByNumber(root->right, seatNumber);
}

void collectSeatsInOrder(Seat* root, Seat** arr, int* index) {
    if(root == NULL) {
        return;
    }
    collectSeatsInOrder(root->left, arr, index);
    arr[(*index)++] = root;
    collectSeatsInOrder(root->right, arr, index);
}

int countSeats(Seat* root) {
    if(root == NULL) {
        return 0;
    }
    return 1 + countSeats(root->left) + countSeats(root->right);
}







// ========================== PASSENGER AVL HELPERS ========================= //
// Node creation, balancing, insert/delete/search/traversal for passenger trees.

Passenger* createPassengerNode(const char* name, const char* gender, const char* DOB, int age) {
    Passenger* node = (Passenger*)xmalloc(sizeof(Passenger));
    strcpy(node->name, name);
    strcpy(node->gender, gender);
    strcpy(node->DOB, DOB);
    node->age = age;
    node->pnrNumber = 0;
    node->coachNumber = -1;
    node->seatNumber = -1;
    node->berthType[0] = '\0';
    node->coachType[0] = '\0';
    node->bookingOrder = 0;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}


// ------------------------ Passenger balance helpers ------------------------ //

static int passengerHeight(Passenger* node) {
    return node ? node->height : 0;
}

static int passengerBalance(Passenger* node) {
    return node ? passengerHeight(node->left) - passengerHeight(node->right) : 0;
}

static Passenger* passengerRotateRight(Passenger* y) {
    Passenger* x = y->left;
    Passenger* t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = 1 + maxInt(passengerHeight(y->left), passengerHeight(y->right));
    x->height = 1 + maxInt(passengerHeight(x->left), passengerHeight(x->right));
    return x;
}

static Passenger* passengerRotateLeft(Passenger* x) {
    Passenger* y = x->right;
    Passenger* t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = 1 + maxInt(passengerHeight(x->left), passengerHeight(x->right));
    y->height = 1 + maxInt(passengerHeight(y->left), passengerHeight(y->right));
    return y;
}


// --------------------- Passenger insert key comparators -------------------- //

static int compareBookingOrder(const Passenger* a, const Passenger* b) {
    if(a->bookingOrder < b->bookingOrder) {
        return -1;
    }
    if(a->bookingOrder > b->bookingOrder) {
        return 1;
    }
    return 0;
}

static int compareSeatNumber(const Passenger* a, const Passenger* b) {
    if(a->seatNumber < b->seatNumber) {
        return -1;
    }
    if(a->seatNumber > b->seatNumber) {
        return 1;
    }
    return 0;
}



// --------------------- Passenger insert helper/wrappers -------------------- //
// Shared AVL insert helper for confirmed passengers and waitlist passengers.
static Passenger* passengerInsertInternal(Passenger* root, Passenger* node, int (*compare)(const Passenger*, const Passenger*)) {
    // Shared AVL insert helper for confirmed passengers and waitlist passengers.
    // The caller decides the sort key through the compare function.
    if(root == NULL) {
        return node;
    }

    if(compare(node, root) < 0) {
        root->left = passengerInsertInternal(root->left, node, compare);
    } else if(compare(node, root) > 0) {
        root->right = passengerInsertInternal(root->right, node, compare);
    } else {
        return root;
    }

    root->height = 1 + maxInt(passengerHeight(root->left), passengerHeight(root->right));
    int balance = passengerBalance(root);

    if(balance > 1 && compare(node, root->left) < 0) {
        return passengerRotateRight(root);
    }
    if(balance < -1 && compare(node, root->right) > 0) {
        return passengerRotateLeft(root);
    }
    if(balance > 1 && compare(node, root->left) > 0) {
        root->left = passengerRotateLeft(root->left);
        return passengerRotateRight(root);
    }
    if(balance < -1 && compare(node, root->right) < 0) {
        root->right = passengerRotateRight(root->right);
        return passengerRotateLeft(root);
    }

    return root;
}

Passenger* insertPassengerByBookingOrder(Passenger* root, Passenger* node) {
    // Confirmed passengers are stored by booking order, so display follows reservation order.
    return passengerInsertInternal(root, node, compareBookingOrder);
}

Passenger* insertWaitlistPassenger(Passenger* root, Passenger* node) {
    // Waitlist passengers use seatNumber as the waiting-list position.
    return passengerInsertInternal(root, node, compareSeatNumber);
}


// ----------------------- Passenger search/extract ops ---------------------- //

Passenger* findPassengerByBookingOrder(Passenger* root, int bookingOrder) {
    // Booking order is unique, so it is the safest key for deletion and lookup.
    if(root == NULL) {
        return NULL;
    }
    if(root->bookingOrder == bookingOrder) {
        return root;
    }
    if(bookingOrder < root->bookingOrder) {
        return findPassengerByBookingOrder(root->left, bookingOrder);
    }
    return findPassengerByBookingOrder(root->right, bookingOrder);
}

static Passenger* passengerDeleteMinNode(Passenger* root, Passenger** removed) {
    if(root == NULL) {
        return NULL;
    }

    if(root->left == NULL) {
        *removed = root;
        return root->right;
    }

    root->left = passengerDeleteMinNode(root->left, removed);

    root->height = 1 + maxInt(passengerHeight(root->left), passengerHeight(root->right));
    int balance = passengerBalance(root);

    if(balance > 1 && passengerBalance(root->left) >= 0) {
        return passengerRotateRight(root);
    }
    if(balance > 1 && passengerBalance(root->left) < 0) {
        root->left = passengerRotateLeft(root->left);
        return passengerRotateRight(root);
    }
    if(balance < -1 && passengerBalance(root->right) <= 0) {
        return passengerRotateLeft(root);
    }
    if(balance < -1 && passengerBalance(root->right) > 0) {
        root->right = passengerRotateRight(root->right);
        return passengerRotateLeft(root);
    }

    return root;
}

Passenger* extractMinPassenger(Passenger** root) {
    // Remove the earliest waitlisted passenger from the AVL tree.
    Passenger* removed = NULL;
    *root = passengerDeleteMinNode(*root, &removed);
    if(removed != NULL) {
        removed->left = NULL;
        removed->right = NULL;
        removed->height = 1;
    }
    return removed;
}





// ------------------------ Passenger delete operations ---------------------- //
// Delete a confirmed passenger by booking order while preserving AVL balance.
static Passenger* passengerDeleteInternal(Passenger* root, int bookingOrder) {
    if(root == NULL) {
        return NULL;
    }

    if(bookingOrder < root->bookingOrder) {
        root->left = passengerDeleteInternal(root->left, bookingOrder);
    } else if(bookingOrder > root->bookingOrder) {
        root->right = passengerDeleteInternal(root->right, bookingOrder);
    } else {
        // Node with only one child or no child
        if(root->left == NULL || root->right == NULL) {
            Passenger* child = root->left ? root->left : root->right;
            if(child == NULL) {
                free(root);
                return NULL;
            }
            *root = *child;
            free(child);
        } else {
            // Node with 2 children
            Passenger* successor = root->right;
            while(successor->left != NULL) {
                successor = successor->left;
            }

            strcpy(root->name, successor->name);
            strcpy(root->gender, successor->gender);
            strcpy(root->DOB, successor->DOB);
            root->age = successor->age;
            root->pnrNumber = successor->pnrNumber;
            root->coachNumber = successor->coachNumber;
            root->seatNumber = successor->seatNumber;
            strcpy(root->berthType, successor->berthType);
            strcpy(root->coachType, successor->coachType);
            root->bookingOrder = successor->bookingOrder;

            root->right = passengerDeleteInternal(root->right, successor->bookingOrder);
        }
    }

    if(root == NULL) {
        return NULL;
    }

    root->height = 1 + maxInt(passengerHeight(root->left), passengerHeight(root->right));
    int balance = passengerBalance(root);

    if(balance > 1 && passengerBalance(root->left) >= 0) {
        return passengerRotateRight(root);
    }
    if(balance > 1 && passengerBalance(root->left) < 0) {
        root->left = passengerRotateLeft(root->left);
        return passengerRotateRight(root);
    }
    if(balance < -1 && passengerBalance(root->right) <= 0) {
        return passengerRotateLeft(root);
    }
    if(balance < -1 && passengerBalance(root->right) > 0) {
        root->right = passengerRotateRight(root->right);
        return passengerRotateLeft(root);
    }

    return root;
}

Passenger* deletePassengerByBookingOrder(Passenger* root, int bookingOrder) {
    return passengerDeleteInternal(root, bookingOrder);
}


// ---------------------- Passenger traversal/filter ops --------------------- //

void collectPassengersInOrder(Passenger* root, Passenger** arr, int* index) {
    // In-order traversal gives confirmed passengers in booking order.
    if(root == NULL) {
        return;
    }
    collectPassengersInOrder(root->left, arr, index);
    arr[(*index)++] = root;
    collectPassengersInOrder(root->right, arr, index);
}

void collectPassengersByPNR(Passenger* root, int pnr, Passenger** arr, int* index) {
    if(root == NULL) {
        return;
    }
    collectPassengersByPNR(root->left, pnr, arr, index);
    if(root->pnrNumber == pnr) {
        arr[(*index)++] = root;
    }
    collectPassengersByPNR(root->right, pnr, arr, index);
}

void collectPassengersByCoachNumber(Passenger* root, int coachNumber, Passenger** arr, int* index) {
    // Coach number is also a filter, so we scan the tree and collect matching passengers.
    if(root == NULL) {
        return;
    }
    collectPassengersByCoachNumber(root->left, coachNumber, arr, index);
    if(root->coachNumber == coachNumber) {
        arr[(*index)++] = root;
    }
    collectPassengersByCoachNumber(root->right, coachNumber, arr, index);
}

int countPassengers(Passenger* root) {
    if(root == NULL) {
        return 0;
    }
    return 1 + countPassengers(root->left) + countPassengers(root->right);
}


// --------------------- Passenger rebuild/waitlist ops --------------------- //

Passenger* buildPassengerTreeFromArray(Passenger** arr, int start, int end) {
    if(start > end) {
        return NULL;
    }

    int mid = start + (end - start) / 2;
    Passenger* root = arr[mid];
    root->left = buildPassengerTreeFromArray(arr, start, mid - 1);
    root->right = buildPassengerTreeFromArray(arr, mid + 1, end);
    root->height = 1 + maxInt(passengerHeight(root->left), passengerHeight(root->right));
    return root;
}

// Reassign waitlist positions in sorted order after promotions or cancellations.
void rebuildWaitlistNumbers(Passenger** root) {
    if(root == NULL || *root == NULL) {
        return;
    }

    int count = countPassengers(*root);
    Passenger** arr = (Passenger**)xmalloc(sizeof(Passenger*) * count);
    int index = 0;
    collectPassengersInOrder(*root, arr, &index);

    for(int i = 0; i < count; i++) {
        arr[i]->seatNumber = i + 1;
        arr[i]->left = NULL;
        arr[i]->right = NULL;
        arr[i]->height = 1;
    }

    *root = buildPassengerTreeFromArray(arr, 0, count - 1);
    free(arr);
}
