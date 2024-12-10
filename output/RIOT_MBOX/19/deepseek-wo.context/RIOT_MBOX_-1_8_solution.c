
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include "msg.h"

#define MSG_QUEUE_SIZE 16
#define STACK_SIZE 256

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(
        stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");

    if (pid <= 0) {
        printf("Failed to create event thread\n");
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 1000000); // 1 second interval
        msg_t msg;
        msg.content.value = 42; // Example event value
        mbox_put(&mbox, &msg);
    }

    return 0;
}
