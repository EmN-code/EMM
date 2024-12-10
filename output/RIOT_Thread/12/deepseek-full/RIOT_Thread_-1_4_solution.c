
#include <evtimer_msg.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <stdatomic.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <stdbool.h>
#include <MQTTClient.h>
#include <msg.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <string.h>
#include <stddef.h>
#include <ztimer.h>

#define STACKSIZE 1024
#define NUM_MESSAGES 16

static msg_t msg_queue[NUM_MESSAGES];

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        msg_send_receive(&msg, &msg, thread_getpid());
        xtimer_sleep(1);
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t sender_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, receiver_thread, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}
