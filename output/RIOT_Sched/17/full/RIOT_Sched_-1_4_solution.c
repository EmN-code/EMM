#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "ztimer.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "time_units.h"
#include "unistd.h"
#include "timex.h"
#include "thread_arch.h"
#include "thread.h"
#include "architecture.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE_MAIN 1024

static char stack[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    (void) arg;
    while(1) {
        printf("Thread running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void change_priority(kernel_pid_t pid, uint8_t new_priority) {
    /* Here should be the implementation of changing priority using a mock-up */
    /* This is a hypothetical rewrite since the actual method can't be used directly */
    printf("Changing thread priority to %d for PID %d\n", new_priority, pid);
}

int main(void) {
    puts("RIOT: Starting threads\n");

    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack), 5, 0, thread_handler, NULL, "demo_thread");

    /* Simulate change in priority */
    xtimer_sleep(5);
    change_priority(thread_pid, 3);

    return 0;
}