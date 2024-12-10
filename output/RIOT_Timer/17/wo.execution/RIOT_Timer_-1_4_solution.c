#include <xtimer.h> // Required for timers
#include <unistd.h> // For usleep
#include <timex.h> // For timex conversion functions
#include <thread.h> // For threading functions
#include <stdio.h> // For printf
#include <stdbool.h> // For boolean type
#include <stdlib.h> // Standard library functions
#include <stddef.h> // For standard type definitions
#include <stdatomic.h> // For atomic operations
#include <msg.h> // Inter-thread messaging
#include <time_units.h> // For time unit conversion
#include <kernel_defines.h> // Kernel specific definitions
#include <MQTTClient.h> // For MQTT functionalities (if needed)
#include <evtimer_msg.h> // For evtimer functions
#include <ztimer.h> // For high precision timers
#include <mbox.h> // Mailbox messaging
#include <event/thread.h> // Event handling in threads
#include <evtimer_mbox.h> // Mailbox event timers

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second in microseconds
#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)
#define LED_PIN 2 // Example LED pin; choose appropriate GPIO pin

evtimer_t evtimer;
evtimer_msg_event_t event;
kernel_pid_t main_pid;

void timer_handler(void *arg)
{
    (void)arg;
    // Here, toggle or switch the LED state
    printf("LED toggled\n");
}

int main(void)
{
    main_pid = thread_getpid();

    // Initialize the evtimer
    evtimer_init_msg(&evtimer);

    // Set up the event
    event.event.callback = timer_handler;

    // Add the event to the timer with the main thread pid
    evtimer_add_msg(&evtimer, &event, main_pid);

    // Start timer
    while (1) {
        // Wait EVENT_PERIOD (1 second) between toggles
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
        evtimer_msg(Event); // Trigger the timer event
    }

    return 0;
}