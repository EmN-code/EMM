
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <unistd.h>
#include <stdatomic.h>
#include <MQTTClient.h>

#define STACKSIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static msg_t msg_queue[8];

void periodic_callback(void) {
    printf("Periodic callback triggered!\n");
}

void* periodic_thread(void* arg) {
    (void)arg;
    while (1) {
        periodic_callback();
        xtimer_sleep(1); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    thread_create(stack, sizeof(stack), PRIORITY, 0, periodic_thread, NULL, "periodic_thread");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle incoming messages if needed
    }

    return 0;
}
