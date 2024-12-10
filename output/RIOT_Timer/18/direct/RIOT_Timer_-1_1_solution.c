#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_B, 0)

void *led_thread(void *arg) {
    (void)arg;
    while(1) {
        gpio_toggle(LED_PIN);
        xtimer_sleep(1);  // wait for 1 second
    }
    return NULL;
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Start the LED thread
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, 
                  led_thread, NULL, "LED thread");

    // This loop can be used to perform other tasks
    while (1) {
        // You can put other code here if needed
        xtimer_sleep(1);  // main thread can sleep
    }

    return 0;
}