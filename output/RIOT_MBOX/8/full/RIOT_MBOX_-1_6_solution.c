#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "string.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "netinet/in.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "dht.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

void initialize_mbox(void) {
    // An example array to store message info (in practice this must be sized)
    msg_t msg_queue[4];  // Ensure this is a power of two for msg_init_queue
    msg_init_queue(msg_queue, 4);
}

void* sender_thread(void* arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1234; // Example payload content

    while (1) {
        msg_send(&msg, thread_getpid());
        xtimer_sleep(2); // Sleep for 2 seconds before sending the next message
    }
    
    return NULL; // To satisfy the function definition
}

void* receiver_thread(void* arg) {
    (void)arg;
    msg_t msg;
    
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return NULL; // To satisfy the function definition
}

int main(void) {
    initialize_mbox();

    char sender_stack[THREAD_STACKSIZE_MAIN];
    char receiver_stack[THREAD_STACKSIZE_MAIN];

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}