#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "unistd.h"
#include "stdbool.h"
#include "thread_config.h"
#include "kernel_defines.h"
#include "sched.h"
#include "event/thread.h"
#include "thread.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"

#define MAIN_QUEUE_SIZE     (8)

static char stack_1[THREAD_STACKSIZE_MAIN];
static char stack_2[THREAD_STACKSIZE_MAIN];
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *thread_func_1(void *arg) {
    /* Thread code here */
    atomic_fetch_add((atomic_uint *)arg, 1);
    return NULL;
}

void *thread_func_2(void *arg) {
    /* Thread code here */
    atomic_fetch_sub((atomic_uint *)arg, 1);
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    atomic_uint counter = ATOMIC_VAR_INIT(0);

    kernel_pid_t pid1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, thread_func_1,
                                      &counter, "thread_1");

    kernel_pid_t pid2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN - 2,
                                      THREAD_CREATE_STACKTEST, thread_func_2,
                                      &counter, "thread_2");

    while (1) {
        xtimer_sleep(5);
        int_priority_t irq_state = irq_disable();
        sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
        sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);
        irq_restore(irq_state);
    }

    return 0;
}