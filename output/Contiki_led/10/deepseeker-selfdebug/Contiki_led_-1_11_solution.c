
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/leds.h"
#include "dev/dht11-sensor.h" // Ensure this header is available

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  1

#define RGB_LED_RED     0x01
#define RGB_LED_GREEN   0x02
#define RGB_LED_BLUE    0x04
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

static void read_dht11(void) {
    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    int status = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER, &humidity_integer);
    if (status == DHT11_STATUS_OKAY) {
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_DECIMAL, &humidity_decimal);
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_integer);
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature_decimal);
        printf("Humidity: %d.%d %% Temperature: %d.%d C\n", humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize DHT11 sensor
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    // Set timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor
        read_dht11();

        // Turn on RGB LED (Yellow in this case)
        leds_set(RGB_LED_YELLOW);

        // Reset timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize Contiki
    contiki_init();

    // Start the process
    process_start(&dht11_rgb_led_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
