
#include <stdio.h>
#include <thread.h>
#include <sched.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <msg.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <MQTTClient.h>
#include <net/sock/udp.h>
#include <mbox.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>
#include <net/emcute.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <mma8x5x_params.h>

#define THREAD_STACKSIZE_DEFAULT (1024)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

void *thread_func_1(void *arg)
{
    (void)arg;
    printf("Thread 1 running\n");
    xtimer_sleep(5);
    printf("Thread 1 done\n");
    return NULL;
}

void *thread_func_2(void *arg)
{
    (void)arg;
    printf("Thread 2 running\n");
    xtimer_sleep(5);
    printf("Thread 2 done\n");
    return NULL;
}

int main(void)
{
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN, 0, thread_func_2, NULL, "thread_2");

    xtimer_sleep(2);

    printf("Changing priority of Thread 1\n");
    sched_change_priority(pid_1, THREAD_PRIORITY_MAIN + 1);

    xtimer_sleep(2);

    printf("Changing priority of Thread 2\n");
    sched_change_priority(pid_2, THREAD_PRIORITY_MAIN + 1);

    xtimer_sleep(10);

    return 0;
}
