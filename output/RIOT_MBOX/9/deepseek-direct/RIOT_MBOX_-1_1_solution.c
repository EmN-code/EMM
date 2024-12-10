
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
    msg.content.value = 0;  // Initialize message content

    while (1) {
        msg_send(&msg, thread_getpid());  // Send message to main thread
        xtimer_usleep(INTERVAL_MS);      // Wait for 1 second
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_t *sender_thread = thread_create(stack, sizeof(stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST,
                                            event_sender, NULL, "event_sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);  // Receive message from event_sender thread
        printf("Received event with value: %d\n", msg.content.value);
    }

    return 0;
}
