#include <xtimer.h>
#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

char thread_one_stack[THREAD_STACKSIZE_MAIN];
char thread_two_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[8];

void *thread_one(void *arg) {
    (void) arg;
    msg_t msg;
    msg.content.value = 10;
    while (1) {
        printf("Thread one: Sending message with value %lu\n", (uint32_t)msg.content.value);
        msg_send_receive(&msg, &msg, thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Thread two: Received message with value %lu\n", (uint32_t)msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    thread_create(thread_one_stack, sizeof(thread_one_stack),
                  THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST,
                  thread_one, NULL, "thread_one");

    thread_create(thread_two_stack, sizeof(thread_two_stack),
                  THREAD_PRIORITY_MAIN + 2, THREAD_CREATE_STACKTEST,
                  thread_two, NULL, "thread_two");
    
    return 0;
}