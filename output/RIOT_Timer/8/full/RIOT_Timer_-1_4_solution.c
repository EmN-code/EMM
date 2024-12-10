#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <stdatomic.h>
#include <timex.h>
#include <time_units.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <mbox.h>

#define TIMER_INTERVAL   (1U * 1000000U) // 1 second in microseconds
#define GPIO_PIN         2 // example pin for LED

static ztimer_msg_t timer;

void toggle_led(void) {
    // Placeholder: You will toggle the GPIO here
    printf("LED toggled\n");
}

void *timer_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));
    while (1) {
        msg_receive(&msg);
        toggle_led();
    }
    return NULL;
}

int main(void) {
    // Initialize the timer message
    evtimer_msg_init_queue(&timer, msg_queue, sizeof(msg_queue) / sizeof(msg_t));

    // Set periodic timer
    evtimer_msg_set_periodic(&timer, TIMER_INTERVAL);
    
    // Create a thread to handle the toggling
    thread_create(timer_thread, NULL, KERNEL_THREAD_STACK_DEFAULT, "timer_thread");

    while (1) {
        // Keep the main function alive
        sleep(1);
    }
    return 0;
}