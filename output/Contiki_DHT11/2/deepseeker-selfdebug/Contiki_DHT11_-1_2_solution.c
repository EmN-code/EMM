
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11.h"
#include <stdio.h>

// Define the GPIO pin for DHT11
#define DHT11_GPIO_PIN 2

// Process to read DHT11 sensor
PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

// Function to read DHT11 sensor
static void dht11_read(void) {
    uint8_t humidity_int, humidity_dec, temperature_int, temperature_dec;
    int status = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity_int);
    if (status == DHT11_STATUS_OKAY) {
        dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL, &humidity_dec);
        dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_int);
        dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature_dec);
        printf("Humidity: %d.%d %% Temperature: %d.%d C\n", humidity_int, humidity_dec, temperature_int, temperature_dec);
    } else {
        printf("Failed to read DHT11 sensor, status: %d\n", status);
    }
}

// Process definition
PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Configure the GPIO pin for DHT11
    gpio_hal_pin_configure(DHT11_GPIO_PIN, GPIO_HAL_PIN_MODE_INPUT);

    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        dht11_read();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
