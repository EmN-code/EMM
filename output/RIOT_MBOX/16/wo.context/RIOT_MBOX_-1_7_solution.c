#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "msg.h"

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval
#define MAIN_QUEUE_SIZE (8)

static msg_t msg_queue[MAIN_QUEUE_SIZE];

void *message_sender(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;

    while (1) {
        /* Prepare the message */
        msg.type = 1;
        msg.content.value = (intptr_t)counter;
        printf("Sending message with value: %d\n", counter);

        /* Send the message */
        if (msg_send(&msg, thread_getpid()) <= 0) {
            puts("Error: unable to send the message\n");
        }

        /* Increase counter */
        counter++;
        
        /* Sleep for the defined interval */
        xtimer_sleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("ESP32 Message Box Example\n");

    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);
    
    /* Create the message sender thread */
    kernel_pid_t sender_pid = thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST,
                                            message_sender, NULL, "sender");

    /* Process messages in the main thread */
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", (uint32_t)msg.content.value);
    }

    return 0;
}