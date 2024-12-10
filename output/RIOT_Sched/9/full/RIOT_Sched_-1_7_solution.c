#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread_config.h>
#include <thread.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>

#define THREAD_STACKSIZE_MAIN    512
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 2)
#define THREAD_PRIORITY_SECOND   (THREAD_PRIORITY_MAIN - 1)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func_1(void *arg) {
    (void)arg;
    while (1) {
        // Sample task for thread 1
        puts("This is thread 1");
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return NULL;
}

void *thread_func_2(void *arg) {
    (void)arg;
    while (1) {
        // Sample task for thread 2
        puts("This is thread 2");
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_func_1, NULL, "thread_1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_SECOND, 0, thread_func_2, NULL, "thread_2");
    
    int rc;
    while (1) {
        rc = NetworkConnect(NULL, "192.168.1.1", 1883);
        if (rc != 0) {
            // Network connection failed, retry
            puts("Network connection failed, retrying...");
            ztimer_sleep(ZTIMER_USEC, 1000000);
            continue;
        } else {
            puts("Connected to network successfully.");
            break;
        }
    }
    
    // Change priorities
    thread_t *thr1 = thread_get(pid1);
    thread_t *thr2 = thread_get(pid2);

    sched_change_priority(thr1, THREAD_PRIORITY_SECOND);
    sched_change_priority(thr2, THREAD_PRIORITY_MAIN);
    
    return 0;
}