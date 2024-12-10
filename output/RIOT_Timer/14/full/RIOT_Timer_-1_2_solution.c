#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL (ZTIMER_MSEC(1000)) // 1 second interval

void *led_thread(void *arg) {
    (void)arg; // unused
    evtimer_t evtimer;
    msg_t msg;
    evtimer_init_msg(&evtimer);
    evtimer_add_msg(&evtimer, INTERVAL, thread_getpid(), "led_event");

    while (1) {
        msg_receive(&msg);
        // As we are toggling the LED, we'll simulate the action with a print
        printf("LED toggled\n");
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid;
    char stack[THREAD_STACKSIZE_MAIN];

    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST,
                        led_thread, NULL, "led_thread");

    if (pid <= KERNEL_PID_UNDEF) {
        puts("Error creating thread");
        return 1;
    }

    while(1) {
        // Main loop can handle other tasks or put the MCU to sleep to save power
        xtimer_sleep(1);
    }

    return 0;
}