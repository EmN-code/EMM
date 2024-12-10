#include <xtimer.h>
#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

static char sender_thread_stack[THREAD_STACKSIZE];
static char receiver_thread_stack[THREAD_STACKSIZE];

static kernel_pid_t receiver_pid;

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Sample message data

    while (1) {
        msg_send(&msg, receiver_pid);
        printf("Message sent from sender thread\n");
        xtimer_sleep(1);
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Message received: %ld\n", (long)msg.content.value);  // Cast to long for printing
    }
    return NULL;
}

int main(void) {
    printf("Starting thread communication example\n");

    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                 THREAD_PRIORITY_RECEIVER, 0, receiver_thread,
                                 NULL, "receiver");

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_SENDER, 0, sender_thread,
                  NULL, "sender");

    return 0;
}