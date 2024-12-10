#include <msg.h>
#include <kernel.h>
#include <stdio.h>
#include <xtimer.h>

#define QUEUE_SIZE (8)
#define INTERVAL   (1U * US_PER_SEC)

static msg_t queue[QUEUE_SIZE];

void *message_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received a message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, QUEUE_SIZE);
    kernel_pid_t msg_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, message_thread, NULL, "message");
    msg_t msg;
    msg.content.value = 42; // Example message content
    while (1) {
        msg_send(&msg, msg_thread_pid);
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
    return 0;
}