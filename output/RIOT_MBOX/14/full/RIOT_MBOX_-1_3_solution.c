#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "mma8x5x_regs.h"
#include "ztimer.h"

#define THREAD_STACKSIZE_MAIN (CONFIG_MAIN_QUEUE_SIZE * sizeof(msg_t))
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    evtimer_msg_event_t evtimer;

    msg_init_queue((msg_t *)arg, CONFIG_MAIN_QUEUE_SIZE);

    evtimer_msg_event_init(&evtimer);
    evtimer_msg_periodic_add(&evtimer, ztimer_now(), ztimer_msec_from_ms(1000));

    while (1) {
        evtimer_msg_wait_period((msg_t *)arg);
        msg.content.value = 1;  // Example message
        msg_send(&msg, thread_getpid());
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue((msg_t *)arg, CONFIG_MAIN_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    char sender_stack[THREAD_STACKSIZE_MAIN];
    char receiver_stack[THREAD_STACKSIZE_MAIN];

    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender_thread");

    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver_thread");

    return 0;
}