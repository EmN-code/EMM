
#include <paho_mqtt.h>
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <dht_params.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <thread.h>
#include <string.h>
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>

#define STACKSIZE 512
#define NUM_MESSAGES 16

msg_t msg_array[NUM_MESSAGES];

void high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received: %lu\n", (unsigned long)msg.content.value);
    }
}

void low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received: %lu\n", (unsigned long)msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(msg_array, NUM_MESSAGES);

    char stack_high[STACKSIZE];
    char stack_low[STACKSIZE];

    kernel_pid_t high_pid = thread_create(stack_high, sizeof(stack_high),
                                          THREAD_PRIORITY_MAIN - 1, 0,
                                          high_priority_thread, NULL, "high");

    kernel_pid_t low_pid = thread_create(stack_low, sizeof(stack_low),
                                         THREAD_PRIORITY_MAIN - 2, 0,
                                         low_priority_thread, NULL, "low");

    msg_t msg;
    msg.type = 1;

    for (uint32_t i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, high_pid);
        } else {
            msg_send(&msg, low_pid);
        }
        xtimer_sleep(1);
    }

    return 0;
}
