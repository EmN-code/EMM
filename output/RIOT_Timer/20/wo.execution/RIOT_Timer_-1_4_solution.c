#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <msg.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <mbox.h>
#include <stdatomic.h>

#define LED0_PIN 2 /* assuming LED is connected to GPIO 2 */

static char led_blinker_stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t led_blinker_pid;

void *led_blinker(void *arg) {
    (void)arg;
    while (1) {
        //gpio_toggle(LED0_PIN); // Toggle LED pin
        puts("LED toggled");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("RIOT application to toggle an LED every second using a timer\n");
    //gpio_init(LED0_PIN, GPIO_OUT);

    led_blinker_pid = thread_create(led_blinker_stack, sizeof(led_blinker_stack),
                                    THREAD_PRIORITY_MAIN - 1, 0,
                                    led_blinker, NULL, "led_blinker");

    if (led_blinker_pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to create thread\n");
        return 1;
    }

    return 0;
}