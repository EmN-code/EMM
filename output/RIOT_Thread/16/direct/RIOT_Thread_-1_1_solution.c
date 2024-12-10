#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5
#define THREAD_PRIORITY_HIGH 3
#define THREAD_PRIORITY_LOW 7
#define QUEUE_SIZE 8

// Message queue for each thread
static msg_t msg_queue[QUEUE_SIZE];

// Thread function for receiving messages
void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        // Wait for a message
        msg_receive(&msg);
        printf("Received message with value: %ld\n", msg.content.value);
    }
    return NULL;
}

// Thread function for sending messages
void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    for (int i = 0; i < 5; i++) {
        // Prepare a message
        msg.content.value = i + 1;
        // Send a message
        msg_send(&msg, thread_getpid());
        printf("Sent message with value: %ld\n", msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("Starting RIOT message passing example...");

    // Initialize the message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);

    // Thread handles
    kernel_pid_t receiver_pid, sender_pid_high, sender_pid_low;

    // Create the receiver thread
    receiver_pid = thread_create(
        malloc(THREAD_STACKSIZE_MAIN), THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
        receiver_thread, NULL, "receiver");

    // Create sender threads with different priorities
    sender_pid_high = thread_create(
        malloc(THREAD_STACKSIZE_MAIN), THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_HIGH, THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender_high");

    sender_pid_low = thread_create(
        malloc(THREAD_STACKSIZE_MAIN), THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_LOW, THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender_low");

    // Join threads
    thread_join(receiver_pid);
    thread_join(sender_pid_high);
    thread_join(sender_pid_low);

    return 0;
}