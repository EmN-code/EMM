
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include "periph/timer.h"
#include "fmt.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static msg_t msg_queue[8];

static void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received event: %" PRIu32 "\n", (uint32_t)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_t *event_thread_pid = thread_create(
        (char *)"event_thread",
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        event_thread,
        NULL,
        "event_thread"
    );

    if (event_thread_pid == NULL) {
        printf("Failed to create event thread\n");
        return 1;
    }

    xtimer_t timer;
    uint32_t event_count = 0;

    while (1) {
        xtimer_sleep(1);
        msg_t msg;
        msg.content.value = event_count++;
        mbox_put(&mbox, &msg);
    }

    return 0;
}
