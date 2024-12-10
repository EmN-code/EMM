
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE (8)
#define INTERVAL_MS (1000000U)  // 1 second interval

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void)
{
    msg_t msg;
    while (1) {
        msg.content.value = 42;  // Example message content
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL_MS);
    }
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Create the sender thread
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    // Main thread can handle other tasks or just wait
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, 0);  // Non-blocking get
        if (msg.content.value == 42) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }

    return 0;
}
