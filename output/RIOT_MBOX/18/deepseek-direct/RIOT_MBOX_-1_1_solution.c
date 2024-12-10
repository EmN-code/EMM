
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_DEFAULT];

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        // Send a message to the main thread
        msg.content.value = 1;  // Dummy value
        msg_send(&msg, thread_getpid());

        // Wait for the interval
        xtimer_usleep(INTERVAL_MS);
    }

    return NULL;
}

int main(void)
{
    puts("Message Box Example");

    // Create the event sender thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, event_sender, NULL, "event_sender");

    msg_t msg;
    while (1) {
        // Wait for a message from the event sender
        msg_receive(&msg);

        // Process the message
        printf("Received event at %" PRIu32 "\n", xtimer_now_usec());
    }

    return 0;
}
