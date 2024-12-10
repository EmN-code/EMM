
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000

static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void)
{
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        msg_send(&msg, thread_getpid());
        xtimer_usleep(INTERVAL_MS);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t sender_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %lu\n", (unsigned long)msg.content.value);
    }

    return 0;
}
