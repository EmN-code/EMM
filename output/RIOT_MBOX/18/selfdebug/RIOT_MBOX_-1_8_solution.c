#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <timex.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void sender(void)
{
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    printf("Message queue initialized. Sending messages every second.\n");
    sender();
    return 0;
}