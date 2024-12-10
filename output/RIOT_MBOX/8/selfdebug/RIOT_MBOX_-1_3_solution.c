#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "msg.h"
#include "kernel.h"

#define QUEUE_SIZE (8)
#define INTERVAL (2) // Interval in seconds

static msg_t queue[QUEUE_SIZE];
static msg_t msg;
static kernel_pid_t main_pid;

void *message_sender(void *arg) {
    (void)arg;
    msg_t msg;
    while(1) {
        xtimer_sleep(INTERVAL);
        msg.content.value = 42; // Example content
        msg_send(&msg, main_pid);
        printf("Message sent to main thread.\n");
    }
    return NULL;
}

int main(void) {
    main_pid = thread_getpid();
    msg_init_queue(queue, QUEUE_SIZE);

    char sender_stack[THREAD_STACKSIZE_MAIN];
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, message_sender, NULL, "sender");

    while (1) {
        msg_receive(&msg);
        printf("Message received with content: %lu\n", msg.content.value);
    }

    return 0;
}