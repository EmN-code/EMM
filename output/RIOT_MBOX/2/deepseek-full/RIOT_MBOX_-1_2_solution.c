
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "timex.h"

#define QUEUE_SIZE 8
#define INTERVAL_MS 1000

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void send_event(void *arg)
{
    (void)arg;
    static uint32_t event_count = 0;
    msg_t msg;
    msg.content.value = event_count++;

    if (mbox_try_put(&mbox, &msg) != 1) {
        puts("Failed to send message, mailbox full.");
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_msg(&evtimer, &mbox);

    evtimer_add_msg(&evtimer, INTERVAL_MS * US_PER_MS, send_event, NULL);

    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Received event: %" PRIu32 "\n", msg.content.value);
        }
    }

    return 0;
}
