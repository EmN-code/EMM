#include <stdio.h>
#include "periph/ztimer.h"
#include "msg.h"

#define INTERVAL (1000U)

static msg_t msg_queue[8];

// Thread function to handle incoming messages
void *msg_handler(void *arg) {
    msg_t msg;
    (void)arg;
    
    // Set up a message receive loop
    while (1) {
        // Receive message (blocking call)
        msg_receive(&msg);
        printf("Message received!\n");
    }

    return NULL;
}

void send_event(void) {
    msg_t msg;
    // Example payload, payload can be adjusted as needed
    msg.content.value = 42;
    
    printf("Sending message...\n");
    msg_send(&msg, thread_getpid());
}

int main(void) {
    printf("ESP32 Message Box Example\n");
    
    // Initialize the message queue
    msg_init_queue(msg_queue, 8);
    
    // Create a thread for handling messages
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  msg_handler, NULL, "msg_handler");

    // Run a timer to send messages at regular intervals
    ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    send_event();
    
    return 0;
}