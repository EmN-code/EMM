
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MAIN_QUEUE_SIZE     (8)
#define INTERVAL_MS         (1000000U)  // 1 second interval

static msg_t msg_queue[MAIN_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 42;  // Example message content
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL_MS);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MAIN_QUEUE_SIZE);

    thread_create(
        stack, sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}
