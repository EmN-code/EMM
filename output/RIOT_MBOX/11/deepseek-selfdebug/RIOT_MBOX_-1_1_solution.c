
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000

static mbox_t msg_box;
static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        xtimer_sleep(INTERVAL_MS / MS_IN_USEC);
        msg.content.value = 1; // Example event value
        mbox_put(&msg_box, &msg);
    }

    return NULL;
}

static void *event_receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&msg_box, &msg, 1); // Blocking call to get message
        printf("Received event with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&msg_box, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, event_receiver, NULL, "event_receiver");

    return 0;
}
