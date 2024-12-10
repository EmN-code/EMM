
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "lib/random.h"
#include "sys/etimer.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1
#define RGB_LED_GPIO_PORT 2
#define RGB_LED_GPIO_PIN 3

#define READ_INTERVAL (5 * CLOCK_SECOND)

static struct etimer read_timer;

void read_dht11(void) {
    // Simulate reading DHT11 sensor
    // In a real implementation, you would read the sensor values here
    uint8_t humidity_integer = 50; // Example value
    uint8_t temperature_integer = 25; // Example value

    // Turn on RGB LED if temperature is above 25 degrees
    if (temperature_integer > 25) {
        gpio_hal_set(RGB_LED_GPIO_PORT, RGB_LED_GPIO_PIN, 1);
    } else {
        gpio_hal_set(RGB_LED_GPIO_PORT, RGB_LED_GPIO_PIN, 0);
    }
}

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data) {
    PROCESS_BEGIN();

    // Configure GPIO pins for DHT11 and RGB LED
    gpio_hal_set_input(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    gpio_hal_set_output(RGB_LED_GPIO_PORT, RGB_LED_GPIO_PIN);

    // Set initial timer for reading DHT11
    etimer_set(&read_timer, READ_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));

        read_dht11();

        // Reset the timer for the next reading
        etimer_reset(&read_timer);
    }

    PROCESS_END();
}
