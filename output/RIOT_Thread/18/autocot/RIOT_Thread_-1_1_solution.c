#include <xtimer.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_PRIORITY_HIGH  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW   (THREAD_PRIORITY_MAIN + 1)
#define MSG_QUEUE_SIZE        (8)

static char thread_stack_low[THREAD_STACKSIZE_DEFAULT];
static char thread_stack_high[THREAD_STACKSIZE_DEFAULT];

static msg_t msg_queue_low[MSG_QUEUE_SIZE];
static msg_t msg_queue_high[MSG_QUEUE_SIZE];

static void *low_priority_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_low, MSG_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Low priority thread received message: %ld\n", msg.content.value);
    }

    return NULL;
}

static void *high_priority_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_high, MSG_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("High priority thread received message: %ld\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    thread_create(thread_stack_low, sizeof(thread_stack_low), THREAD_PRIORITY_LOW,
                  THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    thread_create(thread_stack_high, sizeof(thread_stack_high), THREAD_PRIORITY_HIGH,
                  THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    while (1) {
        msg_t msg_high, msg_low;
        msg_high.content.value = xtimer_usec_from_ticks(xtimer_now()) % 100;
        msg_low.content.value = xtimer_usec_from_ticks(xtimer_now()) % 100 + 100;

        msg_send(&msg_high, thread_get(thread_getpid()-1));
        msg_send(&msg_low, thread_get(thread_getpid()-2));

        xtimer_sleep(1);
    }

    return 0;
}