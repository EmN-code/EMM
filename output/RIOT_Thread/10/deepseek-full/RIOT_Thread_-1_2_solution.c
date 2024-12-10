
#include "evtimer_msg.h"
#include "event/thread.h"
#include "msg.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "ztimer.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

static char stack_buffer_high[STACKSIZE];
static char stack_buffer_low[STACKSIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

static msg_t msg_queue[8];

void *sender_thread(void *arg)
{
    (void)arg;

    msg_t msg;
    msg.type = 1;
    msg.content.value = 123;

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg)
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
    msg_init_queue(msg_queue, 8);

    sender_pid = thread_create(stack_buffer_high, sizeof(stack_buffer_high), PRIORITY_HIGH, 0, sender_thread, NULL, "sender");
    receiver_pid = thread_create(stack_buffer_low, sizeof(stack_buffer_low), PRIORITY_LOW, 0, receiver_thread, NULL, "receiver");

    return 0;
}
