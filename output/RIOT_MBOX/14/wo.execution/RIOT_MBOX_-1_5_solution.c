#include <stdio.h>
#include <stdint.h>
#include <msg.h>

#define NUM_MESSAGES   (32)

static msg_t msg_queue[NUM_MESSAGES];

void sender_thread(void) {
    kernel_pid_t pid = thread_getpid();
    while (1) {
        msg_t msg;
        msg.type = 0;
        msg.content.value = pid;
        printf("Sending message from PID: %" PRIu32 "\n", (uint32_t)pid);
        msg_send(&msg, mbox_recipient_pid);
        xtimer_sleep(5);
    }
}

int main(void) {
    msg_init_queue(msg_queue, NUM_MESSAGES);
    printf("Message queue initialized\n");

    kernel_pid_t sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                                            THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                            sender_thread, NULL, "sender");
    printf("Sender thread started with PID: %" PRIu16 "\n", sender_pid);
    return 0;
}