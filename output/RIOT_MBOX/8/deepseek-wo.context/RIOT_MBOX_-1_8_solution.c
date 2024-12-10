
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE (8)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received event: %" PRIu32 "\n", (uint32_t)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY,
                                     THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    uint32_t event_counter = 0;
    while (1) {
        msg_t msg;
        msg.content.value = event_counter++;
        msg_send(&msg, pid);
        xtimer_sleep(1);
    }

    return 0;
}
