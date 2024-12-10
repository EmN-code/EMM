#include <msg.h>
#include <kernel.h>
#include <stdio.h>

#define MSG_QUEUE_SIZE   (8)
#define SENDER_INTERVAL_MS (1000U)

static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender(void) {
    while (1) {
        msg_t msg;
        msg.content.value = 42; // Example message content
        msg_send(&msg, KERNEL_PID_MAIN);
        printf("Sent message with value: %d\n", msg.content.value);
        xtimer_sleep(SENDER_INTERVAL_MS / 1000U);
    }
}

void receiver(void) {
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    while(1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void) {
    printf("Message box example\n");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, sender, NULL, "sender");
    
    receiver();
    return 0;
}