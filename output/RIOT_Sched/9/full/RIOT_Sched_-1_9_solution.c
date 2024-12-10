#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "stdio.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "kernel_defines.h"
#include "mqttclient.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "paho_mqtt.h"
#include "sched.h"

void *thread_func(void *arg) {
    while (1) {
        // Simulating thread activity
        sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread_pid;

    // Create two threads
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                               thread_func, NULL, "thread1");
    printf("Thread 1 PID: %d\n", thread_pid);

    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 2, 0,
                               thread_func, NULL, "thread2");
    printf("Thread 2 PID: %d\n", thread_pid);

    // Change priority of the first thread
    thread_t *thread = thread_get(thread_pid);
    if (!thread) {
        printf("Error retrieving thread control block.\n");
        return -1;
    }
    sched_change_priority(thread, THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of thread 2.\n");

    return 0;
}