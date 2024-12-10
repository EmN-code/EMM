
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/timer.h"
#include "timex.h"
#include "fmt.h"

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL_MS         (1000000U)  /* 1 second */

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;
static msg_t mbox_queue[MAIN_QUEUE_SIZE];

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL_MS);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&mbox, mbox_queue, MAIN_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
