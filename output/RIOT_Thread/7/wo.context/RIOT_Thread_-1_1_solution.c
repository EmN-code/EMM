// This code demonstrates how to create two threads on an ESP32 using the RIOT operating system. One thread sends messages to another using the msg_t structures provided by RIOT, with threads having different priorities.

#include <xtimer.h> // Timer header
#include <thread.h> // Thread management
#include <msg.h>    // Message API

#define THREAD_STACKSIZE_DEFAULT (1024)
#define THREAD_PRIORITY_MAIN (1)
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN + 2)

static char sender_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_stack[THREAD_STACKSIZE_DEFAULT];

static msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int message_content = 0;

    while (1) {
        message_content++;
        msg.content.value = message_content;
        printf("Sender: Sending message with content %d\n", message_content);
        msg_send(&msg, thread_getpid_of((thread_t *)arg));
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with content %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    
    thread_create(receiver_stack, sizeof(receiver_stack),
                  THREAD_PRIORITY_RECEIVER, 0,
                  receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, 0,
                  sender_thread, NULL, "sender");

    while (1) {}

    return 0;
}