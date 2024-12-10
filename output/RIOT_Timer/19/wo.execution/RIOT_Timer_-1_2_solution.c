#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"

char led_blinker_stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t blink_pid;

void *led_blinker(void *arg) {
    (void)arg;
    while (1) {
        // This function will toggle the state of the LED using GPIO
        puts("LED on");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    blink_pid = thread_create(led_blinker_stack, sizeof(led_blinker_stack),
                              THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                              led_blinker, NULL, "blinker");
    if (blink_pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to create blinker thread");
        return 1;
    }
    return 0;
}