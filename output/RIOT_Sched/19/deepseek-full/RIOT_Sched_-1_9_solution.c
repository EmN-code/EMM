
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define STACKSIZE 512
#define NUM_THREADS 2

static char stacks[NUM_THREADS][STACKSIZE];
static kernel_pid_t pids[NUM_THREADS];

void thread_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread logic here
    }
}

int main(void) {
    for (int i = 0; i < NUM_THREADS; i++) {
        pids[i] = thread_create(stacks[i], STACKSIZE, THREAD_PRIORITY_MAIN - i, 0, thread_func, NULL, "thread");
    }

    // Change priority of the first thread after some time
    ztimer_sleep(ZTIMER_MSEC, 5000);
    thread_change_priority(pids[0], THREAD_PRIORITY_MAIN - 1);

    return 0;
}
