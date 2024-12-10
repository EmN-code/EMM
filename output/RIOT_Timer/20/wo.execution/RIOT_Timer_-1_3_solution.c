#include <ztimer.h>
#include <timex.h>
#include <stdio.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <stdlib.h>
#include <time_units.h>
#include <stddef.h>
#include <mbox.h>
#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>

#define STACK_SIZE THREAD_STACKSIZE_MAIN
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
define INTERVAL (1U * US_PER_SEC)

static char led_blinker_stack[STACK_SIZE];
static kernel_pid_t led_blinker_pid;

// LED blinker thread function
void *led_blinker(void *arg) {
    (void)arg;
    while (1) {
        // Toggle LED
        // gpio_toggle(LED_PIN);
        printf("LED toggled\n"); // Simulate LED toggle with a print statement
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    puts("RIOT LED blinker on ESP32\n");
    
    // Initialize LED pin
    // gpio_init(LED_PIN, GPIO_OUT);
    
    // Create blinker thread
    led_blinker_pid = thread_create(led_blinker_stack, sizeof(led_blinker_stack),
                                    THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                    led_blinker, NULL, "led_blinker");
    
    // Set up timer to wake up blinker thread every second
    ztimer_sleep(ZTIMER_SEC, 1);
    printf("Timer set to wake blinker thread every second\n");
    
    return 0;
}