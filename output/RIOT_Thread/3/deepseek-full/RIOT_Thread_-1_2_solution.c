
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "time_units.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 15

char thread_stack_high[STACKSIZE];
char thread_stack_low[STACKSIZE];

kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    msg_t msg, reply;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
        msg_reply(&msg, &reply);
    }
    return NULL;
}

void *thread_low(void *arg)
{
    msg_t msg;
    while (1) {
        msg.content.value = 42;
        msg_send_receive(&msg, &msg, pid_high);
        printf("Low priority thread received reply: %d\n", msg.content.value);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    pid_high = thread_create(thread_stack_high, STACKSIZE, PRIORITY_HIGH, 0, thread_high, NULL, "thread_high");
    pid_low = thread_create(thread_stack_low, STACKSIZE, PRIORITY_LOW, 0, thread_low, NULL, "thread_low");

    return 0;
}
