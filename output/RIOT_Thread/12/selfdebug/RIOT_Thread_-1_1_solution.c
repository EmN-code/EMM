#include <stdio.h>
#include <msg.h>

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO       (THREAD_PRIORITY_MAIN + 1)
#define STACK_SIZE          (THREAD_STACKSIZE_MAIN)

static char sender_stack[STACK_SIZE];
static char receiver_stack[STACK_SIZE];

static msg_t queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Arbitrary value to send

    while (1) {
        printf("Sender: sending message\n");
        msg_send(&msg, thread_get(2)); // Assumes receiver thread PID = 2
        thread_yield_higher();
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: received message with value %d\n", msg.content.value);
        }
        thread_yield_higher();
    }
    return NULL;
}

int main(void) {
    printf("Starting RIOT application on ESP32\n");

    // Initialize the message queue for the receiver
    msg_init_queue(queue, 8);

    // Create the sender thread
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                  sender_thread, NULL, "sender");

    // Create the receiver thread
    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0,
                  receiver_thread, NULL, "receiver");

    return 0;
}