
#include "thread.h"
#include "mbox.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "fmt.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define MBOX_SIZE           (16)
static mbox_t mbox;
static msg_t mbox_buffer[MBOX_SIZE];

#define SEND_INTERVAL       (1 * 1000 * 1000U) /* 1 second */

void sender_thread(void *arg)
{
    (void)arg;
    uint32_t count = 0;

    while (1) {
        msg_t msg;
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_usleep(SEND_INTERVAL);
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, mbox_buffer, MBOX_SIZE);

    thread_create(
        thread_stack(sender_thread),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, 0); // Fix: Added timeout parameter
        printf("Received message with value: %" PRIu32 "\n", msg.content.value); // Fix: Use PRIu32 for uint32_t
    }

    return 0;
}
