
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL (1 * US_PER_SEC)

static char stack[STACK_SIZE];
static msg_t msg_queue[8];
static mbox_t mbox;

void event_handler(void *arg) {
    (void)arg;
    printf("Event received!\n");
}

void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.ptr = NULL;
    msg.type = 0;

    while (1) {
        xtimer_sleep(INTERVAL);
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    mbox_init(&mbox, msg_queue, 8);

    kernel_pid_t pid = thread_create(stack, STACK_SIZE, PRIORITY, THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 0);
        event_handler(NULL);
    }

    return 0;
}
