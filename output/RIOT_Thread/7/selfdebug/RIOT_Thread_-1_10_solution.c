#include <thread.h>
#include <xtimer.h>

// Define stack size and priorities
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN - 2)

// Message queue
static msg_t queue[8];

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0xABCD;
    msg.content.value = 42;
    while (1) {
        // Send message
        msg_send(&msg, sched_active_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(queue, 8);
    while (1) {
        // Receive message
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }
    return NULL;
}

int main(void) {
    char sender_stack[THREAD_STACKSIZE];
    char receiver_stack[THREAD_STACKSIZE];

    // Create threads
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIORITY, 0,
                  sender_thread, NULL, "sender_thread");
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIORITY, 0,
                  receiver_thread, NULL, "receiver_thread");

    return 0;
}