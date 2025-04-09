/* COP 3502C Assignment 2
* This program is written by: Ruba Ismail
* UCF ID: 5186181
* Date: 2/5/2025
* Class: COP 3205C, Professor Tanvir
* Purpose: process ticket lines as queues
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EMPTY -1
#define MAXSTRING 16


typedef struct Customer {
    char *lastName;
    int lineNumber;
    int arrivalTime;
    int numTickets;
}Customer;

typedef struct node {
    Customer* data;
    struct node* next;
}node;

typedef struct queue {
    struct node* front;
    struct node* back;
}queue;


void init(queue *qPtr);
int enqueue(queue* qPtr, Customer *customer);
Customer* dequeue(queue* qPtr);
Customer* peek(queue* qPtr);
int empty(queue* qPtr);

Customer* createCustomer(int arrivalTime, int lineNumber, char lName[], int numTickets);
void freeCustomer(Customer *c);
int allQueuesEmpty(queue ticketLine[]);
void processCustomers(queue ticketLine[]);


int main(void) {

    int numTestCases = 0;
    scanf("%d", &numTestCases);

    while (numTestCases--) {

        int numCustomers = 0;
        scanf("%d", &numCustomers);

        queue ticketLine[12];     // 12 queues, one for each ticket line
        for (int i = 0; i < 12; i ++) {
            init(&ticketLine[i]);
        }

        for (int i = 0; i < numCustomers; i++) {
            int arrivalTime, lineNumber, numTickets = 0;
            char tempName[MAXSTRING];

            scanf("%d", &arrivalTime);
            scanf("%d", &lineNumber);
            scanf("%s", tempName);
            scanf("%d", &numTickets);

            Customer *customer = createCustomer(arrivalTime, lineNumber, tempName, numTickets);

            // add the customer to the back of a queue based on line number
            enqueue(&ticketLine[lineNumber-1], customer);
        }

        processCustomers(ticketLine);
        printf("======\n");

        // frees any remaining customers
        for (int i = 0; i < 12; i ++) {
            while (!empty(&ticketLine[i])) {
                Customer *served = dequeue(&ticketLine[i]);
                freeCustomer(served);
            }
        }
    }
    return 0;
}


// creates a customer struct and fills out its data
Customer* createCustomer(int arrivalTime, int lineNumber, char lName[], int numTickets) {

    Customer *customer = malloc(sizeof(Customer));
    customer->arrivalTime = arrivalTime;
    customer->lineNumber = lineNumber;
    customer->numTickets = numTickets;

    customer->lastName = malloc(strlen(lName)+1 * sizeof(char));
    strcpy(customer->lastName, lName);

    return customer;
}

void freeCustomer(Customer *c) {
    free(c->lastName);
    free(c);
}


// loops through all queues to find the customer with the lowest arrival time, creates a pointer to the customer to serve and dequeues it
void processCustomers(queue ticketLine[]) {

    int curTime = 0;

    while (!allQueuesEmpty(ticketLine)) {

        int minTickets = 101;
        int selectedLine = -1;
        Customer *selectedCustomer = NULL;

        for (int i = 0; i < 12; i++) {
            if (!empty(&ticketLine[i])) {
                Customer *customer = peek(&ticketLine[i]);

                // looking at customers that arrived before or at curTime
                if (customer->arrivalTime <= curTime) {
                                                            // choosing from the lowest line number if multiple customers have same # tickets
                    if (customer->numTickets < minTickets || customer->numTickets == minTickets && customer->lineNumber < selectedLine) {

                        minTickets = customer->numTickets;
                        selectedLine = customer->lineNumber;
                        selectedCustomer = customer;
                  }
                }
            }
        }

        // moving the curTime forward if all customers have an arrival time greater than current time
        if (selectedCustomer == NULL) {

            int earliestTime = pow(10, 9) + 1;
            for (int i = 0; i < 12; i++) {
                if (!empty(&ticketLine[i])) {
                    Customer *c = peek(&ticketLine[i]);
                    if (c->arrivalTime < earliestTime) {
                        earliestTime = c->arrivalTime;
                        selectedLine = c->lineNumber;
                        selectedCustomer = c;
                    }
                }
            }
            curTime = earliestTime;
        }
        // serving the selected customer
        Customer *customerServed = dequeue(&ticketLine[selectedLine - 1]);
        //int processingTime = curTime + (20 + (customerServed->numTickets * 10));
        //curTime = processingTime;
        int startTime = 0;

        if (curTime < customerServed->arrivalTime) {
            startTime = customerServed->arrivalTime;  // Wait until customer arrives
        } else {
            startTime = curTime;  // Start immediately if already past arrival time
        }

        int checkoutTime = startTime + 20 + (customerServed->numTickets * 10);

        curTime = checkoutTime;  // Update simulation time

        printf("%s left the counter at time %d from line %d.\n", customerServed->lastName, checkoutTime, customerServed->lineNumber);

        freeCustomer(customerServed);
    }
}


// checks to see if all 12 queues are empty
int allQueuesEmpty(queue ticketLine[]) {
    for (int i = 0; i < 12; i++) {
        if (!empty(&ticketLine[i])) return 0; // At least one queue is not empty
    }
    return 1;   // All queues are empty
}


// initializes a queue
void init(struct queue* qPtr) {
    qPtr->front = NULL;
    qPtr->back = NULL;
}

// checks if a queue is empty
int empty(queue *qPtr) {
    return (qPtr->front == NULL);
}

// adds a new node to the back of the queue
int enqueue(queue *qPtr, Customer *customer) {
    node *newNode = (node*) malloc(sizeof(node));
    newNode->data = customer;
    newNode->next = NULL;

    // if the queue is empty, the new node is the front and back of the queue
    if (empty(qPtr)) {
        qPtr->front = newNode;
        qPtr->back = newNode;
        return 1;
    }

    // otherwise add the new node to the back of the queue
    else {
        qPtr->back->next = newNode;
        qPtr->back = newNode;
        return 1;
    }
}

// removed the first node at the front of the queue, returns that node
Customer* dequeue(queue *qPtr) {

    if (empty(qPtr))
        return EMPTY;

    Customer *customer = qPtr->front->data;
    node *temp = qPtr->front;
    qPtr->front = qPtr->front->next;
    free(temp);

    // set the back to NULL if the queue becomes empty after the removal
    if (empty(qPtr))
        qPtr->back = NULL;

    return customer;
}

// returns the data of the node first in queue, but does not remove it
Customer* peek(queue *qPtr) {

    if (empty(qPtr))
        return EMPTY;

    else
        return qPtr->front->data;
}
