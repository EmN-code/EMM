
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

static mbox_t mbox;
static msg_t msg_queue[8];

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        msg_send(&msg, mbox_pid(&mbox));
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, 8);

    thread_create(sender_stack, sizeof(sender_stack), PRIORITY, 0, sender_thread, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), PRIORITY, 0, receiver_thread, NULL, "receiver");

    return 0;
}
