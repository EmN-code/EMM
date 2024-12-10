
#include <stdio.h>
#include <periph/gpio.h>
#include <xtimer.h>
#include <mma8x5x.h>
#include <periph/rtc.h>
#include <MQTTClient.h>
#include <periph/i2c.h>
#include <event/thread.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <thread.h>
#include <mbox.h>
#include <mma8x5x_regs.h>
#include <dht.h>
#include <evtimer_msg.h>
#include <time_units.h>

#define DHT_PIN            GPIO_PIN(0, 18)
#define LED_PIN            GPIO_PIN(0, 21)
#define TEMP_THRESHOLD     30

void gpio_toggle(gpio_t pin) {
    gpio_set(pin);
    xtimer_usleep(500000); // 500 ms
    gpio_clear(pin);
}

int main(void) {
    int16_t temperature, humidity;
    gpio_t dht_pin = DHT_PIN;
    gpio_t led_pin = LED_PIN;

    // Initialize GPIO pins
    gpio_init(dht_pin, GPIO_IN);
    gpio_init(led_pin, GPIO_OUT);
    gpio_clear(led_pin);

    while (1) {
        // Read DHT11 sensor
        if (dht_read(&dht_pin, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Check if temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_toggle(led_pin);
            } else {
                gpio_clear(led_pin);
            }
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
