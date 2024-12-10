#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static void *sender_thread(void *arg) {
    (void)arg;
    evtimer_t evtimer;
    evtimer_msg_event_init(&evtimer);

    evtimer_msg_periodic_add(&evtimer, ztimer_msec_from_ms(500), (void*)12345);
    while (1) {
        evtimer_msg_wait_period(&evtimer);
        puts("Sender: sending message");
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: got message: %ld\n", (long)msg.content.ptr);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");
    if(sender_pid < 0) {
        puts("Error: unable to create sender thread\n");
        return 1;
    }

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");
    if(receiver_pid < 0) {
        puts("Error: unable to create receiver thread\n");
        return 1;
    }

    return 0;
}