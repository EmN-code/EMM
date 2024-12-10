
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 16
#define INTERVAL_MS 1000000  // 1 second in microseconds

msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void)
{
    msg_t msg;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        msg_send(&msg, thread_getpid());
        xtimer_usleep(INTERVAL_MS);
    }
}

int main(void)
{
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return 0;
}
