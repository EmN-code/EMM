
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>

#define MSG_QUEUE_SIZE 10
#define SEND_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void) {
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        msg_send(&msg, thread_getpid());
        xtimer_usleep(SEND_INTERVAL);
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_t *sender = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender_thread"
    );

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
