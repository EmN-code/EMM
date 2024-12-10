#include <thread.h>
#include <stdio.h>
#include <timex.h>
#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)  // Adjust pin according to your setup

void *led_blinker(void *arg) {
    (void)arg;
    while (1) {
        gpio_toggle(LED_PIN);
        ztimer_sleep(ZTIMER_MSEC, 1000U); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Start the LED blinker thread
    thread_create(led_blinker_stack, sizeof(led_blinker_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACK_SIZE_MAIN, led_blinker, NULL, "led_blinker");

    // Main thread should not terminate
    while (1) {
        thread_yield();
    }
    return 0;
}
