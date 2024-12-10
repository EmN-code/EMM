#include <stdio.h>
#include <event.h>

#define MSG_QUEUE_SIZE   (8)

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *msg_sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Arbitrary message content value
    while (1) {
        mbox_put(&mbox, &msg);
        printf("Message sent\n");
        xtimer_sleep(1);  // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    printf("Starting message box example on ESP32\n");

    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Create a thread that sends messages
    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, msg_sender_thread, NULL, "msg_sender");

    // Main thread loop to receive messages
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 1);  // Block and wait for a message
        printf("Received message with value: %ld\n", msg.content.value);
    }
    return 0;
}