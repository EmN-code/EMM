#include <thread.h>
#include <stdio.h>

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

static void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    const char *message = "Hello from sender!";
    msg.content.ptr = (char *)message;
    msg_send(&msg, thread_getpid_of(receiver_id));
    printf("Sender: Message sent.\n");
    return NULL;
}

static void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    msg_receive(&msg);
    printf("Receiver: %s\n", (char *)msg.content.ptr);

    reply.content.ptr = "Got it!";
    msg_reply(&msg, &reply);
    printf("Receiver: Reply sent.\n");

    return NULL;
}

int main(void) {
    kernel_pid_t sender_id, receiver_id;

    // Create receiver thread with higher priority
    receiver_id = thread_create(receiver_stack, STACKSIZE,
                                THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST,
                                receiver, NULL, "receiver");

    // Create sender thread with lower priority
    sender_id = thread_create(sender_stack, STACKSIZE,
                              THREAD_PRIORITY_MAIN + 1,
                              THREAD_CREATE_STACKTEST,
                              sender, NULL, "sender");

    (void)sender_id; // Variable to avoid warning when it's not used further

    // Let the threads run
    thread_yield_higher();

    return 0;
}