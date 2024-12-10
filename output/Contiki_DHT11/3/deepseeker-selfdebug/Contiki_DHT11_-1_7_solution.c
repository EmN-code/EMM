
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int humidity_integer, humidity_decimal;
    static int temperature_integer, temperature_decimal;
    static int status;

    PROCESS_BEGIN();

    // Configure the DHT11 GPIO port and pin
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor values
        humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        humidity_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
        temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        temperature_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
        status = dht11_sensor.status(DHT11_STATUS_OKAY);

        if(status == DHT11_STATUS_OKAY) {
            printf("Humidity: %d.%d %%\n", humidity_integer, humidity_decimal);
            printf("Temperature: %d.%d C\n", temperature_integer, temperature_decimal);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
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
    while(1) {
        process_run();
    }

    return 0;
}
