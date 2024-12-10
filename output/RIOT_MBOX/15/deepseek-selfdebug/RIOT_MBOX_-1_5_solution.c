
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "fmt.h"

#define MSG_QUEUE_SIZE (8)
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t sender_pid;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        msg_send(&msg, sender_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    char stack[SENDER_STACKSIZE];
    sender_pid = thread_create(stack, sizeof(stack), SENDER_PRIO,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with content: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
