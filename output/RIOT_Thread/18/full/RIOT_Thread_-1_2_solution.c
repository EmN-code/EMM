#include <xtimer.h>
#include <msg.h>
#include <stdio.h>
#include <thread.h>

#define RECEIVER_PRIO    (THREAD_PRIORITY_MAIN - 1)
#define SENDER_PRIO      (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE        THREAD_STACKSIZE_DEFAULT

#define MSG_QUEUE_SIZE   8

char receiver_thread_stack[STACKSIZE];
char sender_thread_stack[STACKSIZE];

static msg_t receiver_queue[MSG_QUEUE_SIZE];

void *receiver_function(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(receiver_queue, MSG_QUEUE_SIZE);

    while (1) {
        if (msg_receive(&msg) < 0) {
            printf("Failed to receive message.\n");
        } else {
            printf("Received message with value: %lu\n", msg.content.value);
        }
    }
    return NULL;
}

void *sender_function(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 42;

    while (1) {
        if (msg_send(&msg, KERNEL_PID_FIRST) != 1) {
            printf("Failed to send message. Retrying...\n");
            xtimer_sleep(1);
        } else {
            printf("Message sent successfully!\n");
            break;
        }
    }
    return NULL;
}

int main(void) {
    printf("Starting RIOT application with inter-thread communication\n");

    thread_create(
        receiver_thread_stack, STACKSIZE,
        RECEIVER_PRIO, 0,
        receiver_function, NULL, "receiver_thread"
    );

    thread_create(
        sender_thread_stack, STACKSIZE,
        SENDER_PRIO, 0,
        sender_function, NULL, "sender_thread"
    );

    return 0;
}