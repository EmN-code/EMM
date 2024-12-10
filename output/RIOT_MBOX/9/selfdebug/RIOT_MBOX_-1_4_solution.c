#include <stdio.h>
#include <string.h>
#include <msg.h>
#include <random.h>
#include <xtimer.h>
#include <ztimer.h>

#define MBOX_SIZE 8
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE];
static msg_t msg_queue[MBOX_SIZE];
static mbox_t mbox;

void *thread_handler(void *arg) {
    msg_t msg;
    (void)arg; // suppress unused parameter warning

    while (1) {
        random_bytes(&msg.content.value, sizeof(msg.content.value));
        if (mbox_try_put(&mbox, &msg) != 1) {
            puts("Failed to put message in mailbox");
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    mbox_init(&mbox, msg_queue, MBOX_SIZE);
    
    thread_create(stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "mbox_thread");

    return 0;
}