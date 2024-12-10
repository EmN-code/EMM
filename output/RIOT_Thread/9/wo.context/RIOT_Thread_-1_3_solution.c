#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_LOW     (THREAD_PRIORITY_MAIN + 2)
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define MAIN_QUEUE_SIZE         (8)

static char thread_stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[MAIN_QUEUE_SIZE];

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    printf("Sending message with content: %lu\n", (unsigned long)msg.content.value);
    msg_send(&msg, thread_getpid());
    return NULL;
}

void *msg_receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);

    thread_create(thread_stack, sizeof(thread_stack),
                  THREAD_PRIORITY_LOW, THREAD_CREATE_STACKTEST,
                  msg_sender, NULL, "sender");

    msg_receiver(NULL);
    return 0;
}