#include <stdio.h>
#include <msg.h>
#include <xtimer.h>
#include <ztimer.h>

#define INTERVAL (1U * US_PER_SEC) // 1 second interval

static msg_t msg_queue[8];

void event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example content
    while(1) {
        msg_send(&msg, thread_getpid());
        printf("Message sent.\n");
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }
}

int main(void)
{
    msg_init_queue(msg_queue, 8);
    printf("Message queue initialized.\n");

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "sender");

    return 0;
}