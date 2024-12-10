#include <ztimer.h>   // For timer functions
#include <msg.h>      // For message functions
#include <stdio.h>    // For standard input/output functions

#define INTERVAL (1000000UL)  // Define interval as 1 second in microseconds
#define MSG_QUEUE_SIZE (8)   // Define size of the message queue

static msg_t msg_queue[MSG_QUEUE_SIZE];  // Message queue array
static ztimer_t timer;       // Timer object

// Function that simulates sending a message and receiving it
void send_receive_message(void) {
    printf("Sending and receiving messages at intervals...\n");

    // Allocate a message structure
    msg_t msg;
    msg.content.value = 0;  // Initialize message content to zero

    // Infinite loop to simulate continuous operation
    while (1) {
        // Wait for the timer to expire
        ztimer_sleep(ZTIMER_SEC, INTERVAL / 1000000UL);

        // Send the message
        printf("Sending message with value: %d\n", msg.content.value);
        msg_send(&msg, msg_queue);

        // Receive the message
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);

        // Increment message content
        msg.content.value++;
    }
}

int main(void) {
    printf("RIOT ESP32 Message Box Example\n");

    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the timer and set the target time
    ztimer_set_msg(&timer, ZTIMER_SEC, INTERVAL / 1000000UL, (msg_t*)msg_queue);

    // Run the message sending and receiving function
    send_receive_message();

    return 0;
}
