
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static msg_t msg_queue[8];
static mbox_t mbox;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(5);
        mbox_put(&mbox, &msg);
    }
}

void receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, 8);

    kernel_pid_t sender_pid = thread_create(stack, sizeof(stack), PRIORITY,
                                            THREAD_CREATE_STACKTEST,
                                            (thread_task_func_t)sender_thread,
                                            NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(stack, sizeof(stack), PRIORITY,
                                              THREAD_CREATE_STACKTEST,
                                              (thread_task_func_t)receiver_thread,
                                              NULL, "receiver");

    (void)sender_pid;  // Suppress unused variable warning

    return 0;
}
