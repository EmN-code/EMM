#include <ztimer.h>
#include <timex.h>
#include <msg.h>
#include <stdio.h>

#define QUEUE_SIZE 8
#define INTERVAL (1000000U) // 1 second in microseconds

static msg_t queue[QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    unsigned counter = 0;
    while (1) {
        msg.content.value = counter++;
        printf("Sending message: %u\n", msg.content.value);
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_USEC, ZTIMER_USEC_TO_TICKS(INTERVAL));
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, QUEUE_SIZE);
    kernel_pid_t sender_pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT,
                                            THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST,
                                            sender, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %u\n", msg.content.value);
    }

    return 0;
}