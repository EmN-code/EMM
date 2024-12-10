
#include <MQTTClient.h>
#include <timex.h>
#include <time_units.h>
#include <mbox.h>
#include <xtimer.h>

#define STACKSIZE 1024
#define NUM_MESSAGES 8

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t high_pid = thread_create(stack_high, sizeof(stack_high),
                                          THREAD_PRIORITY_MAIN - 1, 0,
                                          high_priority_thread, NULL, "high");

    kernel_pid_t low_pid = thread_create(stack_low, sizeof(stack_low),
                                         THREAD_PRIORITY_MAIN - 2, 0,
                                         low_priority_thread, NULL, "low");

    msg_t msg;
    msg.type = 0;

    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, high_pid);
        } else {
            msg_send(&msg, low_pid);
        }
        xtimer_sleep(1);
    }

    return 0;
}
