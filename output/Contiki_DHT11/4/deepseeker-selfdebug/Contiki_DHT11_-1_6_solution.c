
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    static int status;

    PROCESS_BEGIN();

    // Configure the GPIO pin for the DHT11 sensor
    gpio_hal_set_mode(DHT11_GPIO_PIN, GPIO_HAL_PIN_MODE_INPUT);

    while (1) {
        // Set a timer to read the DHT11 sensor every 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        status = sensors_read(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER, &humidity_integer, sizeof(humidity_integer));
        if (status == DHT11_STATUS_OKAY) {
            sensors_read(&dht11_sensor, DHT11_VALUE_HUMIDITY_DECIMAL, &humidity_decimal, sizeof(humidity_decimal));
            sensors_read(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_integer, sizeof(temperature_integer));
            sensors_read(&dht11_sensor, DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature_decimal, sizeof(temperature_decimal));

            printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
        } else {
            printf("Failed to read DHT11 sensor, status: %d\n", status);
        }
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize the Contiki system
    process_init();
    process_start(&etimer_process, NULL);
    process_start(&dht11_read_process, NULL);

    // Enter the main loop
    while (1) {
        process_run();
    }

    return 0;
}
