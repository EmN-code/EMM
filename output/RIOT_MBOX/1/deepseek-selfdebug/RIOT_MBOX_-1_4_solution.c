
#include <stdio.h>
#include <thread.h>
#include <mbox.h>
#include <xtimer.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static char stack[THREAD_STACKSIZE];
static kernel_pid_t handler_pid;

void *handler_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;
    char message[] = "Hello, RIOT!";

    mbox_init(&mbox, NULL);

    handler_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY,
                                THREAD_CREATE_STACKTEST, handler_thread, NULL, "handler");

    while (1) {
        msg.content.ptr = message;
        mbox_put(&mbox, &msg);
        xtimer_sleep(5);
    }

    return 0;
}
