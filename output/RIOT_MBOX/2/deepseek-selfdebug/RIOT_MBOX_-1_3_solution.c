
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "event.h"
#include "event/periodic_callback.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL (1000000U)  // 1 second in microseconds

static char stack[STACK_SIZE];
static mbox_t mbox;
static msg_t msg_array[8];

static void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example value
    msg_send(&msg, thread_getpid());
}

static void *event_thread(void *arg)
{
    (void)arg;
    event_periodic_callback_t periodic_event;
    event_periodic_callback_create(&periodic_event, ZTIMER_USEC, INTERVAL, NULL, event_callback);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_array, 8);
    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");

    return 0;
}
