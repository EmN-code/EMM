
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <MQTTClient.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <net/emcute.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <mma8x5x_params.h>

#define STACKSIZE 512
#define NUM_THREADS 2

char stacks[NUM_THREADS][STACKSIZE];
kernel_pid_t pids[NUM_THREADS];

void *thread_func(void *arg) {
    int id = (int)arg;
    printf("Thread %d started\n", id);
    while (1) {
        printf("Thread %d running\n", id);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_t msg_queue[NUM_THREADS];
    msg_init_queue(msg_queue, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        pids[i] = thread_create(stacks[i], STACKSIZE, PRIORITY_MAIN - 1, 0, thread_func, (void *)i, "thread");
    }

    xtimer_sleep(5);

    for (int i = 0; i < NUM_THREADS; i++) {
        uint8_t old_priority = thread_get_priority(thread_get_from_pid(pids[i]));
        printf("Thread %d old priority: %d\n", i, old_priority);
        thread_set_priority(pids[i], old_priority + 1);
        uint8_t new_priority = thread_get_priority(thread_get_from_pid(pids[i]));
        printf("Thread %d new priority: %d\n", i, new_priority);
    }

    return 0;
}
