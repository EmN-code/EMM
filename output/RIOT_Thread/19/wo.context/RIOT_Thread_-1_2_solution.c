#include <xtimer.h> // For xtimer_sleep() function
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE         (THREAD_STACKSIZE_MAIN)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

static kernel_pid_t receiver_pid;

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    printf("Receiver thread started.\n");
    while (1) {
        msg_receive(&msg);  // Blocking receive
        printf("Received message with value: %lu\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    printf("Sender thread started.\n");
    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Sent message with value: %lu\n", msg.content.value);
        msg.content.value++;
        xtimer_sleep(1);  // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    printf("Main function started.\n");

    // Initialize receiver thread
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver_thread, NULL, "receiver");

    // Initialize sender thread
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, 0,
                  sender_thread, NULL, "sender");

    return 0;
}