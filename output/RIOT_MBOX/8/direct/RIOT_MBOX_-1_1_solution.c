#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <kernel.h>

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN - 2)
#define MESSAGE_QUEUE_SIZE  (8)
#define INTERVAL            (1U * US_PER_SEC) // 1 second interval

static msg_t message_queue[MESSAGE_QUEUE_SIZE];
static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
        msg.content.value = counter++;
        if (msg_send(&msg, thread_getpid()) <= 0) {
            printf("Failed to send message.\n");
        } else {
            printf("Sent message: %d\n", msg.content.value);
        }
    }

    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    printf("Message Box Example with RIOT on ESP32\n");

    /* Initialize message queue for the main thread */
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);

    /* Create sender and receiver threads */
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                  sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0,
                  receiver, NULL, "receiver");

    /* Start the main loop */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}