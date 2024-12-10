
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "MQTTClient.h"

#define STACKSIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(5);
        thread_set_priority(thread_getpid(), PRIORITY + 1);
        printf("Thread 1 priority changed\n");
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(5);
        thread_set_priority(thread_getpid(), PRIORITY + 2);
        printf("Thread 2 priority changed\n");
    }
    return NULL;
}

int main(void)
{
    thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread1_func, NULL, "thread1");
    thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread2_func, NULL, "thread2");

    return 0;
}
