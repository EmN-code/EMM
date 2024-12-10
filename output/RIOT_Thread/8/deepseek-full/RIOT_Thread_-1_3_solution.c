
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[8];
static msg_t msg;

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg_received;

    while (1) {
        msg_receive(&msg_received);
        printf("Received message with value: %lu\n", (unsigned long)msg_received.content.value);
    }

    return NULL;
}

void *sender_thread(void *arg)
{
    (void)arg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;

    while (1) {
        msg.content.value = 1234;
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, 8);

    kernel_pid_t receiver_pid = thread_create(stack_low, STACKSIZE, PRIORITY_LOW, 0, receiver_thread, NULL, "receiver");
    kernel_pid_t sender_pid = thread_create(stack_high, STACKSIZE, PRIORITY_HIGH, 0, sender_thread, &receiver_pid, "sender");

    (void)sender_pid;

    return 0;
}
