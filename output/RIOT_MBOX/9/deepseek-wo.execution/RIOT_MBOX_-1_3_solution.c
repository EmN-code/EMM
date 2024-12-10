
#include <stdio.h>
#include <unistd.h>
#include "mbox.h"
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "thread.h"

#define STACKSIZE 512
#define QUEUE_SIZE 8

static char stack[STACKSIZE];
static msg_t queue[QUEUE_SIZE];
static mbox_t mbox;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    uint32_t count = 0;

    while (1) {
        msg.content.value = count++;
        mbox_put(&mbox, &msg);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, queue, QUEUE_SIZE);

    thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0,
                  (thread_task_func_t)sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return 0;
}
