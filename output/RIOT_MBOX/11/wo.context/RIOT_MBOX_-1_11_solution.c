#include <xtimer.h>
#include <sys/types.h>
#include <stddef.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/callback.h>
#include <event/timeout.h>

#define MESSAGE_QUEUE_SIZE (8)
#define INTERVAL (1U * US_PER_SEC)

static char _stack[THREAD_STACKSIZE_MAIN];
static msg_t _queue[MESSAGE_QUEUE_SIZE];
static kernel_pid_t _pid;

static void send_message_box(void) {
    mbox_t mbox;
    mbox_init(&mbox, _queue, MESSAGE_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg.content.value = 42; // Arbitrary event data
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL);
    }
}

int main(void) {
    _pid = thread_create(_stack, sizeof(_stack), THREAD_PRIORITY_MAIN - 1, 0, send_message_box, NULL, "msg_box");
    msg_t msg;

    while (1) {
        if (mbox_get(&_queue, &msg, 1)) {
            printf("Received message with value: %ld\n", (long int)msg.content.value);
        }
    }

    return 0;
}