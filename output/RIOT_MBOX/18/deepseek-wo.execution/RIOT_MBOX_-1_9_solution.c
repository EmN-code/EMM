
#include "periph/gpio.h"
#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define QUEUE_SIZE (16)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE];
static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0x1234;

    while (1) {
        msg_send_receive(&msg, &msg, thread_getpid());
        xtimer_sleep(1);
    }
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %u\n", (unsigned int)msg.content.value);
    }

    return 0;
}
