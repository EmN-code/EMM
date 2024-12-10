
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL (1 * 1000 * 1000U)  // 1 second interval

static char thread_stack[THREAD_STACKSIZE];
static msg_t msg_queue[8];
static mbox_t mbox;

void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 0x1234;  // Example event data
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
