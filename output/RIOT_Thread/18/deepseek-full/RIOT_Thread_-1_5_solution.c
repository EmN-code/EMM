
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "dht_params.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_params.h"
#include "sched.h"
#include "string.h"
#include "mma8x5x_regs.h"
#include "net/emcute.h"
#include "unistd.h"
#include "mma8x5x.h"
#include "MQTTClient.h"

#define STACKSIZE 1024
#define NUM_MESSAGES 10

kernel_pid_t thread1_pid, thread2_pid;
msg_t msg_queue[NUM_MESSAGES];

void *thread1_func(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Thread 1 received message with value: %lu\n", (uint32_t)msg.content.value);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Thread 2 received message with value: %lu\n", (uint32_t)msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    thread1_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, thread2_func, NULL, "thread2");

    msg_t msg;
    for (uint32_t i = 0; i < NUM_MESSAGES; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, thread1_pid);
        } else {
            msg_send(&msg, thread2_pid);
        }
        xtimer_sleep(1);
    }

    return 0;
}
