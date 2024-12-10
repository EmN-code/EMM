
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

static char stack_buffer_high[STACKSIZE];
static char stack_buffer_low[STACKSIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

static void *sender_thread(void *arg)
{
    msg_t msg, reply;
    msg.content.value = 123;

    while (1) {
        msg_send_receive(&msg, &reply, receiver_pid);
        xtimer_sleep(1); // Simulate some work
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
        msg_reply(&msg, &msg); // Reply with the same message
    }

    return NULL;
}

int main(void)
{
    sender_pid = thread_create(stack_buffer_high, sizeof(stack_buffer_high), PRIORITY_HIGH, 0, sender_thread, NULL, "sender");
    receiver_pid = thread_create(stack_buffer_low, sizeof(stack_buffer_low), PRIORITY_LOW, 0, receiver_thread, NULL, "receiver");

    return 0;
}
