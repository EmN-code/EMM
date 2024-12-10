#include <xtimer.h> // Required for timer functionalities
#include <msg.h> // Required for message functionalities, including msg_t
#include <kernel.h> // Required for core kernel functionalities

#define INTERVAL (1U * US_PER_SEC) // Interval of 1 second

static void *sender_thread(void *arg) {
    msg_t msg;
    (void)arg;
    kernel_pid_t receiver_pid = (kernel_pid_t)(intptr_t)arg;
    while (1) {
        xtimer_sleep(1); // Sleep for 1 second interval
        msg.content.value = xtimer_now_usec(); // Assign current time as message content
        msg_send(&msg, receiver_pid); // Send message to the receiver thread
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8); // Initialize a message queue with size 8
    msg_t msg;
    (void)arg;
    while (1) {
        if (msg_receive(&msg) == 1) { // Wait for a message
            // Process the received message (for example, print the received content)
            printf("Received message with timestamp: %lu\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING, receiver_thread, NULL, "receiver");
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, sender_thread, (void *)(intptr_t)receiver_pid, "sender");
    return 0;
}