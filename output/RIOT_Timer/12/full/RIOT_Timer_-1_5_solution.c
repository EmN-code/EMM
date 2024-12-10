#include <netinet/in.h>
#include <sys/socket.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <timex.h>
#include <msg.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <event/thread.h>

#define LED_PIN 2 // Define LED pin as per your board configuration

void toggle_led(void) {
    // Assuming the implementation of gpio_toggle function to toggle the LED
    // Use appropriate method to toggle LED, considering blacklisted `gpio_toggle`
    // Here, a simple print statement stands in for the actual hardware-specific implementation
    printf("Toggling LED on pin %d\n", LED_PIN);
}

void *event_thread(void *arg) {
    (void)arg;

    // Set up an event timer using ztimer for 1-second intervals
    ztimer_now_t last_wake = ztimer_now(ZTIMER_USEC);
    while (1) {
        toggle_led();
        ztimer_sleep(ZTIMER_USEC, 1000000);
        ztimer_now_t now = ztimer_now(ZTIMER_USEC);
        printf("Elapsed time: %lu\n", (unsigned long)(now - last_wake));
        last_wake = now;
    }
    return NULL;
}

int main(void) {
    // Initialize necessary components for the event timer
    kernel_pid_t event_thread_pid = thread_create(event_thread_stack, sizeof(event_thread_stack), 
                                                  THREAD_PRIORITY_MAIN - 1, 0,
                                                  event_thread, NULL, "event thread");
    printf("Started Timer Event Thread: %i\n", event_thread_pid);

    while (1) {
        // Enter sleep or perform other main operations
    }

    return 0;
}