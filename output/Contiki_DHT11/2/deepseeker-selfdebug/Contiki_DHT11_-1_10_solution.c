
#include "contiki.h"
#include "dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int humidity_integer, temperature_integer;

    PROCESS_BEGIN();

    // Configure the DHT11 sensor to use GPIO 2
    configure(DHT11_CONFIGURE_GPIO_PORT, 0); // Assuming GPIO port 0
    configure(DHT11_CONFIGURE_GPIO_PIN, 2);  // GPIO pin 2
    configure(SENSORS_HW_INIT, 0);           // Initialize the sensor

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the sensor values
        humidity_integer = value(DHT11_VALUE_HUMIDITY_INTEGER);
        temperature_integer = value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Print the values
        printf("Humidity: %d %%\n", humidity_integer);
        printf("Temperature: %d C\n", temperature_integer);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    contiki_init();
    return 0;
}
