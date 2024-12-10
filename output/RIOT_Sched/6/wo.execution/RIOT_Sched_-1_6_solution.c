#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "paho_mqtt.h"
#include "mma8x5x_params.h"
#include "stdlib.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "architecture.h"
#include "stdatomic.h"
#include "mbox.h"
#include "thread_arch.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "dht.h"
#include "dht_params.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE 1024
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_NORMAL)
#define THREAD_PRIORITY_SECOND (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE];
static char stack2[THREAD_STACKSIZE];

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread running with priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, thread_func, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_SECOND, 0, thread_func, NULL, "thread2");

    xtimer_sleep(5);
    
    // Simulating a priority change by restarting thread2 with a different priority
    thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func, NULL, "thread2_resumed");

    while (1) {
        xtimer_sleep(10);
    }
    return 0;
}