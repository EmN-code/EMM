#include "ztimer.h"
#include "xtimer.h"
#include "thread_arch.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 6

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_handler(void *arg) {
    uint32_t count = 0;
    while (1) {
        // simulate work with a busy loop
        volatile uint32_t i;
        for (i = 0; i < 50000; ++i) {}
        count++;
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Create the first thread
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_handler, NULL, "thread1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        return -1;  // Failed to create thread
    }

    // Create the second thread
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0, thread_handler, NULL, "thread2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        return -1;  // Failed to create thread
    }

    // Simulate running by just changing thread priority for demonstration
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 1);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);

    // We use a while loop to simulate the main processing
    while (1) {
        // Place holder for main task processing
    }

    return 0;
}