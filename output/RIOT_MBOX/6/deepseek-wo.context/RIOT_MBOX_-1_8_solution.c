
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/timer.h"

#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL (1 * US_PER_SEC)  // 1 second interval

static char _sender_stack[SENDER_STACKSIZE];
static mbox_t mbox;
static msg_t msg_queue[8];

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_sleep(INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, 8);

    // Create the sender thread
    thread_create(_sender_stack, SENDER_STACKSIZE, SENDER_PRIORITY,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    // Main loop to receive messages from the message box
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 1);  // Use the correct mbox_get function
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
