#include <thread.h>
#include <msg.h>

#define THREAD_PRIORITY_MAIN     (3U)
#define THREAD_PRIORITY_SENDER   (2U)
#define THREAD_PRIORITY_RECEIVER (1U)
#define THREAD_STACKSIZE         (THREAD_STACKSIZE_DEFAULT)

static char sender_thread_stack[THREAD_STACKSIZE];
static char receiver_thread_stack[THREAD_STACKSIZE];

msg_t msg_queue[8];

void *receiver_thread(void *arg) {
    msg_t msg;
    (void)arg;
    
    msg_init_queue(msg_queue, 8);

    while (1) {
        if (msg_receive(&msg) == 1) {
            // Process message
            printf("Received message with value: %d\n", msg.content.value);
        }
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example value

    while (1) {
        // Send the message to the receiver thread
        msg_send(&msg, thread_getpid_of(receiver_thread));
        puts("Message sent.");

        // Sleep or yield to allow the receiver to process
        thread_yield_higher();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid, receiver_pid;

    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                 THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                                 receiver_thread, NULL, "receiver");

    sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                               THREAD_PRIORITY_SENDER, THREAD_CREATE_STACKTEST,
                               sender_thread, NULL, "sender");

    // The main thread can perform additional tasks here, or simply sleep/loop
    while (1) {
        // This can be an idle task or additional management
        thread_yield();
    }

    return 0;
}