
#include "thread.h"
#include "mbox.h"
#include "ztimer.h"
#include "fmt.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static msg_t msg_queue[16];

void sender_thread(void)
{
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, 1000); // Send event every 1 second
    }
}

void receiver_thread(void)
{
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1); // Blocking get
        printf("Received message: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, 16);

    thread_create(NULL, THREAD_STACKSIZE, THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)sender_thread, NULL, "sender");

    thread_create(NULL, THREAD_STACKSIZE, THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)receiver_thread, NULL, "receiver");

    return 0;
}
