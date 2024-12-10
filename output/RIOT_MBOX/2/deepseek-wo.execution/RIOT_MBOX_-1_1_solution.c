
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 1; // Example event value
        mbox_put(&mbox, &msg);
        xtimer_sleep(INTERVAL_MS / 1000);
    }

    return NULL;
}

static void *event_receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1); // Blocking call to get message
        printf("Received event with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, event_sender, NULL, "event_sender");
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, event_receiver, NULL, "event_receiver");

    return 0;
}
