
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE (8)
#define EVENT_INTERVAL (1000000U)  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void event_sender(void)
{
    msg_t msg;
    while (1) {
        xtimer_usleep(EVENT_INTERVAL);
        msg.content.value = 1;  // Dummy event value
        mbox_put(&mbox, &msg);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);  // Corrected function call
        printf("Received event with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
