#include <thread.h>
#include <xtimer.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define HIGH_PRIO (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIO  (THREAD_PRIORITY_MAIN + 1)
#define QUEUE_SIZE (8)

static char stack_buf[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[QUEUE_SIZE];
static kernel_pid_t high_prio_thread;
static kernel_pid_t low_prio_thread;

void *low_prio_thread_func(void *arg) {
    msg_t msg;
    (void)arg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Low priority thread received: %d\n", msg.content.value);
        }
    }
    return NULL;
}

void *high_prio_thread_func(void *arg) {
    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 42;
    (void)arg;
    while (1) {
        printf("High priority thread sending: %d\n", msg.content.value);
        if (msg_send(&msg, low_prio_thread) == 1) {
            printf("Message sent successfully by high priority thread\n");
        } else {
            printf("Failed to send message by high priority thread\n");
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    high_prio_thread = thread_create(
        stack_buf, sizeof(stack_buf), HIGH_PRIO, THREAD_CREATE_STACKTEST,
        high_prio_thread_func, NULL, "high_prio");

    low_prio_thread = thread_create(
        stack_buf, sizeof(stack_buf), LOW_PRIO, THREAD_CREATE_STACKTEST,
        low_prio_thread_func, NULL, "low_prio");

    printf("Threads created, starting communication\n");
    return 0;
}