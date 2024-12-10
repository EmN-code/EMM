#include <xtimer.h>
#include <stdio.h>
#include <thread.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define QUEUE_SIZE (8)

msg_t msg_queue[QUEUE_SIZE];

static char sender_thread_stack[STACKSIZE];
static char receiver_thread_stack[STACKSIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.ptr = "Hello from sender!";
    while (1) {
        xtimer_sleep(2); // Send a message every 2 seconds
        msg_send(&msg, thread_get(active_pid));
        puts("Message sent");
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Received: %s\n", (char *)msg.content.ptr);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);

    thread_create(sender_thread_stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender_thread");

    thread_create(receiver_thread_stack, STACKSIZE, THREAD_PRIORITY_MAIN, 0, receiver_thread, NULL, "receiver_thread");

    return 0;
}