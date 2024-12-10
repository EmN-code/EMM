#include <msg.h>
#include <kernel.h>
#include <stdio.h>

#define MAIN_QUEUE_SIZE    (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 42;
        msg_send(&msg, thread_getpid());
        printf("Sent message with value %d\n", msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("Message queue test\n");

    kernel_pid_t sender_pid = thread_create(
        sender_thread_stack, sizeof(sender_thread_stack),
        THREAD_PRIORITY_MAIN - 1, 0,
        sender_thread, NULL, "sender"
    );
    (void)sender_pid;

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value %d\n", msg.content.value);
    }
    return 0;
}

char sender_thread_stack[THREAD_STACKSIZE_MAIN];