
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "paho_mqtt.h"
#include "ztimer.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "xtimer.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"

#define STACKSIZE 512
#define NUM_THREADS 2

static char stack[NUM_THREADS][STACKSIZE];
static kernel_pid_t pids[NUM_THREADS];

void *thread_func(void *arg) {
    int thread_id = (int)arg;
    printf("Thread %d started\n", thread_id);
    while (1) {
        // Simulate some work
        sleep(1);
    }
    return NULL;
}

void change_priority(kernel_pid_t pid, uint8_t new_priority) {
    thread_t *thread = thread_get_from_pid(pid);
    if (thread) {
        thread_set_priority(thread, new_priority);
    }
}

int main(void) {
    printf("Starting threads...\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        pids[i] = thread_create(stack[i], STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, thread_func, (void *)i, "thread");
    }

    // Change priority of the first thread after some time
    sleep(5);
    change_priority(pids[0], THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread 0\n");

    return 0;
}
