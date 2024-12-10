#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define MAIN_THREAD_PRIORITY    (THREAD_PRIORITY_MAIN - 1)
#define STACKSIZE_MAIN_THREAD   (THREAD_STACKSIZE_MAIN)

static char main_thread_stack[STACKSIZE_MAIN_THREAD];
mbox_t mbox;
msg_t mbox_buffer[8];

void *main_thread(void *arg) {
    (void) arg;  // do not use parameter
    msg_init_queue(mbox_buffer, 8);

    mbox_t mbox;
    msg_t msg;
    evtimer_t evtimer;
    evtimer_mbox_event_t evtimer_event;

    evtimer_init_mbox(&evtimer);
    /* Simulate an event every second */
    evtimer_add_mbox(&evtimer, &evtimer_event, &mbox);

    while (1) {
        /* Block and wait for any message in our mbox */
        msg_receive(&msg);
        printf("Received message from %d\n", msg.sender_pid);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t pid;
    
    /* Initialize mbox for communication */
    mbox_init(&mbox, mbox_buffer, sizeof(mbox_buffer));

    /* Create main thread and set its priority and stack */
    pid = thread_create(main_thread_stack, sizeof(main_thread_stack),
                        MAIN_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                        main_thread, NULL, "main thread");

    /* Set the priority for the created thread */
    sched_change_priority(pid, MAIN_THREAD_PRIORITY);

    return 0;
}