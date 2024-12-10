
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "mbox.h"
#include "xtimer.h"

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL_MS         (1000)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;

void event_handler(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("Event received!\n");
}

int main(void)
{
    mbox_init(&mbox, main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, event_handler, NULL, "event_handler");

    msg_t msg;
    msg.content.ptr = NULL;
    msg.sender_pid = pid;

    while (1) {
        ztimer_set(ZTIMER_MSEC, (ztimer_t *)&msg, INTERVAL_MS);
        mbox_put(&mbox, &msg);
        xtimer_sleep(1);
    }

    return 0;
}
