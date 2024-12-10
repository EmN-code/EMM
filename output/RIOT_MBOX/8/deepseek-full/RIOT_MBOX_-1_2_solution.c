
#include <stdio.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>

#define QUEUE_SIZE 8
#define SENDER_STACK_SIZE 512
#define RECEIVER_STACK_SIZE 512
#define SEND_INTERVAL (1 * US_PER_SEC)

static char sender_stack[SENDER_STACK_SIZE];
static char receiver_stack[RECEIVER_STACK_SIZE];
static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_usleep(SEND_INTERVAL);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
