#include "ztimer.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "msg.h"
#include "mutex.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "arpa/inet.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"

// Define LED GPIO pin
#define LED_PIN GPIO_PIN(0, 2)  // Dummy example, adjust for actual hardware

void initialize_led(void) {
    // Initialize the GPIO pin where the LED is connected as output
    gpio_init(LED_PIN, GPIO_OUT);
}

void turn_on_led(void) {
    // This function will turn the LED on
    gpio_set(LED_PIN);
}

void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initializations
    initialize_led();

    // Initialize timer
    ztimer_t timer;
    timer.interval = ZTIME_SEC(1); // 1 second interval
    timer.callback = timer_callback;
    timer_arg = NULL;

    // Start timer
    ztimer_start(&timer);

    while (1) {
        // Main loop can have idle tasks
        thread_sleep();
    }

    return 0;
}