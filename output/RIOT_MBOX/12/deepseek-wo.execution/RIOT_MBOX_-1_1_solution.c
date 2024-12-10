
#include <stdio.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void) {
    msg_t msg;
    msg.content.value = 0;  // Initialize message content

    while (1) {
        msg.content.value++;  // Increment message content
        msg_send(&msg, mbox);
        xtimer_usleep(INTERVAL_MS);  // Wait for 1 second
    }
}

void receiver_thread(void) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }
}

int main(void) {
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)sender_thread, NULL, "sender");

    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  (thread_task_func_t)receiver_thread, NULL, "receiver");

    return 0;
}
