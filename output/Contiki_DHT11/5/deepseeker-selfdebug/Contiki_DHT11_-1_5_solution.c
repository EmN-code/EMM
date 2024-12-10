
#include "contiki.h"
#include "dev/dht11-sensor.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    static int status;

    PROCESS_BEGIN();

    // Configure the DHT11 GPIO
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        humidity_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
        temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        temperature_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
        status = dht11_sensor.status(DHT11_STATUS_OKAY);

        if(status == DHT11_STATUS_OKAY) {
            printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", 
                   humidity_integer, humidity_decimal, 
                   temperature_integer, temperature_decimal);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        etimer_reset(&timer); // Reset the timer
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize Contiki
    contiki_init();

    // Start the Contiki system
    while(1) {
        process_run();
    }

    return 0;
}
