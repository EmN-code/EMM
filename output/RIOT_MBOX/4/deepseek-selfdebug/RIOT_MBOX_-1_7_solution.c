
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static char thread_stack[STACK_SIZE];
static mbox_t mbox;
static msg_t mbox_queue[8];

void sender_thread(void)
{
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_try_put(&mbox, &msg);
        xtimer_usleep(INTERVAL);
    }
}

int main(void)
{
    msg_t msg;
    kernel_pid_t pid;

    mbox_init(&mbox, mbox_queue, sizeof(mbox_queue) / sizeof(mbox_queue[0]));

    pid = thread_create(thread_stack, sizeof(thread_stack), PRIORITY,
                        THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    while (1) {
        mbox_get(&mbox, &msg, TIMEOUT_NEVER);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
