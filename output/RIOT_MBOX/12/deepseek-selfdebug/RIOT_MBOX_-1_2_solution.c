
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE (8)
#define INTERVAL_MS (1000000U)  // 1 second interval

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        msg.content.value = 1;  // Example message content
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL_MS);
    }
}

int main(void)
{
    msg_t msg;

    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Create a sender thread
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    // Main thread acts as the receiver
    while (1) {
        mbox_get(&mbox, &msg);  // Corrected function call
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
