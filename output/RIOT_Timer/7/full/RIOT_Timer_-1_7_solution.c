#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "time_units.h"
#include "timex.h"
#include "stdio.h"
#include "stddef.h"

#include "net/sock/util.h"
#include "esp_wifi.h"
#include "net/gcoap.h"
#include "esp_system.h"
#include "msg.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "periph/rtc.h"
#include "evtimer_msg.h"
#include "periph/gpio.h"
#include "mbox.h"
#include "net/sock/udp.h"
#include "periph/i2c.h"
#include "mutex.h"
#include "net/emcute.h"
#include "unistd.h"

#define LED_GPIO_PIN  GPIO_PIN(0, 2)  // Define the GPIO pin for the LED
#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval

static xtimer_t timer;

void toggle_led(void) {
    static bool led_state = false;
    if (led_state) {
        gpio_clear(LED_GPIO_PIN);
    } else {
        gpio_set(LED_GPIO_PIN);
    }
    led_state = !led_state;
}

static void timer_callback(void *arg) {
    (void)arg;
    toggle_led();
    // Reset the timer
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    // Initialize the GPIO
    if(gpio_init(LED_GPIO_PIN, GPIO_OUT)) {
        printf("GPIO Init failed!\n");
        return 1;
    }
    
    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    // Start the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    return 0;
}