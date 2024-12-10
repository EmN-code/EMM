
#include "xtimer.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

static char stack_buffer_high[STACKSIZE];
static char stack_buffer_low[STACKSIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1234;

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, receiver_pid);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    sender_pid = thread_create(stack_buffer_high, sizeof(stack_buffer_high), PRIORITY_HIGH, 0, sender_thread, NULL, "sender");
    receiver_pid = thread_create(stack_buffer_low, sizeof(stack_buffer_low), PRIORITY_LOW, 0, receiver_thread, NULL, "receiver");

    return 0;
}
