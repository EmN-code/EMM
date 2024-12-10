#include "ztimer.h"
#include "msg.h"
#include "periph/gpio.h"

#define INTERVAL (1U * ZTIMER_MSEC)
#define QUEUE_SIZE    (8U)

static msg_t msg_queue[QUEUE_SIZE];

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
        msg_send(&msg, sched_active_thread());
        printf("Message sent with value: %d\n", msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    puts("Starting Message Box Example\n");

    kernel_pid_t sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Main thread received message with value: %d\n", msg.content.value);
    }

    return 0;
}