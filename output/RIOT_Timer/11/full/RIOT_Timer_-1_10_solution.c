#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <netinet/in.h>
#include <mutex.h>
#include <msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>

#define TIMER_INTERVAL (1U * 1000000U) // Timer interval set to 1 second
#define LED_PIN (GPIO_PIN(PORT_C, 9)) // Example LED pin; adjust per board

// Function to simulate network connection
int NetworkConnect(void *n, char* address_ip, int port_number) {
    // Simulated function
    // Return 0 to indicate success
    return 0;
}

void init_gpio_for_led(void) {
    printf("Initializing GPIO for LED\n");
    // Initialize GPIO for LED functionality
    gpio_init(LED_PIN, GPIO_OUT);
}

void timer_callback(void *arg) {
    printf("Timer callback triggered\n");
    // Toggle LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    puts("RIOT Timer and LED example\n");

    // Initialize the LED pin
    init_gpio_for_led();

    // Setup timer struct
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    timer.alarm = TIMER_INTERVAL;

    while (true) {
        // Network connection attempt loop
        int rc = NetworkConnect(NULL, "192.168.1.100", 1883);

        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            sleep(1); // wait before retry
            continue;
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }

    // Start the timer
    ztimer_set(&ztimer_msec, &timer, TIMER_INTERVAL);
    
    // Main loop
    while (1) {
        pause(); // Pause the program waiting for any signal, effectively letting the timer run
    }

    return 0;
}
