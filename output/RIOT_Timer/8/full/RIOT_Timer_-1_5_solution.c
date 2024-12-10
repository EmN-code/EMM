#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <msg.h>
#include <event/thread.h>
#include <mma8x5x_regs.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <mbox.h>

#define TIMER_INTERVAL 1000000 // 1 second
#define LED_PIN GPIO_PIN(PORT_A, 0)

kernel_pid_t main_pid;

static void *led_thread(void *arg) {
    (void)arg;
    while (1) {
        // Toggle LED, assuming your board's GPIO library has some toggle function. 
        // gpio_toggle(LED_PIN);
        xtimer_usleep(TIMER_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("RIOT LED blink example\n");

    // Initialize message queue
    msg_t msg_queue[32];  // Array size should be power of two
    msg_init_queue(msg_queue, 32);

    // Create a thread to handle LED blinking
    char led_thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(led_thread_stack, sizeof(led_thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, led_thread, NULL, "LED thread");

    while (1) {
        // Main thread can perform other tasks or low-power sleep
        msg_t msg;
        msg_receive(&msg);
    }
    return 0;
}