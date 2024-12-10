
#include "riot.h"

#define QUEUE_SIZE 8
#define INTERVAL 1000000  // 1 second in microseconds

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        msg_send(&msg, thread_getpid(), false);
        xtimer_usleep(INTERVAL);
    }
}

void receiver_thread(void) {
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)sender_thread, NULL, "sender");
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)receiver_thread, NULL, "receiver");

    return 0;
}
