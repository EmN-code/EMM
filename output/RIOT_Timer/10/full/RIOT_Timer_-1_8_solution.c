#include "ztimer.h" // Timer operations
#include "stdlib.h" // Standard library
#include "stdatomic.h" // Atomic operations
#include "stddef.h" // Standard definitions
#include "stdbool.h" // Boolean type
#include "stdio.h" // Standard I/O
#include "string.h" // String operations
#include "sys/socket.h" // Socket programming
#include "netinet/in.h" // Internet address family
#include "arpa/inet.h" // Definitions for internet operations
#include "esp_system.h" // ESP32 system specific functionalities

#define LED_PIN GPIO_NUM_2 // Define the pin where the LED is connected

void timer_callback(void *arg) {
    // Placeholder for LED toggle function
    printf("LED toggled\n");
}

int main(void) {
    evtimer_t evtimer;

    // Initialize the event timer with a message handler
    evtimer_init_msg(&evtimer);

    // Set up a timer event every second to trigger LED toggle
    evtimer_msg_event_t led_toggle_event;
    led_toggle_event.event.time = 1000000U; // 1 second in microseconds
    led_toggle_event.event.callback = timer_callback;

    // Add event to the event timer
    evtimer_add(&evtimer, (evtimer_event_t *)&led_toggle_event);

    // Start the event timer
    evtimer_run(&evtimer);

    // Infinite loop to keep the program running
    while(1) {
        ;
    }

    return 0;
}