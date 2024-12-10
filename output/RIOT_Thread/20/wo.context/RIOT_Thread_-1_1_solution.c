#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define SENDER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN + 1)
#define MSG_QUEUE_SIZE           (8)

static char sender_thread_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_thread_stack[THREAD_STACKSIZE_DEFAULT];

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example message content

    while (1) {
        // Send a message to the receiver thread every second
        msg_send(&msg, thread_get(RECEIVER_THREAD_PRIORITY));
        xtimer_sleep(1);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        // Wait for a message
        msg_receive(&msg);
        // Process the received message
        printf("Received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create sender thread
    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender_thread");

    // Create receiver thread
    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "receiver_thread");

    // Enter the main loop
    while (1) {
        // In a real application, the main thread would perform other tasks
        xtimer_sleep(10);
    }

    return 0;
}