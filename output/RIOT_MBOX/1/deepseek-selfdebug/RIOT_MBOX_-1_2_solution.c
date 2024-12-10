
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "fmt.h"

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL_MS         (1000)  // Interval in milliseconds

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t main_mbox;

void event_sender(void)
{
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_MS);
        msg.content.value = count++;
        mbox_put(&main_mbox, &msg);
    }
}

int main(void)
{
    msg_t msg;
    uint32_t received_count;

    mbox_init(&main_mbox, main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    while (1) {
        mbox_get(&main_mbox, &msg);
        received_count = msg.content.value;
        printf("Received event count: %" PRIu32 "\n", received_count);
    }

    return 0;
}
