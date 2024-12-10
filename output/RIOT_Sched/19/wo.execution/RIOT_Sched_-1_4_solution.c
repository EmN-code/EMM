#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "unistd.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "architecture.h"
#include "MQTTClient.h"

#define STACKSIZE_MAIN_THREAD 1024
#define STACKSIZE_SECONDARY_THREAD 1024

char stack[STACKSIZE_SECONDARY_THREAD];

void *secondary_thread(void *arg) {
    (void)arg;
    for (;;) {
        printf("Secondary thread running with priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t secondary_thread_pid;

    // Create a secondary thread
    secondary_thread_pid = thread_create(
        stack,
        sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        secondary_thread,
        NULL,
        "secondary_thread"
    );

    if (secondary_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating secondary thread\n");
        return 1;
    }

    for (;;) {
        printf("Main thread running with priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(3);

        uint8_t new_priority = THREAD_PRIORITY_MAIN + 1;
        sched_change_priority(thread_get(secondary_thread_pid), new_priority);
        printf("Changed secondary thread priority to %d\n", new_priority);
    }

    return 0;
}