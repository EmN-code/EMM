#include "xtimer.h"
#include "msg.h"
#include "thread.h"

#define INTERVAL (1U * 1000000U) // 1 second in microseconds

static char stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        printf("Sending message with value: %ld\n", msg.content.value);
        msg_send(&msg, thread_getpid());
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg)) {
            printf("Received message with value: %ld\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    puts("Starting message box example");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    receiver_thread(NULL);

    return 0;
}