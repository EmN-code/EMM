
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE     (8)
#define SEND_INTERVAL_MS   (1000000U)  /* 1 second */

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(&mbox, &msg);
        xtimer_usleep(SEND_INTERVAL_MS);
    }
}

int main(void)
{
    msg_t msg;

    /* Initialize the message box */
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    /* Create the sender thread */
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    while (1) {
        /* Receive a message from the message box */
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
