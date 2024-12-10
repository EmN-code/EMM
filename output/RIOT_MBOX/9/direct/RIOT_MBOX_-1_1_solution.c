#include <xtimer.h>
#include <msg.h>
#include <stdio.h>
#include <stdlib.h>

#define INTERVAL (2U * US_PER_SEC) // 2-second interval for sending messages
#define MSG_QUEUE_SIZE (8)

static kernel_pid_t pid;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *_receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with content: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    printf("RIOT-ESP32 Message Box Demo\n");

    // Initialize a thread for receiving messages
    pid = thread_create((char[THREAD_STACKSIZE_DEFAULT]){}, THREAD_STACKSIZE_DEFAULT,
                        THREAD_PRIORITY_MAIN - 1, 0, _receiver, NULL, "receiver");
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    msg_t msg;
    int value = 0;
    while (1) {
        // Fill the message with content
        msg.content.value = value;

        // Send message to the receiver thread
        msg_send(&msg, pid);
        printf("Sent message with value: %d\n", value);

        // Sleep for defined interval before sending next message
        xtimer_sleep(INTERVAL / US_PER_SEC);

        value++;  // Increment value for the next message
    }

    return 0;
}