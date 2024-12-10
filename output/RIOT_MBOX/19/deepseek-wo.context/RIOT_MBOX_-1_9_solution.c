
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/timer.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static char stack[STACK_SIZE];
static mbox_t mbox;
static msg_t msg_queue[8];

void timer_cb(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Example message content
    mbox_put(&mbox, &msg);
}

void msg_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, 8);

    kernel_pid_t pid = thread_create(stack, STACK_SIZE, PRIORITY, THREAD_CREATE_STACKTEST, msg_handler, NULL, "msg_handler");

    timer_t timer;
    timer_init(TIMER_DEV(0), 1000000, timer_cb, NULL);
    timer_set_periodic(TIMER_DEV(0), 0, INTERVAL, true);

    return 0;
}
