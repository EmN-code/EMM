
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/uart.h"

#define THREAD_STACKSIZE_SENDER    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_RECEIVER  (THREAD_STACKSIZE_DEFAULT)

#define PRIORITY_SENDER            (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_RECEIVER          (THREAD_PRIORITY_MAIN - 2)

#define MSG_QUEUE_SIZE             (8)

static char stack_sender[THREAD_STACKSIZE_SENDER];
static char stack_receiver[THREAD_STACKSIZE_RECEIVER];

static msg_t msg_queue_sender[MSG_QUEUE_SIZE];
static msg_t msg_queue_receiver[MSG_QUEUE_SIZE];

static void *sender_thread(void *arg)
{
    (void)arg;

    msg_t msg;
    msg.content.value = 42; // Example message content

    while (1) {
        msg_send(&msg, thread_getpid()); // Send message to the receiver thread
        xtimer_sleep(1); // Sleep for 1 second before sending the next message
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;

    msg_t msg;

    while (1) {
        msg_receive(&msg); // Receive message from the sender thread
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize the message queues for both threads
    msg_init_queue(msg_queue_sender, MSG_QUEUE_SIZE);
    msg_init_queue(msg_queue_receiver, MSG_QUEUE_SIZE);

    // Create the sender thread with higher priority
    thread_create(stack_sender, sizeof(stack_sender), PRIORITY_SENDER,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    // Create the receiver thread with lower priority
    thread_create(stack_receiver, sizeof(stack_receiver), PRIORITY_RECEIVER,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    // Main thread can continue with other tasks or simply wait
    while (1) {
        xtimer_sleep(10); // Sleep for 10 seconds
    }

    return 0;
}
