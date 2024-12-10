
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example message content

    while (1) {
        msg_send(&msg, mbox_pid);
        xtimer_usleep(INTERVAL_MS);
    }
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Start the sender thread
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");

    // Main thread acts as the receiver
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
