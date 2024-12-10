
#include "contiki.h"
#include "dev/gpio.h"  // Ensure this header is included for gpio functions
#include "dht11-sensor.h"  // Ensure this header is included for dht11 functions
#include <stdio.h>

#define DHT11_GPIO_PIN 2  // Define the GPIO pin for DHT11

PROCESS(dht11_reading_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_reading_process);

PROCESS_THREAD(dht11_reading_process, ev, data)
{
    static struct etimer timer;
    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    uint8_t status;

    PROCESS_BEGIN();

    // Configure the GPIO pin for DHT11 as input
    gpio_set_input(DHT11_GPIO_PIN);

    while (1) {
        // Set a timer to read the DHT11 sensor every 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        status = dht11_read(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal);

        if (status == DHT11_STATUS_OKAY) {
            printf("Humidity: %d.%d %%, Temperature: %d.%d C\n",
                   humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
        } else {
            printf("Failed to read DHT11 sensor, status: %d\n", status);
        }
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize the Contiki system
    contiki_init();

    // Start the Contiki processes
    PROCESS_RUN();

    return 0;
}
