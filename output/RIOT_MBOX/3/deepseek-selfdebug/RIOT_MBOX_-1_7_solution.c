
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 16
#define INTERVAL_MS 1000000

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(INTERVAL_MS);
        msg_send(&msg, thread_getpid());
    }
}

void receiver_thread(void)
{
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);  // Corrected call to mbox_get
        printf("Received message with value: %d\n", msg.content.value);
    }
}

int main(void)
{
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    while (1) {
        msg_receive(&msg);
    }

    return 0;
}
