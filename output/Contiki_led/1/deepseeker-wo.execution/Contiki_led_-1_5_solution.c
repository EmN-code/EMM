
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include "sys/clock.h"

#define DHT11_GPIO_PIN 2  // Example GPIO pin for DHT11
#define RGB_LED_PIN 3     // Example GPIO pin for RGB LED

static struct etimer et;

void read_dht11(void) {
    // Simulate reading DHT11 data
    // In a real implementation, you would read the actual sensor data here
    uint8_t humidity = 50;  // Example humidity value
    uint8_t temperature = 25;  // Example temperature value

    // Turn on the RGB LED
    gpio_hal_set(RGB_LED_PIN);
}

int main(void) {
    // Initialize GPIO pins
    gpio_hal_init();
    gpio_hal_output(DHT11_GPIO_PIN);
    gpio_hal_output(RGB_LED_PIN);

    // Set the initial timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read DHT11 sensor
        read_dht11();

        // Reset the timer for the next 5 seconds
        etimer_reset(&et);
    }

    return 0;
}
