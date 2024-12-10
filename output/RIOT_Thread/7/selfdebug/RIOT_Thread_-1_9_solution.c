#include <thread.h>
#include <msg.h>
#include <stdio.h>
#include <xtimer.h>

#define SENDER_PRIORITY   (THREAD_PRIORITY_MAIN - 2)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define STACK_SIZE        (THREAD_STACKSIZE_DEFAULT)

static char sender_stack[STACK_SIZE];
static char receiver_stack[STACK_SIZE];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content

    while (1) {
        msg_send(&msg, thread_get(thread_getpid() + 1)); // Send to receiver thread
        puts("Message sent");
        xtimer_sleep(2); // Simulate delay
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("Starting thread communication example");

    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIORITY, 0,
                  receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIORITY, 0,
                  sender_thread, NULL, "sender");

    return 0;
}