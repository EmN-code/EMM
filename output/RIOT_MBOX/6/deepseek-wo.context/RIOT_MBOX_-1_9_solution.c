
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 16
#define INTERVAL_MS 1000

kernel_pid_t main_pid;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;

    while (1) {
        xtimer_sleep(INTERVAL_MS / MS_PER_SEC);
        msg_send(&msg, main_pid);
    }
}

int main(void)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    main_pid = thread_getpid();

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
