
#include "contiki.h"
#include "contiki-net.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include "dev/leds.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1
#define RGB_LED_PIN 2

static struct etimer et;

void read_dht11(void) {
    // Simulate reading DHT11 sensor
    // In a real implementation, you would read the sensor values here
    uint8_t humidity_integer = 50; // Example value
    uint8_t temperature_integer = 25; // Example value

    // Turn on RGB LED based on sensor reading
    if (humidity_integer > 60) {
        leds_set(LEDS_RED);
    } else if (temperature_integer > 30) {
        leds_set(LEDS_BLUE);
    } else {
        leds_set(LEDS_GREEN);
    }
}

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    PROCESS_BEGIN();

    // Configure GPIO for DHT11 and RGB LED
    gpio_hal_init();
    gpio_hal_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    gpio_hal_output(DHT11_GPIO_PORT, RGB_LED_PIN);

    while (1) {
        // Set timer for 5 seconds
        etimer_set(&et, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read DHT11 sensor and turn on RGB LED
        read_dht11();
    }

    PROCESS_END();
}
