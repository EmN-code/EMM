#include <stdio.h>
#include <xtimer.h>
#include <timex.h>
#include <periph/gpio.h>

#define INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(0, 2)  // Example LED pin, please update according to your setup

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    uint64_t last_wakeup = xtimer_now64();

    while (1) {
        // Attempts to wake up periodically every second
        last_wakeup = xtimer_now64();
        while (1) {
            int rc = xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
            if (rc != 0) {
                printf("xtimer_periodic_wakeup failed\n");
                xtimer_sleep(1);
                continue;
            } else {
                printf("Timer wakeup successful\n");
                break;
            }
        }

        // Toggle LED
        gpio_toggle(LED_PIN);
    }

    return 0;
}