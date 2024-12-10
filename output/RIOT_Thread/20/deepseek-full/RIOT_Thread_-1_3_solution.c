
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ztimer.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <mbox.h>
#include <xtimer.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <MQTTClient.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 15

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static kernel_pid_t pid_high, pid_low;

static void *high_priority_thread(void *arg)
{
    msg_t msg, reply;
    msg.content.value = 1234;

    printf("High priority thread sending message...\n");
    msg_send_receive(&msg, &reply, pid_low);

    printf("High priority thread received reply: %" PRIu32 "\n", reply.content.value);

    return NULL;
}

static void *low_priority_thread(void *arg)
{
    msg_t msg, reply;

    printf("Low priority thread waiting for message...\n");
    msg_receive(&msg);

    printf("Low priority thread received message: %" PRIu32 "\n", msg.content.value);

    reply.content.value = 5678;
    msg_reply(&msg, &reply);

    return NULL;
}

int main(void)
{
    puts("Starting inter-thread communication example");

    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, high_priority_thread, NULL, "high_priority");
    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, low_priority_thread, NULL, "low_priority");

    return 0;
}
