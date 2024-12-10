#include <xtimer.h>
#include <msg.h>
#include <stdio.h>

#define SENDER_THREAD_STACK_SIZE  (THREAD_STACKSIZE_MAIN)
#define RECEIVER_THREAD_STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define QUEUE_SIZE (8)

static char sender_thread_stack[SENDER_THREAD_STACK_SIZE];
static char receiver_thread_stack[RECEIVER_THREAD_STACK_SIZE];

msg_t msg_queue[QUEUE_SIZE];

void *receiver_thread(void *arg) {
    msg_t msg;
    (void) arg;
    while (1) {
        msg_receive(&msg);
        printf("Received Message: %d\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    msg_t msg;
    int counter = 0;
    (void) arg;
    while (1) {
        xtimer_sleep(2);
        msg.content.value = counter++;
        msg_send(&msg, thread_get(SCHED_PRIO_MAIN));
        printf("Sent Message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    printf("Starting message box example\n");

    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, receiver_thread, NULL, "receiver");

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");

    return 0;
}