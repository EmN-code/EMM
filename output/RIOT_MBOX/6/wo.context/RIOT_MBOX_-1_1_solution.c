#include <ztimer.h>             // For timer functionality
#include <msg.h>               // For message box functionality
#include <stdio.h>             // For standard IO operations

#define INTERVAL 1000000       // Define the interval between messages in microseconds
#define MBOX_SIZE 4            // Define the size of the message box

static msg_t _msg_queue[MBOX_SIZE]; // Message queue storage
static ztimer_t timer;              // Timer
static msg_t msg;                  // Message

void timer_handler(void *arg) {
    (void)arg; // Prevent unused variable warning
    printf("Message sent!\n");
    msg_send(&msg, NULL, MBOX_SIZE); // Send the message to the message box
}

int main(void) {
    msg_init_queue(_msg_queue, MBOX_SIZE); // Initialize the message box

    ztimer_set_periodic(ZTIMER_USEC, &timer, INTERVAL, timer_handler, NULL);
    printf("Starting message box example.\n");

    while (1) {
        msg_receive(&msg); // Receive messages
        printf("Message received!\n");
    }

    return 0;
}