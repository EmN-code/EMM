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
#include <dht.h>

// Define constants
#define MAIN_QUEUE_SIZE     (8)
#define THREAD_PRIORITY     (THREAD_PRIORITY_MAIN - 1)
#define SLEEP_TIME_SECONDS  (5)

static msg_t msg_queue[MAIN_QUEUE_SIZE];

void *msg_sender(void *arg) {
    (void) arg;
    msg_t message;
    message.type = 0x1234;  // Arbitrary message type

    while (1) {
        // Prepare a message and send it
        printf("Sending message with type: %u\n", message.type);
        msg_send(&message, thread_getpid());
        // Sleep for defined interval
        xtimer_sleep(SLEEP_TIME_SECONDS);
    }
    return NULL;
}

void *msg_receiver(void *arg) {
    (void) arg;
    msg_t message;
    msg_init(&message); // Prepare message structure

    while (1) {
        // Block until a message is received
        msg_receive(&message);
        printf("Received message with type: %u\n", message.type);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);

    // Create the sender and receiver threads
    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    char receiver_stack[THREAD_STACKSIZE_DEFAULT];

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY, 0, msg_sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY, 0, msg_receiver, NULL, "receiver");

    // Keep the main thread running
    while (1) {
        sleep(1);
    }
    return 0;
}