#include <ztimer.h>
#include <timex.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <thread.h>
#include <xtimer.h>
#include <event/thread.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN  (THREAD_PRIORITY_MAIN - 1)
#define SENDER_INTERVAL (1U * US_PER_SEC)

static msg_t msg_queue[8];

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 0x100;
    while(1) {
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_USEC, SENDER_INTERVAL);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, 8);

    char sender_stack[THREAD_STACKSIZE_MAIN];
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_MAIN, 0,
                  sender_thread, NULL, "sender");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }
    return 0;
}