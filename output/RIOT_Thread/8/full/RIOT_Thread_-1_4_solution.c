#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "MQTTClient.h"
#include "time_units.h"

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1) 
#define THREAD_STACKSIZE_MAIN   THREAD_STACKSIZE_DEFAULT

static char thread_stack[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Received message type: %u\n", msg.type);
        }
    }
    return NULL;
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    kernel_pid_t thread_pid = thread_create(
        thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST, thread_handler, NULL, "message_handler");

    if (thread_pid < 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    msg_t msg;
    msg.type = 1;

    while (1) {
        msg_send(&msg, thread_pid);
        printf("Sent message type: %u\n", msg.type);
        xtimer_sleep(2);
    }

    return 0;
}
