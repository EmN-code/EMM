#include <event/callback.h>
#include <event.h>
#include <xtimer.h>
#include <stdio.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)

#define MSG_CONTENT "Hello, RIOT!"
#define INTERVAL (5000000U) /* 5 seconds */

static char sender_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_stack[THREAD_STACKSIZE_DEFAULT];

static msg_t msg_queue[8]; // Message queue to hold messages

static void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        printf("Sending message: %s with value %d\n", MSG_CONTENT, msg.content.value);
        mbox_post(&msg_queue, &msg, 1);
        xtimer_usleep(INTERVAL); // Wait for a 5 seconds interval
    }
    return NULL;
}

static void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (mbox_get(&msg_queue, &msg, 1) == 1) {
            printf("Received message: %s with value %d\n", MSG_CONTENT, msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, 8);

    // Create sender and receiver threads
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, THREAD_CREATE_STACKTEST, sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");

    // Main loop does nothing, control is passed indefinitely to threads
    return 0;
}
