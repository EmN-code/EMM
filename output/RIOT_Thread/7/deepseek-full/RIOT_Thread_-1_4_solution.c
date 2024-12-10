
#include "riot/thread.h"
#include "riot/msg.h"
#include "riot/kernel.h"
#include "riot/periph/gpio.h"
#include "riot/periph/uart.h"
#include "riot/periph/timer.h"

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 16

static char stack_high[STACK_SIZE];
static char stack_low[STACK_SIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

kernel_pid_t high_priority_pid;
kernel_pid_t low_priority_pid;

void *high_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    high_priority_pid = thread_create(stack_high,
                                      STACK_SIZE,
                                      THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST,
                                      high_priority_thread,
                                      NULL,
                                      "high_priority");

    low_priority_pid = thread_create(stack_low,
                                     STACK_SIZE,
                                     THREAD_PRIORITY_MAIN - 2,
                                     THREAD_CREATE_STACKTEST,
                                     low_priority_thread,
                                     NULL,
                                     "low_priority");

    msg_t msg;
    msg.type = 0;
    msg.content.value = 42;

    while (1) {
        msg_send(&msg, high_priority_pid);
        msg_send(&msg, low_priority_pid);
        xtimer_sleep(1);
    }

    return 0;
}
