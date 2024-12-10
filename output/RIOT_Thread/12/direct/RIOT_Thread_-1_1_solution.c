#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define PRIO_LOW     (THREAD_PRIORITY_MAIN - 1)
#define PRIO_HIGH    (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE   (THREAD_STACKSIZE_MAIN)
#define MSG_QUEUE_SIZE     (8)

char stack_low[THREAD_STACKSIZE];
char stack_high[THREAD_STACKSIZE];

static msg_t msg_queue_low[MSG_QUEUE_SIZE];
static msg_t msg_queue_high[MSG_QUEUE_SIZE];

void *low_prio_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue_low, MSG_QUEUE_SIZE);
    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Low priority thread received message: %ld\n", msg.content.value);
        }
    }
    return NULL;
}

void *high_prio_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue_high, MSG_QUEUE_SIZE);
    while (1) {
        msg.content.value = xtimer_now_usec(); // use a timestamp as a message content
        printf("High priority thread sending message: %ld\n", msg.content.value);
        msg_send(&msg, thread_getpid()); // send the message to self or another thread
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    msg_t msg;

    thread_create(stack_low, THREAD_STACKSIZE, PRIO_LOW, THREAD_CREATE_STACKTEST, low_prio_thread, NULL, "low");
    thread_create(stack_high, THREAD_STACKSIZE, PRIO_HIGH, THREAD_CREATE_STACKTEST, high_prio_thread, NULL, "high");

    msg_init_queue(msg_queue_high, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Main thread received message: %ld\n", msg.content.value);
    }

    return 0;
}