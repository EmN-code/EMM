#include <unistd.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <MQTTClient.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_LEVELS - 1)
#define THREAD_PRIORITY_HIGH    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW     (THREAD_PRIORITY_MAIN + 1)
#define MSG_QUEUE_SIZE          8

static char stack_high[THREAD_STACKSIZE_DEFAULT];
static char stack_low[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %u\n", msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %u\n", msg.content.value);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t high_pid = thread_create(stack_high, sizeof(stack_high),
                                          THREAD_PRIORITY_HIGH, 0,
                                          high_priority_thread, NULL, "high");

    kernel_pid_t low_pid = thread_create(stack_low, sizeof(stack_low),
                                         THREAD_PRIORITY_LOW, 0,
                                         low_priority_thread, NULL, "low");
    msg_t msg;
    while (1) {
        msg.content.value = (uint32_t) (xtimer_usec_from_ticks(xtimer_now()) % 1000);
        msg_send(&msg, high_pid);
        msg_send(&msg, low_pid);
        xtimer_sleep(1);
    }
    return 0;
}
