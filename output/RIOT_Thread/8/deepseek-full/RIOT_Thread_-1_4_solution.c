
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "paho_mqtt.h"
#include "ztimer.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "mbox.h"
#include "xtimer.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 10

static char stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 1234;

    while (1) {
        msg_send_receive(&msg, &msg, thread_getpid());
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", (unsigned int)msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, receiver_thread, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}
