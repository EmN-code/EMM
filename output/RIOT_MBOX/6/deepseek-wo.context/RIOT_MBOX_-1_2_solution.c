
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 16
#define INTERVAL_MS 1000000  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t receiver_pid;

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create the receiver thread
    receiver_pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread, NULL, "receiver");

    // Send messages at intervals
    while (1) {
        msg_t msg;
        msg.content.value = 42;  // Example message content
        msg.sender_pid = thread_getpid();
        msg_send(&msg, receiver_pid);

        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
