
#include <stdlib.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 5  // Assuming GPIO5 is connected to an LED

static void *timer_thread(void *arg)
{
    (void)arg;

    while (1) {
        // Toggle the LED (assuming GPIO5 is connected to an LED)
        // Note: This is a placeholder for the actual GPIO toggle function
        // which is not allowed by the blacklist.
        printf("LED toggled\n");

        // Sleep for 1 second
        xtimer_usleep(TIMER_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Initialize the message queue for the current thread
    msg_t msg_array[16];
    msg_init_queue(msg_array, 16);

    // Create and start the timer thread
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, timer_thread, NULL, "timer_thread");

    return 0;
}
