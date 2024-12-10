#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stddef.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread.h"
#include "mbox.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_LEVELS - 1)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_function(void *arg) {
    while (1) {
        // Simulate work
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, 
                                          (thread_task_func_t)thread_function, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, 
                                          (thread_task_func_t)thread_function, NULL, "thread2");

    if (thread1 != KERNEL_PID_UNDEF && thread2 != KERNEL_PID_UNDEF) {
        sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 1);
        sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 2);

        // Example of network reconnection logic
        Network network;
        int rc;
        while (1) {
            rc = NetworkConnect(&network, "192.168.1.1", 1883);
            if (rc != 0) {
                // Print error message and retry
                xtimer_sleep(1);
                continue;
            } else {
                // Connection successful
                break;
            }
        }
    }
    return 0;
}