#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_LARGE  (THREAD_STACKSIZE_MAIN + 256)
#define THREAD_PRIO_HIGH        (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIO_LOW         (THREAD_PRIORITY_MAIN - 2)

char stack_low[THREAD_STACKSIZE_LARGE];
char stack_high[THREAD_STACKSIZE_LARGE];

msg_t msg_queue_1[4];
msg_t msg_queue_2[4];

static char high_message[] = "High priority message";
static char low_message[] = "Low priority message";

static void *high_prio_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_1, 4);
    msg_t msg;
    while (1) {
        if (msg_receive(&msg)) {
            printf("High-priority thread received: %s\n", (char *)msg.content.ptr);
        }
    }
    return NULL;
}

static void *low_prio_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue_2, 4);
    msg_t msg;
    while (1) {
        if (msg_receive(&msg)) {
            printf("Low-priority thread received: %s\n", (char *)msg.content.ptr);
        }
    }
    return NULL;
}

int main(void) {
    thread_create(stack_high, sizeof(stack_high), THREAD_PRIO_HIGH,
                  THREAD_CREATE_STACKTEST | THREAD_CREATE_WOUT_YIELD,
                  high_prio_thread, NULL, "High Priority Thread");

    thread_create(stack_low, sizeof(stack_low), THREAD_PRIO_LOW,
                  THREAD_CREATE_STACKTEST | THREAD_CREATE_WOUT_YIELD,
                  low_prio_thread, NULL, "Low Priority Thread");

    msg_t msg;
    msg.content.ptr = (void *)high_message;
    msg_send(&msg, thread_getpid());

    msg.content.ptr = (void *)low_message;
    msg_send(&msg, thread_getpid());
    
    return 0;
}