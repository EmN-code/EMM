
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    while (1) {
        // Set the timer to 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor values
        humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        humidity_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
        temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        temperature_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);

        // Print the values
        printf("Humidity: %d.%d %%, Temperature: %d.%d C\n",
               humidity_integer, humidity_decimal,
               temperature_integer, temperature_decimal);
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize Contiki
    contiki_init();

    // Start the Contiki system
    contiki_start();

    return 0;
}
