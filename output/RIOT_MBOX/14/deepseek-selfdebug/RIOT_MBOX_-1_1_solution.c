
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void) {
    msg_t msg;
    msg.content.value = 0;  // Initialize message content

    while (1) {
        msg.content.value++;  // Increment message content
        msg_send(&msg, mbox_pid, false);  // Send message to the message box
        xtimer_usleep(INTERVAL_MS);  // Wait for the specified interval
    }
}

void receiver_thread(void) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);  // Receive message from the message box
        printf("Received message with value: %d\n", (int)msg.content.value);
    }
}

int main(void) {
    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Create sender thread
    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender");

    // Create receiver thread
    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver");

    return 0;
}
