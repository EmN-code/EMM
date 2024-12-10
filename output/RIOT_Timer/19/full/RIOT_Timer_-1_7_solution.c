#include <xtimer.h>
#include <stdio.h>
#include <board.h>
#include <periph/gpio.h>

#define LED_PIN     GPIO_PIN(PORT_A, 5) // Assuming you are using pin 5 on PORT_A.
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval

void timer_callback(void *arg)
{
    (void)arg;
    static bool led_state = false;
    led_state = !led_state; // Toggle LED state
    if (led_state) {
        gpio_set(LED_PIN);
        printf("LED ON\n");
    } else {
        gpio_clear(LED_PIN);
        printf("LED OFF\n");
    }
}

int main(void)
{
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing GPIO\n");
        return 1; // Return error code
    }
    
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}