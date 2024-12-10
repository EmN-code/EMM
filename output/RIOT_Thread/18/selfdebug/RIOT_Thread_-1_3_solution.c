#include <thread.h>
#include <msg.h>
#include <inttypes.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN)

#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN + 1)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8];  // Queue for message exchange

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Simplified message content
    while (1) {
        msg_send(&msg, thread_getpid_of(receiver_stack));
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %lu\n", msg.content.value);  // Correct format specifier for uint32_t
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, THREAD_CREATE_STACKTEST,
                  sender_thread, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack),
                  THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                  receiver_thread, NULL, "receiver");
    return 0;
}