
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 10
#define SEND_INTERVAL (1 * US_PER_SEC)

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void)
{
    msg_t msg;
    kernel_pid_t sender_pid = thread_getpid();

    while (1) {
        msg.content.value = sender_pid;
        mbox_put(&mbox, &msg);
        xtimer_usleep(SEND_INTERVAL);
    }
}

int main(void)
{
    msg_t msg;
    kernel_pid_t sender_pid;

    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0,
                  (thread_task_func_t)sender_thread, NULL, "sender");

    while (1) {
        mbox_get(&mbox, &msg, 1);
        sender_pid = msg.content.value;
        printf("Received message from thread with PID: %d\n", sender_pid);
    }

    return 0;
}
