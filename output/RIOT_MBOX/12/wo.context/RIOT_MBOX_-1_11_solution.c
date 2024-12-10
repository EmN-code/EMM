#include <xtimer.h>
#include <msg.h>
#include <kernel.h>
#include <stdio.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *_sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example value
    while (1) {
        msg_send(&msg, thread_get(thread_getpid()));
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    /* initialize the message queue */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(
        sender_stack, sizeof(sender_stack),
        THREAD_PRIORITY_MAIN - 1, 0,
        _sender_thread, NULL, "sender");

    if (sender_pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to create the sender thread");
        return 1;
    }

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %d\n", msg.content.value);
    }
    return 0;
}