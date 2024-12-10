
#include "thread.h"
#include "mbox.h"
#include "ztimer.h"
#include "fmt.h"

#define MSG_QUEUE_SIZE (8)
#define INTERVAL (1000000U)  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void)
{
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
