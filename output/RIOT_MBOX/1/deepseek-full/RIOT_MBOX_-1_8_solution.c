
#include <stdio.h>
#include <stdint.h>
#include <thread.h>
#include <xtimer.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>

#define QUEUE_SIZE (16)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static char thread_stack[STACK_SIZE];

void sender_thread(void)
{
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_usleep(1000000); // Sleep for 1 second
    }
}

void receiver_thread(void)
{
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, (thread_task_func_t)sender_thread, NULL, "sender");

    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, (thread_task_func_t)receiver_thread, NULL, "receiver");

    return 0;
}
