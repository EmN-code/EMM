
#include "xtimer.h" 
#include "ztimer.h" 
#include "ztimer.h"
#include "unistd.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net_gcoap.h"
#include "net_emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "irq.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"
#include "atomic_fetch_add"
#include "atomic_fetch_sub"
#include "thread_config.h"
#include "xtimer.h"
#include "irq.h"
#include "esp_wifi.h"
#include "esp_system.h"

void *thread_func1(void *arg) {
    (void)arg;
    while(1) {
        // Simulate work with a sleep
        xtimer_sleep(2);
    }
}

void *thread_func2(void *arg) {
    (void)arg;
    while(1) {
        // Simulate work with a sleep
        xtimer_sleep(2);
    }
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    if(thread1 <= KERNEL_PID_UNDEF || thread2 <= KERNEL_PID_UNDEF) {
        return -1; // Error creating threads
    }

    // Change priority of the threads
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 3);

    while (1) {
        // Main execution continues...
        xtimer_sleep(1);
    }

    return 0;
}