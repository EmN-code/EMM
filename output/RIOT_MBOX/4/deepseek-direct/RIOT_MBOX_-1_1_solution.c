
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>

#define MSG_QUEUE_SIZE    16
#define SEND_INTERVAL_MS  1000000  // 1 second

static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void) {
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        // Prepare the message
        msg.content.value = 1;  // Example event value

        // Send the message to the main thread
        msg_send(&msg, thread_getpid());

        // Wait for the next interval
        xtimer_usleep(SEND_INTERVAL_MS);
    }
}

int main(void) {
    puts("Message Box Example");

    // Create the sender thread
    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    msg_t msg;
    while (1) {
        // Wait for a message
        msg_receive(&msg);

        // Process the message
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
