
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "event.h"

#define MAIN_QUEUE_SIZE     (8)
#define THREAD_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY     (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[THREAD_STACK_SIZE];
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static event_queue_t main_event_queue;

void event_handler(void *arg)
{
    (void)arg;
    printf("Event received!\n");
}

void *event_thread(void *arg)
{
    (void)arg;
    event_queue_init(&main_event_queue);

    while (1) {
        event_t *event = event_wait(&main_event_queue);
        event->handler(event->arg);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                                     THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create event thread\n");
        return 1;
    }

    event_t event;
    event.handler = event_handler;
    event.arg = NULL;

    while (1) {
        xtimer_sleep(5);
        event_post(&main_event_queue, &event);
    }

    return 0;
}
