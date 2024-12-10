#include <thread.h>
#include <msg.h>
#include <xtimer.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    (2 * THREAD_STACKSIZE_DEFAULT)
#define INTERVAL                (1U * US_PER_SEC)

static char thread_stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg.content.value = 1;  // Example message content
        msg_send(&msg, msg_queue);
        puts("Message sent");
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    kernel_pid_t sender_thread = thread_create(thread_stack, sizeof(thread_stack),
                                               THREAD_PRIORITY_MAIN - 1, 0,
                                               msg_sender, NULL, "msg_sender");

    if (sender_thread <= KERNEL_PID_UNDEF) {
        puts("Error during thread creation");
        return 1;
    }

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return 0;
}