#include <ztimer.h>
#include <timex.h>
#include <stdio.h>
#include <msg.h>
#include <xtimer.h>

#define INTERVAL (1000000U) // 1 second in microseconds
#define MSG_QUEUE_SIZE (8)

// Message queue
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *msg_sender(void *arg) {
    (void)arg;
    msg_t msg;
    
    while (1) {
        // Create and send a message
        msg.content.value = 42; // some arbitrary value
        printf("Sending message with value %d\n", (int)msg.content.value);
        msg_send(&msg, sched_active_thread);

        // Sleep for INTERVAL seconds
        ztimer_sleep(ZTIMER_SEC, xtimer_usec_from_sysusec(INTERVAL));
    }
    return NULL;
}

void *msg_receiver(void *arg) {
    (void)arg;
    msg_t msg;
    
    while (1) {
        // Receive a message
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create sender and receiver threads
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  msg_sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack),
                  THREAD_PRIORITY_MAIN - 2, 0,
                  msg_receiver, NULL, "receiver");
    
    // Idle loop
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}