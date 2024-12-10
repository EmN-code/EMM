#include <thread.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>

#define GPIO_PIN_LED 2          // GPIO pin for the LED
#define TEMP_THRESHOLD 30      // Temperature threshold
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

void turn_on_led(void) {
    gpio_set(GPIO_PIN_LED);
}

void turn_off_led(void) {
    gpio_clear(GPIO_PIN_LED);
}

int main(void) {
    int16_t temperature, humidity;
    dht_t dht_device;
    
    // Assuming initialization function dht_init exists and works
    dht_init(&dht_device, 22, DHT11);
    
    // Initialize the LED pin
gpio_init(GPIO_PIN_LED, GPIO_OUT);

    while (1) {
        if (dht_read(&dht_device, &temperature, &humidity) != ESP_OK) {
            // Handle error
            continue;
        }

        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

        if (temperature > TEMP_THRESHOLD) {
            turn_on_led();
        } else {
            turn_off_led();
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    // This part is not reached
    return 0;
}