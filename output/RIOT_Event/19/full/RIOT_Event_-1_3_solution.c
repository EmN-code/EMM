#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static evtimer_t timer;
static evtimer_mbox_event_t periodic_event;
static msg_t queue[8];

void event_periodic(void *arg)
{
    (void)arg;
    while(1) {
        evtimer_mbox_event_wait(&periodic_event);
        printf("Periodic event triggered\n");
    }
}

int main(void)
{
    msg_init_queue(queue, 8);

    evtimer_mbox_init(&periodic_event, &timer);
    evtimer_set(&timer, TIMER_INTERVAL, 0);

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     0, event_periodic, NULL, "event_periodic");

    evtimer_mbox_start(&periodic_event);

    return 0;
}