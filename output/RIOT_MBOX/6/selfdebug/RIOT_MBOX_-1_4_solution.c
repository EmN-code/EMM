#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define STACK_SIZE        (THREAD_STACKSIZE_DEFAULT)
#define MAIN_QUEUE_SIZE   (8)
#define MSG_QUEUE_SIZE    (8)

char thread_stack[STACK_SIZE];
msg_t msg_queue[MAIN_QUEUE_SIZE];

static void *event_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %d\n", msg.type);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);

    thread_create(thread_stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thr");

    ztimer_sleep(ZTIMER_MSEC, 1000);

    msg_t msg;
    msg.type = 1;

    while (1) {
        msg_send_receive(&msg, &msg, thread_getpid());
        printf("Sent message with type: %d\n", msg.type);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}