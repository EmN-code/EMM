
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *event_sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 1;  // Dummy event value
        msg_send(&msg, (kernel_pid_t)arg, false);
        xtimer_usleep(INTERVAL_MS);
    }

    return NULL;
}

int main(void) {
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_sender,
        (void *)&mbox,
        "event_sender"
    );

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 1);  // Blocking call to get message from mbox
        printf("Received event with value: %d\n", msg.content.value);
    }

    return 0;
}
