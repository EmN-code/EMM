#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <evtimer_msg.h>
#include <thread.h>

#define LED_PIN GPIO_PIN(0, 2)
#define TIMER_INTERVAL (1U * US_PER_SEC)

void *led_thread(void *arg) {
    (void)arg;
    while (1) {
        gpio_toggle(LED_PIN);
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Create a thread for LED toggling
    char led_thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(led_thread_stack, sizeof(led_thread_stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, led_thread, NULL, "led_thread");

    // Main thread should do other tasks or sleep
    while (1) {
        xtimer_sleep(10);
    }
    return 0;
}