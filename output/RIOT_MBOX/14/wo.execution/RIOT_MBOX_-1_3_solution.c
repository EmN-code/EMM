#include "ztimer.h"
#include "msg.h"
#include "mbox.h"

#define EVENT_INTERVAL (5U * US_PER_SEC)

static mbox_t mbox;
static msg_t msg_queue[4];

void sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        msg_send(&msg, mbox_recipient_pid(&mbox));
        ztimer_sleep(ZTIMER_USEC, EVENT_INTERVAL);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, 4);

    msg_t msg;
    while (1) {
        _mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %u\n", msg.content.value);
    }

    return 0;
}