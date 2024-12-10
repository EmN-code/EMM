#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <event/callback.h>
#include <MQTTClient.h>

#define INTERVAL (1000000U)

static char thread_stack[THREAD_STACKSIZE_MAIN];

static void *thread_handler(void *arg) {
    (void)arg;
    while(1) {
        // Perform periodic task
        printf("Periodic task executing.\n");
        // Sleep for INTERVAL time
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("Starting periodic task example.\n");

    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "periodic_thread");

    return 0;
}