// Include necessary header files in alphabetical order
#include <thread.h>
#include <msg.h>
#include <xtimer.h> // For timing utilities

#define THREAD_PRIORITY_RECEIVER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER     (THREAD_PRIORITY_MAIN + 1)
#define THREAD_STACKSIZE_DEFAULT   1024

static msg_t msg_queue_sender[8];
static msg_t msg_queue_receiver[8];

static char receiver_stack[THREAD_STACKSIZE_DEFAULT];
static char sender_stack[THREAD_STACKSIZE_DEFAULT];

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue_receiver, 8);

    while (1) {
        // Receive a message
        if (msg_receive(&msg) == 1) {
            puts("Message received by receiver thread");
        }
        xtimer_sleep(1); // Sleep for a bit before receiving the next message
    }
    return NULL;
}

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content

    msg_init_queue(msg_queue_sender, 8);

    while (1) {
        // Send a message to the receiver thread
        if (msg_send(&msg, thread_getpid_of(receiver_stack)) == 1) {
            puts("Message sent by sender thread");
        }
        xtimer_sleep(2); // Sleep before sending the next message
    }
    return NULL;
}

int main(void) {
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_RECEIVER, 0, receiver_thread, NULL, "receiver");
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER, 0, sender_thread, NULL, "sender");
    return 0;
}