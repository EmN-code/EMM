
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static char sender_stack[SENDER_STACK_SIZE];
static char receiver_stack[RECEIVER_STACK_SIZE];

static kernel_pid_t mbox_pid;
static mbox_t mbox;

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg, reply;
    msg.content.value = 1;

    while (1) {
        xtimer_sleep(5);
        msg_send_receive(&msg, &reply, mbox_pid);
        printf("Sender: Sent message and received reply: %" PRIu32 "\n", reply.content.value);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg, reply;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message: %" PRIu32 "\n", msg.content.value);
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, NULL, 0);
    mbox_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1,
                             THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    return 0;
}
