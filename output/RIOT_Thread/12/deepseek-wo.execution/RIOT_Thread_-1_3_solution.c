
#include <inttypes.h>
#include <msg.h>
#include <thread.h>
#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 16

static char stack_low_prio[STACKSIZE];
static char stack_high_prio[STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *low_prio_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

void *high_prio_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t low_prio_pid, high_prio_pid;
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    low_prio_pid = thread_create(stack_low_prio, sizeof(stack_low_prio),
                                 THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                 low_prio_thread, NULL, "low_prio");

    high_prio_pid = thread_create(stack_high_prio, sizeof(stack_high_prio),
                                  THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                  high_prio_thread, NULL, "high_prio");

    msg.type = 0;
    msg.content.value = 1234;

    msg_send(&msg, low_prio_pid);
    msg_send(&msg, high_prio_pid);

    return 0;
}
