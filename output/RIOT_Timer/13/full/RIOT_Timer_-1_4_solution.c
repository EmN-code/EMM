#include <unistd.h>
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <time_units.h>

atomic_bool led_state = false;

void toggle_led(void) {
    led_state = !led_state;
    printf("LED is now %s\n", led_state ? "ON" : "OFF");
}

void* timer_thread(void* arg) {
    (void)arg;
    evtimer_t evtimer;
    mbox_t mbox;
    msg_t msg_queue[8]; // example size, must be power of two

    mbox_init(&mbox, msg_queue, 8);
    evtimer_init_mbox(&evtimer);

    while (1) {
        evtimer_mbox_event_t event;
        evtimer_add_mbox(&evtimer, &event, &mbox);

        msg_t msg;
        if (mbox_receive(&mbox, &msg) == 0) {
            toggle_led();
        }

        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t tpid = thread_create(timer_thread, NULL, KERNEL_THREAD_STACKSIZE_DEFAULT, "timer thread");
    if (tpid == KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Thread created successfully\n");
        thread_join(tpid);
    }
    return 0;
}