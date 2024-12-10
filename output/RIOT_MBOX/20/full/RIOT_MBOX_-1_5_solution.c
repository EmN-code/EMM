#include <xtimer.h>
#include <thread.h>
#include <sched.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <stdlib.h>
#include <stdio.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static char event_thread_stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t event_thread_pid;

void *event_thread(void *arg) {
    (void)arg;
    msg_t msg;
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize evtimer for IPC
    evtimer_init_msg(&evtimer);

    while (1) {
        msg_receive(&msg);
        printf("Received message type %u from PID %u\n", msg.type, msg.sender_pid);
    }

    return NULL;
}

int main(void)
{
    printf("RIOT message box example on ESP32\n");

    event_thread_pid = thread_create(event_thread_stack, sizeof(event_thread_stack),
                                     THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                     event_thread, NULL, "event thread");

    // Correctly setting the priority
    kernel_pid_t pid = thread_getpid();
    sched_change_priority(&pid, THREAD_PRIORITY_MAIN - 2);

    msg_t msg;
    while (1) {
        // Send a message at intervals
        xtimer_sleep(2);
        msg_send(&msg, event_thread_pid);
    }

    return 0;
}