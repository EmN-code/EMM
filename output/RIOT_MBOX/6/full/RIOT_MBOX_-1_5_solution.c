#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <mma8x5x.h>
#include <thread.h>
#include <arpa/inet.h>
#include <timex.h>

// Define the interval for the message event
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

// Thread stack size
#define STACKSIZE (THREAD_STACKSIZE_MAIN)

// Message queue size
#define MSG_QUEUE_SIZE (8U)

// Instantiate message queue
static msg_t _msg_queue[MSG_QUEUE_SIZE];

// Function to periodically send a message
void *_send_event_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x10; // Custom message type

    while (1) {
        // Send message to self
        msg_send(&msg, sched_active_pid);
        printf("Event sent!\n");
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(_msg_queue, MSG_QUEUE_SIZE);

    // Create a thread to send messages periodically
    char send_event_thread_stack[STACKSIZE];
    thread_create(send_event_thread_stack, sizeof(send_event_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, _send_event_thread, NULL, "send_event_thread");

    // Process messages
    msg_t msg;
    while (1) {
        // Wait for a message
        msg_receive(&msg);
        if (msg.type == 0x10) {
            printf("Event received!\n");
        }
    }

    return 0;
}