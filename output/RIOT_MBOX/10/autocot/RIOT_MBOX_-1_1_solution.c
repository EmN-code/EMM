#include <ztimer.h>  // For timer functionalities in RIOT
#include <msg.h>    // For message box functionalities in RIOT
#include <stdio.h>  // For input/output functions

#define MAIN_QUEUE_SIZE     (8)  // Define size for the main message queue

static msg_t msg_queue[MAIN_QUEUE_SIZE];  // Message queue for storing messages

#define INTERVAL            (1LU * ZTIMER_MSEC)  // Timer interval set at 1 second

void send_event(void) {
    static uint32_t counter = 0;  // Counter to track the number of messages sent
    msg_t msg;  // Message variable
    msg.content.value = counter;  // Set message content with current counter value
    msg_send(&msg, thread_getpid());  // Send message to the current thread
    printf("Sent message with counter value: %lu\n", (unsigned long)counter);
    counter++;  // Increment counter
}

int main(void) {
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);  // Initialize the message queue
    printf("Message box example with ESP32 and RIOT\n");

    ztimer_t timer;  // Timer variable
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL, send_event, NULL);  // Start timer

    while (1) {
        msg_t msg;  // Variable to receive messages
        msg_receive(&msg);  // Receive a message and block until message is received
        printf("Received message with counter value: %lu\n", (unsigned long)msg.content.value);
    }

    return 0;  // Not reached, included to comply with standard main function signature
}