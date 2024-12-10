
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <mbox.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define INTERVAL (1 * 1000 * 1000U)  // 1 second interval

static mbox_t mbox;
static msg_t msg_queue[8];

void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example event value

    while (1) {
        xtimer_sleep(1);  // Send event every second
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

void *event_receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);  // Blocking call to get message
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void) {
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    thread_create(NULL, STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_receiver, NULL, "event_receiver");

    return 0;
}
