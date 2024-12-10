
#include "contiki.h"
#include "dev/gpio.h"  // Include the correct GPIO header file
#include "dht11-sensor.h"
#include <stdio.h>

#define DHT11_GPIO_PIN 2  // Define the GPIO pin for DHT11

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int humidity_integer, humidity_decimal;
    static int temperature_integer, temperature_decimal;
    static int status;

    PROCESS_BEGIN();

    // Configure the GPIO pin for DHT11
    gpio_set_input(DHT11_GPIO_PIN);

    while(1) {
        // Set a timer to read the DHT11 sensor every 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        status = dht11_read(DHT11_GPIO_PIN, &humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal);

        if(status == DHT11_STATUS_OKAY) {
            printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
        } else {
            printf("Failed to read DHT11 sensor, status: %d\n", status);
        }
    }

    PROCESS_END();
}
