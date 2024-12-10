
#include "thread.h"
#include "mbox.h"
#include "ztimer.h"
#include "stdio.h"  // Include stdio.h for printf

#define QUEUE_SIZE 8
#define INTERVAL_MS 1000

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

static void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_MS);
        msg.content.value++;
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

int main(void) {
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);  // Corrected mbox_get usage
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
