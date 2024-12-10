
#include "thread.h"
#include "msg.h"
#include "mbox.h"
#include "ztimer.h"
#include "event.h"
#include "event/periodic_callback.h"

#define MAIN_QUEUE_SIZE     (8)
#define EVENT_INTERVAL      (1000000U)  // 1 second in microseconds
#define THREAD_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t main_mbox;
static char thread_stack[THREAD_STACK_SIZE];

static void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example value
    mbox_put(&main_mbox, &msg);
}

static void *thread_func(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_get(&main_mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&main_mbox, main_msg_queue, MAIN_QUEUE_SIZE);

    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_thread_queues[EVENT_QUEUE_PRIO_MEDIUM], event_callback, NULL);
    event_periodic_callback_set_interval(&periodic_event, EVENT_INTERVAL);
    event_periodic_callback_start(&periodic_event);

    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func, NULL, "event_thread");

    return 0;
}
