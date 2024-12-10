
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/dht11.h" // Ensure this header is available in your Contiki environment
#include "dev/leds.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  1

#define RGB_LED_RED     (1 << 0)
#define RGB_LED_GREEN   (1 << 1)
#define RGB_LED_BLUE    (1 << 2)

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

static void read_dht11(void) {
    uint8_t data[5];
    if (dht11_read(DHT11_GPIO_PORT, DHT11_GPIO_PIN, data) == DHT11_STATUS_OKAY) {
        printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
               data[DHT11_VALUE_TEMPERATURE_INTEGER],
               data[DHT11_VALUE_TEMPERATURE_DECIMAL],
               data[DHT11_VALUE_HUMIDITY_INTEGER],
               data[DHT11_VALUE_HUMIDITY_DECIMAL]);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        read_dht11();
        leds_set(RGB_LED_WHITE); // Turn on RGB LED

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize Contiki processes
    process_init();
    process_start(&etimer_process, NULL);
    process_start(&dht11_rgb_led_process, NULL);

    // Main loop
    while (1) {
        process_run();
    }

    return 0;
}
