
#include <stdio.h>
#include <xtimer.h>
#include <msg.h>

#define MSG_QUEUE_SIZE 10

static msg_t msg_queue[MSG_QUEUE_SIZE];

void periodic_callback(void)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        periodic_callback();
        xtimer_sleep(1);
    }

    return 0;
}
