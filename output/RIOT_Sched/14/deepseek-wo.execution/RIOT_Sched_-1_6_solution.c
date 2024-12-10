
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <stdatomic.h>
#include <stddef.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <event/thread.h>
#include <dht_params.h>
#include <netinet/in.h>
#include <msg.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <mbox.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <net/sock/util.h>
#include <time_units.h>
#include <kernel_defines.h>

#define STACKSIZE 1024
#define NUM_THREADS 2

msg_t msg_array[NUM_THREADS];

void thread_func(void *arg) {
    int id = (int)arg;
    printf("Thread %d started with priority %d\n", id, sched_get_priority(thread_getpid()));
    while (1) {
        // Simulate some work
        for (int i = 0; i < 1000000; i++) {}
        printf("Thread %d is running\n", id);
    }
}

int main(void) {
    msg_init_queue(msg_array, NUM_THREADS);

    char stacks[NUM_THREADS][STACKSIZE];
    kernel_pid_t pids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pids[i] = thread_create(stacks[i], STACKSIZE, THREAD_PRIORITY_MAIN, 0, thread_func, (void*)i, "thread");
    }

    // Change priority of the first thread
    sched_change_priority(thread_get(pids[0]), THREAD_PRIORITY_MAIN + 1);

    // Change priority of the second thread
    sched_change_priority(thread_get(pids[1]), THREAD_PRIORITY_MAIN - 1);

    printf("Main thread finished setting priorities\n");

    return 0;
}
