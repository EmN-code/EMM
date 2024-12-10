#include "thread.h"
#include "msg.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)

char high_priority_thread_stack[THREAD_STACKSIZE_MAIN];
char low_priority_thread_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[4];

void *low_priority_thread(void *arg) {
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with data: %p\n", msg.content.ptr);
        }
    }
    return NULL;
}

void *high_priority_thread(void *arg) {
    msg_t msg;
    msg.content.ptr = (void *)0x1234;
    while (1) {
        msg_send(&msg, thread_getpid());
        thread_yield();
    }
    return NULL;
}

int main(void) {
    /* Initialize the message queue for the main thread */
    msg_init_queue(msg_queue, 4);

    /* Create a thread with lower priority */
    thread_create(low_priority_thread_stack, sizeof(low_priority_thread_stack),
                  THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  low_priority_thread, NULL, "low_priority_thread");

    /* Create a thread with higher priority */
    thread_create(high_priority_thread_stack, sizeof(high_priority_thread_stack),
                  THREAD_PRIORITY_HIGH, THREAD_CREATE_STACKTEST,
                  high_priority_thread, NULL, "high_priority_thread");

    return 0;
}