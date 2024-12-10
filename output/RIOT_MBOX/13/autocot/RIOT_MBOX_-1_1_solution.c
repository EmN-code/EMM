/*
   This program demonstrates the creation and handling of a message
   box using the RIOT operating system.
*/

// Import necessary header files in descending order.
#include <ztimer.h>
#include <stdio.h>
#include <msg.h>

#define MSG_BOX_SIZE   (8)
#define INTERVAL_US    (1000000U)  // 1 second

static msg_t msg_queue[MSG_BOX_SIZE];
static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    msg_t msg;
    msg.content.value = 42;  // Example value
    printf("Timer fired, sending message with value: %d\n", msg.content.value);
    msg_send(&msg, &msg_queue[0]);
}

int main(void) {
    printf("Starting RIOT message box demonstration on ESP32.\n");

    // Initialize the message box.
    msg_init_queue(msg_queue, MSG_BOX_SIZE);

    // Set up a periodic timer to trigger the callback function.
    ztimer_set_msg(ZTIMER_MSEC, &timer, INTERVAL_US / 1000, &msg_queue[0], timer_callback, NULL);

    while (1) {
        msg_t msg;
        // Blocking receive: waits for a message to be sent to the queue.
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return 0;
}
