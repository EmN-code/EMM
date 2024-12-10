#include "xtimer.h"
#include "unistd.h"
#include "typedefs.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdint.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "string.h"
#include "timex.h"
#include "time_units.h"
#include "stdatomic.h"

#define INTERVAL (1U * US_PER_SEC)
#define QUEUE_SIZE 4

static msg_t msg_queue[QUEUE_SIZE];

static void send_message(void) {
    msg_t msg;
    msg.type = 1; // Define a message type
    while (1) {
        xtimer_usleep(INTERVAL);
        printf("Sending message...\n");
        if (msg_send(&msg, msg_query_pid) != 1) {
            printf("Error: Could not send message.\n");
        }
    }
}

static void receive_message(void) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message of type: %d\n", msg.type);
    }
}

int main(void) {
    printf("Initializing message queue...\n");
    msg_init_queue(msg_queue, QUEUE_SIZE);
    
    printf("Starting message sender thread...\n");
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, send_message, NULL, "sender");

    printf("Starting message receiver...\n");
    receive_message();

    return 0;
}