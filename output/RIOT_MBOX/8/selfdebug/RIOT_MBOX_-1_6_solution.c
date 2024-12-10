#include <xtimer.h>
#include <msg.h>
#include <event.h>
#include <stdio.h>
#include <stddef.h> // Required for the size_t type

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL            (1000000U) // 1 second

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *message_sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        xtimer_sleep(INTERVAL / 1000000U); // Convert microseconds to seconds
        printf("Sending message...\n");
        msg.type = 0x1234; // Example message type
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    char sender_thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, message_sender, NULL, "sender_thread");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %04x\n", msg.type);
    }

    return 0;
}