
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACK_SIZE 512
#define PRIORITY THREAD_PRIORITY_MAIN - 1

static char stack[STACK_SIZE];
static event_queue_t event_queue;

static void periodic_event_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_thread_init(&event_queue, stack, sizeof(stack), PRIORITY);

    xtimer_t timer;
    timer.callback = periodic_event_callback;
    timer.arg = NULL;

    xtimer_set(&timer, 1000000); // 1 second in microseconds

    return 0;
}